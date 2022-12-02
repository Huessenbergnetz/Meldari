/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "utils.h"
#include <Cutelyst/Context>
#include <Cutelyst/Response>
#include <QTimeZone>
#include <QJsonObject>
#include <QJsonArray>

QStringList Utils::getTimezoneList()
{
    static QStringList tzs = ([]() -> QStringList {
        QStringList lst;
        const QList<QByteArray> availableTzIds = QTimeZone::availableTimeZoneIds();
        lst.reserve(availableTzIds.size());
        for (const QByteArray &tz : availableTzIds) {
            lst << QString::fromLatin1(tz);
        }
        return lst;
    })();
    return tzs;
}

std::vector<OptionItem> Utils::getTimezoneOptionsList(const QString &selected)
{
    std::vector<OptionItem> lst;
    const QList<QByteArray> availableTzIds = QTimeZone::availableTimeZoneIds();
    lst.reserve(availableTzIds.size());
    for (const QByteArray &tz : availableTzIds) {
        const auto tzStr = QString::fromLatin1(tz);
        lst.emplace_back(tzStr, tzStr, tzStr == selected);
    }
    return lst;
}

void Utils::setJsonResponse(Cutelyst::Context *c, const QJsonObject &data, const QString &messageTitle, const QString &messageText, int status)
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

void Utils::setJsonResponse(Cutelyst::Context *c, const QJsonArray &data, const QString &messageTitle, const QString &messageText, int status)
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
