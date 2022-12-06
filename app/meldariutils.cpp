/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldariutils.h"

#include "objects/error.h"

#include <Cutelyst/Context>
#include <Cutelyst/Request>
#include <Cutelyst/Response>

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
