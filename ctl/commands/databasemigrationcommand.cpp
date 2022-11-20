/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "databasemigrationcommand.h"

#include <Firfuorida/Migrator>

#include <QCoreApplication>
#include <QCommandLineParser>

DatabaseMigrationCommand::DatabaseMigrationCommand(QObject *parent)
    : DatabaseCommand{parent}
{
    setObjectName(QStringLiteral("migrate"));
}

void DatabaseMigrationCommand::init()
{

}

CLI::RC DatabaseMigrationCommand::exec(QCommandLineParser *parser)
{
    CLI::RC rc = RC::OK;

    parser->parse(QCoreApplication::arguments());

    if (checkShowHelp(parser)) {
        return rc;
    }

    setGlobalOptions(parser);

    rc = openDb(dbConName());
    if (rc != RC::OK) {
        return rc;
    }

    initMigrations();

    if (!m_migrator->migrate()) {
        rc = dbError(m_migrator->lastError().text());
    }

    return rc;
}

QString DatabaseMigrationCommand::summary() const
{
    //: CLI command summary
    //% "Run database migrations"
    return qtTrId("melctl-command-database-migration-summary");
}

QString DatabaseMigrationCommand::description() const
{
    //: CLI command description
    //% "Performs all database migrations that have not already be performed before. You should run this command after upgrading Meldari."
    return qtTrId("melctl-command-database-migration-description");
}
