/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldariconfig.h"
#include "logging.h"
#include "confignames.h"

#include <Cutelyst/Plugins/Memcached/Memcached>
#include <Cutelyst/Plugins/Utils/Sql>

#include <QGlobalStatic>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QSqlQuery>
#include <QSqlError>

#define MEMC_CONFIG_GROUP_KEY "options"
#define MEMC_CONFIG_STORAGE_DURATION 7200

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(MEL_CONF, "meldari.config")
#else
Q_LOGGING_CATEGORY(MEL_CONF, "meldari.config", QtInfoMsg)
#endif

struct ConfigValues {
    mutable QReadWriteLock lock{QReadWriteLock::Recursive};

    QString tmpl = QStringLiteral(MELDARI_CONF_MEL_TEMPLATE_DEFVAL);
    QString tmplDir = QStringLiteral(MELDARI_TEMPLATESDIR);
    MeldariConfig::StaticPlugin staticPlugin = MeldariConfig::StaticSimple;
    bool useMemcached = MELDARI_CONF_MEL_USEMEMCACHED_DEFVAL;
    bool useMemcachedSession = MELDARI_CONF_MEL_USEMEMCACHEDSESSION_DEFVAL;

    bool loaded = false;
};

Q_GLOBAL_STATIC(ConfigValues, cfg)

void MeldariConfig::load(const QVariantMap &meldari, const QVariantMap &email)
{
    QWriteLocker locker(&cfg->lock);

    if (cfg->loaded) {
        return;
    }

    qCDebug(MEL_CONF) << "Loading configuration";

    cfg->tmpl = meldari.value(QStringLiteral(MELDARI_CONF_MEL_TEMPLATE), QStringLiteral(MELDARI_CONF_MEL_TEMPLATE_DEFVAL)).toString();
    if (cfg->tmpl.startsWith(QLatin1Char('/'))) {
        QStringList fullPathParts = cfg->tmpl.split(QLatin1Char('/'), Qt::SkipEmptyParts);
        cfg->tmpl = fullPathParts.takeLast();
        cfg->tmplDir = QLatin1Char('/') + fullPathParts.join(QLatin1Char('/'));
    }

    const QString _statPlug = meldari.value(QStringLiteral(MELDARI_CONF_MEL_STATICPLUGIN), QStringLiteral(MELDARI_CONF_MEL_STATICPLUGIN_DEFVAL)).toString();
    if (_statPlug.compare(u"none") == 0) {
        cfg->staticPlugin = StaticNone;
    } else if (_statPlug.compare(u"simple") == 0) {
        cfg->staticPlugin = StaticSimple;
    } else if (_statPlug.compare(u"compressed") == 0) {
        cfg->staticPlugin = StaticCompressed;
    } else {
        qCWarning(MEL_CONF) << "Invalid value for" << MELDARI_CONF_MEL_STATICPLUGIN << "in section" << MELDARI_CONF_MEL << ": using default value " << MELDARI_CONF_MEL_STATICPLUGIN_DEFVAL;
    }

    cfg->useMemcached = meldari.value(QStringLiteral(MELDARI_CONF_MEL_USEMEMCACHED), MELDARI_CONF_MEL_USEMEMCACHED_DEFVAL).toBool();
    cfg->useMemcachedSession = meldari.value(QStringLiteral(MELDARI_CONF_MEL_USEMEMCACHEDSESSION), MELDARI_CONF_MEL_USEMEMCACHEDSESSION_DEFVAL).toBool();

    cfg->loaded = true;
}

QString MeldariConfig::tmpl()
{
    QReadLocker locker(&cfg->lock);
    return cfg->tmpl;
}

QString MeldariConfig::tmplPath()
{
    QReadLocker locker(&cfg->lock);
    return cfg->tmplDir + QLatin1Char('/') + cfg->tmpl;
}

QString MeldariConfig::tmplPath(QStringView path)
{
    return tmplPath() + QLatin1Char('/') + path;
}

QString MeldariConfig::tmplPath(const QStringList &pathParts)
{
    return tmplPath(pathParts.join(QLatin1Char('/')));
}

MeldariConfig::StaticPlugin MeldariConfig::staticPlugin()
{
    QReadLocker locker(&cfg->lock);
    return cfg->staticPlugin;
}

bool MeldariConfig::useMemcached()
{
    QReadLocker locker(&cfg->lock);
    return cfg->useMemcached;
}

bool MeldariConfig::useMemcachedSession()
{
    QReadLocker locker(&cfg->lock);
    return cfg->useMemcachedSession;
}

template< typename T >
T MeldariConfig::getDbOption(const QString &option, const T &defVal)
{
    T retVal = defVal;

    if (cfg->useMemcached) {
        Cutelyst::Memcached::MemcachedReturnType rt;
        T val = Cutelyst::Memcached::getByKey<T>(QStringLiteral(MEMC_CONFIG_GROUP_KEY), option, nullptr, &rt);
        if (rt == Cutelyst::Memcached::Success) {
            return val;
        }
    }

    QSqlQuery q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT option_value FROM options WHERE option_name = :option_name"));
    q.bindValue(QStringLiteral(":option_name"), option);

    if (Q_LIKELY(q.exec())) {
        if (Q_LIKELY(q.next())) {
            retVal = q.value(0).value<T>();
        } else {
            qCWarning(MEL_CONF) << "Can not find option" << option << "in the database";
        }
    } else {
        qCCritical(MEL_CONF) << "Failed to query option" << option << "from database:" << q.lastError().text();
    }

    if (cfg->useMemcached) {
        Cutelyst::Memcached::setByKey<T>(QStringLiteral(MEMC_CONFIG_GROUP_KEY), option, retVal, MEMC_CONFIG_STORAGE_DURATION);
    }

    return retVal;
}

template< typename T >
bool MeldariConfig::setDbOption(const QString &option, const T &value)
{
    bool rv = false;

    QSqlQuery q = CPreparedSqlQueryThread(QStringLiteral("INSERT INTO options (option_name, option_value) "
                                                         "VALUES (:option_name, :option_value) "
                                                         "ON DUPLICATE KEY UPDATE "
                                                         "option_value = :option_value"));
    q.bindValue(QStringLiteral(":optin_name"), option);
    q.bindValue(QStringLiteral(":optin_value"), QVariant::fromValue<T>(value));

    rv = q.exec();

    if (Q_UNLIKELY(!rv)) {
        qCCritical(MEL_CONF) << "Failed to save value" << value << "for option" << option << "in database:" << q.lastError().text();
    }

    if (rv && cfg->useMemcached) {
        Cutelyst::Memcached::setByKey<T>(QStringLiteral(MEMC_CONFIG_GROUP_KEY), option, value, MEMC_CONFIG_STORAGE_DURATION);
    }

    return rv;
}
