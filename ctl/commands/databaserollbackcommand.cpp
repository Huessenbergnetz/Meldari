/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "databaserollbackcommand.h"

#include <Firfuorida/Migrator>

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

DatabaseRollbackCommand::DatabaseRollbackCommand(QObject *parent)
    : DatabaseCommand{parent}
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
    setObjectName(u"rollback");
#else
    setObjectName(QStringLiteral("rollback"))
#endif
}

DatabaseRollbackCommand::~DatabaseRollbackCommand() = default;

void DatabaseRollbackCommand::init()
{
    const QString defVal = QStringLiteral("1");
    m_cliOptions.append(QCommandLineOption({QStringLiteral("s"), QStringLiteral("steps")},
                                           //: CLI option description, %1 will be replaced by the default value
                                           //% "Number of migration steps to roll back. Default: %1."
                                           qtTrId("melctl-opt-database-rollback-steps-desc").arg(defVal),
                                           //: CLI option value name
                                           //% "steps"
                                           qtTrId("melctl-opt-database-rollback-steps-value"),
                                           defVal));
}

CLI::RC DatabaseRollbackCommand::exec(QCommandLineParser *parser)
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
        //% "Can not perform database migration rollback. “%1“ is not a valid integer value for steps."
        rc = inputError(qtTrId("melctl-error-invalid-rollback-steps-integer").arg(stepsString));
        return rc;
    }

    setGlobalOptions(parser);

    rc = openDb(dbConName());
    if (rc != RC::OK) {
        return rc;
    }

    initMigrations();

    if (!m_migrator->rollback(steps)) {
        return dbError(m_migrator->lastError().text());
    }

    return rc;
}

QString DatabaseRollbackCommand::summary() const
{
    //: CLI command summary
    //% "Roll back database migrations"
    return qtTrId("melctl-command-database-rollback-summary");
}

QString DatabaseRollbackCommand::description() const
{
    //: CLI command description
    //% "Rolls back the specified number of migration steps. Note that this will result in data loss. This command is for development purposes only."
    return qtTrId("melctl-command-database-rollback-description");
}

#include "moc_databaserollbackcommand.cpp"
