/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterusers.h"
#include "objects/user.h"
#include "objects/error.h"
#include "objects/simpleuser.h"
#include "utils.h"
#include "meldariconfig.h"
#include "meldariutils.h"

#include <Cutelyst/Plugins/Utils/Validator>
#include <Cutelyst/Plugins/Utils/ValidatorResult>
#include <Cutelyst/Plugins/Utils/validatorrequired.h>
#include <Cutelyst/Plugins/Utils/validatordatetime.h>
#include <Cutelyst/Plugins/Utils/validatorconfirmed.h>
#include <Cutelyst/Plugins/Utils/validatoremail.h>
#include <Cutelyst/Plugins/Utils/validatorin.h>
#include <Cutelyst/Plugins/Utils/validatorinteger.h>
#include <Cutelyst/Plugins/Utils/validatorpwquality.h>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

ControlCenterUsers::ControlCenterUsers(QObject *parent) : Controller{parent}
{

}

void ControlCenterUsers::index(Context *c)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"GET")) {
        return;
    }

    const User u = User::fromStash(c);

    c->stash({
                 {QStringLiteral("password_min_length"), MeldariConfig::pwMinLength()},
                 {QStringLiteral("user_types_translated"), User::typesTranslated(c)},
                 {QStringLiteral("user_type_options"), QVariant::fromValue<std::vector<OptionItem>>(User::typeOptions(c, User::Invalid, u.type()))},
                 {QStringLiteral("timezones"), QVariant::fromValue<std::vector<OptionItem>>(Utils::getTimezoneOptionsList(MeldariConfig::defTimezone()))},
                 {QStringLiteral("locales"), QVariant::fromValue<std::vector<OptionItem>>(MeldariConfig::supportedLocaleOptionItems(c, MeldariConfig::defLanguage()))},
                 {QStringLiteral("template"), QStringLiteral("users/index.html")}
             });
}

void ControlCenterUsers::list(Context *c)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"GET")) {
        return;
    }

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
    if (!MeldariUtils::checkAllowedMethod(c, u"POST")) {
        return;
    }

    const User u = User::fromStash(c);

    c->setStash(QStringLiteral("_userTypeValues"), User::typeValues(u.type()));

    static Validator v({
                           new ValidatorRequired(QStringLiteral("username")),
                           new ValidatorRequired(QStringLiteral("email")),
                           new ValidatorEmail(QStringLiteral("email")),
                           new ValidatorRequired(QStringLiteral("password")),
                           new ValidatorConfirmed(QStringLiteral("password")),
                           new ValidatorPwQuality(QStringLiteral("password"), MeldariConfig::pwQualityThreshold(), MeldariConfig::pwQualitySettingsFile(), QStringLiteral("username")),
                           new ValidatorRequired(QStringLiteral("type")),
                           new ValidatorIn(QStringLiteral("type"), QStringLiteral("_userTypeValues")),
                           new ValidatorInteger(QStringLiteral("type"), QMetaType::Int),
                           new ValidatorDateTime(QStringLiteral("validUntil"), QString(), "yyyy-MM-ddTHH:mm"),
                           new ValidatorRequired(QStringLiteral("timezone")),
                           new ValidatorIn(QStringLiteral("timezone"), Utils::getTimezoneList()),
                           new ValidatorRequired(QStringLiteral("language")),
                           new ValidatorIn(QStringLiteral("language"), MeldariConfig::supportedLocaleNames())
                       });

    const ValidatorResult vr = v.validate(c, Validator::BodyParamsOnly);

    if (vr) {
        Error e;
        const User u = User::add(c, e, vr.values());
        if (!e) {
            MeldariUtils::setJsonResponse(c, u.toJson(), c->translate("ControlCenterUsers", "User created"), c->translate("ControlCenterUsers", "Successfully created new user “%1” (ID: %2).").arg(u.username(), QString::number(u.id())));
        } else {
            e.toStash(c, true);
        }
    } else {
        c->res()->setStatus(400);
        c->res()->setJsonObjectBody(QJsonObject({{QStringLiteral("fielderrors"), vr.errorsJsonObject()}}));
    }
}

void ControlCenterUsers::get(Context *c, const QString &id)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"GET")) {
        return;
    }

    bool ok = false;
    const User::dbid_t _id = User::stringToDbId(id, &ok, c, true);
    if (!ok) {
        return;
    }

    Error error;
    const User user = User::get(c, error, _id);
    if (error.isError()) {
        error.toStash(c, true);
        return;
    }

    c->res()->setJsonObjectBody(user.toJson());
}

void ControlCenterUsers::remove(Context *c, const QString &id)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"POST")) {
        return;
    }

    bool ok = false;
    const User::dbid_t _id = User::stringToDbId(id, &ok, c, true);
    if (!ok) {
        return;
    }

    Error error;
    const User user = User::get(c, error, _id);
    if (error.isError()) {
        error.toStash(c, true);
        return;
    }

    c->setStash(QStringLiteral("_username"), user.username());

    static Validator v({
                           new ValidatorIn(QStringLiteral("username"), QStringLiteral("_username"))
                       });

    const ValidatorResult vr = v.validate(c, Validator::BodyParamsOnly);

    if (vr) {
        if (!user.remove(c, error)) {
            error.toStash(c, true);
            return;
        }

        MeldariUtils::setJsonResponse(c, user.toJson(), c->translate("ControlCenterUsers", "User removed"), c->translate("ControlCenterUsers", "Successfully removed user “%1” (ID: %2).").arg(user.username(), QString::number(user.id())));
    } else {
        c->res()->setStatus(400);
        c->res()->setJsonObjectBody(QJsonObject({{QStringLiteral("fielderrors"), vr.errorsJsonObject()}}));
    }
}

void ControlCenterUsers::edit(Context *c, const QString &id)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"POST")) {
        return;
    }

    bool ok = false;
    const User::dbid_t _id = User::stringToDbId(id, &ok, c, true);
    if (!ok) {
        return;
    }

    Error error;
    User user = User::get(c, error, _id);
    if (error.isError()) {
        error.toStash(c, true);
        return;
    }

    c->setStash(QStringLiteral("_userTypeValues"), User::typeValues(User::fromStash(c).type()));

    static Validator v({
                           new ValidatorRequired(QStringLiteral("email")),
                           new ValidatorEmail(QStringLiteral("email")),
                           new ValidatorConfirmed(QStringLiteral("newpassword")),
                           new ValidatorPwQuality(QStringLiteral("newpassword"), MeldariConfig::pwQualityThreshold(), MeldariConfig::pwQualitySettingsFile(), QStringLiteral("username")),
                           new ValidatorRequired(QStringLiteral("type")),
                           new ValidatorIn(QStringLiteral("type"), QStringLiteral("_userTypeValues")),
                           new ValidatorInteger(QStringLiteral("type"), QMetaType::Int),
                           new ValidatorDateTime(QStringLiteral("validUntil"), QString(), "yyyy-MM-ddTHH:mm"),
                           new ValidatorRequired(QStringLiteral("timezone")),
                           new ValidatorIn(QStringLiteral("timezone"), Utils::getTimezoneList()),
                           new ValidatorRequired(QStringLiteral("language")),
                           new ValidatorIn(QStringLiteral("language"), MeldariConfig::supportedLocaleNames())
                       });

    const ValidatorResult vr = v.validate(c, Validator::BodyParamsOnly);

    if (vr) {
        Error e;
        if (user.update(c, e, vr.values())) {
            MeldariUtils::setJsonResponse(c, user.toJson(), c->translate("ControlCenterUsers", "User updated"), c->translate("ControlCenterUsers", "Successfully updated the data for user “%1” (ID: %2).").arg(user.username(), QString::number(user.id())));
        } else {
            e.toStash(c, true);
        }
    } else {
        c->res()->setStatus(Response::BadRequest);
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
