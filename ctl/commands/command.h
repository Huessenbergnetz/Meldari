/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_COMMAND_H
#define MELDARICTL_COMMAND_H

#include <database.h>
#include <QCommandLineOption>

class QCommandLineParser;
class QTextStream;

class Command : public Database
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = nullptr);
    ~Command() override = default;

    virtual RC exec(QCommandLineParser *parser) = 0;

    virtual QString summary() const = 0;
    virtual QString description() const = 0;

    void showHelp() const;

protected:
    void showGlobalOptions(QTextStream *out) const;
    void showUsage(QTextStream *out) const;
    void showSubCommands(QTextStream *out) const;
    void showOptions(QTextStream *out) const;
    void setGlobalOptions(QCommandLineParser *parser);
    bool checkShowHelp(QCommandLineParser *parser);
    RC runSubCommand(const QString &command, QCommandLineParser *parser);
    RC showInvalidCommand(const QString &command) const;

    QList<QCommandLineOption> m_cliOptions;

private:
    int getLongestOptionName(const QList<QCommandLineOption> &options) const;
    void showOptions(QTextStream *out, const QList<QCommandLineOption> &options) const;
};

#endif // MELDARICTL_COMMAND_H
