/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "credentialbotan_p.h"
#include <Cutelyst/Plugins/Authentication/authenticationrealm.h>
#include <botan/argon2.h>
#include <botan/bcrypt.h>
#include <botan/system_rng.h>
#include <string>
#include <memory>

CredentialBotan::CredentialBotan(QObject *parent)
    : AuthenticationCredential{parent}, d_ptr(new CredentialBotanPrivate)
{

}

CredentialBotan::~CredentialBotan()
{
    delete d_ptr;
}

AuthenticationUser CredentialBotan::authenticate(Context *c, AuthenticationRealm *realm, const ParamsMultiMap &authinfo)
{
    AuthenticationUser user;
    Q_D(CredentialBotan);
    AuthenticationUser _user = realm->findUser(c, authinfo);
    if (!_user.isNull()) {
        if (d->checkPassword(_user, authinfo)) {
            user = _user;
        }
    }
    return user;
}

QString CredentialBotan::passwordField() const
{
    Q_D(const CredentialBotan);
    return d->passwordField;
}

void CredentialBotan::setPasswordField(const QString &fieldName)
{
    Q_D(CredentialBotan);
    d->passwordField = fieldName;
}

//CredentialBotan::PasswordType CredentialBotan::passwordType() const
//{
//    Q_D(const CredentialBotan);
//    return d->passwordType;
//}

//void CredentialBotan::setPasswordType(CredentialBotan::PasswordType type)
//{
//    Q_D(CredentialBotan);
//    d->passwordType = type;
//}

QString CredentialBotan::passwordPreSalt() const
{
    Q_D(const CredentialBotan);
    return d->passwordPreSalt;
}

void CredentialBotan::setPasswordPreSalt(const QString &passwordPreSalt)
{
    Q_D(CredentialBotan);
    d->passwordPreSalt = passwordPreSalt;
}

QString CredentialBotan::passwordPostSalt() const
{
    Q_D(const CredentialBotan);
    return d->passwordPostSalt;
}
void CredentialBotan::setPasswordPostSalt(const QString &passwordPostSalt)
{
    Q_D(CredentialBotan);
    d->passwordPostSalt = passwordPostSalt;
}

bool CredentialBotan::validatePassword(const QByteArray &password, const QByteArray &correctHash)
{
    const CredentialBotan::PasswordType type = CredentialBotanPrivate::checkPwType(correctHash);

    if (type == Argon2d || type == Argon2i || type == Argon2id) {

        const std::string hash = correctHash.toStdString();
        return Botan::argon2_check_pwhash(password.constData(), password.size(), hash);

    } else if (type == Bcrypt) {

        const std::string hash = correctHash.toStdString();
        const std::string pw = password.toStdString();
        return Botan::check_bcrypt(pw, hash);
    }

    return false;
}

QByteArray CredentialBotan::createArgon2Password(const QByteArray &password, PasswordType type, size_t parallelization, size_t memory, size_t iterations, size_t saltLength, size_t outputLength)
{
    uint8_t argonType = 0;
    switch (type) {
    case Argon2d:
        argonType = 0;
        break;
    case Argon2i:
        argonType = 1;
        break;
    case Argon2id:
        argonType = 2;
        break;
    default:
        return QByteArray();
    }

    auto rng = std::make_unique<Botan::System_RNG>();

    return QByteArray::fromStdString(Botan::argon2_generate_pwhash(password.constData(), password.size(), *rng.get(), parallelization, memory, iterations, argonType, saltLength, outputLength));
}

QByteArray CredentialBotan::createArgon2Password(const QByteArray &password)
{
    return createArgon2Password(password, Argon2id, 2, 65536, 2, 16, 32);
}

QByteArray CredentialBotan::createBcryptPassword(const QByteArray &password, uint16_t workFactor, char version)
{
    if ((workFactor < 4) || (workFactor > 18)) {
        return QByteArray();
    }

    const std::string pw = password.toStdString();

    auto rng = std::make_unique<Botan::System_RNG>();

    return QByteArray::fromStdString(Botan::generate_bcrypt(pw, *rng.get(), workFactor, version));
}

QByteArray CredentialBotan::createBcryptPassword(const QByteArray &password)
{
    return createBcryptPassword(password, 12, 'a');
}

bool CredentialBotanPrivate::checkPassword(const AuthenticationUser &user, const ParamsMultiMap &authinfo)
{
    QString password = authinfo.value(passwordField);
    const QString storedPassword = user.value(passwordField).toString();

    if (!passwordPreSalt.isEmpty()) {
        password.prepend(passwordPreSalt);
    }
    if (!passwordPostSalt.isEmpty()) {
        password.append(passwordPostSalt);
    }

    return CredentialBotan::validatePassword(password.toUtf8(), storedPassword.toUtf8());
}

CredentialBotan::PasswordType CredentialBotanPrivate::checkPwType(const QByteArray &hashedPw)
{
    if (hashedPw.startsWith("$argon2d$")) {
        return CredentialBotan::Argon2d;
    } else if (hashedPw.startsWith("$argon2i$")) {
        return CredentialBotan::Argon2i;
    } else if (hashedPw.startsWith("$argon2id$")) {
        return CredentialBotan::Argon2id;
    } else if (hashedPw.startsWith("$2")) {
        return CredentialBotan::Bcrypt;
    } else {
        return CredentialBotan::Invalid;
    }
}

#include "moc_credentialbotan.cpp"
