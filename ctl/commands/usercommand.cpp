/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "usercommand.h"
#include "useraddcommand.h"

#include <QCoreApplication>
#include <QCommandLineParser>

UserCommand::UserCommand(QObject *parent)
    : Command{parent}
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
    setObjectName(u"user");
#else
    setObjectName(QStringLiteral("user"))
#endif
}

UserCommand::~UserCommand() = default;

void UserCommand::init()
{
    new UserAddCommand(this);
}

CLI::RC UserCommand::exec(QCommandLineParser *parser)
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

QString UserCommand::summary() const
{
    //: CLI command summary
    //% "Manage users"
    return qtTrId("melctl-command-user-summary");
}

QString UserCommand::description() const
{
    //: CLI command description
    //% "Add, edit, delete and list users."
    return qtTrId("melctl-command-user-description");
}

#include "moc_usercommand.cpp"
