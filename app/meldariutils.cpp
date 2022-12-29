/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldariutils.h"

#include "objects/error.h"

#include <Cutelyst/Context>
#include <Cutelyst/Request>
#include <Cutelyst/Response>

#include <QJsonObject>
#include <QJsonArray>

bool MeldariUtils::checkAllowedMethod(Cutelyst::Context *c, QStringView allowedMethod, bool detachOnError)
{
    if (c->req()->method().compare(allowedMethod) == 0) {
        return true;
    } else {
        const QString ams = allowedMethod.toString();
        c->res()->setStatus(Cutelyst::Response::MethodNotAllowed);;
        c->res()->setHeader(QStringLiteral("Allow"), ams);
        Error::toStash(c, Cutelyst::Response::MethodNotAllowed, c->translate("MeldariUtils", "The route “%1” only accepts requests of type “%2”.").arg(c->req()->path(), ams), detachOnError);
        return false;
    }
}

void MeldariUtils::setJsonResponse(Cutelyst::Context *c, const QJsonObject &data, const QString &messageTitle, const QString &messageText, int status)
{
    QJsonObject o;
    o.insert(u"status", status);
    QJsonObject m;
    m.insert(u"title", messageTitle);
    m.insert(u"text", messageText);
    o.insert(u"message", m);
    o.insert(u"data", data);
    c->res()->setJsonObjectBody(o);
}

void MeldariUtils::setJsonResponse(Cutelyst::Context *c, const QJsonArray &data, const QString &messageTitle, const QString &messageText, int status)
{
    QJsonObject o;
    o.insert(u"status", status);
    QJsonObject m;
    m.insert(u"title", messageTitle);
    m.insert(u"text", messageText);
    o.insert(u"message", m);
    o.insert(u"data", data);
    c->res()->setJsonObjectBody(o);
}
