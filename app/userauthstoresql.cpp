/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "userauthstoresql.h"
#include "logging.h"

#include <Cutelyst/Context>
#include <Cutelyst/Request>
#include <Cutelyst/Plugins/Utils/Sql>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(MEL_AUTHN, "meldari.authn")
Q_LOGGING_CATEGORY(MEL_AUTHZ, "meldari.authz")
#else
Q_LOGGING_CATEGORY(MEL_AUTHN, "meldari.authn", QtInfoMsg)
Q_LOGGING_CATEGORY(MEL_AUTHZ, "meldari.authz", QtInfoMsg)
#endif

UserAuthStoreSql::UserAuthStoreSql(QObject *parent)
    : AuthenticationStore{parent}
{

}

AuthenticationUser UserAuthStoreSql::findUser(Context *c, const ParamsMultiMap &userinfo)
{
    AuthenticationUser user;

    const QString username = userinfo.value(QStringLiteral("username"));

    QSqlQuery q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT id, type, password FROM users WHERE username = :username"));
    q.bindValue(QStringLiteral(":username"), username);

    if (Q_LIKELY(q.exec())) {
        if (Q_LIKELY(q.next())) {
            user.setId(q.value(0));
            user.insert(QStringLiteral("username"), username);
            user.insert(QStringLiteral("type"), q.value(1));
            user.insert(QStringLiteral("password"), q.value(2));

        } else {
            qCWarning(MEL_AUTHN, "Login failed: username %s not found. IP: %s", qUtf8Printable(username), qUtf8Printable(c->req()->addressString()));
        }
    } else {
        qCCritical(MEL_AUTHN) << "Failed to execute database query to get user" << username << "from the database:" << q.lastError().text();
    }

    return user;
}
