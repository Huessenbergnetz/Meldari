/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterdomains.h"
#include "meldariutils.h"
#include "objects/domain.h"

ControlCenterDomains::ControlCenterDomains(QObject *parent)
    : Controller{parent}
{

}

void ControlCenterDomains::index(Context *c)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"GET")) {
        return;
    }

    c->stash({
                 {QStringLiteral("domain_stati_translated"), Domain::statiTranslated(c)},
                 {QStringLiteral("domain_status_options"), QVariant::fromValue<std::vector<OptionItem>>(Domain::statusOptions(c, Domain::Invalid))},
                 {QStringLiteral("template"), QStringLiteral("domains/index.html")}
             });
}

void ControlCenterDomains::list(Context *c)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"GET")) {
        return;
    }
}

void ControlCenterDomains::add(Context *c)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"POST")) {
        return;
    }
}

bool ControlCenterDomains::Auto(Context *c)
{
    c->setStash(QStringLiteral("site_title"), c->translate("ControlCenterDomains", "Domains"));
    return true;
}
