/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_CONTROLCENTERUSERSETTINGS_H
#define MELDARI_CONTROLCENTERUSERSETTINGS_H

#include <Cutelyst/Controller>

using namespace Cutelyst;

class ControlCenterUsersettings final : public Controller
{
    Q_OBJECT
    C_NAMESPACE("cc/usersettings")
public:
    explicit ControlCenterUsersettings(QObject *parent = nullptr);
    ~ControlCenterUsersettings() final = default;

    C_ATTR(index, :Path :Args(0))
    void index(Context *c);
};

#endif // MELDARI_CONTROLCENTERUSERSETTINGS_H
