/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "objects/domain.h"

#include <QTest>
#include <QDataStream>

class DomainTest final : public QObject
{
    Q_OBJECT
public:
    DomainTest(QObject *parent = nullptr) : QObject(parent) {}

    ~DomainTest() final = default;

private slots:
    void testDefaultConstructor();
    void testConstructorWithArgs();
    void testCopy();
    void testMove();
    void testSwap();
    void testComparison();
    void testDatastream();
};

void DomainTest::testDefaultConstructor()
{
    Domain d;
    QVERIFY(d.isNull());
    QVERIFY(!d.isValid());
}

void DomainTest::testConstructorWithArgs()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QDateTime created = now.addDays(-5);
    const QDateTime updated = now.addSecs(-8);
    const QDateTime validUntil = now.addYears(2);
    const QDateTime lockedAt = now;

    Domain d(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), created, updated, validUntil, lockedAt, 789, QStringLiteral("admin"));
    QVERIFY(!d.isNull());
    QVERIFY(d.isValid());
    QCOMPARE(d.id(), 123);
    QCOMPARE(d.name(), QStringLiteral("example.net"));
    QCOMPARE(d.status(), BaseDomain::Enabled);
    QCOMPARE(d.ownerId(), 456);
    QCOMPARE(d.ownerName(), QStringLiteral("user"));
    QCOMPARE(d.created(), created);
    QCOMPARE(d.updated(), updated);
    QCOMPARE(d.validUntil(), validUntil);
    QCOMPARE(d.lockedAt(), lockedAt);
    QCOMPARE(d.lockedById(), 789);
    QCOMPARE(d.lockedByName(), QStringLiteral("admin"));
}

void DomainTest::testCopy()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    // test copy constructor
    {
        Domain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
        Domain d2(d1);

        QCOMPARE(d1.id(), d2.id());
        QCOMPARE(d1.name(), d2.name());
        QCOMPARE(d1.status(), d2.status());
    }

    // test copy assginment
    {
        Domain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
        Domain d2;
        d2 = d1;

        QCOMPARE(d1.id(), d2.id());
        QCOMPARE(d1.name(), d2.name());
        QCOMPARE(d1.status(), d2.status());
    }
}

void DomainTest::testMove()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    // test move constructor
    {
        Domain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
        Domain d2(std::move(d1));

        QCOMPARE(d2.id(), 123);
        QCOMPARE(d2.name(), QStringLiteral("example.net"));
        QCOMPARE(d2.status(), BaseDomain::Enabled);
    }

    // test move assignment
    {
        Domain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
        Domain d2(124, QStringLiteral("example.com"), BaseDomain::Disabled, 457, QStringLiteral("otheruser"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
        d2 = std::move(d1);

        QCOMPARE(d2.id(), 123);
        QCOMPARE(d2.name(), QStringLiteral("example.net"));
        QCOMPARE(d2.status(), BaseDomain::Enabled);
    }
}

void DomainTest::testSwap()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    Domain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
    Domain d2(124, QStringLiteral("example.com"), BaseDomain::Disabled, 457, QStringLiteral("otheruser"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
    Domain d3;

    swap(d1, d3);
    QVERIFY(d1.isNull());
    QCOMPARE(d3.id(), 123);
    d3.swap(d2);
    QCOMPARE(d3.id(), 124);
    QCOMPARE(d2.id(), 123);
}

void DomainTest::testComparison()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    Domain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
    Domain d2(124, QStringLiteral("example.com"), BaseDomain::Disabled, 457, QStringLiteral("otheruser"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
    Domain d3(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), now, now, QDateTime(), now, 789, QStringLiteral("admin"));
    Domain d4;
    Domain d5(0, QString(), BaseDomain::Invalid, 0, QString(), QDateTime(), QDateTime(), QDateTime(), QDateTime(), 0, QString());
    Domain d6 = d1;

    QVERIFY(d1 != d2);
    QVERIFY(d1 == d3);
    QVERIFY(d1 != d4);
    QVERIFY(d1 != d5);
    QVERIFY(d1 == d6);

    QVERIFY(d4 == d5);
}

void DomainTest::testDatastream()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QDateTime created = now.addDays(-5);
    const QDateTime updated = now.addSecs(-8);
    const QDateTime validUntil = now.addYears(2);
    const QDateTime lockedAt = now;

    // test valid into null
    {
        Domain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), created, updated, validUntil, lockedAt, 789, QStringLiteral("admin"));

        QByteArray outBa;
        QDataStream out(&outBa, QIODeviceBase::WriteOnly);
        out << d1;

        const QByteArray inBa = outBa;
        QDataStream in(inBa);
        Domain d2;
        in >> d2;

        QCOMPARE(d1, d2);
    }

    // test valid into valid
    {
        Domain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("user"), created, updated, validUntil, lockedAt, 789, QStringLiteral("admin"));
        Domain d2(124, QStringLiteral("example.com"), BaseDomain::Disabled, 457, QStringLiteral("otheruser"), now, now, QDateTime(), QDateTime(), 0, QString());

        QByteArray outBa;
        QDataStream out(&outBa, QIODeviceBase::WriteOnly);
        out << d1;

        const QByteArray inBa = outBa;
        QDataStream in(inBa);
        in >> d2;

        QCOMPARE(d1, d2);
    }

    // test null into valid
    {
        Domain d1;
        Domain d2(124, QStringLiteral("example.com"), BaseDomain::Disabled, 457, QStringLiteral("otheruser"), now, now, QDateTime(), QDateTime(), 0, QString());

        QByteArray outBa;
        QDataStream out(&outBa, QIODeviceBase::WriteOnly);
        out << d1;

        const QByteArray inBa = outBa;
        QDataStream in(inBa);
        in >> d2;

        QVERIFY(d2.isNull());
    }

    // test null into null
    {
        Domain d1;
        Domain d2;

        QByteArray outBa;
        QDataStream out(&outBa, QIODeviceBase::WriteOnly);
        out << d1;

        const QByteArray inBa = outBa;
        QDataStream in(inBa);
        in >> d2;

        QVERIFY(d2.isNull());
    }
}

QTEST_MAIN(DomainTest)

#include "testdomain.moc"
