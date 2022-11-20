/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef M0001_CREATE_USERS_TABLE_H
#define M0001_CREATE_USERS_TABLE_H

#include <Firfuorida/Migration>

class M0001_Create_Users_Table final : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M0001_Create_Users_Table)
public:
    explicit M0001_Create_Users_Table(Firfuorida::Migrator *parent);
    ~M0001_Create_Users_Table() final;

    void up() final;
    void down() final;
};

#endif // M0001_CREATE_USERS_TABLE_H

