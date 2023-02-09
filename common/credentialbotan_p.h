/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_CREDENTIALBOTAN_P_H
#define MELDARICOMMON_CREDENTIALBOTAN_P_H

#include "credentialbotan.h"

class CredentialBotanPrivate
{
public:
    bool checkPassword(const AuthenticationUser &user, const ParamsMultiMap &authinfo);
    static CredentialBotan::PasswordType checkPwType(const QByteArray &hashedPw);

    QString passwordField {QStringLiteral("password")};
    QString passwordPreSalt;
    QString passwordPostSalt;
};

#endif // MELDARICOMMON_CREDENTIALBOTAN_P_H
