/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "databaseresetcommand.h"

#include <Firfuorida/Migrator>

#include <QCommandLineParser>

DatabaseResetCommand::DatabaseResetCommand(QObject *parent)
    : DatabaseCommand{parent}
{
    setObjectName(QStringLiteral("reset"));
}

DatabaseResetCommand::~DatabaseResetCommand() = default;

CLI::RC DatabaseResetCommand::exec(QCommandLineParser *parser)
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

    if (!m_migrator->reset()) {
        rc = dbError(m_migrator->lastError().text());
    }

    return rc;
}

QString DatabaseResetCommand::summary() const
{
    //: CLI command summary
    //% "Reset database migrations"
    return qtTrId("melctl-command-database-reset-summary");
}

QString DatabaseResetCommand::description() const
{
    //: CLI command description
    //% "Resets all database migrations. A reset will roll back and reapply all migrations. Note that this will result in data loss. This command is for development purposes only."
    return qtTrId("melctl-command-database-reset-description");
}

#include "moc_databaseresetcommand.cpp"
