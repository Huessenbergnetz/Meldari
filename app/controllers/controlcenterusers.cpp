/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterusers.h"
#include "objects/user.h"
#include "objects/error.h"
#include "objects/simpleuser.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

ControlCenterUsers::ControlCenterUsers(QObject *parent) : Controller{parent}
{

}

void ControlCenterUsers::index(Context *c)
{
    c->stash({
                 {QStringLiteral("template"), QStringLiteral("users/index.html")}
             });
}

void ControlCenterUsers::list(Context *c)
{
    auto params = c->req()->queryParams();

    User::Type minType = User::typeStringToEnum(params.value(QStringLiteral("minType")));

    Error e;
    const QJsonArray users = params.value(QStringLiteral("details")) == u"full" ? User::listJson(c, e, minType) : SimpleUser::listJson(c, e, minType);
    if (e.type() != Error::NoError) {
        e.toStash(c, true);
        return;
    }

    c->res()->setJsonArrayBody(users);
}

bool ControlCenterUsers::Auto(Context *c)
{
    const User currentUser = User::fromStash(c);
    if (!currentUser.isAdmin()) {
        Error::toStash(c, Error::AuthorizationError, c->translate("ControlCenterUsers", "You do not have the required permissions."), true);
        return false;
    }

    c->setStash(QStringLiteral("site_title"), c->translate("ControlCenterUsers", "Users"));
    return true;
}

#include "moc_controlcenterusers.cpp"
