/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterusersettings.h"

ControlCenterUsersettings::ControlCenterUsersettings(QObject *parent) : Controller{parent}
{

}

void ControlCenterUsersettings::index(Context *c)
{
    c->stash({
                 {QStringLiteral("template"), QStringLiteral("usersettings/index.html")},
                 {QStringLiteral("site_title"), c->translate("ControlCenterUsersettings", "My settings")}
             });
}

#include "moc_controlcenterusersettings.cpp"
