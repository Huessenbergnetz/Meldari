/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "objects/simpleuser.h"

#include <QTest>
#include <QDataStream>

class SimpleUserTest : public QObject
{
    Q_OBJECT
public:
    SimpleUserTest(QObject *parent = nullptr) : QObject{parent} {

    }

    ~SimpleUserTest() final = default;

private slots:
    void testDefaulConstructor();
    void testConstructorWithArgs();
    void testCopy();
    void testMove();
    void testComparison();
    void testDatastream();
    void testIsValid();
};

void SimpleUserTest::testDefaulConstructor()
{
    SimpleUser u;
    QVERIFY(u.isNull());
    QVERIFY(!u.isValid());
    QVERIFY(u.username().isEmpty());
    QCOMPARE(u.id(), 0);
}

void SimpleUserTest::testConstructorWithArgs()
{
    SimpleUser u(1, QStringLiteral("user"));
    QVERIFY(!u.isNull());
    QVERIFY(u.isValid());
    QCOMPARE(u.id(), 1);
    QCOMPARE(u.username(), QStringLiteral("user"));
}

void SimpleUserTest::testCopy()
{
    // test copy constructor
    {
        SimpleUser u1(1, QStringLiteral("user"));
        SimpleUser u2(u1);

        QCOMPARE(u1.id(), u2.id());
        QCOMPARE(u1.username(), u2.username());
    }

    // test copy assignment
    {
        SimpleUser u1(1, QStringLiteral("user"));
        SimpleUser u2 = u1;

        QCOMPARE(u1.id(), u2.id());
        QCOMPARE(u1.username(), u2.username());
    }
}

void SimpleUserTest::testMove()
{
    // test move constructor
    {
        SimpleUser u1(1, QStringLiteral("user"));
        SimpleUser u2(std::move(u1));

        QCOMPARE(u2.id(), 1);
        QCOMPARE(u2.username(), QStringLiteral("user"));
    }

    // test move assignment
    {
        SimpleUser u1(1, QStringLiteral("user"));
        SimpleUser u2(2, QStringLiteral("otheruser"));
        u2 = std::move(u1);
        QCOMPARE(u2.id(), 1);
        QCOMPARE(u2.username(), QStringLiteral("user"));
    }
}

void SimpleUserTest::testComparison()
{
    SimpleUser u1(1, QStringLiteral("user"));
    SimpleUser u2(2, QStringLiteral("otheruser"));
    SimpleUser u3(1, QStringLiteral("user"));
    SimpleUser u4;
    SimpleUser u5(0, QString());

    QVERIFY(u1 != u2);
    QVERIFY(u1 == u3);
    QVERIFY(u1 != u4);
    QVERIFY(u1 != u5);

    QVERIFY(u4 == u5);
}

void SimpleUserTest::testDatastream()
{
    SimpleUser u1(123, QStringLiteral("some user"));

    QByteArray outBa;
    QDataStream out(&outBa, QIODeviceBase::WriteOnly);
    out << u1;

    const QByteArray inBa = outBa;
    QDataStream in(inBa);
    SimpleUser u2;
    in >> u2;

    QCOMPARE(u1, u2);
    QCOMPARE(u2.id(), 123);
    QCOMPARE(u2.username(), QStringLiteral("some user"));
}

void SimpleUserTest::testIsValid()
{
    SimpleUser u1(0, QStringLiteral("user"));
    QVERIFY(!u1.isValid());

    SimpleUser u2(1, QStringLiteral("user"));
    QVERIFY(u2.isValid());
}

QTEST_MAIN(SimpleUserTest)

#include "testsimpleuser.moc"
