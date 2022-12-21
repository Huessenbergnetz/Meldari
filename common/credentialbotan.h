/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_CREDENTIALBOTAN_H
#define MELDARICOMMON_CREDENTIALBOTAN_H

#include "meldaricommon_export.h"
#include <Cutelyst/Plugins/Authentication/authentication.h>

using namespace Cutelyst;

class CredentialBotanPrivate;
class MELDARICOMMON_EXPORT CredentialBotan final : public AuthenticationCredential
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CredentialBotan)
public:
    enum PasswordType {
        Invalid,
        Argon2d,
        Argon2i,
        Argon2id,
        Bcrypt
    };
    Q_ENUM(PasswordType)

    explicit CredentialBotan(QObject *parent = nullptr);
    ~CredentialBotan() final;

    AuthenticationUser authenticate(Context *c, AuthenticationRealm *realm, const ParamsMultiMap &authinfo) final;

    QString passwordField() const;

    void setPasswordField(const QString &fieldName);

    QString passwordPreSalt() const;

    void setPasswordPreSalt(const QString &passwordPreSalt);

    QString passwordPostSalt() const;

    void setPasswordPostSalt(const QString &passwordPostSalt);

    static bool validatePassword(const QByteArray &password, const QByteArray &correctHash);

    inline static bool validatePassword(const QString &password, const QString &correctHash);

    static QByteArray createArgon2Password(const QByteArray &password, PasswordType type, size_t parallelization, size_t memory, size_t iterations, size_t saltLength, size_t outputLength);

    static QByteArray createArgon2Password(const QByteArray &password);

    inline static QString createArgon2Password(const QString &password);

    static QByteArray createBcryptPassword(const QByteArray &password, uint16_t workFactor, char version);

    static QByteArray createBcryptPassword(const QByteArray &password);

    inline static QString createBcryptPassword(const QString &password);

protected:
    CredentialBotanPrivate *d_ptr;
};

inline bool CredentialBotan::validatePassword(const QString &password, const QString &correctHash)
{
    return validatePassword(password.toUtf8(), correctHash.toUtf8());
}

inline QString CredentialBotan::createArgon2Password(const QString &password)
{
    return QString::fromLatin1(CredentialBotan::createArgon2Password(password.toUtf8()));
}

inline QString CredentialBotan::createBcryptPassword(const QString &password)
{
    return QString::fromLatin1(CredentialBotan::createBcryptPassword(password.toUtf8()));
}

#endif // MELDARICOMMON_CREDENTIALBOTAN_H
