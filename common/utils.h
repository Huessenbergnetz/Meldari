/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_UTILS_H
#define MELDARICOMMON_UTILS_H

#include "meldaricommon_global.h"
#include "optionitem.h"
#include <QStringList>
#include <vector>

class QJsonObject;
class QJsonArray;
class QJsonDocument;
namespace Cutelyst {
class Context;
}

class MELDARICOMMON_LIBRARY Utils
{
public:
    static QStringList getTimezoneList();
    static std::vector<OptionItem> getTimezoneOptionsList(const QString &selected = QString());
    inline static std::vector<OptionItem> getTimezoneOptionsList(const QByteArray &selected = QByteArray());
    static void setJsonResponse(Cutelyst::Context *c, const QJsonObject &data, const QString &messageTitle = QString(), const QString &messageText = QString(), int status = 200);
    static void setJsonResponse(Cutelyst::Context *c, const QJsonArray &data, const QString &messageTitle = QString(), const QString &messageText = QString(), int status = 200);
};

inline std::vector<OptionItem> Utils::getTimezoneOptionsList(const QByteArray &selected)
{
    return Utils::getTimezoneOptionsList(QString::fromLatin1(selected));
}

#endif // MELDARICOMMON_UTILS_H
