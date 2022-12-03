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
    o.insert(u"created", created.toMSecsSinceEpoch());
    o.insert(u"updated", updated.toMSecsSinceEpoch());
    o.insert(u"validUntil", validUntil.isNull() ? QJsonValue() : validUntil.toMSecsSinceEpoch());
    o.insert(u"lastSeen", lastSeen.isNull() ? QJsonValue() : lastSeen.toMSecsSinceEpoch());
    o.insert(u"lockedAt", lockedAt.isNull() ? QJsonValue() : lockedAt.toMSecsSinceEpoch());
    o.insert(u"lockedById", static_cast<qint64>(lockedById));
    o.insert(u"lockedByName", lockedByName);
    o.insert(u"settings", QJsonObject::fromVariantMap(settings));

    return o;
}

BaseUser::BaseUser()
{

}

BaseUser::BaseUser(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName, const QVariantMap &settings)
    : data(new BaseUserData)
{
    data->id = id;
    data->type = type;
    data->username = username;
    data->email = email;
    data->created = created;
    data->created.setTimeSpec(Qt::UTC);
    data->updated = updated;
    data->updated.setTimeSpec(Qt::UTC);
    data->validUntil = validUntil;
    if (data->validUntil.isValid()) {
        data->validUntil.setTimeSpec(Qt::UTC);
    }
    data->lastSeen = lastSeen;
    if (data->lastSeen.isValid()) {
        data->lastSeen.setTimeSpec(Qt::UTC);
    }
    data->lockedAt = lockedAt;
    if (data->lockedAt.isValid()) {
        data->lockedAt.setTimeSpec(Qt::UTC);
    }
    data->lockedById = lockedById;
    data->lockedByName = lockedByName;
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

BaseUser::dbid_t BaseUser::lockedById() const
{
    return data ? data->lockedById : 0;
}

QString BaseUser::lockedByName() const
{
    return data ? data->lockedByName : QString();
}

QVariantMap BaseUser::settings() const
{
    return data ? data->settings : QVariantMap();
}

QString BaseUser::tz() const
{
    return data ? data->settings.value(QStringLiteral("timezone")).toString() : QStringLiteral("UTC");
}

QString BaseUser::lang() const
{
    return data ? data->settings.value(QStringLiteral("language")).toString() : QStringLiteral("en_US");
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

QString BaseUser::logInfo() const
{
    if (data) {
        return data->username + u" (ID: " + QString::number(data->id) + u')';
    } else {
        return QStringLiteral("NULL");
    }
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

    if (id() != other.id()) {
        return false;
    }

    if (type() != other.type()) {
        return false;
    }

    if (username() != other.username()) {
        return false;
    }

    if (email() != other.email()) {
        return false;
    }

    if (created() != other.created()) {
        return false;
    }

    if (updated() != other.updated()) {
        return false;
    }

    if (validUntil() != other.validUntil()) {
        return false;
    }

    if (lastSeen() != other.lastSeen()) {
        return false;
    }

    if (lockedAt() != other.lockedAt()) {
        return false;
    }

    if (lockedById() != other.lockedById()) {
        return false;
    }

    if (settings() != other.settings()) {
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
    dbg.nospace() << "BaseUser(";
    dbg << "ID: " << user.id();
    dbg << ", Username: " << user.username();
    dbg << ", Type: " << user.type();
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
           << user.data->lockedById
           << user.data->lockedByName
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
    stream >> user.data->lockedById;
    stream >> user.data->lockedByName;
    stream >> user.data->settings;

    return stream;
}

#include "moc_baseuser.cpp"
