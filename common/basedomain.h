/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_BASEDOMAIN_H
#define MELDARICOMMON_BASEDOMAIN_H

#include "meldaricommon_export.h"
#include "baseuser.h"

#include <QObject>
#include <QSharedDataPointer>
#include <QDateTime>
#include <QJsonObject>

class BaseDomainData;

class MELDARICOMMON_EXPORT BaseDomain
{
    Q_GADGET
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
    ~BaseDomain() noexcept;

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

    void swap(BaseDomain &other) noexcept
    { data.swap(other.data); }

    void clear();

    QJsonObject toJson() const;

    bool operator==(const BaseDomain &other) const noexcept;

    bool operator!=(const BaseDomain &other) const noexcept
    { return !(*this == other); }

    static Status statusStringToEnum(const QString &str);

    static QString statusEnumToString(Status status);

    static QStringList supportedStati();

    static QStringList statusValues();

private:
    QSharedDataPointer<BaseDomainData> data;

    friend QDataStream &operator<<(QDataStream &stream, const BaseDomain &domain);
    friend QDataStream &operator>>(QDataStream &stream, BaseDomain &domain);
};

Q_DECLARE_METATYPE(BaseDomain)
Q_DECLARE_TYPEINFO(BaseDomain, Q_MOVABLE_TYPE);

inline void BaseDomain::clear()
{ if (!isNull()) *this = BaseDomain(); }

MELDARICOMMON_EXPORT QDebug operator<<(QDebug dbg, const BaseDomain &domain);

MELDARICOMMON_EXPORT QDataStream &operator<<(QDataStream &stream, const BaseDomain &domain);

MELDARICOMMON_EXPORT QDataStream &operator>>(QDataStream &stream, BaseDomain &domain);

MELDARICOMMON_EXPORT void swap(BaseDomain &a, BaseDomain &b) noexcept;

#endif // MELDARICOMMON_BASEDOMAIN_H
