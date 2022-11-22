/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "user.h"

#include <QMetaObject>
#include <QMetaEnum>

class UserData : public QSharedData
{
public:
    QString username;
    QString email;
    QDateTime created;
    QDateTime updated;
    QDateTime validUntil;
    QDateTime lockedAt;
    QDateTime lastSeen;
    User::dbid_t lockedBy;
    User::dbid_t id = 0;
    User::Type type = User::Invalid;
};

User::User()
{

}

User::User(const User &other)
    : data{other.data}
{

}

User::User(User &&other) noexcept = default;

User &User::operator=(const User &other)
{
    if (this != &other)
        data.operator=(other.data);
    return *this;
}

User &User::operator=(User &&other) noexcept = default;

User::~User() = default;

User::dbid_t User::id() const
{
    return data ? data->id : 0;
}

User::Type User::type() const
{
    return data ? data->type : User::Invalid;
}

QString User::username() const
{
    return data ? data->username : QString();
}

QString User::email() const
{
    return data ? data->email : QString();
}

QDateTime User::created() const
{
    return data ? data->created : QDateTime();
}

QDateTime User::updated() const
{
    return data ? data->updated : QDateTime();
}

QDateTime User::validUntil() const
{
    return data ? data->validUntil : QDateTime();
}

QDateTime User::lockedAt() const
{
    return data ? data->lockedAt : QDateTime();
}

User::dbid_t User::lockedBy() const
{
    return data ? data->lockedBy : 0;
}

QDateTime User::lastSeen() const
{
    return data ? data->lastSeen : QDateTime();
}

User::Type User::typeStringToEnum(const QString &str)
{
    if (str.compare(u"disabled", Qt::CaseInsensitive) == 0) {
        return User::Disabled;
    } else if (str.compare(u"registered", Qt::CaseInsensitive) == 0) {
        return User::Registered;
    } else if (str.compare(u"administrator", Qt::CaseInsensitive) == 0) {
        return User::Administrator;
    } else if (str.compare(u"superuser", Qt::CaseInsensitive) == 0) {
        return User::SuperUser;
    } else {
        return User::Invalid;
    }
}

QString User::typeEnumToString(User::Type type)
{
    QString str;

    if (type != User::Invalid) {
        const QMetaObject mo = User::staticMetaObject;
        const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Type"));

        str = QString::fromLatin1(me.valueToKey(static_cast<int>(type)));
    }

    return str;
}

QStringList User::supportedTypes()
{
    QStringList lst;

    const QMetaObject mo = User::staticMetaObject;
    const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Type"));

    lst.reserve(me.keyCount() - 1);

    for (int i = 1; i < me.keyCount(); ++i) {
        lst.append(QString::fromLatin1(me.key(i)));
    }

    return lst;
}
