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
Q_LOGGING_CATEGORY(GIK_CONFIG, "meldari.config")
#else
Q_LOGGING_CATEGORY(GIK_CONFIG, "meldari.config", QtInfoMsg)
#endif

struct ConfigValues {
    mutable QReadWriteLock lock{QReadWriteLock::Recursive};

    QString tmpl = QStringLiteral(MELDARI_CONF_MEL_TEMPLATE_DEFVAL);
    QString tmplDir = QStringLiteral(MELDARI_TEMPLATESDIR);

    bool loaded = false;
};

Q_GLOBAL_STATIC(ConfigValues, cfg)

void MeldariConfig::load(const QVariantMap &meldari, const QVariantMap &email)
{
    QWriteLocker locker(&cfg->lock);

    if (cfg->loaded) {
        return;
    }

    cfg->tmpl = meldari.value(QStringLiteral(MELDARI_CONF_MEL_TEMPLATE), QStringLiteral(MELDARI_CONF_MEL_TEMPLATE_DEFVAL)).toString();
    if (cfg->tmpl.startsWith(QLatin1Char('/'))) {
        QStringList fullPathParts = cfg->tmpl.split(QLatin1Char('/'), Qt::SkipEmptyParts);
        cfg->tmpl = fullPathParts.takeLast();
        cfg->tmplDir = QLatin1Char('/') + fullPathParts.join(QLatin1Char('/'));
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

QString MeldariConfig::tmplPath(const QString &path)
{
    return tmplPath() + QLatin1Char('/') + path;
}

QString MeldariConfig::tmplPath(const QStringList &pathParts)
{
    return tmplPath(pathParts.join(QLatin1Char('/')));
}
