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
