/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "databasecommand.h"
#include "databasemigrationcommand.h"
#include "databaserollbackcommand.h"
#include "databaserefreshcommand.h"
#include "databaseresetcommand.h"

#include "../dbmigrations/m0001_create_users_table.h"
#include "../dbmigrations/m0002_create_options_table.h"
#include "../dbmigrations/m0003_create_usersettings_table.h"
#include "../dbmigrations/m0004_create_domains_table.h"

#include <Firfuorida/Migrator>

#include <QCoreApplication>
#include <QCommandLineParser>

DatabaseCommand::DatabaseCommand(QObject *parent)
    : Command{parent}
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
    setObjectName(u"database");
#else
    setObjectName(QStringLiteral("database"));
#endif
}

DatabaseCommand::~DatabaseCommand() = default;

CLI::RC DatabaseCommand::exec(QCommandLineParser *parser)
{
    init();

    parser->addPositionalArgument(QStringLiteral("subcommand"), QString());
    parser->parse(QCoreApplication::arguments());

    setGlobalOptions(parser);

    QStringList args = parser->positionalArguments();
    if (args.takeFirst() != objectName()) {
        qFatal("%s", "Invalid command name! Aborting...");
    }

    const QString command = args.empty() ? QLatin1String("") : args.takeFirst().toLower();

    return runSubCommand(command, parser);
}

QString DatabaseCommand::summary() const
{
    //: CLI command summary
    //% "Manage database migrations"
    return qtTrId("melctl-command-database-summary");
}

QString DatabaseCommand::description() const
{
    //: CLI command description
    //% "Run, roll back, reset and refresh database migrations. Be careful, these commands can easily lead to data loss! Make sure that you have made a backup of your database beforehand."
    return qtTrId("melctl-command-database-description");
}

QString DatabaseCommand::dbConName() const
{
    return QStringLiteral("dbmigrations");
}

void DatabaseCommand::initMigrations()
{
    m_migrator = std::make_unique<Firfuorida::Migrator>(dbConName(), QStringLiteral("migrations"));
    new M0001_Create_Users_Table(m_migrator.get());
    new M0002_Create_Options_Table(m_migrator.get());
    new M0003_Create_Usersettings_Table(m_migrator.get());
    new M0004_Create_Domains_Table(m_migrator.get());
}

void DatabaseCommand::init()
{
    new DatabaseMigrationCommand(this);
    new DatabaseRollbackCommand(this);
    new DatabaseRefreshCommand(this);
    new DatabaseResetCommand(this);
}

#include "moc_databasecommand.cpp"
