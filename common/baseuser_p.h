/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_BASEUSER_P_H
#define MELDARICOMMON_BASEUSER_P_H

#include "baseuser.h"

class BaseUserData : public QSharedData
{
public:
    BaseUserData() noexcept = default;
    BaseUserData(BaseUser::dbid_t _id, BaseUser::Type _type, const QString &_username, const QString &_email, const QDateTime &_created, const QDateTime &_updated, const QDateTime &_validUntil, const QDateTime &_lastSeen, const QDateTime &_lockedAt, BaseUser::dbid_t _lockedById, const QString &_lockedByName, const QVariantMap &_settings)
        : QSharedData(),
          username{_username},
          email{_email},
          lockedByName{_lockedByName},
          created{_created},
          updated{_updated},
          validUntil{_validUntil},
          lockedAt{_lockedAt},
          lastSeen{_lastSeen},
          settings{_settings},
          lockedById{_lockedById},
          id{_id},
          type{_type}
    {
        created.setTimeSpec(Qt::UTC);
        updated.setTimeSpec(Qt::UTC);
        if (validUntil.isValid()) {
            validUntil.setTimeSpec(Qt::UTC);
        }
        if (lockedAt.isValid()) {
            lockedAt.setTimeSpec(Qt::UTC);
        }
        if (lastSeen.isValid()) {
            lastSeen.setTimeSpec(Qt::UTC);
        }
    }
    BaseUserData(const BaseUserData &) noexcept = default;
    ~BaseUserData() noexcept = default;
    BaseUserData &operator=(const BaseUserData &) = delete;

    QString username;
    QString email;
    QString lockedByName;
    QDateTime created;
    QDateTime updated;
    QDateTime validUntil;
    QDateTime lockedAt;
    QDateTime lastSeen;
    QVariantMap settings;
    BaseUser::dbid_t lockedById;
    BaseUser::dbid_t id {0};
    BaseUser::Type type {BaseUser::Invalid};

    QJsonObject toJson() const;
};

#endif // MELDARICOMMON_BASEUSER_P_H
