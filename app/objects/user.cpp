/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "user.h"
#include "logging.h"
#include "error.h"
#include "credentialbotan.h"
#include "qtimezonevariant_p.h"
#include "meldariconfig.h"
#include <baseuser_p.h>

#include <Cutelyst/Context>
#include <Cutelyst/Plugins/Authentication/authenticationuser.h>
#include <Cutelyst/Plugins/Utils/Sql>
#include <Cutelyst/Plugins/Session/Session>
#include <Cutelyst/Plugins/Memcached/Memcached>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMetaObject>
#include <QMetaEnum>
#include <QLocale>

#include <limits>

#define USER_STASH_KEY "user"
#define MEMC_USERS_GROUP_KEY "users"
#define MEMC_USERS_STORAGE_DURATION 7200

User::User() : BaseUser()
{

}

User::User(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName)
    : BaseUser(id, type, username, email, created, updated, validUntil, lastSeen, lockedAt, lockedById, lockedByName, {})
{

}

User::User(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName, const QVariantMap &settings)
    : BaseUser(id, type, username, email, created, updated, validUntil, lastSeen, lockedAt, lockedById, lockedByName, settings)
{

}

User::User(const User &other) = default;

User::User(User &&other) noexcept = default;

User &User::operator=(const User &other) = default;

User &User::operator=(User &&other) noexcept = default;

User::~User() noexcept = default;

void User::toStash(Cutelyst::Context *c) const
{
    Q_ASSERT(c);
    c->setStash(QStringLiteral(USER_STASH_KEY), QVariant::fromValue<User>(*this));
}

User User::fromStash(Cutelyst::Context *c)
{
    Q_ASSERT(c);
    return c->stash(QStringLiteral(USER_STASH_KEY)).value<User>();
}

std::vector<User> User::list(Cutelyst::Context *c, Error &e, User::Type minType)
{
    std::vector<User> users;

    QSqlQuery q;

    if (minType == User::Invalid) {
        q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT u1.id, u1.type, u1.username, u1.email, u1.created_at, u1.updated_at, u1.valid_until, u1.last_seen, u1.locked_at, u2.id AS locked_by_id, u2.username AS locked_by_username FROM users u1 LEFT JOIN users u2 ON u2.id = u1.locked_by"));
    } else {
        q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT u1.id, u1.type, u1.username, u1.email, u1.created_at, u1.updated_at, u1.valid_until, u1.last_seen, u1.locked_at, u2.id AS locked_by_id, u2.username AS locked_by_username FROM users u1 LEFT JOIN users u2 ON u2.id = u1.locked_by WHERE u1.type >= :minType"));
        q.bindValue(QStringLiteral(":minType"), static_cast<int>(minType));
    }

    if (Q_LIKELY(q.exec())) {
        if (q.size() > 0) {
            users.reserve(q.size());
        }
        while (q.next()) {
            users.emplace_back(q.value(0).toUInt(),
                               static_cast<User::Type>(q.value(1).toInt()),
                               q.value(2).toString(),
                               q.value(3).toString(),
                               q.value(4).toDateTime(),
                               q.value(5).toDateTime(),
                               q.value(6).toDateTime(),
                               q.value(7).toDateTime(),
                               q.value(8).toDateTime(),
                               q.value(9).toUInt(),
                               q.value(10).toString());
        }
    } else {
        e = std::move(Error(q, c->translate("User", "Failed to query list of users from the database.")));
        qCCritical(MEL_CORE) << "Failed to query list of users from the database:" << q.lastError().text();
    }

    return users;
}

QJsonArray User::listJson(Cutelyst::Context *c, Error &e, User::Type minType)
{
    QJsonArray json;

    const std::vector<User> users = User::list(c, e);

    if (e.isError()) {
        return json;
    }

    for (const User &user : users) {
        json.append(user.toJson());
    }

    return json;
}

User User::add(Cutelyst::Context *c, Error &e, const QVariantHash &values)
{
    User u;

    const QString   username   = values.value(QStringLiteral("username")).toString();
    const QString   email      = values.value(QStringLiteral("email")).toString();
    const QString   password   = values.value(QStringLiteral("password")).toString();
    const Type      type       = static_cast<Type>(values.value(QStringLiteral("type")).toInt());
          QDateTime validUntil = values.value(QStringLiteral("validUntil")).toDateTime();
    const QDateTime now        = QDateTime::currentDateTimeUtc();

    qCDebug(MEL_CORE) << "Adding new user" << username;

    if (validUntil.isValid()) {
        validUntil.setTimeZone(Cutelyst::Session::value(c, QStringLiteral("tz")).value<QTimeZone>());
        validUntil = validUntil.toUTC();
    }

    const QString passwordEnc = CredentialBotan::createArgon2Password(password);
    if (Q_UNLIKELY(passwordEnc.isEmpty())) {
        e = Error(Cutelyst::Response::InternalServerError, c->translate("User", "Failed to encrypt password."));
        qCCritical(MEL_CORE) << "Failed to encrypt password for new user" << username;
        return u;
    }

    QSqlDatabase db = Cutelyst::Sql::databaseThread();
    Cutelyst::Sql::Transaction dbtrans(db);

    QSqlQuery q = CPreparedSqlQueryThread(QStringLiteral("INSERT INTO users (type, username, password, email, created_at, updated_at, valid_until) "
                                                         "VALUES (:type, :username, :password, :email, :created_at, :updated_at, :valid_until)"));
    q.bindValue(QStringLiteral(":type"), static_cast<int>(type));
    q.bindValue(QStringLiteral(":username"), username);
    q.bindValue(QStringLiteral(":password"), passwordEnc);
    q.bindValue(QStringLiteral(":email"), email);
    q.bindValue(QStringLiteral(":created_at"), now);
    q.bindValue(QStringLiteral(":updated_at"), now);
    q.bindValue(QStringLiteral(":valid_until"), validUntil);

    if (Q_UNLIKELY(!q.exec())) {
        e = Error(q, c->translate("User", "Failed to insert new user “%1“ into database.").arg(username));
        qCCritical(MEL_CORE) << "Failed to insert new user" << username << "into database:" << q.lastError().text();
        return u;
    }

    const dbid_t id = q.lastInsertId().toUInt();

    const QStringList settingsKeys({QStringLiteral("language"), QStringLiteral("timezone")});

    for (const QString &key : settingsKeys) {
        QSqlQuery q2 = CPreparedSqlQueryThread(QStringLiteral("INSERT INTO usersettings (user_id, name, value) "
                                                              "VALUES (:user_id, :name, :value)"));
        q2.bindValue(QStringLiteral(":user_id"), id);
        q2.bindValue(QStringLiteral(":name"), key);
        q2.bindValue(QStringLiteral(":value"), values.value(key).toString());

        if (Q_UNLIKELY(!q2.exec())) {
            e = Error(q2, c->translate("User", "Failed to insert key “%1” into the database.").arg(key));
            qCCritical(MEL_CORE) << "Failed to insert setting key" << key << "for user" << username << "into the database:" << q2.lastError().text();
            return u;
        }
    }

    if (Q_UNLIKELY(!dbtrans.commit())) {
        e = Error(db.lastError(), c->translate("User", "Failed to insert new user “%1“ into database.").arg(username));
        return u;
    }

    QVariantMap settings;
    for (const QString &key : settingsKeys) {
        settings.insert(key, values.value(key));
    }

    u = User(id, type, username, email, now, now, validUntil, QDateTime(), QDateTime(), 0, QString(), settings);

    qCInfo(MEL_CORE) << User::fromStash(c) << "created new" << u;

    return u;
}

bool User::update(Cutelyst::Context *c, Error &e, const QVariantHash &values)
{
    qCDebug(MEL_CORE) << "Updating data for" << *this;

    const QString   newPassword = values.value(QStringLiteral("newpassword")).toString();
    const QString   email       = values.value(QStringLiteral("email")).toString();
          QDateTime validUntil  = values.value(QStringLiteral("validUntil")).toDateTime();
    const Type      type        = static_cast<Type>(values.value(QStringLiteral("type")).toInt());

    if (validUntil.isValid()) {
        validUntil.setTimeZone(Cutelyst::Session::value(c, QStringLiteral("tz")).value<QTimeZone>());
        validUntil = validUntil.toUTC();
    }

    const auto now = QDateTime::currentDateTimeUtc();

    QSqlDatabase db = Cutelyst::Sql::databaseThread();
    Cutelyst::Sql::Transaction dbtrans(db);

    QSqlQuery q(db);

    if (!newPassword.isEmpty()) {
        const QString newPwEnc = CredentialBotan::createArgon2Password(newPassword);
        if (newPwEnc.isEmpty()) {
            e = Error(Cutelyst::Response::InternalServerError, c->translate("User", "Failed to encrypt new password."));
            qCCritical(MEL_CORE) << "Failed to encrypt new password for" << *this;
            return false;
        }

        q = CPreparedSqlQueryThread(QStringLiteral("UPDATE users SET password = :password, updated_at = :updated_at, email = :email, valid_until = :valid_until, type = :type WHERE id = :id"));
        q.bindValue(QStringLiteral(":password"), newPwEnc);
    } else {
        q = CPreparedSqlQueryThread(QStringLiteral("UPDATE users SET updated_at = :updated_at, email = :email, valid_until = :valid_until, type = :type WHERE id = :id"));
    }
    q.bindValue(QStringLiteral(":updated_at"), now);
    q.bindValue(QStringLiteral(":email"), email);
    q.bindValue(QStringLiteral(":valid_until"), validUntil);
    q.bindValue(QStringLiteral(":type"), static_cast<int>(type));
    q.bindValue(QStringLiteral(":id"), id());

    if (Q_UNLIKELY(!q.exec())) {
        e = Error(q, c->translate("User", "Failed to update user data in the database."));
        return false;
    }

    const QStringList settingsKeys({QStringLiteral("language"), QStringLiteral("timezone")});

    for (const QString &key : settingsKeys) {
        q = CPreparedSqlQueryThread(QStringLiteral("INSERT INTO usersettings (user_id, name, value) "
                                                   "VALUES (:user_id, :name, :value) "
                                                   "ON DUPLICATE KEY UPDATE "
                                                   "value = :value"));
        q.bindValue(QStringLiteral(":user_id"), id());
        q.bindValue(QStringLiteral(":name"), key);
        q.bindValue(QStringLiteral(":value"), values.value(key).toString());

        if (Q_UNLIKELY(!q.exec())) {
            e = Error(q, c->translate("User", "Failed to update settings key “%1” in the database.").arg(key));
            return false;
        }
    }

    if (!dbtrans.commit()) {
        e = Error(db.lastError(), c->translate("User", "Failed to update user settings in the database."));
        return false;
    }

    QVariantMap newSettings;
    for (const QString &key : settingsKeys) {
        newSettings.insert(key, values.value(key));
    }
    setSettings(newSettings);

    setUpdated(now);
    setEmail(email);
    setType(type);
    setValidUntil(validUntil);

    if (MeldariConfig::useMemcached()) {
        Cutelyst::Memcached::setByKey<User>(QStringLiteral(MEMC_USERS_GROUP_KEY), QString::number(id()), *this, MEMC_USERS_STORAGE_DURATION);
    }

    return true;
}

bool User::updateLastSeen(Cutelyst::Context *c, Error &e)
{
    const auto now = QDateTime::currentDateTimeUtc();

    QSqlQuery q = CPreparedSqlQueryThread(QStringLiteral("UPDATE users SET last_seen = :last_seen WHERE id = :id"));
    q.bindValue(QStringLiteral(":last_seen"), now);
    q.bindValue(QStringLiteral(":id"), id());

    if (Q_UNLIKELY(!q.exec())) {
        e = Error(q, c->translate("User", "Failed to update last seen state for user “%1”.").arg(username()));
        qCWarning(MEL_CORE) << "Failed to update last seen state for" << *this;
        return false;
    }

    setLastSeen(now);

    if (MeldariConfig::useMemcached()) {
        Cutelyst::Memcached::setByKey<User>(QStringLiteral(MEMC_USERS_GROUP_KEY), QString::number(id()), *this, MEMC_USERS_STORAGE_DURATION);
    }

    return true;
}

bool User::remove(Cutelyst::Context *c, Error &e) const
{
    if (MeldariConfig::useMemcached()) {
        Cutelyst::Memcached::MemcachedReturnType rt;
        if (Q_UNLIKELY(!Cutelyst::Memcached::removeByKey(QStringLiteral(MEMC_USERS_GROUP_KEY), QString::number(id()), &rt))) {
            e = Error(Cutelyst::Response::InternalServerError, c->translate("User", "Failed to remove user %1 (ID: %2) from the memcache: %3").arg(username(), QString::number(id()), Cutelyst::Memcached::errorString(c, rt)));
            return false;
        }
    }

    QSqlQuery q = CPreparedSqlQueryThread(QStringLiteral("DELETE FROM users WHERE id = :id"));
    q.bindValue(QStringLiteral(":id"), id());

    if (Q_UNLIKELY(!q.exec())) {
        e = Error(q, c->translate("User", "Failed to remove user %1 (ID: %2) from the database.").arg(username(), QString::number(id())));
        qCCritical(MEL_CORE) << "Failed to remove" << *this << "from the database:" << q.lastError().text();
        return false;
    }

    qCInfo(MEL_CORE) << User::fromStash(c) << "removed" << *this;

    return true;
}

User User::get(Cutelyst::Context *c, Error &e, dbid_t id)
{
    User u;

    if (MeldariConfig::useMemcached()) {
        Cutelyst::Memcached::MemcachedReturnType rt;
        u = Cutelyst::Memcached::getByKey<User>(QStringLiteral(MEMC_USERS_GROUP_KEY), QString::number(id), nullptr, &rt);
        if (rt == Cutelyst::Memcached::Success) {
            qCDebug(MEL_CORE) << "Found user with ID" << id << "in memcache";
            return u;
        }
    }

    qCDebug(MEL_CORE) << "Query user with ID" << id << "from the database";

    QSqlQuery q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT u1.type, u1.username, u1.email, u1.created_at, u1.updated_at, u1.valid_until, u1.last_seen, u1.locked_at, u2.id AS locked_by_id, u2.username AS locked_by_username FROM users u1 LEFT JOIN users u2 ON u2.id = u1.locked_by WHERE u1.id = :id"));
    q.bindValue(QStringLiteral(":id"), id);

    if (Q_UNLIKELY(!q.exec())) {
        e = Error(q, c->translate("User", "Failed to get user with ID %s from database.").arg(id));
        qCCritical(MEL_CORE) << "Failed to get user with ID" << id << "from database:" << q.lastError().text();
        return u;
    }

    if (Q_UNLIKELY(!q.next())) {
        e = Error(Cutelyst::Response::NotFound, c->translate("User", "Can not find user with ID %s in the database.").arg(id));
        qCWarning(MEL_CORE) << "Can not find user ID" << id << "in the database";
        return u;
    }

    const User::Type type = static_cast<User::Type>(q.value(0).toInt());
    const QString username = q.value(1).toString();
    const QString email = q.value(2).toString();
    const QDateTime created = q.value(3).toDateTime();
    const QDateTime updated = q.value(4).toDateTime();
    const QDateTime validUntil = q.value(5).toDateTime();
    const QDateTime lastSeen = q.value(6).toDateTime();
    const QDateTime lockedAt = q.value(7).toDateTime();
    const User::dbid_t lockedById = q.value(8).toUInt();
    const QString lockedByName = q.value(9).toString();

    q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT name, value FROM usersettings WHERE user_id = :user_id"));
    q.bindValue(QStringLiteral(":user_id"), id);

    if (Q_UNLIKELY(!q.exec())) {
        e = Error(q, c->translate("User", "Failed to get usersettings for user ID %s from database.").arg(id));
        qCCritical(MEL_CORE) << "Failed to get usersettings for user ID" << id << "from database:" << q.lastError().text();
        return u;
    }

    QVariantMap settings;
    while (q.next()) {
        settings.insert(q.value(0).toString(), q.value(1).toString());
    }

    u = User(id, type, username, email, created, updated, validUntil, lastSeen, lockedAt, lockedById, lockedByName, settings);

    if (MeldariConfig::useMemcached()) {
        Cutelyst::Memcached::setByKey<User>(QStringLiteral(MEMC_USERS_GROUP_KEY), QString::number(id), u, MEMC_USERS_STORAGE_DURATION);
    }

    return u;
}

bool User::toStash(Context *c, Error &e, dbid_t id)
{
    User u = User::get(c, e, id);
    if (!u.isNull()) {
        u.toStash(c);
        return true;
    }
    return false;
}

QString User::typeTranslated(Cutelyst::Context *c, Type type)
{
    Q_ASSERT(c);

    switch (type) {
    case Invalid:
        //: user type
        return c->translate("User", "Invalid");
    case Disabled:
        //: user type
        return c->translate("User", "Disabled");
    case Registered:
        //: user type
        return c->translate("User", "Registered");
    case Administrator:
        //: user type
        return c->translate("User", "Administrator");
    case SuperUser:
        //: user type
        return c->translate("User", "Super User");
    }

    return QString();
}

QJsonObject User::typesTranslated(Cutelyst::Context *c)
{
    QJsonObject o;

    const QMetaObject mo = User::staticMetaObject;
    const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Type"));

    for (int i = 0; i < me.keyCount(); ++i) {
        o.insert(QString::number(me.value(i)), User::typeTranslated(c, static_cast<User::Type>(me.value(i))));
    }

    return o;
}

std::vector<OptionItem> User::typeOptions(Cutelyst::Context *c, Type selected)
{
    std::vector<OptionItem> opts;

    const QMetaObject mo = User::staticMetaObject;
    const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Type"));

    opts.reserve(me.keyCount() - 1);

    for (int i = 1; i < me.keyCount(); ++i) {
        const int v = me.value(i);
        const Type t = static_cast<User::Type>(v);
        opts.emplace_back(User::typeTranslated(c, t), v, t == selected);
    }

    return opts;
}

std::vector<OptionItem> User::typeOptions(Context *c, Type selected, Type below)
{
    std::vector<OptionItem> opts;

    if (below == User::SuperUser) {
        opts = typeOptions(c, selected);
    } else {
        const QMetaObject mo = User::staticMetaObject;
        const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Type"));

        for (int i = 1; i < me.keyCount(); ++i) {
            const int v = me.value(i);
            const Type t = static_cast<User::Type>(v);
            if (t < below) {
                opts.emplace_back(User::typeTranslated(c, t), v, t == selected);
            }
        }
    }

    return opts;
}

User::dbid_t User::stringToDbId(const QString &str, bool *ok, Cutelyst::Context *c, bool detach)
{
    Q_ASSERT(ok);

    const qulonglong _id = str.toULongLong(ok);

    if (*ok) {
        if (_id > static_cast<qulonglong>(std::numeric_limits<User::dbid_t>::max())) {
            *ok = false;
            Error::toStash(c, Response::BadRequest, c->translate("User", "Invalid user ID."), detach);
            return 0;
        }
    } else {
        Error::toStash(c, Response::BadRequest, c->translate("User", "Invalid user ID."), detach);
        return 0;
    }

    return static_cast<User::dbid_t>(_id);
}

QDebug operator<<(QDebug dbg, const User &user)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver)
    dbg.nospace() << "User(";
    dbg << "ID: " << user.id();
    dbg << ", Username: " << user.username();
    dbg << ", Type: " << user.type();
    dbg << ')';
    return dbg.maybeSpace();
}

#include "moc_user.cpp"
