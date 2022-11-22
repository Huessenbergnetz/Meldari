/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_USER_H
#define MELDARICOMMON_USER_H

#include <QObject>
#include <QSharedDataPointer>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

class UserData;

class User
{
    Q_GADGET
    Q_PROPERTY(User::dbid_t id READ id CONSTANT)
    Q_PROPERTY(User::Type type READ type CONSTANT)
    Q_PROPERTY(QString username READ username CONSTANT)
    Q_PROPERTY(QString email READ email CONSTANT)
    Q_PROPERTY(QDateTime created READ created CONSTANT)
    Q_PROPERTY(QDateTime updated READ updated CONSTANT)
    Q_PROPERTY(QDateTime validUntil READ validUntil CONSTANT)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen CONSTANT)
    Q_PROPERTY(QDateTime lockedAt READ lockedAt CONSTANT)
    Q_PROPERTY(User::dbid_t lockedBy READ lockedBy CONSTANT)
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

    User();
    User(User::dbid_t id, User::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, User::dbid_t lockedBy, const QVariantMap &settings);
    User(const User &other);
    User(User &&other) noexcept;
    User &operator=(const User &other);
    User &operator=(User &&other) noexcept;
    ~User();

    dbid_t id() const;

    User::Type type() const;

    QString username() const;

    QString email() const;

    QDateTime created() const;

    QDateTime updated() const;

    QDateTime validUntil() const;

    QDateTime lastSeen() const;

    QDateTime lockedAt() const;

    dbid_t lockedBy() const;

    QVariantMap settings() const;

    bool isAdmin() const;

    bool isValid() const;

    bool isNull() const;

    QJsonObject toJson() const;

    static User::Type typeStringToEnum(const QString &str);

    static QString typeEnumToString(User::Type type);

    static QStringList supportedTypes();

private:
    QSharedDataPointer<UserData> data;

    friend QDataStream &operator<<(QDataStream &stream, const User &user);
    friend QDataStream &operator>>(QDataStream &stream, User &user);
};

Q_DECLARE_METATYPE(User)
Q_DECLARE_TYPEINFO(User, Q_MOVABLE_TYPE);

QDebug operator<<(QDebug dbg, const User &user);

QDataStream &operator<<(QDataStream &stream, const User &user);

QDataStream &operator>>(QDataStream &stream, User &user);

#endif // MELDARICOMMON_USER_H
