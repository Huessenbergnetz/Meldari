/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_DATABASEREFRESHCOMMAND_H
#define MELDARICTL_DATABASEREFRESHCOMMAND_H

#include "databasecommand.h"
#include <QObject>

class DatabaseRefreshCommand final : public DatabaseCommand
{
    Q_OBJECT
public:
    explicit DatabaseRefreshCommand(QObject *parent = nullptr);
    ~DatabaseRefreshCommand() final;

    CLI::RC exec(QCommandLineParser *parser) final;

    QString summary() const final;

    QString description() const final;

private:
    void init();
};

#endif // MELDARICTL_DATABASEREFRESHCOMMAND_H
