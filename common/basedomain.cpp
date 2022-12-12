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
    data->createdAt = created;
    data->updatedAt = updated;
    data->validUntil = validUntil;
    data->lockedAt = lockedAt;
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
    return data ? data->createdAt : QDateTime();
}

QDateTime BaseDomain::updated() const
{
    return data ? data->updatedAt : QDateTime();
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

QJsonObject BaseDomainData::toJson() const
{
    QJsonObject o;

    o.insert(u"id", static_cast<qint64>(id));
    o.insert(u"name", name);
    o.insert(u"status", static_cast<int>(status));
    o.insert(u"ownerId", static_cast<qint64>(ownerId));
    o.insert(u"ownerName", ownerName);
    o.insert(u"created", createdAt.toMSecsSinceEpoch());
    o.insert(u"updated", updatedAt.toMSecsSinceEpoch());
    o.insert(u"validUntil", validUntil.isNull() ? QJsonValue() : validUntil.toMSecsSinceEpoch());
    o.insert(u"lockedAt", lockedAt.isNull() ? QJsonValue() : lockedAt.toMSecsSinceEpoch());
    o.insert(u"lockedById", static_cast<qint64>(lockedById));
    o.insert(u"lockedByName", lockedByName);

    return o;
}
