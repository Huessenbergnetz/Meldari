/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_CONTROLCENTERUSERS_H
#define MELDARI_CONTROLCENTERUSERS_H

#include <Cutelyst/Controller>

using namespace Cutelyst;

class ControlCenterUsers final : public Controller
{
    Q_OBJECT
    C_NAMESPACE("cc/users")
public:
    explicit ControlCenterUsers(QObject *parent = nullptr);
    ~ControlCenterUsers() final = default;

    C_ATTR(index, :Path :Args(0))
    void index(Context *c);

    C_ATTR(list, :Local :Args(0))
    void list(Context *c);

    C_ATTR(add, :Local :Args(0))
    void add(Context *c);

    C_ATTR(get, :Local :Args(1))
    void get(Context *c, const QString &id);

    C_ATTR(remove, :Local :Args(1))
    void remove(Context *c, const QString &id);

    C_ATTR(edit, :Local :Args(1))
    void edit(Context *c, const QString &id);

private:
    C_ATTR(Auto, :Private)
    bool Auto(Context *c);
};

#endif // MELDARI_CONTROLCENTERUSERS_H
