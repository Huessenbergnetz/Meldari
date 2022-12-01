/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "simpleuser.h"
#include "error.h"
#include "logging.h"

#include <Cutelyst/Context>
#include <Cutelyst/Plugins/Utils/Sql>

#include <QSqlQuery>
#include <QSqlError>

class SimpleUserData : public QSharedData
{
public:
    BaseUser::dbid_t id = 0;
    QString username;
};

SimpleUser::SimpleUser()
{

}

SimpleUser::SimpleUser(BaseUser::dbid_t id, const QString &username)
    : data{new SimpleUserData}
{
    data->id = id;
    data->username = username;
}

SimpleUser::SimpleUser(const SimpleUser &other)
    : data{other.data}
{

}

SimpleUser::SimpleUser(SimpleUser &&other) noexcept = default;

SimpleUser &SimpleUser::operator=(const SimpleUser &other)
{
    if (this != &other)
        data.operator=(other.data);
    return *this;
}

SimpleUser &SimpleUser::operator=(SimpleUser &&other) noexcept = default;

SimpleUser::~SimpleUser() = default;

BaseUser::dbid_t SimpleUser::id() const
{
    return data ? data->id : 0;
}

QString SimpleUser::username() const
{
    return data ? data->username : QString();
}

QJsonObject SimpleUser::toJson() const
{
    QJsonObject o;

    if (data) {
        o.insert(u"id", static_cast<qint64>(data->id));
        o.insert(u"username", data->username);
    }

    return o;
}

bool SimpleUser::operator==(const SimpleUser &other) const noexcept
{
    if (data == other.data) {
        return true;
    }

    if (id() != other.id()) {
        return false;
    }

    if (username() != other.username()) {
        return false;
    }

    return true;
}

std::vector<SimpleUser> SimpleUser::list(Cutelyst::Context *c, Error &e, BaseUser::Type minType)
{
    std::vector<SimpleUser> users;

    QSqlQuery q;
    if (minType == BaseUser::Invalid) {
        q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT id, username FROM users"));
    } else {
        q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT id, username FROM users WHERE type >= :type"));
        q.bindValue(QStringLiteral(":type"), static_cast<int>(minType));
    }

    if (Q_LIKELY(q.exec())) {
        if (q.size() > 0) {
            users.reserve(q.size());
        }
        while (q.next()) {
            users.emplace_back(q.value(0).toUInt(), q.value(1).toString());
        }
    } else {
        e = std::move(Error(q, c->translate("SimpleUser", "Failed to query list of users from the database.")));
        qCCritical(MEL_CORE) << "Failed to query list of users from the database:" << q.lastError().text();
    }

    return users;
}

bool SimpleUser::isValid() const
{
    return data ? data->id > 0 : false;
}

bool SimpleUser::isNull() const
{
    return data ? false : true;
}

QJsonArray SimpleUser::listJson(Cutelyst::Context *c, Error &e, BaseUser::Type minType)
{
    QJsonArray json;

    const std::vector<SimpleUser> users = SimpleUser::list(c, e, minType);
    if (e.isError()) {
        return json;
    }

    for (const SimpleUser &user : users) {
        json.append(user.toJson());
    }

    return json;
}

QDebug operator<<(QDebug dbg, const SimpleUser &user)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver)
    dbg.nospace() << "SimpleUser(";
    dbg << "ID: " << user.id();
    dbg << " , Username: " << user.username();
    dbg << ')';
    return dbg.maybeSpace();
}

QDataStream &operator<<(QDataStream &stream, const SimpleUser &user)
{
    if (user.data) {
        stream << user.data->id
               << user.data->username;
    } else {
        stream << static_cast<BaseUser::dbid_t>(0)
               << QString();
    }

    return stream;
}

QDataStream &operator>>(QDataStream &stream, SimpleUser &user)
{
    if (user.data == nullptr) {
        user.data = new SimpleUserData;
    }

    stream >> user.data->id;
    stream >> user.data->username;

    return stream;
}

#include "moc_simpleuser.cpp"
