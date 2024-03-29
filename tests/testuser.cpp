/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "objects/user.h"

#include <QTest>
#include <QDataStream>

class UserTest : public QObject
{
    Q_OBJECT
public:
    UserTest(QObject *parent = nullptr) : QObject{parent} {

    }

    ~UserTest() final = default;

private slots:
    void testDefaulConstructor();
    void testConstructorWithArgs();
    void testCopy();
    void testMove();
    void testSwap();
    void testClear();
    void testComparison();
    void testDatastream();
};

void UserTest::testDefaulConstructor()
{
    User u;
    QVERIFY(u.isNull());
    QVERIFY(!u.isValid());
}

void UserTest::testConstructorWithArgs()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QDateTime created = now.addDays(-5);
    const QDateTime updated = now.addSecs(-99);
    const QDateTime validUntil = now.addYears(2);
    const QDateTime lastSeen = now.addSecs(-500);
    const QDateTime lockedAt = now;

    const QVariantMap settings = QVariantMap({
                                                 {QStringLiteral("option"), QStringLiteral("value")}
                                             });
    User user(1, User::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), created, updated, validUntil, lastSeen, lockedAt, 2, QStringLiteral("admin"), settings);

    QCOMPARE(user.id(), 1);
    QCOMPARE(user.type(), User::Registered);
    QCOMPARE(user.username(), QStringLiteral("user"));
    QCOMPARE(user.email(), QStringLiteral("user@example.net"));
    QCOMPARE(user.created(), created);
    QCOMPARE(user.updated(), updated);
    QCOMPARE(user.validUntil(), validUntil);
    QCOMPARE(user.lastSeen(), lastSeen);
    QCOMPARE(user.lockedAt(), lockedAt);
    QCOMPARE(user.lockedById(), 2);
    QCOMPARE(user.lockedByName(), QStringLiteral("admin"));
    QCOMPARE(user.settings(), settings);
}

void UserTest::testCopy()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    // test copy constructor
    {
        User user1(1, User::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        User user2(user1);

        QCOMPARE(user1.id(), user2.id());
        QCOMPARE(user1.type(), user2.type());
        QCOMPARE(user1.email(), user2.email());
    }

    // test copy assignment
    {
        User user1(1, User::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        User user2;
        user2 = user1;

        QCOMPARE(user1.id(), user2.id());
        QCOMPARE(user1.type(), user2.type());
        QCOMPARE(user1.email(), user2.email());
    }
}

void UserTest::testMove()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    // test move constructor
    {
        User user1(1, User::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        User user2(std::move(user1));
        QCOMPARE(user2.id(), 1);
        QCOMPARE(user2.username(), QStringLiteral("user"));
        QCOMPARE(user2.email(), QStringLiteral("user@example.net"));
        QCOMPARE(user2.created(), now);
    }

    // test move assignment
    {
        User u1(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        User u2(2, User::Registered, QStringLiteral("user2"), QStringLiteral("user2@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
        u2 = std::move(u1);
        QCOMPARE(u2.id(), 1);
        QCOMPARE(u2.username(), QStringLiteral("user1"));
        QCOMPARE(u2.email(), QStringLiteral("user1@example.net"));
    }
}

void UserTest::testSwap()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    User u1(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    User u2(2, User::Registered, QStringLiteral("user2"), QStringLiteral("user2@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    User u3;

    swap(u1, u3);
    QVERIFY(u1.isNull());
    QCOMPARE(u3.id(), 1);
    u3.swap(u2);
    QCOMPARE(u3.id(), 2);
    QCOMPARE(u2.id(), 1);
}

void UserTest::testClear()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    User u1(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    u1.clear();
    QVERIFY(u1.isNull());
}

void UserTest::testComparison()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    User u1(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    User u2(2, User::Registered, QStringLiteral("user2"), QStringLiteral("user2@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    User u3(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    User u4;
    User u5(0, User::Invalid, QString(), QString(), QDateTime(), QDateTime(), QDateTime(), QDateTime(), QDateTime(), 0, QString(), QVariantMap());
    User u6 = u1;

    QVERIFY(u1 != u2);
    QVERIFY(u1 == u3);
    QVERIFY(u1 != u4);
    QVERIFY(u1 != u5);
    QVERIFY(u1 == u6);

    QVERIFY(u4 == u5);
}

void UserTest::testDatastream()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QDateTime created = now.addDays(-1);
    const QDateTime updated = now;
    const QDateTime validUntil = now.addMonths(12);
    const QDateTime lastSeen = now.addSecs(-600);
    const QDateTime lockedAt = now;
    const QVariantMap settings = {
        {QStringLiteral("option"), QStringLiteral("value")}
    };


    // test valid into null
    {
        User u1(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), created, updated, validUntil, lastSeen, lockedAt, 2, QStringLiteral("user2"), settings);

        QByteArray outBa;
        QDataStream out(&outBa, QIODeviceBase::WriteOnly);
        out << u1;

        const QByteArray inBa = outBa;
        QDataStream in(inBa);
        User u2;
        in >> u2;

        QCOMPARE(u1, u2);
        QCOMPARE(u2.id(), 1);
        QCOMPARE(u2.username(), QStringLiteral("user1"));
        QCOMPARE(u2.email(), QStringLiteral("user1@example.net"));
        QCOMPARE(u2.created(), created);
        QCOMPARE(u2.updated(), updated);
        QCOMPARE(u2.validUntil(), validUntil);
        QCOMPARE(u2.lastSeen(), lastSeen);
        QCOMPARE(u2.lockedAt(), lockedAt);
        QCOMPARE(u2.lockedById(), 2);
        QCOMPARE(u2.lockedByName(), QStringLiteral("user2"));
        QCOMPARE(u2.settings(), settings);
    }

    // test valid into valid
    {
        User u1(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), created, updated, validUntil, lastSeen, lockedAt, 2, QStringLiteral("user2"), settings);
        User u2(2, User::Administrator, QStringLiteral("user2"), QStringLiteral("user2@example.net"), created, updated, QDateTime(), QDateTime(), QDateTime(), 0, QString(), {});

        QByteArray outBa;
        QDataStream out(&outBa, QIODeviceBase::WriteOnly);
        out << u1;

        const QByteArray inBa = outBa;
        QDataStream in(inBa);
        in >> u2;

        QCOMPARE(u1, u2);
        QCOMPARE(u2.id(), 1);
        QCOMPARE(u2.username(), QStringLiteral("user1"));
        QCOMPARE(u2.email(), QStringLiteral("user1@example.net"));
        QCOMPARE(u2.created(), created);
        QCOMPARE(u2.updated(), updated);
        QCOMPARE(u2.validUntil(), validUntil);
        QCOMPARE(u2.lastSeen(), lastSeen);
        QCOMPARE(u2.lockedAt(), lockedAt);
        QCOMPARE(u2.lockedById(), 2);
        QCOMPARE(u2.lockedByName(), QStringLiteral("user2"));
        QCOMPARE(u2.settings(), settings);
    }

    // test null into valid
    {
        User u1;
        User u2(2, User::Administrator, QStringLiteral("user2"), QStringLiteral("user2@example.net"), created, updated, QDateTime(), QDateTime(), QDateTime(), 0, QString(), {});

        QByteArray outBa;
        QDataStream out(&outBa, QIODeviceBase::WriteOnly);
        out << u1;

        const QByteArray inBa = outBa;
        QDataStream in(inBa);
        in >> u2;

        QVERIFY(u2.isNull());
    }

    // test null into null
    {
        User u1;
        User u2;

        QByteArray outBa;
        QDataStream out(&outBa, QIODeviceBase::WriteOnly);
        out << u1;

        const QByteArray inBa = outBa;
        QDataStream in(inBa);
        in >> u2;

        QVERIFY(u2.isNull());
    }
}

QTEST_MAIN(UserTest)

#include "testuser.moc"
