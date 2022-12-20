/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "basedomain.h"

#include <QTest>
#include <QDataStream>

class BaseDomainTest : public QObject
{
    Q_OBJECT
public:
    BaseDomainTest(QObject *parent = nullptr) : QObject(parent) {}

    ~BaseDomainTest() override = default;

private slots:
    void testDefaultConstructor();
    void testConstructorWithArgs();
    void testCopy();
    void testMove();
    void testComparison();
    void testDatastream();
    void testToJson();
};

void BaseDomainTest::testDefaultConstructor()
{
    BaseDomain dom;
    QVERIFY(dom.isNull());
    QVERIFY(!dom.isValid());
    QCOMPARE(dom.id(), 0);
    QVERIFY(dom.name().isNull());
    QCOMPARE(dom.status(), BaseDomain::Invalid);
    QCOMPARE(dom.ownerId(), 0);
    QVERIFY(dom.ownerName().isNull());
    QVERIFY(dom.created().isNull());
    QVERIFY(dom.updated().isNull());
    QVERIFY(dom.validUntil().isNull());
    QVERIFY(dom.lockedAt().isNull());
    QCOMPARE(dom.lockedById(), 0);
    QVERIFY(dom.lockedByName().isNull());
}

void BaseDomainTest::testConstructorWithArgs()
{
    const BaseDomain::dbid_t    id              = 123;
    const QString               name            = QStringLiteral("example.com");
    const BaseDomain::Status    status          = BaseDomain::Enabled;
    const BaseUser::dbid_t      ownerId         = 456;
    const QString               ownerName       = QStringLiteral("bob");
    const QDateTime             created         = QDateTime::currentDateTimeUtc().addYears(-1);
    const QDateTime             updated         = QDateTime::currentDateTimeUtc().addDays(-1);
    const QDateTime             validUntil      = QDateTime::currentDateTimeUtc().addYears(1);
    const QDateTime             lockedAt        = QDateTime::currentDateTimeUtc();
    const BaseUser::dbid_t      lockedById      = 789;
    const QString               lockedByName    = QStringLiteral("alice");

    BaseDomain dom(id, name, status, ownerId, ownerName, created, updated, validUntil, lockedAt, lockedById, lockedByName);

    QVERIFY(!dom.isNull());
    QVERIFY(dom.isValid());
    QCOMPARE(dom.id(), id);
    QCOMPARE(dom.name(), name);
    QCOMPARE(dom.status(), status);
    QCOMPARE(dom.ownerId(), ownerId);
    QCOMPARE(dom.ownerName(), ownerName);
    QCOMPARE(dom.created(), created);
    QCOMPARE(dom.updated(), updated);
    QCOMPARE(dom.validUntil(), validUntil);
    QCOMPARE(dom.lockedAt(), lockedAt);
    QCOMPARE(dom.lockedById(), lockedById);
    QCOMPARE(dom.lockedByName(), lockedByName);
}

void BaseDomainTest::testCopy()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    // test copy constructor
    {
        BaseDomain d1(1, QStringLiteral("example.net"), BaseDomain::Enabled, 1, QStringLiteral("user"), now, now, QDateTime(), now, 2, QStringLiteral("user2"));
        BaseDomain d2(d1);

        QCOMPARE(d1.id(), d2.id());
        QCOMPARE(d1.name(), d2.name());
        QCOMPARE(d1.status(), d2.status());
    }

    // test copy assignment
    {
        BaseDomain d1(1, QStringLiteral("example.net"), BaseDomain::Enabled, 1, QStringLiteral("user"), now, now, QDateTime(), now, 2, QStringLiteral("user2"));
        BaseDomain d2 = d1;

        QCOMPARE(d1.id(), d2.id());
        QCOMPARE(d1.name(), d2.name());
        QCOMPARE(d1.status(), d2.status());
    }
}

void BaseDomainTest::testMove()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    // test move constructor
    {
        BaseDomain d1(1, QStringLiteral("example.net"), BaseDomain::Enabled, 1, QStringLiteral("user"), now, now, QDateTime(), now, 2, QStringLiteral("user2"));
        BaseDomain d2(std::move(d1));

        QCOMPARE(d2.id(), 1);
        QCOMPARE(d2.name(), QStringLiteral("example.net"));
        QCOMPARE(d2.status(), BaseDomain::Enabled);
    }

    // test move assignment
    {
        BaseDomain d1(1, QStringLiteral("example.net"), BaseDomain::Enabled, 1, QStringLiteral("user"), now, now, QDateTime(), now, 2, QStringLiteral("user2"));
        BaseDomain d2(2, QStringLiteral("example.com"), BaseDomain::Disabled, 1, QStringLiteral("user"), now, now, QDateTime(), now, 2, QStringLiteral("user2"));
        d2 = std::move(d1);

        QCOMPARE(d2.id(), 1);
        QCOMPARE(d2.name(), QStringLiteral("example.net"));
        QCOMPARE(d2.status(), BaseDomain::Enabled);
    }
}

void BaseDomainTest::testComparison()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();

    BaseDomain d1(1, QStringLiteral("example.net"), BaseDomain::Enabled, 1, QStringLiteral("user"), now, now, QDateTime(), now, 2, QStringLiteral("user2"));
    BaseDomain d2(2, QStringLiteral("example.com"), BaseDomain::Disabled, 1, QStringLiteral("user"), now, now, QDateTime(), now, 2, QStringLiteral("user2"));
    BaseDomain d3(1, QStringLiteral("example.net"), BaseDomain::Enabled, 1, QStringLiteral("user"), now, now, QDateTime(), now, 2, QStringLiteral("user2"));
    BaseDomain d4;
    BaseDomain d5(0, QString(), BaseDomain::Invalid, 0, QString(), QDateTime(), QDateTime(), QDateTime(), QDateTime(), 0, QString());
    BaseDomain d6 = d1;

    QVERIFY(d1 != d2);
    QVERIFY(d1 == d3);
    QVERIFY(d1 != d4);
    QVERIFY(d1 != d5);
    QVERIFY(d1 == d6);
    QVERIFY(d4 == d5);
}

void BaseDomainTest::testDatastream()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QDateTime created = now.addDays(-1);
    const QDateTime updated = now;
    const QDateTime validUntil = now.addMonths(12);
    const QDateTime lockedAt = now;

    BaseDomain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("superuser"), created, updated, validUntil, lockedAt, 456, QStringLiteral("superduper"));

    QByteArray outBa;
    QDataStream out(&outBa, QIODeviceBase::WriteOnly);
    out << d1;

    const QByteArray inBa = outBa;
    QDataStream in(inBa);
    BaseDomain d2;
    in >> d2;

    QCOMPARE(d1, d2);
}

void BaseDomainTest::testToJson()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QDateTime created = now.addDays(-1);
    const QDateTime updated = now;
    const QDateTime validUntil = now.addMonths(12);
    const QDateTime lockedAt = now;

    BaseDomain d1(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("superuser"), created, updated, validUntil, lockedAt, 456, QStringLiteral("superduper"));

    QJsonObject json = d1.toJson();
    QCOMPARE(json.value(u"id"), QJsonValue(123));
    QCOMPARE(json.value(u"name"), QJsonValue(QStringLiteral("example.net")));
    QCOMPARE(json.value(u"status"), QJsonValue(static_cast<int>(BaseDomain::Enabled)));
    QCOMPARE(json.value(u"ownerId"), QJsonValue(456));
    QCOMPARE(json.value(u"ownerName"), QJsonValue(QStringLiteral("superuser")));
    QCOMPARE(json.value(u"created"), QJsonValue(created.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"updated"), QJsonValue(updated.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"validUntil"), QJsonValue(validUntil.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"lockedAt"), QJsonValue(lockedAt.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"lockedById"), QJsonValue(456));
    QCOMPARE(json.value(u"lockedByName"), QJsonValue(QStringLiteral("superduper")));

    BaseDomain d2(123, QStringLiteral("example.net"), BaseDomain::Enabled, 456, QStringLiteral("superuser"), created, updated, QDateTime(), QDateTime(), 0, QString());

    json = d2.toJson();
    QCOMPARE(json.value(u"validUntil"), QJsonValue());
    QCOMPARE(json.value(u"lockedAt"), QJsonValue());

    BaseDomain d3;
    json = d3.toJson();
    QVERIFY(json.isEmpty());
}

QTEST_MAIN(BaseDomainTest)

#include "testbasedomain.moc"
