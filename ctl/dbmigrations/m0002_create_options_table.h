/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef M0002_CREATE_OPTIONS_TABLE_H
#define M0002_CREATE_OPTIONS_TABLE_H

#include <Firfuorida/Migration>

class M0002_Create_Options_Table final : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M0002_Create_Options_Table)
public:
    explicit M0002_Create_Options_Table(Firfuorida::Migrator *parent);
    ~M0002_Create_Options_Table() final;

    void up() final;
    void down() final;
};

#endif // M0002_CREATE_OPTIONS_TABLE_H

