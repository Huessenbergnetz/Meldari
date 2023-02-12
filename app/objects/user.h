/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_USER_H
#define MELDARI_USER_H

#include <baseuser.h>
#include "optionitem.h"
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <vector>

namespace Cutelyst {
class Context;
}

class Error;

class User : public BaseUser
{
    Q_GADGET
    Q_PROPERTY(BaseUser::dbid_t id READ id CONSTANT)
    Q_PROPERTY(BaseUser::Type type READ type CONSTANT)
    Q_PROPERTY(QString username READ username CONSTANT)
    Q_PROPERTY(QString email READ email CONSTANT)
    Q_PROPERTY(QDateTime created READ created CONSTANT)
    Q_PROPERTY(QDateTime updated READ updated CONSTANT)
    Q_PROPERTY(QDateTime validUntil READ validUntil CONSTANT)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen CONSTANT)
    Q_PROPERTY(QDateTime lockedAt READ lockedAt CONSTANT)
    Q_PROPERTY(BaseUser::dbid_t lockedById READ lockedById CONSTANT)
    Q_PROPERTY(QString lockedByName READ lockedByName CONSTANT)
    Q_PROPERTY(QVariantMap settings READ settings CONSTANT)
    Q_PROPERTY(QString tz READ tz CONSTANT)
    Q_PROPERTY(QString lang READ lang CONSTANT)
    Q_PROPERTY(bool isAdmin READ isAdmin CONSTANT)
public:
    User();
    User(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName);
    User(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName, const QVariantMap &settings);
    User(const User &other);
    User(User &&other) noexcept;
    User &operator=(const User &other);
    User &operator=(User &&other) noexcept;
    ~User() noexcept;

    void toStash(Cutelyst::Context *c) const;

    static User fromStash(Cutelyst::Context *c);

    static std::vector<User> list(Cutelyst::Context *c, Error &e, Type minType = User::Invalid);

    static QJsonArray listJson(Cutelyst::Context *c, Error &e, Type minType = User::Invalid);

    static User add(Cutelyst::Context *c, Error &e, const QVariantHash &values);

    bool update(Cutelyst::Context *c, Error &e, const QVariantHash &values);

    bool updateLastSeen(Cutelyst::Context *c, Error &e);

    bool remove(Cutelyst::Context *c, Error &e) const;

    static User get(Cutelyst::Context *c, Error &e, BaseUser::dbid_t id);

    static bool toStash(Cutelyst::Context *c, Error &e, BaseUser::dbid_t id);

    static QString typeTranslated(Cutelyst::Context *c, Type type);

    static QJsonObject typesTranslated(Cutelyst::Context *c);

    static std::vector<OptionItem> typeOptions(Cutelyst::Context *c, Type selected);

    static std::vector<OptionItem> typeOptions(Cutelyst::Context *c, Type selected, Type below);

    static dbid_t stringToDbId(const QString &str, bool *ok, Cutelyst::Context *c, bool detach = false);
};

Q_DECLARE_METATYPE(User)
Q_DECLARE_TYPEINFO(User, Q_MOVABLE_TYPE);

QDebug operator<<(QDebug dbg, const User &user);

#endif // MELDARI_USER_H
