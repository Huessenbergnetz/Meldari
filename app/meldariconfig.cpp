/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldariconfig.h"
#include "logging.h"
#include "confignames.h"

#include <QGlobalStatic>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

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
