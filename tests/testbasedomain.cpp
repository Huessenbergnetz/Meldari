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

QTEST_MAIN(BaseDomainTest)

#include "testbasedomain.moc"
