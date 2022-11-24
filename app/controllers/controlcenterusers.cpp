/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterusers.h"
#include "objects/user.h"
#include "objects/error.h"

ControlCenterUsers::ControlCenterUsers(QObject *parent) : Controller{parent}
{

}

void ControlCenterUsers::index(Context *c)
{
    const User currentUser = User::fromStash(c);
    if (!currentUser.isAdmin()) {
        Error::toStash(c, Error::AuthorizationError, c->translate("ControlCenterUsers", "You do not have the necessary permissions."), true);
        return;
    }

    c->stash({
                 {QStringLiteral("template"), QStringLiteral("users/index.html")}
             });
}

bool ControlCenterUsers::Auto(Context *c)
{
    c->setStash(QStringLiteral("site_title"), c->translate("ControlCenterUsers", "Users"));
    return true;
}
