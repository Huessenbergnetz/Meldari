/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef M0003_CREATE_USERSETTINGS_TABLE_H
#define M0003_CREATE_USERSETTINGS_TABLE_H

#include <Firfuorida/Migration>

class M0003_Create_Usersettings_Table final : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M0003_Create_Usersettings_Table)
public:
    explicit M0003_Create_Usersettings_Table(Firfuorida::Migrator *parent);
    ~M0003_Create_Usersettings_Table() final;

    void up() final;
    void down() final;
};

#endif // M0003_CREATE_USERSETTINGS_TABLE_H

