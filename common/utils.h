/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_UTILS_H
#define MELDARICOMMON_UTILS_H

#include "optionitem.h"
#include <QStringList>
#include <vector>

class Utils
{
public:
    static QStringList getTimezoneList();
    static std::vector<OptionItem> getTimezoneOptionsList(const QString &selected = QString());
    inline static std::vector<OptionItem> getTimezoneOptionsList(const QByteArray &selected = QByteArray());
};

inline std::vector<OptionItem> Utils::getTimezoneOptionsList(const QByteArray &selected)
{
    return Utils::getTimezoneOptionsList(QString::fromLatin1(selected));
}

#endif // MELDARICOMMON_UTILS_H
