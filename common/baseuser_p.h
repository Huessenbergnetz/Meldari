/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_BASEUSER_P_H
#define MELDARICOMMON_BASEUSER_P_H

#include "baseuser.h"

class UserData : public QSharedData
{
public:
    QString username;
    QString email;
    QDateTime created;
    QDateTime updated;
    QDateTime validUntil;
    QDateTime lockedAt;
    QDateTime lastSeen;
    QVariantMap settings;
    BaseUser::dbid_t lockedBy;
    BaseUser::dbid_t id = 0;
    BaseUser::Type type = BaseUser::Invalid;

    QJsonObject toJson() const;
};

#endif // MELDARICOMMON_BASEUSER_P_H
