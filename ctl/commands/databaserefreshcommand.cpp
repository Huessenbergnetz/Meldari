/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "databaserefreshcommand.h"

#include <Firfuorida/Migrator>

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

DatabaseRefreshCommand::DatabaseRefreshCommand(QObject *parent)
    : DatabaseCommand{parent}
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
    setObjectName(u"refresh");
#else
    setObjectName(QStringLiteral("refresh"));
#endif
}

DatabaseRefreshCommand::~DatabaseRefreshCommand() = default;

void DatabaseRefreshCommand::init()
{
    const QString defVal = QStringLiteral("0");
    m_cliOptions.append(QCommandLineOption(QStringList({QStringLiteral("s"), QStringLiteral("steps")}),
                                           //: CLI option description, %1 will be replaced by the default value
                                           //% "Number of migration steps to refresh. If 0, all migrations will be refreshed. Default: %1."
                                           qtTrId("melctl-opt-database-refresh-steps-desc").arg(defVal),
                                           //: CLI option value name
                                           //% "steps"
                                           qtTrId("melctl-opt-database-refresh-steps-value"),
                                           defVal));
}

CLI::RC DatabaseRefreshCommand::exec(QCommandLineParser *parser)
{
    CLI::RC rc = RC::OK;

    init();

    parser->addOptions(m_cliOptions);
    parser->parse(QCoreApplication::arguments());

    if (checkShowHelp(parser)) {
        return rc;
    }

    const QString stepsString = parser->value(QStringLiteral("steps"));
    bool ok = false;
    uint steps = stepsString.toUInt(&ok);

    if (!ok) {
        //% "Can not perform database migration refresh. “%1“ is not a valid integer value for steps."
        rc = inputError(qtTrId("melctl-error-invalid-refresh-steps-integer").arg(stepsString));
        return rc;
    }

    setGlobalOptions(parser);

    rc = openDb(dbConName());
    if (rc != RC::OK) {
        return rc;
    }

    initMigrations();

    if (!m_migrator->refresh(steps)) {
        return dbError(m_migrator->lastError().text());
    }

    return rc;
}

QString DatabaseRefreshCommand::summary() const
{
    //: CLI command summary
    //% "Refresh database migrations"
    return qtTrId("melctl-command-database-refresh-summary");
}

QString DatabaseRefreshCommand::description() const
{
    //: CLI command description
    //% "Refreshes the specified number of migration steps. A refresh will roll back the specified number of migrations and will rerun them afterwards. Note that this will result in data loss. This command is for development purposes only."
    return qtTrId("melctl-command-database-refresh-description");
}

#include "moc_databaserefreshcommand.cpp"
