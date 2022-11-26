/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "credentialbotan.h"

#include <QTest>

class CredentialBotanTest : public QObject
{
    Q_OBJECT
public:
    explicit CredentialBotanTest(QObject *parent = nullptr) : QObject{parent} {

    }

    ~CredentialBotanTest() final = default;

private slots:
    void testArgon2();
    void testBcrypt();
};

void CredentialBotanTest::testArgon2()
{
    const QString password = QStringLiteral("Das Gesetz hat zum Schneckengang verdorben, was Adlerflug geworden waere!");
    const QString hashed = CredentialBotan::createArgon2Password(password);
    QVERIFY(!hashed.isEmpty());
    QVERIFY(CredentialBotan::validatePassword(password, hashed));
}

void CredentialBotanTest::testBcrypt()
{
    const QString password = QStringLiteral("Das Gesetz hat zum Schneckengang verdorben, was Adlerflug geworden waere!");
    const QString hashed = CredentialBotan::createBcryptPassword(password);
    QVERIFY(!hashed.isEmpty());
    QVERIFY(CredentialBotan::validatePassword(password, hashed));
}

QTEST_MAIN(CredentialBotanTest)

#include "testcredentialbotan.moc"
