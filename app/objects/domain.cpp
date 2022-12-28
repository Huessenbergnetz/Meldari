/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "domain.h"
#include "error.h"

#include <Cutelyst/Context>

#include <QMetaObject>
#include <QMetaEnum>

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
