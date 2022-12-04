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
#include <Cutelyst/Plugins/Authentication/authentication.h>
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

User::User(const Cutelyst::AuthenticationUser &user) : BaseUser()
{
    data = new BaseUserData;
    data->id = user.id().toUInt();
    data->type = static_cast<BaseUser::Type>(user.value(QStringLiteral("type")).toInt());
    data->username = user.value(QStringLiteral("username")).toString();
    data->email = user.value(QStringLiteral("email")).toString();
    data->created = user.value(QStringLiteral("created_at")).toDateTime();
    data->created.setTimeSpec(Qt::UTC);
    data->updated = user.value(QStringLiteral("updated_at")).toDateTime();
    data->updated.setTimeSpec(Qt::UTC);
    data->validUntil = user.value(QStringLiteral("valid_until")).toDateTime();
    if (data->validUntil.isValid()) {
        data->validUntil.setTimeSpec(Qt::UTC);
    }
    data->lastSeen = user.value(QStringLiteral("last_seen")).toDateTime();
    if (data->lastSeen.isValid()) {
        data->lastSeen.setTimeSpec(Qt::UTC);
    }
    data->lockedAt = user.value(QStringLiteral("locked_at")).toDateTime();
    if (data->lockedAt.isValid()) {
        data->lockedAt.setTimeSpec(Qt::UTC);
    }
    data->lockedById = user.value(QStringLiteral("locked_by")).toUInt();
    data->settings = user.value(QStringLiteral("settings")).toMap();
}

User::User(const User &other) = default;

User::User(User &&other) noexcept = default;

User &User::operator=(const User &other) = default;

User &User::operator=(User &&other) noexcept = default;

User::~User() = default;

bool User::operator==(const User &other) const noexcept
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

void User::toStash(Cutelyst::Context *c)
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
        e = Error(q, c->translate("User", "Failed to insert new user “%s“ into database.").arg(username));
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
            e = Error(q2, c->translate("User", "Failed to setting key “%1” into the database.").arg(key));
            qCCritical(MEL_CORE) << "Failed to insert setting key" << key << "for user" << username << "into the database:" << q2.lastError().text();
            return u;
        }
    }

    if (Q_UNLIKELY(!dbtrans.commit())) {
        e = Error(db.lastError(), c->translate("User", "Failed to insert new user “%s“ into database.").arg(username));
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

    bool pwChanged = false;

    const QString oldPassword   = values.value(QStringLiteral("password")).toString();
    const QString newPassword   = values.value(QStringLiteral("newpassword")).toString();
    const QString language      = values.value(QStringLiteral("language")).toString();
    const QString timezone      = values.value(QStringLiteral("timezone")).toString();

    if (!newPassword.isEmpty()) {
        if (oldPassword != newPassword) {
            pwChanged = true;
            if (oldPassword.isEmpty()) {
                e = Error(Cutelyst::Response::Forbidden, c->translate("User", "Can not update password: empty current password."));
                return false;
            }

            Cutelyst::AuthenticationUser authUser = Cutelyst::Authentication::user(c);
            if (!CredentialBotan::validatePassword(oldPassword.toUtf8(), authUser.value(QStringLiteral("password")).toString().toUtf8())) {
                e = Error(Cutelyst::Response::Forbidden, c->translate("User", "Can not update password: invalid current password."));
                return false;
            }
        }
    }

    const auto now = QDateTime::currentDateTimeUtc();

    QSqlDatabase db = Cutelyst::Sql::databaseThread();
    Cutelyst::Sql::Transaction dbtrans(db);

    QSqlQuery q(db);

    if (!newPassword.isEmpty()) {
        const QString newPwEnc = CredentialBotan::createArgon2Password(newPassword);
        if (newPwEnc.isEmpty()) {
            e = Error(Cutelyst::Response::InternalServerError, c->translate("User", "Failed to encrypt new password."));
            qCCritical(MEL_CORE, "Failed to encrypt new password for user '%s' (ID: %i)", qUtf8Printable(data->username), data->id);
            return false;
        }

        q = CPreparedSqlQueryThread(QStringLiteral("UPDATE users SET password = :password, updated_at = :updated_at WHERE id = :id"));
        q.bindValue(QStringLiteral(":password"), newPwEnc);
    } else {
        q = CPreparedSqlQueryThread(QStringLiteral("UPDATE users SET updated_at = :updated_at WHERE id = :id"));
    }
    q.bindValue(QStringLiteral(":updated_at"), now);
    q.bindValue(QStringLiteral(":id"), data->id);

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
        q.bindValue(QStringLiteral(":user_id"), data->id);
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

    for (const QString &key : settingsKeys) {
        data->settings.insert(key, values.value(key));
    }

    data->updated = now;

    QLocale ln(language);
    Cutelyst::Session::setValue(c, QStringLiteral("lang"), QVariant::fromValue<QLocale>(ln));
    QTimeZone tz(timezone.toLatin1());
    Cutelyst::Session::setValue(c, QStringLiteral("tz"), QVariant::fromValue<QTimeZone>(tz));

    if (MeldariConfig::useMemcached()) {
        Cutelyst::Memcached::setByKey<User>(QStringLiteral(MEMC_USERS_GROUP_KEY), QString::number(data->id), *this, MEMC_USERS_STORAGE_DURATION);
    }

    if (pwChanged) {
        ParamsMultiMap userParams;
        userParams.insert(QStringLiteral("username"), data->username);
        userParams.insert(QStringLiteral("password"), newPassword);
        Cutelyst::Authentication::authenticate(c, userParams, QStringLiteral("users"));
    }

    return true;
}

bool User::updateLastSeen(Context *c, Error &e)
{
    const auto now = QDateTime::currentDateTimeUtc();

    QSqlQuery q = CPreparedSqlQueryThread(QStringLiteral("UPDATE users SET last_seen = :last_seen WHERE id = :id"));
    q.bindValue(QStringLiteral(":last_seen"), now);
    q.bindValue(QStringLiteral(":id"), data->id);

    if (Q_UNLIKELY(!q.exec())) {
        e = Error(q, c->translate("User", "Failed to update last seen state for user “%1”.").arg(data->username));
        qCWarning(MEL_CORE) << "Failed to update last seen state for user" << data->username << "(ID:" << data->id << ')';
        return false;
    }

    data->lastSeen = now;

    if (MeldariConfig::useMemcached()) {
        Cutelyst::Memcached::setByKey<User>(QStringLiteral(MEMC_USERS_GROUP_KEY), QString::number(data->id), *this, MEMC_USERS_STORAGE_DURATION);
    }

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

QStringList User::typeValues()
{
    QStringList list;

    const QMetaObject mo = User::staticMetaObject;
    const QMetaEnum   me = mo.enumerator(mo.indexOfEnumerator("Type"));
    list.reserve(me.keyCount() - 1);
    for (int i = 1; i < me.keyCount(); ++i) {
        list << QString::number(me.value(i));
    }

    return list;
}

QStringList User::typeValues(User::Type below)
{
    QStringList list;

    if (below == User::SuperUser) {
        list = User::typeValues();
    } else {
        const QMetaObject mo = User::staticMetaObject;
        const QMetaEnum   me = mo.enumerator(mo.indexOfEnumerator("Type"));
        for (int i = 1; i < me.keyCount(); ++i) {
            const int val = me.value(i);
            if (val < static_cast<int>(below)) {
                list << QString::number(val); //clazy:exclude=reserve-candidates
            }
        }
    }

    return list;
}

User::dbid_t User::stringToDbId(const QString &str, bool *ok)
{
    Q_ASSERT(ok);
    return str.toUInt(ok);
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
           << user.data->lockedById
           << user.data->lockedByName
           << user.data->settings;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, User &user)
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

#include "moc_user.cpp"
