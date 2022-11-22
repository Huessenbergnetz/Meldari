/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_USERCOMMAND_H
#define MELDARICTL_USERCOMMAND_H

#include "command.h"

class UserCommand final : public Command
{
    Q_OBJECT
    Q_DISABLE_COPY(UserCommand)
public:
    explicit UserCommand(QObject *parent = nullptr);
    ~UserCommand() final;

    CLI::RC exec(QCommandLineParser *parser) final;

    QString summary() const final;

    QString description() const final;

private:
    void init();
};

#endif // MELDARICTL_USERCOMMAND_H
