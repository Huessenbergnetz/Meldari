/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_DATABASEMIGRATIONCOMMAND_H
#define MELDARICTL_DATABASEMIGRATIONCOMMAND_H

#include "databasecommand.h"
#include <QObject>

class DatabaseMigrationCommand final : public DatabaseCommand
{
    Q_OBJECT
public:
    explicit DatabaseMigrationCommand(QObject *parent = nullptr);
    ~DatabaseMigrationCommand() final = default;

    CLI::RC exec(QCommandLineParser *parser) final;

    QString summary() const final;

    QString description() const final;
};

#endif // MELDARICTL_DATABASEMIGRATIONCOMMAND_H
