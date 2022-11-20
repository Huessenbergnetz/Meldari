/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_DATABASE_H
#define MELDARICTL_DATABASE_H

#include "configuration.h"

class Database : public Configuration
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database() override = default;

protected:
    RC openDb(const QString &connectionName);
};

#endif // MELDARICTL_DATABASE_H
