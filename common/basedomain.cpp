/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "basedomain_p.h"

BaseDomain::BaseDomain()
{

}

BaseDomain::BaseDomain(dbid_t id, const QString &name, Status status, BaseUser::dbid_t ownerId, const QString &ownerName, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName)
    : data{new BaseDomainData}
{
    data->name = name;
    data->ownerName = ownerName;
    data->lockedByName = lockedByName;
    data->created = created;
    data->created.setTimeSpec(Qt::UTC);
    data->updated = updated;
    data->updated.setTimeSpec(Qt::UTC);
    data->validUntil = validUntil;
    if (data->validUntil.isValid()) {
        data->validUntil.setTimeSpec(Qt::UTC);
    }
    data->lockedAt = lockedAt;
    if (data->lockedAt.isValid()) {
        data->lockedAt.setTimeSpec(Qt::UTC);
    }
    data->id = id;
    data->ownerId = ownerId;
    data->lockedById = lockedById;
    data->status = status;
}

BaseDomain::BaseDomain(const BaseDomain &other) = default;

BaseDomain::BaseDomain(BaseDomain &&other) noexcept = default;

BaseDomain &BaseDomain::operator=(const BaseDomain &other) = default;

BaseDomain &BaseDomain::operator=(BaseDomain &&other) noexcept = default;

BaseDomain::~BaseDomain() = default;

BaseDomain::dbid_t BaseDomain::id() const
{
    return data ? data->id : 0;
}

QString BaseDomain::name() const
{
    return data ? data->name : QString();
}

BaseDomain::Status BaseDomain::status() const
{
    return data ? data->status : BaseDomain::Invalid;
}

BaseUser::dbid_t BaseDomain::ownerId() const
{
    return data ? data->ownerId : 0;
}

QString BaseDomain::ownerName() const
{
    return data ? data->ownerName : QString();
}

QDateTime BaseDomain::created() const
{
    return data ? data->created : QDateTime();
}

QDateTime BaseDomain::updated() const
{
    return data ? data->updated : QDateTime();
}

QDateTime BaseDomain::validUntil() const
{
    return data ? data->validUntil : QDateTime();
}

QDateTime BaseDomain::lockedAt() const
{
    return data ? data->lockedAt : QDateTime();
}

BaseUser::dbid_t BaseDomain::lockedById() const
{
    return data ? data->lockedById : 0;
}

QString BaseDomain::lockedByName() const
{
    return data ? data->lockedByName : QString();
}

bool BaseDomain::isNull() const
{
    return data ? false : true;
}

bool BaseDomain::isValid() const
{
    if (data) {
        return data->id > 0 && data->status > BaseDomain::Invalid && !data->name.isEmpty();
    } else {
        return false;
    }
}

QJsonObject BaseDomain::toJson() const
{
    if (isNull() || !isValid()) {
        return QJsonObject();
    }

    return data->toJson();
}

bool BaseDomain::operator==(const BaseDomain &other) const noexcept
{
    if (data == other.data) {
        return true;
    }

    if (id() != other.id()) {
        return false;
    }

    if (name() != other.name()) {
        return false;
    }

    if (status() != other.status()) {
        return false;
    }

    if (ownerId() != other.ownerId()) {
        return false;
    }

    if (ownerName() != other.ownerName()) {
        return false;
    }

    if (created() != other.created()) {
        return false;
    }

    if (updated() != other.updated()) {
        return false;
    }

    if (validUntil() != other.validUntil()) {
        return false;
    }

    if (lockedAt() != other.lockedAt()) {
        return false;
    }

    if (lockedById() != other.lockedById()) {
        return false;
    }

    if (lockedByName() != other.lockedByName()) {
        return false;
    }

    return true;
}

QJsonObject BaseDomainData::toJson() const
{
    QJsonObject o;

    o.insert(u"id", static_cast<qint64>(id));
    o.insert(u"name", name);
    o.insert(u"status", static_cast<int>(status));
    o.insert(u"ownerId", static_cast<qint64>(ownerId));
    o.insert(u"ownerName", ownerName);
    o.insert(u"created", created.toMSecsSinceEpoch());
    o.insert(u"updated", updated.toMSecsSinceEpoch());
    o.insert(u"validUntil", validUntil.isNull() ? QJsonValue() : validUntil.toMSecsSinceEpoch());
    o.insert(u"lockedAt", lockedAt.isNull() ? QJsonValue() : lockedAt.toMSecsSinceEpoch());
    o.insert(u"lockedById", static_cast<qint64>(lockedById));
    o.insert(u"lockedByName", lockedByName);

    return o;
}

QDebug operator<<(QDebug dbg, const BaseDomain &domain)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver)
    dbg.nospace() << "BaseDomain(";
    dbg << "ID: " << domain.id();
    dbg << ", Name: " << domain.name();
    dbg << ", Status: " << domain.status();
    dbg << ')';
    return dbg.maybeSpace();
}

QDataStream &operator<<(QDataStream &stream, const BaseDomain &domain)
{
    stream << domain.data->id
           << domain.data->name
           << static_cast<qint32>(domain.data->status)
           << domain.data->ownerId
           << domain.data->ownerName
           << domain.data->created
           << domain.data->updated
           << domain.data->validUntil
           << domain.data->lockedAt
           << domain.data->lockedById
           << domain.data->lockedByName;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, BaseDomain &domain)
{
    if (domain.data == nullptr) {
        domain.data = new BaseDomainData;
    }

    stream >> domain.data->id;
    stream >> domain.data->name;
    qint32 statusInt = -127;
    stream >> statusInt;
    domain.data->status = static_cast<BaseDomain::Status>(statusInt);
    stream >> domain.data->ownerId;
    stream >> domain.data->ownerName;
    stream >> domain.data->created;
    stream >> domain.data->updated;
    stream >> domain.data->validUntil;
    stream >> domain.data->lockedAt;
    stream >> domain.data->lockedById;
    stream >> domain.data->lockedByName;

    return stream;
}

#include "moc_basedomain.cpp"
