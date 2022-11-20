/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_DATABASECOMMAND_H
#define MELDARICTL_DATABASECOMMAND_H

#include "command.h"

#include <memory>

namespace Firfuorida {
class Migrator;
}

class DatabaseCommand : public Command
{
    Q_OBJECT
public:
    explicit DatabaseCommand(QObject *parent = nullptr);
    ~DatabaseCommand() override;

    void init() override;

    CLI::RC exec(QCommandLineParser *parser) override;

    QString summary() const override;

    QString description() const override;

protected:
    QString dbConName() const;
    void initMigrations();
    std::unique_ptr<Firfuorida::Migrator> m_migrator;
};

#endif // MELDARICTL_DATABASECOMMAND_H
