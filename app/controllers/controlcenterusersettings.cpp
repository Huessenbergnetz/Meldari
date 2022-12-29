/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterusersettings.h"
#include "objects/user.h"
#include "objects/error.h"
#include "utils.h"
#include "meldariconfig.h"
#include "meldariutils.h"
#include "validators/melvalidatorpwcheck.h"

#include <Cutelyst/Plugins/Session/Session>
#include <Cutelyst/Plugins/Authentication/authentication.h>
#include <Cutelyst/Plugins/Utils/Validator>
#include <Cutelyst/Plugins/Utils/ValidatorResult>
#include <Cutelyst/Plugins/Utils/validatorrequired.h>
#include <Cutelyst/Plugins/Utils/validatorin.h>
#include <Cutelyst/Plugins/Utils/validatorconfirmed.h>
#include <Cutelyst/Plugins/Utils/validatorrequired.h>
#include <Cutelyst/Plugins/Utils/validatorpwquality.h>

#include <QTimeZone>

ControlCenterUsersettings::ControlCenterUsersettings(QObject *parent) : Controller{parent}
{

}

void ControlCenterUsersettings::index(Context *c)
{
    c->stash({
                 {QStringLiteral("password_min_length"), MeldariConfig::pwMinLength()},
                 {QStringLiteral("timezones"), QVariant::fromValue<std::vector<OptionItem>>(Utils::getTimezoneOptionsList(Session::value(c, QStringLiteral("tz")).value<QTimeZone>().id()))},
                 {QStringLiteral("locales"), QVariant::fromValue<std::vector<OptionItem>>(MeldariConfig::supportedLocaleOptionItems(c, c->locale()))},
                 {QStringLiteral("template"), QStringLiteral("usersettings/index.html")},
                 {QStringLiteral("site_title"), c->translate("ControlCenterUsersettings", "My settings")}
             });
}

void ControlCenterUsersettings::update(Context *c)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"POST")) {
        return;
    }

    User u = User::fromStash(c);
    c->setStash(QStringLiteral("_username"), u.username());

    static Validator v({
                           new ValidatorRequired(QStringLiteral("timezone")),
                           new ValidatorIn(QStringLiteral("timezone"), Utils::getTimezoneList()),
                           new ValidatorRequired(QStringLiteral("language")),
                           new ValidatorIn(QStringLiteral("language"), MeldariConfig::supportedLocaleNames()),
                           new MelValidatorPwCheck(QStringLiteral("password"), QStringLiteral("_username")),
                           new ValidatorConfirmed(QStringLiteral("newpassword")),
                           new ValidatorPwQuality(QStringLiteral("newpassword"), MeldariConfig::pwQualityThreshold(), MeldariConfig::pwQualitySettingsFile(), QStringLiteral("_username"), QStringLiteral("password")),
                       });

    const ValidatorResult vr = v.validate(c, Validator::BodyParamsOnly);

    if (vr) {
        Error e;
        QVariantHash values = vr.values();
        // put this values into the hash as they are required
        // but should not be settable by the user that updates
        // his own data
        values.insert(QStringLiteral("email"), u.email());
        values.insert(QStringLiteral("validUntil"), u.validUntil().toTimeZone(Cutelyst::Session::value(c, QStringLiteral("tz")).value<QTimeZone>()));
        values.insert(QStringLiteral("type"), static_cast<int>(u.type()));
        if (u.update(c, e, values)) {
            QLocale ln(u.settings().value(QStringLiteral("language")).toString());
            Cutelyst::Session::setValue(c, QStringLiteral("lang"), QVariant::fromValue<QLocale>(ln));
            QTimeZone tz(u.settings().value(QStringLiteral("timezone")).toString().toLatin1());
            Cutelyst::Session::setValue(c, QStringLiteral("tz"), QVariant::fromValue<QTimeZone>(tz));

            const QString newPassword = values.value(QStringLiteral("newpassword")).toString();
            if (!newPassword.isEmpty()) {
                ParamsMultiMap userParams;
                userParams.insert(QStringLiteral("username"), u.username());
                userParams.insert(QStringLiteral("password"), newPassword);
                Cutelyst::Authentication::authenticate(c, userParams, QStringLiteral("users"));
            }

            u.toStash(c);

            MeldariUtils::setJsonResponse(c, u.toJson(), c->translate("ControlCenterUsersettings", "Settings saved"));
        } else {
            e.toStash(c, true);
        }
    } else {
        c->res()->setStatus(Response::BadRequest);
        c->res()->setJsonObjectBody(QJsonObject({{QStringLiteral("fielderrors"), vr.errorsJsonObject()}}));
    }
}

#include "moc_controlcenterusersettings.cpp"
