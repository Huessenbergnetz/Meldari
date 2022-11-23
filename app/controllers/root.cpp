/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "root.h"
#include "logging.h"
#include "meldariconfig.h"
#include "objects/user.h"

#include <Cutelyst/Plugins/Authentication/authentication.h>

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
    const AuthenticationUser authUser = Authentication::user(c);

    if (!authUser.isNull()) {
        User user(authUser);
        user.toStash(c);
    }

    c->setStash(QStringLiteral("site_name"), MeldariConfig::siteName());
    return true;
}

void Root::End(Context *c)
{
    Q_UNUSED(c)
}

void Root::csrfDenied(Context *c)
{
    c->res()->setBody(u"CSRF check failed!");
    c->res()->setContentType(QStringLiteral("text/plain; charset=utf-8"));
}

#include "moc_root.cpp"
