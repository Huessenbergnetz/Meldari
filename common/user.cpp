/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "user.h"

#include <QMetaObject>
#include <QMetaEnum>
#include <QDebug>

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
    QVariantMap settings;
    User::dbid_t lockedBy;
    User::dbid_t id = 0;
    User::Type type = User::Invalid;
};

User::User()
{

}

User::User(dbid_t id, Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, dbid_t lockedBy, const QVariantMap &settings)
    : data(new UserData)
{
    data->id = id;
    data->type = type;
    data->username = username;
    data->email = email;
    data->created = created;
    data->updated = updated;
    data->validUntil = validUntil;
    data->lastSeen = lastSeen;
    data->lockedAt = lockedAt;
    data->lockedBy = lockedBy;
    data->settings = settings;
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

QDateTime User::lastSeen() const
{
    return data ? data->lastSeen : QDateTime();
}

QDateTime User::lockedAt() const
{
    return data ? data->lockedAt : QDateTime();
}

User::dbid_t User::lockedBy() const
{
    return data ? data->lockedBy : 0;
}

QVariantMap User::settings() const
{
    return data ? data->settings : QVariantMap();
}

bool User::isAdmin() const
{
    return data && data->type >= User::Administrator;
}

bool User::isValid() const
{
    return data && data->id > 0 && data->type != Invalid;
}

bool User::isNull() const
{
    return data ? false : true;
}

QJsonObject User::toJson() const
{
    QJsonObject o;

    if (isNull() || !isValid()) {
        return o;
    }

    o.insert(u"id", static_cast<qint64>(data->id));
    o.insert(u"type", static_cast<int>(data->type));
    o.insert(u"username", data->username);
    o.insert(u"email", data->email);
    o.insert(u"created", data->created.toString(Qt::ISODate));
    o.insert(u"updated", data->updated.toString(Qt::ISODate));
    o.insert(u"validUntil", data->validUntil.toString(Qt::ISODate));
    o.insert(u"lastSeen", data->lastSeen.toString(Qt::ISODate));
    o.insert(u"lockedAt", data->lockedAt.toString(Qt::ISODate));
    o.insert(u"lockedBy", static_cast<qint64>(data->lockedBy));
    o.insert(u"settings", QJsonObject::fromVariantMap(data->settings));

    return o;
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
        user.data = new UserData;
    }

    stream >> user.data->id;
    qint32 typeInt = -1;
    stream >> typeInt;
    user.data->type = static_cast<User::Type>(typeInt);
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
