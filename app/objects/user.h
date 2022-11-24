/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_USER_H
#define MELDARI_USER_H

#include <baseuser.h>
#include <QObject>
#include <QJsonArray>
#include <vector>

namespace Cutelyst {
class Context;
class AuthenticationUser;
}

class Error;

class User : public BaseUser
{
    Q_GADGET
public:
    User();
    User(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedBy);
    User(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedBy, const QVariantMap &settings);
    User(const Cutelyst::AuthenticationUser &user);
    User(const User &other);
    User(User &&other) noexcept;
    User &operator=(const User &other);
    User &operator=(User &&other) noexcept;
    ~User();

    bool operator==(const User &other) const noexcept;

    bool operator!=(const User &other) const noexcept
    { return !(*this == other); }

    void toStash(Cutelyst::Context *c);

    static User fromStash(Cutelyst::Context *c);

    static std::vector<User> list(Cutelyst::Context *c, Error &e);

    static QJsonArray listJson(Cutelyst::Context *c, Error &e);

private:
    friend QDataStream &operator<<(QDataStream &stream, const User &user);
    friend QDataStream &operator>>(QDataStream &stream, User &user);
};

Q_DECLARE_METATYPE(User)
Q_DECLARE_TYPEINFO(User, Q_MOVABLE_TYPE);

QDebug operator<<(QDebug dbg, const User &user);

QDataStream &operator<<(QDataStream &stream, const User &user);

QDataStream &operator>>(QDataStream &stream, User &user);

#endif // MELDARI_USER_H
