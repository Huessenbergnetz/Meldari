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

    QSqlQuery q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT id, type, password, email, created_at, updated_at, valid_until, last_seen, locked_at, locked_by FROM users WHERE username = :username"));
    q.bindValue(QStringLiteral(":username"), username);

    if (Q_LIKELY(q.exec())) {
        if (Q_LIKELY(q.next())) {
            user.setId(q.value(0));
            user.insert(QStringLiteral("username"), username);
            user.insert(QStringLiteral("type"), q.value(1));
            user.insert(QStringLiteral("password"), q.value(2));
            user.insert(QStringLiteral("email"), q.value(3));
            user.insert(QStringLiteral("created_at"), q.value(4));
            user.insert(QStringLiteral("updated_at"), q.value(5));
            user.insert(QStringLiteral("valid_until"), q.value(6));
            user.insert(QStringLiteral("last_seen"), q.value(7));
            user.insert(QStringLiteral("locked_at"), q.value(8));
            user.insert(QStringLiteral("locked_by"), q.value(9));

            QSqlQuery qq = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT name, value FROM usersettings WHERE user_id = :user_id"));
            qq.bindValue(QStringLiteral(":user_id"), user.id());

            if (Q_LIKELY(qq.exec())) {
                QVariantMap settings;
                while (qq.next()) {
                    settings.insert(qq.value(0).toString(), qq.value(1));
                }
                user.insert(QStringLiteral("settings"), settings);
            } else {
                qCCritical(MEL_AUTHN) << "Failed to execute database query to get usersettings for user" << username << "from the database:" << qq.lastError().text();
            }

        } else {
            qCWarning(MEL_AUTHN, "Login failed: username %s not found. IP: %s", qUtf8Printable(username), qUtf8Printable(c->req()->addressString()));
        }
    } else {
        qCCritical(MEL_AUTHN) << "Failed to execute database query to get user" << username << "from the database:" << q.lastError().text();
    }

    return user;
}
