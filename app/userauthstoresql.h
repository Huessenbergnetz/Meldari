/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_USERAUTHSTORESQL_H
#define MELDARI_USERAUTHSTORESQL_H

#include <Cutelyst/Plugins/Authentication/authenticationstore.h>

using namespace Cutelyst;

class UserAuthStoreSql final : public AuthenticationStore
{
    Q_OBJECT
    Q_DISABLE_COPY(UserAuthStoreSql)
public:
    explicit UserAuthStoreSql(QObject *parent = nullptr);
    ~UserAuthStoreSql() final = default;

    AuthenticationUser findUser(Context *c, const ParamsMultiMap &userinfo) final;
};

#endif // MELDARI_USERAUTHSTORESQL_H
