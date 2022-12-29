/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "utils.h"
#include <QTimeZone>

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
