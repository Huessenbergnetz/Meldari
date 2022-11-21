/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldari.h"
#include "logging.h"
#include "meldariconfig.h"

#include "common/confignames.h"

#include "controllers/root.h"
#include "controllers/controlcenter.h"

#include <Cutelyst/Engine>
#include <Cutelyst/Plugins/Utils/Sql>

#include <QCoreApplication>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QSqlError>

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(MEL_CORE, "meldari.core")
#else
Q_LOGGING_CATEGORY(MEL_CORE, "meldari.core", QtInfoMsg)
#endif

static QMutex mutex; // clazy:exclude=non-pod-global-static

Meldari::Meldari(QObject *parent) : Application(parent)
{
    QCoreApplication::setApplicationName(QStringLiteral("Meldari"));
    QCoreApplication::setApplicationVersion(QStringLiteral(MELDARI_VERSION));
}

bool Meldari::init()
{
    MeldariConfig::load(engine()->config(QStringLiteral(MELDARI_CONF_MEL)),
                        engine()->config(QStringLiteral(MELDARI_CONF_MAIL)));

    qCDebug(MEL_CORE) << "Registering controllers";
    new Root(this);
    new ControlCenter(this);

    return true;
}

bool Meldari::postFork()
{
    QMutexLocker locker(&mutex);

    return initDb();
}

bool Meldari::initDb() const
{
    qCDebug(MEL_CORE) << "Establishing database connection";

    const QVariantMap dbconf = engine()->config(QStringLiteral(MELDARI_CONF_DB));
    const QString dbtype = dbconf.value(QStringLiteral(MELDARI_CONF_DB_TYPE), QStringLiteral(MELDARI_CONF_DB_TYPE_DEFVAL)).toString();
    const QString dbname = dbconf.value(QStringLiteral(MELDARI_CONF_DB_NAME), QStringLiteral(MELDARI_CONF_DB_NAME_DEFVAL)).toString();
    const QString dbuser = dbconf.value(QStringLiteral(MELDARI_CONF_DB_USER), QStringLiteral(MELDARI_CONF_DB_USER_DEFVAL)).toString();
    const QString dbpass = dbconf.value(QStringLiteral(MELDARI_CONF_DB_PASS)).toString();
    const QString dbhost = dbconf.value(QStringLiteral(MELDARI_CONF_DB_HOST), QStringLiteral(MELDARI_CONF_DB_HOST_DEFVAL)).toString();
    const int     dbport = dbconf.value(QStringLiteral(MELDARI_CONF_DB_PORT), MELDARI_CONF_DB_PORT_DEFVAL).toInt();

    QSqlDatabase db;
    const QString dbConName = Sql::databaseNameThread();
    if (dbtype == u"QMYSQL") {
        if (Q_UNLIKELY(dbhost.isEmpty())) {
            qCCritical(MEL_CORE) << "Can not establish database connection: missing database hostname";
            return false;
        }
        if (Q_UNLIKELY(dbname.isEmpty())) {
            qCCritical(MEL_CORE) << "Can not establish database connection: missing database name";
            return false;
        }

        db = QSqlDatabase::addDatabase(dbtype, dbConName);
        if (Q_LIKELY(db.isValid())) {
            db.setDatabaseName(dbname);
            if (!dbuser.isEmpty()) {
                db.setUserName(dbuser);
            }
            if (!dbpass.isEmpty()) {
                db.setPassword(dbpass);
            }
            if (dbhost[0] == QLatin1Char('/')) {
                db.setConnectOptions(QStringLiteral("UNIX_SOCKET=%1;MYSQL_OPT_RECONNECT=1;CLIENT_INTERACTIVE=1").arg(dbhost));
            } else {
                db.setConnectOptions(QStringLiteral("MYSQL_OPT_RECONNECT=1;CLIENT_INTERACTIVE=1"));
                db.setHostName(dbhost);
                db.setPort(dbport);
            }
        } else {
            qCCritical(MEL_CORE) << "Can not establish database connection: failed to obtain database object";
            return false;
        }
    } else {
        qCCritical(MEL_CORE) << "Can not establish database connection:" << dbtype << "is not a supported database type";
        return false;
    }

    if (Q_UNLIKELY(!db.open())) {
        qCCritical(MEL_CORE) << "Can not establish database connection:" << db.lastError().text();
        return false;
    }

    return true;
}

#include "moc_meldari.cpp"
