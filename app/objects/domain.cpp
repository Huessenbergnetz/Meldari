/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "domain.h"
#include "error.h"
#include "logging.h"
#include "user.h"

#include <Cutelyst/Context>
#include <Cutelyst/Plugins/Utils/Sql>

#include <QMetaObject>
#include <QMetaEnum>
#include <QSqlQuery>
#include <QSqlError>

#include <limits>

#define DOMAIN_STASH_KEY "domain"
#define MEMC_DOMAINS_GROUP_KEY "domains"
#define MEMC_DOMAINS_STORAGE_DURATION 7200

Domain::Domain() : BaseDomain()
{

}

Domain::Domain(dbid_t id, const QString &name, BaseDomain::Status status, BaseUser::dbid_t ownerId, const QString &ownerName, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName)
    : BaseDomain{id, name, status, ownerId, ownerName, created, updated, validUntil, lockedAt, lockedById, lockedByName}
{

}

Domain::Domain(const Domain &other) = default;

Domain::Domain(Domain &&other) noexcept = default;

Domain &Domain::operator=(const Domain &other) = default;

Domain &Domain::operator=(Domain &&other) noexcept = default;

Domain::~Domain() = default;

void Domain::toStash(Cutelyst::Context *c) const
{
    Q_ASSERT(c);
    c->setStash(QStringLiteral(DOMAIN_STASH_KEY), QVariant::fromValue<Domain>(*this));
}

Domain Domain::fromStash(Cutelyst::Context *c)
{
    Q_ASSERT(c);
    return c->stash(QStringLiteral(DOMAIN_STASH_KEY)).value<Domain>();
}

Domain Domain::add(Cutelyst::Context *c, Error &e, const QVariantHash &values)
{
    Domain dom;

    const QString   name    = values.value(QStringLiteral("name")).toString();
    const Status    status  = static_cast<Status>(values.value(QStringLiteral("status")).toInt());
    const QDateTime now     = QDateTime::currentDateTimeUtc();

    qCDebug(MEL_CORE) << "Adding new domain" << name;

    const User u = User::fromStash(c);

    QSqlQuery q = CPreparedSqlQueryThread(QStringLiteral("INSERT INTO domains (owner_id, name, status, created_at, updated_at) "
                                                         "VALUES (:owner_id, :name, :status, :created_at, :updated_at)"));
    q.bindValue(QStringLiteral(":owner_id"), u.id());
    q.bindValue(QStringLiteral(":name"), name);
    q.bindValue(QStringLiteral(":status"), static_cast<int>(status));
    q.bindValue(QStringLiteral(":created_at"), now);
    q.bindValue(QStringLiteral(":updated_at"), now);

    if (Q_UNLIKELY(!q.exec())) {
        e = Error(q, c->translate("Domain", "Failed to insert new domain “%1” into database.").arg(name));
        qCCritical(MEL_CORE) << "Failed to insert new domain" << name << "into databse:" << q.lastError().text();
        return dom;
    }

    const dbid_t id = q.lastInsertId().toUInt();

    dom = Domain(id, name, status, u.id(), u.username(), now, now, QDateTime(), QDateTime(), 0, QString());

    qCInfo(MEL_CORE) << u << "created new" << dom;

    return dom;
}

QString Domain::statusTranslated(Cutelyst::Context *c, Status status)
{
    Q_ASSERT(c);

    switch(status) {
    case Invalid:
        //: domain status
        return c->translate("Domain", "Invalid");
    case Disabled:
        //: domain status
        return c->translate("Domain", "Disabled");
    case Enabled:
        //: domain status
        return c->translate("Domain", "Enabled");
    }

    return QString();
}

QJsonObject Domain::statiTranslated(Cutelyst::Context *c)
{
    QJsonObject o;

    const QMetaObject mo = Domain::staticMetaObject;
    const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Status"));

    for (int i = 0; i < me.keyCount(); ++i) {
        o.insert(QString::number(me.value(i)), Domain::statusTranslated(c, static_cast<Domain::Status>(me.value(i))));
    }

    return o;
}

std::vector<OptionItem> Domain::statusOptions(Cutelyst::Context *c, Status selected)
{
    std::vector<OptionItem> opts;

    const QMetaObject mo = Domain::staticMetaObject;
    const QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Status"));

    opts.reserve(me.keyCount() - 1);

    for (int i = 1; i < me.keyCount(); ++i) {
        const int v = me.value(i);
        const Status s = static_cast<Domain::Status>(v);
        opts.emplace_back(Domain::statusTranslated(c, s), v, s == selected);
    }

    return opts;
}

Domain::dbid_t Domain::stringToDbId(const QString &str, bool *ok, Cutelyst::Context *c, bool detachOnError)
{
    Q_ASSERT(ok);

    const qulonglong _id = str.toULongLong(ok);

    if (*ok) {
        if (_id > static_cast<qulonglong>(std::numeric_limits<Domain::dbid_t>::max())) {
            *ok = false;
            Error::toStash(c, Cutelyst::Response::BadRequest, c->translate("Domain", "Invalid domain ID."), detachOnError);
            return 0;
        }
    } else {
        Error::toStash(c, Cutelyst::Response::BadRequest, c->translate("Domain", "Invalid domain ID."), detachOnError);
        return 0;
    }

    return static_cast<Domain::dbid_t>(_id);
}

QDebug operator<<(QDebug dbg, const Domain &domain)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver)
    dbg.nospace() << "Domain(";
    dbg << "ID: " << domain.id();
    dbg << ", Name: " << domain.name();
    dbg << ", Status: " << domain.status();
    dbg << ')';
    return dbg.maybeSpace();
}

#include "moc_domain.cpp"
