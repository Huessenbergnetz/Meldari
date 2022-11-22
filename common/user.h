/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_USER_H
#define MELDARICOMMON_USER_H

#include <QSharedDataPointer>
#include <QString>
#include <QDateTime>

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
    Q_PROPERTY(QDateTime lockedAt READ lockedAt CONSTANT)
    Q_PROPERTY(User::dbid_t lockedBy READ lockedBy CONSTANT)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen CONSTANT)
    Q_PROPERTY(QVariantMap settings READ settings CONSTANT)
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

    QDateTime lockedAt() const;

    dbid_t lockedBy() const;

    QDateTime lastSeen() const;

    QVariantMap settings() const;

    static User::Type typeStringToEnum(const QString &str);

    static QString typeEnumToString(User::Type type);

    static QStringList supportedTypes();

private:
    QSharedDataPointer<UserData> data;
};

#endif // MELDARICOMMON_USER_H
