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

private:
    C_ATTR(Auto, :Private)
    bool Auto(Context *c);
};

#endif // MELDARI_CONTROLCENTERUSERS_H
