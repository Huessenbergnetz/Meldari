/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "user.h"
#include <baseuser_p.h>

#include <Cutelyst/Context>
#include <Cutelyst/Plugins/Authentication/authenticationuser.h>

#define USER_STASH_KEY "user"

User::User() : BaseUser()
{

}

User::User(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedBy, const QVariantMap &settings)
    : BaseUser(id, type, username, email, created, updated, validUntil, lastSeen, lockedAt, lockedBy, settings)
{

}

User::User(const Cutelyst::AuthenticationUser &user) : BaseUser()
{
    data = new BaseUserData;
    data->id = user.id().toUInt();
    data->type = static_cast<BaseUser::Type>(user.value(QStringLiteral("type")).toInt());
    data->username = user.value(QStringLiteral("username")).toString();
    data->email = user.value(QStringLiteral("email")).toString();
    data->created = user.value(QStringLiteral("created_at")).toDateTime();
    data->created.setTimeSpec(Qt::UTC);
    data->updated = user.value(QStringLiteral("updated_at")).toDateTime();
    data->updated.setTimeSpec(Qt::UTC);
    data->validUntil = user.value(QStringLiteral("valid_until")).toDateTime();
    if (data->validUntil.isValid()) {
        data->validUntil.setTimeSpec(Qt::UTC);
    }
    data->lastSeen = user.value(QStringLiteral("last_seen")).toDateTime();
    if (data->lastSeen.isValid()) {
        data->lastSeen.setTimeSpec(Qt::UTC);
    }
    data->lockedAt = user.value(QStringLiteral("locked_at")).toDateTime();
    if (data->lockedAt.isValid()) {
        data->lockedAt.setTimeSpec(Qt::UTC);
    }
    data->lockedBy = user.value(QStringLiteral("locked_by")).toUInt();
    data->settings = user.value(QStringLiteral("settings")).toMap();
}

User::User(const User &other) = default;

User::User(User &&other) noexcept = default;

User &User::operator=(const User &other) = default;

User &User::operator=(User &&other) noexcept = default;

User::~User() = default;

bool User::operator==(const User &other) const noexcept
{
    if (data == other.data) {
        return true;
    }

    if (data->id != other.data->id) {
        return false;
    }

    if (data->type != other.data->type) {
        return false;
    }

    if (data->username != other.data->username) {
        return false;
    }

    if (data->email != other.data->email) {
        return false;
    }

    if (data->created != other.data->created) {
        return false;
    }

    if (data->updated != other.data->updated) {
        return false;
    }

    if (data->validUntil != other.data->validUntil) {
        return false;
    }

    if (data->lastSeen != other.data->lastSeen) {
        return false;
    }

    if (data->lockedAt != other.data->lockedAt) {
        return false;
    }

    if (data->lockedBy != other.data->lockedBy) {
        return false;
    }

    if (data->settings != other.data->settings) {
        return false;
    }

    return true;
}

void User::toStash(Cutelyst::Context *c)
{
    Q_ASSERT(c);
    c->setStash(QStringLiteral(USER_STASH_KEY), QVariant::fromValue<User>(*this));
}

User User::fromStash(Cutelyst::Context *c)
{
    Q_ASSERT(c);
    return c->stash(QStringLiteral(USER_STASH_KEY)).value<User>();
}

QDebug operator<<(QDebug dbg, const User &user)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver)
    dbg.nospace() << "User(";
    dbg << "ID: " << user.id();
    dbg << ", Username: " << user.username();
    dbg << ", Type: " << user.type();
    dbg << ", Email: " << user.email();
    dbg << ", Created: " << user.created();
    dbg << ", Updated: " << user.updated();
    dbg << ", Valid Until: " << user.validUntil();
    dbg << ", Last Seen: " << user.lastSeen();
    dbg << ')';
    return dbg.maybeSpace();
}

QDataStream &operator<<(QDataStream &stream, const User &user)
{
    stream << user.data->id
           << static_cast<qint32>(user.data->type)
           << user.data->username
           << user.data->email
           << user.data->created
           << user.data->updated
           << user.data->validUntil
           << user.data->lastSeen
           << user.data->lockedAt
           << user.data->lockedBy
           << user.data->settings;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, User &user)
{
    if (user.data == nullptr) {
        user.data = new BaseUserData;
    }

    stream >> user.data->id;
    qint32 typeInt = -1;
    stream >> typeInt;
    user.data->type = static_cast<BaseUser::Type>(typeInt);
    stream >> user.data->username;
    stream >> user.data->email;
    stream >> user.data->created;
    stream >> user.data->updated;
    stream >> user.data->validUntil;
    stream >> user.data->lastSeen;
    stream >> user.data->lockedAt;
    stream >> user.data->lockedBy;
    stream >> user.data->settings;

    return stream;
}

#include "moc_user.cpp"