/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "melvalidatorpwcheck.h"
#include "objects/user.h"
#include "logging.h"
#include "credentialbotan.h"
#include <Cutelyst/Context>
#include <Cutelyst/Plugins/Utils/Sql>
#include <QSqlError>
#include <QSqlQuery>
#include <limits>

MelValidatorPwCheck::MelValidatorPwCheck(const QString &field, const QString &usernameOrId, const ValidatorMessages &messages)
    : ValidatorRule{field, messages, QString()}, m_userNameOrId{usernameOrId}
{

}

ValidatorReturnType MelValidatorPwCheck::validate(Context *c, const ParamsMultiMap &params) const
{
    ValidatorReturnType result;

    const QString v = value(params);

    if (!v.isEmpty()) {
        QString unameOrId = params.value(m_userNameOrId);
        if (unameOrId.isEmpty()) {
            unameOrId = c->stash(m_userNameOrId).toString();
            if (unameOrId.isEmpty()) {
                result.errorMessage = validationDataError(c, static_cast<int>(UsernameOrIdNotFound));
                return result;
            }
        }

        bool isId = false;
        const qulonglong _id = unameOrId.toULongLong(&isId);
        if (isId) {
            if (_id > static_cast<qulonglong>(std::numeric_limits<User::dbid_t>::max())) {
                result.errorMessage = validationDataError(c, static_cast<int>(IdOutOfRange));
                return result;
            }
        }

        QSqlQuery q;

        if (isId) {
            q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT password FROM users WHERE id = :id"));
            q.bindValue(QStringLiteral(":id"), _id);
        } else {
            q = CPreparedSqlQueryThreadFO(QStringLiteral("SELECT password FROM users WHERE username = :username"));
            q.bindValue(QStringLiteral(":username"), unameOrId);
        }

        if (Q_UNLIKELY(!q.exec())) {
            qCCritical(MEL_CORE) << "Failed to get password for user" << (isId ? "ID" : "") << unameOrId << "from the database:" << q.lastError().text();
            result.errorMessage = validationDataError(c, static_cast<int>(SqlFailed));
            return result;
        }

        if (Q_UNLIKELY(!q.next())) {
            qCWarning(MEL_CORE) << "Can not find user" << (isId ? "ID" : "") << unameOrId << "in the database";
            result.errorMessage = validationDataError(c, static_cast<int>(UserNotFound));
            return result;
        }

        const QString hashedPassword = q.value(0).toString();

        if (!CredentialBotan::validatePassword(v, hashedPassword)) {
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

    const QString _label = label(c);
    const ValidationDataErrors errorType = static_cast<ValidationDataErrors>(errorData.toInt());
    if (errorType == UsernameOrIdNotFound) {
        if (_label.isEmpty()) {
            error = c->translate("MelValidatorPwCheck", "Neither the username nor the database ID of the user whose password is to be checked can be found.");
        } else {
            error = c->translate("MelValidatorPwCheck", "Neither the username nor the database ID of the user whose password is to be checked in the “%1” field can be found.").arg(_label);
        }
    } else if (errorType == IdOutOfRange) {
        if (_label.isEmpty()) {
            error = c->translate("MelValidatorPwCheck", "The database ID of the user whose password is to be checked exceeds the allowed range of values.");
        } else {
            error = c->translate("MelValidatorPwCheck", "The database ID of the user whose password is to be checked in the “%1” field exceeds the allowed range of values.").arg(_label);
        }
    } else if (errorType == SqlFailed) {
        if (_label.isEmpty()) {
            error = c->translate("MelValidatorPwCheck", "Failed to get password to check from the database.");
        } else {
            error = c->translate("MelValidatorPwCheck", "Failed to get password to check in the “%1“ field from the database.").arg(_label);
        }
    } else if (errorType == UserNotFound) {
        if (_label.isEmpty()) {
            error = c->translate("MelValidatorPwCheck", "Can not find user in the database.");
        } else {
            error = c->translate("MelValidatorPwCheck", "Can not find user for the “%1” field in the database.").arg(_label);
        }
    }

    return error;
}
