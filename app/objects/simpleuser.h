/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_SIMPLEUSER_H
#define MELDARI_SIMPLEUSER_H

#include <baseuser.h>
#include <QObject>
#include <QSharedDataPointer>
#include <QJsonArray>
#include <QJsonObject>
#include <vector>

namespace Cutelyst {
class Context;
}

class Error;
class SimpleUserData;

class SimpleUser
{
    Q_GADGET
    Q_PROPERTY(BaseUser::dbid_t id READ id CONSTANT)
    Q_PROPERTY(QString username READ username CONSTANT)
public:
    SimpleUser();
    SimpleUser(BaseUser::dbid_t id, const QString &username);
    SimpleUser(const SimpleUser &other);
    SimpleUser(SimpleUser &&other) noexcept;
    SimpleUser &operator=(const SimpleUser &other);
    SimpleUser &operator=(SimpleUser &&other) noexcept;
    ~SimpleUser() noexcept;

    BaseUser::dbid_t id() const;

    QString username() const;

    bool isValid() const;

    bool isNull() const;

    QJsonObject toJson() const;

    bool operator==(const SimpleUser &other) const noexcept;

    bool operator!=(const SimpleUser &other) const noexcept
    { return !(*this == other); }

    static std::vector<SimpleUser> list(Cutelyst::Context *c, Error &e, BaseUser::Type minType = BaseUser::Invalid);

    static QJsonArray listJson(Cutelyst::Context *c, Error &e, BaseUser::Type minType = BaseUser::Invalid);

private:
    QSharedDataPointer<SimpleUserData> data;

    friend QDataStream &operator<<(QDataStream &stream, const SimpleUser &user);
    friend QDataStream &operator>>(QDataStream &stream, SimpleUser &user);
};

Q_DECLARE_METATYPE(SimpleUser)
Q_DECLARE_TYPEINFO(SimpleUser, Q_MOVABLE_TYPE);

QDebug operator<<(QDebug dbg, const SimpleUser &user);

QDataStream &operator<<(QDataStream &stream, const SimpleUser &user);

QDataStream &operator>>(QDataStream &stream, SimpleUser &user);

#endif // MELDARI_SIMPLEUSER_H
