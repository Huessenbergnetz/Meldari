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
    const QVariantMap settings = QVariantMap({
                                                 {QStringLiteral("option"), QStringLiteral("value")}
                                             });
    User user(1, User::Registered, QStringLiteral("user"), QStringLiteral("user@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), settings);
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
        User user2 = user1;

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

void UserTest::testComparison()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    User u1(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    User u2(2, User::Registered, QStringLiteral("user2"), QStringLiteral("user2@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    User u3(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    User u4(u2);

    QVERIFY(u1 != u2);
    QVERIFY(u1 == u3);
    QVERIFY(u2 == u4);
}

void UserTest::testDatastream()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    User u1(1, User::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());

    QByteArray outBa;
    QDataStream out(&outBa, QIODeviceBase::WriteOnly);
    out << u1;

    const QByteArray inBa = outBa;
    QDataStream in(inBa);
    User u2;
    in >> u2;

    QCOMPARE(u1, u2);
}

QTEST_MAIN(UserTest)

#include "testuser.moc"
