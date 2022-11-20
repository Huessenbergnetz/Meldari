/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controller.h"

#include "commands/command.h"
#include "commands/databasecommand.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTextStream>

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    setObjectName(u"controller");
}

CLI::RC Controller::exec()
{
    init();

    QCommandLineParser parser;

    parser.addOptions(m_globalOptions);
    parser.addPositionalArgument(QStringLiteral("command"), QString());

    parser.parse(QCoreApplication::arguments());

    const QStringList args = parser.positionalArguments();
    const QString command = args.empty() ? QLatin1String("") : args.first();

    auto com = findChild<Command *>(command, Qt::FindDirectChildrenOnly);

    if (com) {
        return com->exec(&parser);
    } else {
        if (parser.isSet(QStringLiteral("h")) || parser.isSet(QStringLiteral("help"))) {
            showHelp();
            return CLI::RC::OK;
        } else {
            QTextStream out(stdout, QIODeviceBase::WriteOnly);
            //: CLI error
            //% "\"%1\" is not a valid command. Use -h/--help to show a list of available commands."
            out << qtTrId("melctl-invalid-command").arg(command) << '\n';
            return CLI::RC::InvalidOption;
        }
    }
}

void Controller::init()
{
    m_globalOptions.append(QCommandLineOption(QStringList({QStringLiteral("h"), QStringLiteral("help")}),
                                              //: CLI option description
                                              //% "Displays help on commandline options and commands."
                                              qtTrId("melctl-opt-global-help-desc")));

    const QString iniFileDefVal = QStringLiteral(MELDARI_CONFFILE);
    m_globalOptions.append(QCommandLineOption(QStringList({QStringLiteral("i"), QStringLiteral("ini")}),
                                              //: CLI option description, %1 will be the default value
                                              //% "Path to configuration ini file. Default: %1"
                                              qtTrId("melctl-opt-global-inifile-desc").arg(iniFileDefVal),
                                              //: CLI option value name
                                              //% "ini file"
                                              qtTrId("melctl-opt-global-inifile-val"),
                                              iniFileDefVal));
    m_globalOptions.append(QCommandLineOption(QStringList({QStringLiteral("q"), QStringLiteral("quiet")}),
                                              //: CLI option description
                                              //% "Be quiet and print less output."
                                              qtTrId("melctl-opt-global-quiet-desc")));

    new DatabaseCommand(this);
}

void Controller::showHelp() const
{
    const QStringList args = QCoreApplication::arguments();
    const QString exeName = args.first();

    QTextStream out(stdout, QIODeviceBase::WriteOnly);

    out << qtTrId("melctl-help-usage") << ' ' << exeName << ' ';
    out << qtTrId("melctl-help-usage-global-options") << ' ';
    out << qtTrId("melctl-help-usage-command") << '\n';
    out << '\n';

    //: General description for gikwimictl
    //% "Meldari is a security reporting endpoint based on Cutelyst.\nThis command line client helps to manage several aspects of Meldari from the command line."
    out << qtTrId("melctl-help-description") << '\n';
    out << '\n';

    out << qtTrId("melctl-help-header-global-options") << '\n';
    int longestOptionNameAndValue = 0;
    for (const QCommandLineOption &opt : qAsConst(m_globalOptions)) {
        const QStringList names = opt.names();
        QString longName;
        for (const QString &name : names) {
            if (name.length() > 1) {
                longName = name;
            }
        }
        longestOptionNameAndValue = std::max(longestOptionNameAndValue, static_cast<int>(longName.length() + opt.valueName().length()));
    }
    for (const QCommandLineOption &opt : qAsConst(m_globalOptions)) {
        QString shortName;
        QString longName;
        const QStringList names = opt.names();
        for (const QString &name : names) {
            if (name.length() == 1) {
                shortName = name;
            } else {
                longName = name;
            }
        }
        if (shortName.isEmpty()) {
            shortName = QStringLiteral("    ");
        } else {
            shortName.append(QLatin1String(", ")).prepend(QLatin1Char('-'));
        }

        QString longNameAndValue;
        if (opt.valueName().isEmpty()) {
            longNameAndValue = QLatin1String("--") + longName;
        } else {
            longNameAndValue = QLatin1String("--") + longName + QChar(QChar::Space) + QLatin1Char('<') + opt.valueName() + QLatin1Char('>');
        }

        out << "  " << shortName << qSetFieldWidth(longestOptionNameAndValue + 10) << Qt::left << longNameAndValue << qSetFieldWidth(0) << opt.description() << '\n';
    }
    out << '\n';

    out << qtTrId("melctl-help-header-commands") << '\n';

    const QList<Command*> coms = findChildren<Command *>(QString(), Qt::FindDirectChildrenOnly);
    int maxCommandNameLength = 0;
    for (const auto com : coms) {
        maxCommandNameLength = std::max(maxCommandNameLength, static_cast<int>(com->objectName().length()));
    }
    for (const auto com : coms) {
        out << "  " << qSetFieldWidth(maxCommandNameLength + 4) << Qt::left << com->objectName() << qSetFieldWidth(0) << " " << com->summary() << '\n';
    }
}

QList<QCommandLineOption> Controller::globalOptions() const
{
    return m_globalOptions;
}

#include "moc_controller.cpp"
