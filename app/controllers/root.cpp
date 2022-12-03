/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "root.h"
#include "logging.h"
#include "meldariconfig.h"
#include "objects/user.h"
#include "objects/error.h"

#include <Cutelyst/Plugins/Authentication/authentication.h>
#include <Cutelyst/Plugins/StatusMessage>

Root::Root(QObject *parent) : Controller(parent)
{

}

void Root::index(Context *c)
{
    c->res()->setBody(u"Hello World!");
    c->res()->setContentType(QStringLiteral("text/plain; charset=utf-8"));
}

void Root::pageNotFound(Context *c)
{
    c->res()->setBody(u"Page not found!");
    c->res()->setContentType(QStringLiteral("text/plain; charset=utf-8"));
    c->res()->setStatus(404);
}

bool Root::Auto(Context *c)
{
    StatusMessage::load(c);

    const AuthenticationUser authUser = Authentication::user(c);

    if (!authUser.isNull()) {
        Error e;
        User::toStash(c, e, authUser.id().toUInt());
    }

    c->stash({
                 {QStringLiteral("site_name"), MeldariConfig::siteName()},
                 {QStringLiteral("default_timezone"), MeldariConfig::defTimezone()}
             });
    return true;
}

void Root::End(Context *c)
{
    Q_UNUSED(c)
}

void Root::csrfDenied(Context *c)
{
    if (c->req()->xhr()) {
        QJsonObject error({
                              {QStringLiteral("error"), QJsonObject({
                                   {QStringLiteral("status"), 403},
                                   {QStringLiteral("title"), c->translate("Root", "Access denied")},
                                   {QStringLiteral("text"), c->translate("Root", "CSRF check failed.")}
                               })}
                          });
        c->res()->setJsonObjectBody(error);
    } else {
        c->res()->setBody(u"CSRF check failed!");
        c->res()->setContentType(QStringLiteral("text/plain; charset=utf-8"));
    }
    c->detach();
}

void Root::error(Context *c)
{
    Error e = Error::fromStash(c);
    c->res()->setStatus(e.status());

    if (c->req()->xhr()) {
        c->res()->setJsonObjectBody(e.toJson(c));
    }
    c->detach();
}

#include "moc_root.cpp"
