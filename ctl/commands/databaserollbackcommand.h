/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_DATABASEROLLBACKCOMMAND_H
#define MELDARICTL_DATABASEROLLBACKCOMMAND_H

#include "databasecommand.h"
#include <QObject>

class DatabaseRollbackCommand final : public DatabaseCommand
{
    Q_OBJECT
public:
    explicit DatabaseRollbackCommand(QObject *parent = nullptr);
    ~DatabaseRollbackCommand() final;

    CLI::RC exec(QCommandLineParser *parser) final;

    QString summary() const final;

    QString description() const final;

private:
    void init();
};

#endif // MELDARICTL_DATABASEROLLBACKCOMMAND_H
