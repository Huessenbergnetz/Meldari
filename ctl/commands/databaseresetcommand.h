/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_DATABASERESETCOMMAND_H
#define MELDARICTL_DATABASERESETCOMMAND_H

#include "databasecommand.h"
#include <QObject>

class DatabaseResetCommand final : public DatabaseCommand
{
    Q_OBJECT
public:
    explicit DatabaseResetCommand(QObject *parent = nullptr);
    ~DatabaseResetCommand() final;

    CLI::RC exec(QCommandLineParser *parser) final;

    QString summary() const final;

    QString description() const final;
};

#endif // MELDARICTL_DATABASERESETCOMMAND_H
