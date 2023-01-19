/*
 * SPDX-FileCopyrightText: (C) 2022-2023 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenterdomains.h"
#include "meldariutils.h"
#include "objects/domain.h"
#include "objects/error.h"

#include <Cutelyst/Plugins/Utils/Validator>
#include <Cutelyst/Plugins/Utils/ValidatorResult>
#include <Cutelyst/Plugins/Utils/validatorrequired.h>
#include <Cutelyst/Plugins/Utils/validatordomain.h>
#include <Cutelyst/Plugins/Utils/validatorin.h>
#include <Cutelyst/Plugins/Utils/validatorinteger.h>

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

    Error e;
    const QJsonArray domains = Domain::listJson(c, e);
    if (e) {
        e.toStash(c, true);
        return;
    }

    c->res()->setJsonArrayBody(domains);
}

void ControlCenterDomains::add(Context *c)
{
    if (!MeldariUtils::checkAllowedMethod(c, u"POST")) {
        return;
    }

    static Validator v({
                           new ValidatorRequired(QStringLiteral("name")),
                           new ValidatorDomain(QStringLiteral("name"), true),
                           new ValidatorRequired(QStringLiteral("status")),
                           new ValidatorIn(QStringLiteral("status"), Domain::statusValues()),
                           new ValidatorInteger(QStringLiteral("status"), QMetaType::Int)
                       });

    const ValidatorResult vr = v.validate(c, Validator::BodyParamsOnly);

    if (vr) {
        Error e;
        const Domain dom = Domain::add(c, e, vr.values());
        if (!e) {
            MeldariUtils::setJsonResponse(c, dom.toJson(), c->translate("ControlCenterDomains", "Domain created"), c->translate("ControlCenterDomains", "Successfully created new domain “%1” (ID: %2).").arg(dom.name(), QString::number(dom.id())));
        } else {
            e.toStash(c, true);
        }
    } else {
        c->res()->setStatus(400);
        c->res()->setJsonObjectBody(QJsonObject({{QStringLiteral("fielderrors"), vr.errorsJsonObject()}}));
    }
}

bool ControlCenterDomains::Auto(Context *c)
{
    c->setStash(QStringLiteral("site_title"), c->translate("ControlCenterDomains", "Domains"));
    return true;
}
