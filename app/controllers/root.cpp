/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "root.h"

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
    return true;
}

#include "moc_root.cpp"
