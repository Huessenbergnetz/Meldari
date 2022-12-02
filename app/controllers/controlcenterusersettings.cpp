/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterusersettings.h"
#include "objects/user.h"
#include "objects/error.h"
#include "utils.h"
#include "meldariconfig.h"

#include <Cutelyst/Plugins/Session/Session>
#include <Cutelyst/Plugins/Utils/Validator>
#include <Cutelyst/Plugins/Utils/ValidatorResult>
#include <Cutelyst/Plugins/Utils/validatorrequired.h>
#include <Cutelyst/Plugins/Utils/validatorin.h>
#include <Cutelyst/Plugins/Utils/validatorconfirmed.h>
#include <Cutelyst/Plugins/Utils/validatorrequired.h>

#include <QTimeZone>

ControlCenterUsersettings::ControlCenterUsersettings(QObject *parent) : Controller{parent}
{

}

void ControlCenterUsersettings::index(Context *c)
{
    c->stash({
                 {QStringLiteral("timezones"), QVariant::fromValue<std::vector<OptionItem>>(Utils::getTimezoneOptionsList(Session::value(c, QStringLiteral("tz")).value<QTimeZone>().id()))},
                 {QStringLiteral("locales"), QVariant::fromValue<std::vector<OptionItem>>(MeldariConfig::supportedLocaleOptionItems(c, c->locale()))},
                 {QStringLiteral("template"), QStringLiteral("usersettings/index.html")},
                 {QStringLiteral("site_title"), c->translate("ControlCenterUsersettings", "My settings")}
             });
}

void ControlCenterUsersettings::update(Context *c)
{
    if (!c->req()->isPost()) {
        c->res()->setStatus(Response::MethodNotAllowed);
        c->res()->setHeader(QStringLiteral("Allow"), QStringLiteral("POST"));
        Error e(Response::MethodNotAllowed, c->translate("ControlCenterUsers", "This path only accepts POST requests."));
        c->res()->setJsonObjectBody(e.toJson(c));
        return;
    }

    static Validator v({
                           new ValidatorRequired(QStringLiteral("timezone")),
                           new ValidatorIn(QStringLiteral("timezone"), Utils::getTimezoneList()),
                           new ValidatorRequired(QStringLiteral("language")),
                           new ValidatorIn(QStringLiteral("language"), MeldariConfig::supportedLocaleNames()),
                           new ValidatorConfirmed(QStringLiteral("newpassword"))
                       });

    const ValidatorResult vr = v.validate(c, Validator::BodyParamsOnly);

    if (vr) {
        Error e;
        User u = User::fromStash(c);
        QVariantHash values = vr.values();
        values.insert(QStringLiteral("password"), c->req()->bodyParam(QStringLiteral("password")));
        if (u.update(c, e, values)) {
            Utils::setJsonResponse(c, u.toJson(), c->translate("ControlCenterUsersettings", "Settings saved"));
            u.toStash(c);
        } else {
            e.toStash(c, true);
        }
    } else {
        c->res()->setStatus(Response::BadRequest);
        c->res()->setJsonObjectBody(QJsonObject({{QStringLiteral("fielderrors"), vr.errorsJsonObject()}}));
    }
}

#include "moc_controlcenterusersettings.cpp"
