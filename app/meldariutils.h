/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARIUTILS_H
#define MELDARIUTILS_H

#include <QStringView>
#include <QString>

class QJsonObject;
class QJsonArray;

namespace Cutelyst {
class Context;
}

class MeldariUtils
{
public:
    static bool checkAllowedMethod(Cutelyst::Context *c, QStringView allowedMethod, bool detachOnError = true);
    static void setJsonResponse(Cutelyst::Context *c, const QJsonObject &data, const QString &messageTitle = QString(), const QString &messageText = QString(), int status = 200);
    static void setJsonResponse(Cutelyst::Context *c, const QJsonArray &data, const QString &messageTitle = QString(), const QString &messageText = QString(), int status = 200);
};

#endif // MELDARIUTILS_H
