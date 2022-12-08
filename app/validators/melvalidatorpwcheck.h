/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELVALIDATORPWCHECK_H
#define MELVALIDATORPWCHECK_H

#include <Cutelyst/Plugins/Utils/ValidatorRule>

using namespace Cutelyst;

class UserAuthStoreSql;

class MelValidatorPwCheck final : public ValidatorRule
{
public:
    explicit MelValidatorPwCheck(const QString &field, const QString &username, const Cutelyst::ValidatorMessages &messages = Cutelyst::ValidatorMessages());

    ~MelValidatorPwCheck() final;

protected:
    ValidatorReturnType validate(Context *c, const ParamsMultiMap &params) const final;

    QString genericValidationError(Context *c, const QVariant &errorData) const final;

    QString genericValidationDataError(Context *c, const QVariant &errorData) const final;

private:
    UserAuthStoreSql *m_userStore = nullptr;
    QString m_username;
};

#endif // MELVALIDATORPWCHECK_H
