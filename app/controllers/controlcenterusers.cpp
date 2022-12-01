/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterusers.h"
#include "objects/user.h"
#include "objects/error.h"
#include "objects/simpleuser.h"

#include <Cutelyst/Plugins/Utils/Validator>
#include <Cutelyst/Plugins/Utils/ValidatorResult>
#include <Cutelyst/Plugins/Utils/validatorrequired.h>
#include <Cutelyst/Plugins/Utils/validatordatetime.h>
#include <Cutelyst/Plugins/Utils/validatorconfirmed.h>
#include <Cutelyst/Plugins/Utils/validatoremail.h>
#include <Cutelyst/Plugins/Utils/validatorin.h>
#include <Cutelyst/Plugins/Utils/validatorinteger.h>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

ControlCenterUsers::ControlCenterUsers(QObject *parent) : Controller{parent}
{

}

void ControlCenterUsers::index(Context *c)
{
    const QJsonDocument typesTranslated(User::typesTranslated(c));
    const QString typesTranslatedJson = QString::fromUtf8(typesTranslated.toJson(QJsonDocument::Compact));
    const std::vector<OptionItem> userTypeOptions = User::typeOptions(c);

    c->stash({
                 {QStringLiteral("user_types_translated"), typesTranslatedJson},
                 {QStringLiteral("user_type_options"), QVariant::fromValue<std::vector<OptionItem>>(userTypeOptions)},
                 {QStringLiteral("template"), QStringLiteral("users/index.html")}
             });
}

void ControlCenterUsers::list(Context *c)
{
    auto params = c->req()->queryParams();

    User::Type minType = User::typeStringToEnum(params.value(QStringLiteral("minType")));

    Error e;
    const QJsonArray users = params.value(QStringLiteral("details")) == u"full" ? User::listJson(c, e, minType) : SimpleUser::listJson(c, e, minType);
    if (e) {
        e.toStash(c, true);
        return;
    }

    c->res()->setJsonArrayBody(users);
}

void ControlCenterUsers::add(Context *c)
{
    if (!c->req()->isPost()) {
        c->res()->setStatus(Response::MethodNotAllowed);
        c->res()->setHeader(QStringLiteral("Allow"), QStringLiteral("POST"));
        Error e(Response::MethodNotAllowed, c->translate("ControlCenterUsers", "This path only accepts POST requests."));
        c->res()->setJsonObjectBody(e.toJson(c));
        return;
    }

    static Validator v({
                           new ValidatorRequired(QStringLiteral("username")),
                           new ValidatorRequired(QStringLiteral("email")),
                           new ValidatorEmail(QStringLiteral("email")),
                           new ValidatorRequired(QStringLiteral("password")),
                           new ValidatorConfirmed(QStringLiteral("password")),
                           new ValidatorRequired(QStringLiteral("type")),
                           new ValidatorIn(QStringLiteral("type"), User::typeValues()),
                           new ValidatorInteger(QStringLiteral("type"), QMetaType::Int),
                           new ValidatorDateTime(QStringLiteral("validUntil"), QString(), "yyyy-MM-ddTHH:mm")
                       });

    const ValidatorResult vr = v.validate(c, Validator::BodyParamsOnly);

    if (vr) {
        Error e;
        const User u = User::add(c, e, vr.values());
        if (!e) {
            c->res()->setJsonObjectBody(u.toJson());;
        } else {
            e.toStash(c, true);
        }
    } else {
        c->res()->setStatus(400);
        c->res()->setJsonObjectBody(QJsonObject({{QStringLiteral("fielderrors"), vr.errorsJsonObject()}}));
    }
}

bool ControlCenterUsers::Auto(Context *c)
{
    const User currentUser = User::fromStash(c);
    if (!currentUser.isAdmin()) {
        Error::toStash(c, Response::Forbidden, c->translate("ControlCenterUsers", "You do not have the required permissions."), true);
        return false;
    }

    c->setStash(QStringLiteral("site_title"), c->translate("ControlCenterUsers", "Users"));
    return true;
}

#include "moc_controlcenterusers.cpp"
