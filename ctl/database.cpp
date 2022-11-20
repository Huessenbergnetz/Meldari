/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "database.h"
#include "confignames.h"

#include <QSqlDatabase>

#define DBCONNAME "dbmigrations"

Database::Database(QObject *parent)
    : Configuration{parent}
{

}

CLI::RC Database::openDb(const QString &connectionName)
{
    const RC rc = loadConfig();
    if (rc != RC::OK) {
        return rc;
    }

    //% "Establishing database connection"
    printStatus(qtTrId("melctl-status-open-db"));

    const QString confsec = QStringLiteral(MELDARI_CONF_DB);
    const QString dbType = value(confsec, QStringLiteral(MELDARI_CONF_DB_TYPE), QStringLiteral(MELDARI_CONF_DB_TYPE_DEFVAL)).toString().toUpper();
    const QString dbHost = value(confsec, QStringLiteral(MELDARI_CONF_DB_HOST), QStringLiteral(MELDARI_CONF_DB_HOST_DEFVAL)).toString();
    const QString dbUser = value(confsec, QStringLiteral(MELDARI_CONF_DB_USER), QStringLiteral(MELDARI_CONF_DB_USER_DEFVAL)).toString();
    const QString dbPass = value(confsec, QStringLiteral(MELDARI_CONF_DB_PASS)).toString();
    const QString dbName = value(confsec, QStringLiteral(MELDARI_CONF_DB_NAME), QStringLiteral(MELDARI_CONF_DB_NAME_DEFVAL)).toString();
    const int     dbPort = value(confsec, QStringLiteral(MELDARI_CONF_DB_PORT), MELDARI_CONF_DB_PORT_DEFVAL).toInt();

    auto db = QSqlDatabase::addDatabase(dbType, connectionName);

    if (Q_UNLIKELY(!db.isValid())) {
        printFailed();
        //: Error message
        //% "Failed to obtain database object."
        return dbError(qtTrId("melctl-error-db-invalid-object"));
    }

    if (dbType == u"QMYSQL") {
        db.setDatabaseName(dbName);
        if (!dbUser.isEmpty()) {
            db.setUserName(dbUser);
        }
        if (!dbPass.isEmpty()) {
            db.setPassword(dbPass);
        }
        if (dbHost[0] == QLatin1Char('/')) {
            db.setConnectOptions(QStringLiteral("UNIX_SOCKET=%1;MYSQL_OPT_RECONNECT=1;CLIENT_INTERACTIVE=1").arg(dbHost));
        } else {
            db.setConnectOptions(QStringLiteral("MYSQL_OPT_RECONNECT=1;CLIENT_INTERACTIVE=1"));
            db.setHostName(dbHost);
            db.setPort(dbPort);
        }
    } else {
        printFailed();
        //: Error message, %1 will be the invalid DB type
        //% "Can not open database connection for not supported database type “%1”."
        return configError(qtTrId("melctl-error-config-invalid-dbtype").arg(dbType));
    }

    if (Q_UNLIKELY(!db.open())) {
        printFailed();
        return dbError(db);
    }

    printDone();

    return RC::OK;
}

#include "moc_database.cpp"
