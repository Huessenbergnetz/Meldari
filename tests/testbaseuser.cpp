/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "baseuser.h"

#include <QTest>
#include <QDataStream>

class BaseUserTest : public QObject
{
    Q_OBJECT
public:
    BaseUserTest(QObject *parent = nullptr) : QObject{parent} {

    }

    ~BaseUserTest() final = default;

private slots:
    void initTestCase() {}

    void testDefaultConstructor();
    void testConstructorWithArgs();
    void testCopy();
    void testMove();
    void testComparison();
    void testDatastream();

    void cleanupTestCase() {}
};

void BaseUserTest::testDefaultConstructor()
{
    BaseUser user;
    QVERIFY(user.isNull());
    QVERIFY(!user.isValid());
}

void BaseUserTest::testConstructorWithArgs()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QVariantMap settings = QVariantMap({
                                                 {QStringLiteral("option"), QStringLiteral("value")}
                                             });
    BaseUser user(1, BaseUser::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), settings);

    QCOMPARE(user.id(), 1);
    QCOMPARE(user.type(), BaseUser::Registered);
    QCOMPARE(user.username(), QStringLiteral("user"));
    QCOMPARE(user.email(), QStringLiteral("user@example.net"));
    QCOMPARE(user.created(), now);
    QCOMPARE(user.updated(), now);
    QCOMPARE(user.validUntil(), QDateTime());
    QCOMPARE(user.lastSeen(), now);
    QCOMPARE(user.lockedAt(), QDateTime());
    QCOMPARE(user.lockedById(), 0);
    QCOMPARE(user.settings(), settings);
}

void BaseUserTest::testCopy()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    // test copy constructor
    {
        BaseUser user1(1, BaseUser::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        BaseUser user2(user1);

        QCOMPARE(user1.id(), user2.id());
        QCOMPARE(user1.type(), user2.type());
        QCOMPARE(user1.email(), user2.email());
    }

    // test copy assignment
    {
        BaseUser user1(1, BaseUser::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        BaseUser user2 = user1;

        QCOMPARE(user1.id(), user2.id());
        QCOMPARE(user1.type(), user2.type());
        QCOMPARE(user1.email(), user2.email());
    }
}

void BaseUserTest::testMove()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    // test move constructor
    {
        BaseUser user1(1, BaseUser::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        BaseUser user2(std::move(user1));
        QCOMPARE(user2.id(), 1);
        QCOMPARE(user2.username(), QStringLiteral("user"));
        QCOMPARE(user2.email(), QStringLiteral("user@example.net"));
        QCOMPARE(user2.created(), now);
    }

    // test move assignment
    {
        BaseUser u1(1, BaseUser::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        BaseUser u2(2, BaseUser::Registered, QStringLiteral("user2"), QStringLiteral("user2@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        u2 = std::move(u1);
        QCOMPARE(u2.id(), 1);
        QCOMPARE(u2.username(), QStringLiteral("user1"));
        QCOMPARE(u2.email(), QStringLiteral("user1@example.net"));
    }
}

void BaseUserTest::testComparison()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    BaseUser u1(1, BaseUser::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    BaseUser u2(2, BaseUser::Registered, QStringLiteral("user2"), QStringLiteral("user2@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    BaseUser u3 = u1;

    QVERIFY(u1 != u2);
    QVERIFY(u1 == u3);
}

void BaseUserTest::testDatastream()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    BaseUser u1(1, BaseUser::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());

    QByteArray outBa;
    QDataStream out(&outBa, QIODeviceBase::WriteOnly);
    out << u1;

    const QByteArray inBa = outBa;
    QDataStream in(inBa);
    BaseUser u2;
    in >> u2;

    QCOMPARE(u1, u2);
}

QTEST_MAIN(BaseUserTest)

#include "testbaseuser.moc"
