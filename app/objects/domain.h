/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_DOMAIN_H
#define MELDARI_DOMAIN_H

#include <basedomain.h>
#include <optionitem.h>

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include <vector>

namespace Cutelyst {
class Context;
}

class Error;

class Domain : public BaseDomain
{
    Q_GADGET
public:
    Domain();
    Domain(dbid_t id, const QString &name, BaseDomain::Status status, BaseDomain::dbid_t ownerId, const QString &ownerName, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName);
    Domain(const Domain &other);
    Domain(Domain &&other) noexcept;
    Domain &operator=(const Domain &other);
    Domain &operator=(Domain &&other) noexcept;
    ~Domain();

    void toStash(Cutelyst::Context *c) const;

    static Domain fromStash(Cutelyst::Context *c);

    static Domain add(Cutelyst::Context *c, Error &e, const QVariantHash &values);

    static std::vector<Domain> list(Cutelyst::Context *c, Error &e);

    static QJsonArray listJson(Cutelyst::Context *c, Error &e);

    static QString statusTranslated(Cutelyst::Context *c, Status status);

    static QJsonObject statiTranslated(Cutelyst::Context *c);

    static std::vector<OptionItem> statusOptions(Cutelyst::Context *c, Status selected);

    static dbid_t stringToDbId(const QString &str, bool *ok, Cutelyst::Context *c, bool detachOnError = false);
};

Q_DECLARE_METATYPE(Domain)
Q_DECLARE_TYPEINFO(Domain, Q_MOVABLE_TYPE);

QDebug operator<<(QDebug dbg, const Domain &domain);

#endif // MELDARI_DOMAIN_H
