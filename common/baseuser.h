/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_BASEUSER_H
#define MELDARICOMMON_BASEUSER_H

#include "meldaricommon_global.h"
#include <QObject>
#include <QSharedDataPointer>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

class BaseUserData;

class MELDARICOMMON_LIBRARY BaseUser
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
    Q_PROPERTY(bool isAdmin READ isAdmin CONSTANT)
public:
    using dbid_t = quint32;

    enum Type : int {
        Invalid = -1,
        Disabled = 0,
        Registered = 32,
        Administrator = 64,
        SuperUser = 127
    };
    Q_ENUM(Type)

    BaseUser();
    BaseUser(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName, const QVariantMap &settings);
    BaseUser(const BaseUser &other);
    BaseUser(BaseUser &&other) noexcept;
    BaseUser &operator=(const BaseUser &other);
    BaseUser &operator=(BaseUser &&other) noexcept;
    ~BaseUser();

    dbid_t id() const;

    Type type() const;

    QString username() const;

    QString email() const;

    QDateTime created() const;

    QDateTime updated() const;

    QDateTime validUntil() const;

    QDateTime lastSeen() const;

    QDateTime lockedAt() const;

    dbid_t lockedById() const;

    QString lockedByName() const;

    QVariantMap settings() const;

    bool isAdmin() const;

    bool isValid() const;

    bool isNull() const;

    QJsonObject toJson() const;

    bool operator==(const BaseUser &other) const noexcept;

    bool operator!=(const BaseUser &other) const noexcept
    { return !(*this == other); }

    static Type typeStringToEnum(const QString &str);

    static QString typeEnumToString(Type type);

    static QStringList supportedTypes();

protected:
    QSharedDataPointer<BaseUserData> data;

private:
    friend QDataStream &operator<<(QDataStream &stream, const BaseUser &user);
    friend QDataStream &operator>>(QDataStream &stream, BaseUser &user);
};

Q_DECLARE_METATYPE(BaseUser)
Q_DECLARE_TYPEINFO(BaseUser, Q_MOVABLE_TYPE);

QDebug operator<<(QDebug dbg, const BaseUser &user);

QDataStream &operator<<(QDataStream &stream, const BaseUser &user);

QDataStream &operator>>(QDataStream &stream, BaseUser &user);

#endif // MELDARICOMMON_BASEUSER_H
