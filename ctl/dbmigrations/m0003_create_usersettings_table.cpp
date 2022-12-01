/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "m0003_create_usersettings_table.h"

M0003_Create_Usersettings_Table::M0003_Create_Usersettings_Table(Firfuorida::Migrator *parent) :
    Firfuorida::Migration{parent}
{

}

M0003_Create_Usersettings_Table::~M0003_Create_Usersettings_Table()
{

}

void M0003_Create_Usersettings_Table::up()
{
    auto t = create(QStringLiteral("usersettings"));
    t->bigIncrements();
    t->integer(QStringLiteral("user_id"))->unSigned();
    t->varChar(QStringLiteral("name"), 63);
    t->text(QStringLiteral("value"));
    t->uniqueKey(QStringList({QStringLiteral("user_id"), QStringLiteral("name")}), QStringLiteral("user_id_name_idx"));
    t->foreignKey(QStringLiteral("user_id"), QStringLiteral("users"), QStringLiteral("id"), QStringLiteral("usersettings_user_id_idx"))->onDelete(QStringLiteral("CASCADE"))->onUpdate(QStringLiteral("CASCADE"));

    switch(dbType()) {
    case Firfuorida::Migrator::MariaDB:
    case Firfuorida::Migrator::MySQL:
        t->setEngine(QStringLiteral("InnoDB"));
        t->setCharset(QStringLiteral("utf8mb4"));
        t->setCollation(QStringLiteral("utf8mb4_unicode_ci"));
        break;
    case Firfuorida::Migrator::DB2:
    case Firfuorida::Migrator::InterBase:
    case Firfuorida::Migrator::ODBC:
    case Firfuorida::Migrator::OCI:
    case Firfuorida::Migrator::PSQL:
    case Firfuorida::Migrator::SQLite:
    case Firfuorida::Migrator::Invalid:
        break;
    }
}

void M0003_Create_Usersettings_Table::down()
{
    drop(QStringLiteral("usersettings"));
}

#include "moc_m0003_create_usersettings_table.cpp"

