/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "melvalidatorpwcheck.h"
#include "objects/user.h"
#include "logging.h"
#include "credentialbotan.h"
#include "userauthstoresql.h"
#include <Cutelyst/Context>
#include <limits>

MelValidatorPwCheck::MelValidatorPwCheck(const QString &field, const QString &username, const ValidatorMessages &messages)
    : ValidatorRule{field, messages, QString()}, m_userStore{new UserAuthStoreSql}, m_username{username}
{

}

MelValidatorPwCheck::~MelValidatorPwCheck()
{
    delete m_userStore;
}

ValidatorReturnType MelValidatorPwCheck::validate(Context *c, const ParamsMultiMap &params) const
{
    ValidatorReturnType result;

    const QString v = value(params);

    if (!v.isEmpty()) {
        QString uname = params.value(m_username);
        if (uname.isEmpty()) {
            uname = c->stash(m_username).toString();
            if (uname.isEmpty()) {
                uname = m_username;
            }
        }

        const auto user = m_userStore->findUser(c, ParamsMultiMap({{QStringLiteral("username"), uname}}));

        if (user.isNull()) {
            result.errorMessage = validationDataError(c);
            return result;
        }

        if (!CredentialBotan::validatePassword(v, user.value(QStringLiteral("password")).toString())) {
            result.errorMessage = validationError(c);
        }
    }

    return result;
}

QString MelValidatorPwCheck::genericValidationError(Context *c, const QVariant &errorData) const
{
    QString error;

    const QString _label = label(c);
    if (_label.isEmpty()) {
        error = c->translate("MelValidatorPwCheck", "The password does not match the stored password.");
    } else {
        error = c->translate("MelValidatorPwCheck", "The password in the “%1” field does not match the stored password.").arg(_label);
    }

    return error;
}

QString MelValidatorPwCheck::genericValidationDataError(Context *c, const QVariant &errorData) const
{
    QString error;
    Q_UNUSED(errorData)

    const QString _label = label(c);
    if (_label.isEmpty()) {
        error = c->translate("MelValidatorPwCheck", "Can not find user in the database.");
    } else {
        error = c->translate("MelValidatorPwCheck", "Can not find user for the “%1” field in the database.").arg(_label);
    }

    return error;
}
