/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_BASEDOMAIN_P_H
#define MELDARICOMMON_BASEDOMAIN_P_H

#include "basedomain.h"

class BaseDomainData : public QSharedData
{
public:
    BaseDomainData() noexcept = default;
    BaseDomainData(BaseDomain::dbid_t _id, const QString &_name, BaseDomain::Status _status, BaseUser::dbid_t _ownerId, const QString &_ownerName, const QDateTime &_created, const QDateTime &_updated, const QDateTime &_validUntil, const QDateTime &_lockedAt, BaseUser::dbid_t _lockedById, const QString &_lockedByName)
        : QSharedData(),
          name{_name},
          ownerName{_ownerName},
          lockedByName{_lockedByName},
          created{_created},
          updated{_updated},
          validUntil{_validUntil},
          lockedAt{_lockedAt},
          id{_id},
          ownerId{_ownerId},
          lockedById{_lockedById},
          status{_status}
    {
        created.setTimeSpec(Qt::UTC);
        updated.setTimeSpec(Qt::UTC);
        if (validUntil.isValid()) {
            validUntil.setTimeSpec(Qt::UTC);
        }
        if (lockedAt.isValid()) {
            lockedAt.setTimeSpec(Qt::UTC);
        }
    }
    BaseDomainData(const BaseDomainData &) noexcept = default;
    ~BaseDomainData() noexcept = default;
    BaseDomainData &operator=(const BaseDomainData &) = delete;

    QString name;
    QString ownerName;
    QString lockedByName;
    QDateTime created;
    QDateTime updated;
    QDateTime validUntil;
    QDateTime lockedAt;
    BaseDomain::dbid_t id {0};
    BaseUser::dbid_t ownerId {0};
    BaseUser::dbid_t lockedById {0};
    BaseDomain::Status status {BaseDomain::Invalid};

    QJsonObject toJson() const;
};

#endif // MELDARICOMMON_BASEDOMAIN_P_H
