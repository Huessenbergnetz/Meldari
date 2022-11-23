/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "baseuser_p.h"

#include <QMetaObject>
#include <QMetaEnum>
#include <QDebug>

QJsonObject BaseUserData::toJson() const
{
    QJsonObject o;

    o.insert(u"id", static_cast<qint64>(id));
    o.insert(u"type", static_cast<int>(type));
    o.insert(u"username", username);
    o.insert(u"email", email);
    o.insert(u"created", created.toString(Qt::ISODate));
    o.insert(u"updated", updated.toString(Qt::ISODate));
    o.insert(u"validUntil", validUntil.toString(Qt::ISODate));
    o.insert(u"lastSeen", lastSeen.toString(Qt::ISODate));
    o.insert(u"lockedAt", lockedAt.toString(Qt::ISODate));
    o.insert(u"lockedBy", static_cast<qint64>(lockedBy));
    o.insert(u"settings", QJsonObject::fromVariantMap(settings));

    return o;
}

BaseUser::BaseUser()
{

}

BaseUser::BaseUser(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedBy, const QVariantMap &settings)
    : data(new BaseUserData)
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

BaseUser::BaseUser(const BaseUser &other)
    : data{other.data}
{

}

BaseUser::BaseUser(BaseUser &&other) noexcept = default;

BaseUser &BaseUser::operator=(const BaseUser &other)
{
    if (this != &other)
        data.operator=(other.data);
    return *this;
}

BaseUser &BaseUser::operator=(BaseUser &&other) noexcept = default;

BaseUser::~BaseUser() = default;

BaseUser::dbid_t BaseUser::id() const
{
    return data ? data->id : 0;
}

BaseUser::Type BaseUser::type() const
{
    return data ? data->type : BaseUser::Invalid;
}

QString BaseUser::username() const
{
    return data ? data->username : QString();
}

QString BaseUser::email() const
{
    return data ? data->email : QString();
}

QDateTime BaseUser::created() const
{
    return data ? data->created : QDateTime();
}

QDateTime BaseUser::updated() const
{
    return data ? data->updated : QDateTime();
}

QDateTime BaseUser::validUntil() const
{
    return data ? data->validUntil : QDateTime();
}

QDateTime BaseUser::lastSeen() const
{
    return data ? data->lastSeen : QDateTime();
}

QDateTime BaseUser::lockedAt() const
{
    return data ? data->lockedAt : QDateTime();
}

BaseUser::dbid_t BaseUser::lockedBy() const
{
    return data ? data->lockedBy : 0;
}

QVariantMap BaseUser::settings() const
{
    return data ? data->settings : QVariantMap();
}

bool BaseUser::isAdmin() const
{
    return data && data->type >= BaseUser::Administrator;
}

bool BaseUser::isValid() const
{
    return data && data->id > 0 && data->type != BaseUser::Invalid;
}

bool BaseUser::isNull() const
{
    return data ? false : true;
}

QJsonObject BaseUser::toJson() const
{
    if (isNull() || !isValid()) {
        return QJsonObject();
    }

    return data->toJson();
}

bool BaseUser::operator==(const BaseUser &other) const noexcept
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

BaseUser::Type BaseUser::typeStringToEnum(const QString &str)
{
    if (str.compare(u"disabled", Qt::CaseInsensitive) == 0) {
        return BaseUser::Disabled;
    } else if (str.compare(u"registered", Qt::CaseInsensitive) == 0) {
        return BaseUser::Registered;
    } else if (str.compare(u"administrator", Qt::CaseInsensitive) == 0) {
        return BaseUser::Administrator;
    } else if (str.compare(u"superuser", Qt::CaseInsensitive) == 0) {
        return BaseUser::SuperUser;
    } else {
        return BaseUser::Invalid;
    }
}

QString BaseUser::typeEnumToString(BaseUser::Type type)
{
    QString str;

    if (type != BaseUser::Invalid) {
        const QMetaObject mo = BaseUser::staticMetaObject;
        const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Type"));

        str = QString::fromLatin1(me.valueToKey(static_cast<int>(type)));
    }

    return str;
}

QStringList BaseUser::supportedTypes()
{
    QStringList lst;

    const QMetaObject mo = BaseUser::staticMetaObject;
    const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Type"));

    lst.reserve(me.keyCount() - 1);

    for (int i = 1; i < me.keyCount(); ++i) {
        lst.append(QString::fromLatin1(me.key(i)));
    }

    return lst;
}

QDebug operator<<(QDebug dbg, const BaseUser &user)
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

QDataStream &operator<<(QDataStream &stream, const BaseUser &user)
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

QDataStream &operator>>(QDataStream &stream, BaseUser &user)
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

#include "moc_baseuser.cpp"
