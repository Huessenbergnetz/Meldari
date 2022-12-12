/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "m0004_create_domains_table.h"

M0004_Create_Domains_Table::M0004_Create_Domains_Table(Firfuorida::Migrator *parent) :
    Firfuorida::Migration{parent}
{

}

M0004_Create_Domains_Table::~M0004_Create_Domains_Table()
{

}

void M0004_Create_Domains_Table::up()
{
    auto t = create(QStringLiteral("domains"));
    t->increments();
    t->integer(QStringLiteral("owner_id"))->unSigned();
    t->varChar(QStringLiteral("name"))->unique();
    t->tinyInteger(QStringLiteral("status"))->defaultValue(0);
    t->dateTime(QStringLiteral("created_at"));
    t->dateTime(QStringLiteral("updated_at"));
    t->dateTime(QStringLiteral("valid_until"))->nullable();
    t->dateTime(QStringLiteral("locked_at"))->nullable();
    t->integer(QStringLiteral("locked_by"))->unSigned()->defaultValue(0)->nullable();
    t->foreignKey(QStringLiteral("owner_id"), QStringLiteral("users"), QStringLiteral("id"), QStringLiteral("fk_domain_owner"))->onDelete(QStringLiteral("CASCADE"))->onUpdate(QStringLiteral("CASCADE"));

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

void M0004_Create_Domains_Table::down()
{
    drop(QStringLiteral("domains"));
}

#include "moc_m0004_create_domains_table.cpp"

