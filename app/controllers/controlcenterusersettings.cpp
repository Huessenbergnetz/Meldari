/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterusersettings.h"
#include "objects/user.h"
#include "utils.h"
#include "meldariconfig.h"

#include <Cutelyst/Plugins/Session/Session>

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

#include "moc_controlcenterusersettings.cpp"
