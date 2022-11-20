/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <Cutelyst/Controller>

using namespace Cutelyst;

class ControlCenter final : public Controller
{
    Q_OBJECT
    C_NAMESPACE("cc")
public:
    explicit ControlCenter(QObject *parent = nullptr);
    ~ControlCenter() final = default;

    C_ATTR(index, :Path :Args(0))
    void index(Context *c);

    C_ATTR(login, :Local :Args(0))
    void login(Context *c);

    C_ATTR(logout, :Local :Args(0))
    void logout(Context *c);

private:
    C_ATTR(Auto, :Private)
    bool Auto(Context *c);
};

#endif // CONTROLCENTER_H
