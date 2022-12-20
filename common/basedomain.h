/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_BASEDOMAIN_H
#define MELDARICOMMON_BASEDOMAIN_H

#include "baseuser.h"

#include <QObject>
#include <QSharedDataPointer>
#include <QDateTime>
#include <QJsonObject>

class BaseDomainData;

class BaseDomain
{
    Q_GADGET
    Q_PROPERTY(BaseDomain::dbid_t id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(BaseDomain::Status status READ status CONSTANT)
    Q_PROPERTY(BaseUser::dbid_t ownerId READ ownerId CONSTANT)
    Q_PROPERTY(QString ownerName READ ownerName CONSTANT)
    Q_PROPERTY(QDateTime created READ created CONSTANT)
    Q_PROPERTY(QDateTime updated READ updated CONSTANT)
    Q_PROPERTY(QDateTime validUntil READ validUntil CONSTANT)
    Q_PROPERTY(QDateTime lockedAt READ lockedAt CONSTANT)
    Q_PROPERTY(BaseUser::dbid_t lockedById READ lockedById CONSTANT)
    Q_PROPERTY(QString lockedByName READ lockedByName CONSTANT)
public:
    using dbid_t = quint32;

    enum Status : int {
        Invalid  = -127,
        Disabled = 0,
        Enabled  = 127
    };
    Q_ENUM(Status)

    BaseDomain();
    BaseDomain(dbid_t id, const QString &name, Status status, BaseUser::dbid_t ownerId, const QString &ownerName, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName);
    BaseDomain(const BaseDomain &other);
    BaseDomain(BaseDomain &&other) noexcept;
    BaseDomain &operator=(const BaseDomain &other);
    BaseDomain &operator=(BaseDomain &&other) noexcept;
    ~BaseDomain();

    dbid_t id() const;

    QString name() const;

    Status status() const;

    BaseUser::dbid_t ownerId() const;

    QString ownerName() const;

    QDateTime created() const;

    QDateTime updated() const;

    QDateTime validUntil() const;

    QDateTime lockedAt() const;

    BaseUser::dbid_t lockedById() const;

    QString lockedByName() const;

    bool isNull() const;

    bool isValid() const;

    QJsonObject toJson() const;

    bool operator==(const BaseDomain &other) const noexcept;

    bool operator!=(const BaseDomain &other) const noexcept
    { return !(*this == other); }

private:
    QSharedDataPointer<BaseDomainData> data;
};

Q_DECLARE_METATYPE(BaseDomain)
Q_DECLARE_TYPEINFO(BaseDomain, Q_MOVABLE_TYPE);

QDebug operator<<(QDebug dbg, const BaseDomain &domain);

#endif // MELDARICOMMON_BASEDOMAIN_H
