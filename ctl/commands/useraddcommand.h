/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_USERADDCOMMAND_H
#define MELDARICTL_USERADDCOMMAND_H

#include "command.h"

class UserAddCommand final : public Command
{
    Q_OBJECT
    Q_DISABLE_COPY(UserAddCommand)
public:
    explicit UserAddCommand(QObject *parent = nullptr);
    ~UserAddCommand() final;

    CLI::RC exec(QCommandLineParser *parser) final;

    QString summary() const final;

    QString description() const final;

private:
    void init();
};

#endif // MELDARICTL_USERADDCOMMAND_H
