/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef M0004_CREATE_DOMAINS_TABLE_H
#define M0004_CREATE_DOMAINS_TABLE_H

#include <Firfuorida/Migration>

class M0004_Create_Domains_Table final : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M0004_Create_Domains_Table)
public:
    explicit M0004_Create_Domains_Table(Firfuorida::Migrator *parent);
    ~M0004_Create_Domains_Table() final;

    void up() final;
    void down() final;
};

#endif // M0004_CREATE_DOMAINS_TABLE_H

