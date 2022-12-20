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
    void testToJson();
    void testTypeStringToEnum();
    void testTypeEnumToString();
    void testSupportedType();
    void testTypeValues();

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
    BaseUser u3(1, BaseUser::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), now, now, QDateTime(), now, QDateTime(), 0, QString(), QVariantMap());
    BaseUser u4;
    BaseUser u5(0, BaseUser::Invalid, QString(), QString(), QDateTime(), QDateTime(), QDateTime(), QDateTime(), QDateTime(), 0, QString(), QVariantMap());
    BaseUser u6 = u1;

    QVERIFY(u1 != u2);
    QVERIFY(u1 == u3);
    QVERIFY(u1 != u4);
    QVERIFY(u1 != u5);
    QVERIFY(u1 == u6);

    QVERIFY(u4 == u5);
}

void BaseUserTest::testDatastream()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QDateTime created = now.addDays(-1);
    const QDateTime updated = now;
    const QDateTime validUntil = now.addMonths(12);
    const QDateTime lastSeen = now.addSecs(600);
    const QDateTime lockedAt = now;
    const QVariantMap settings = {
        {QStringLiteral("option"), QStringLiteral("value")}
    };
    BaseUser u1(1, BaseUser::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), created, updated, validUntil, lastSeen, lockedAt, 2, QStringLiteral("user2"), settings);

    QByteArray outBa;
    QDataStream out(&outBa, QIODeviceBase::WriteOnly);
    out << u1;

    const QByteArray inBa = outBa;
    QDataStream in(inBa);
    BaseUser u2;
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

void BaseUserTest::testToJson()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QDateTime created = now.addDays(-1);
    const QDateTime updated = now;
    const QDateTime validUntil = now.addMonths(12);
    const QDateTime lastSeen = now.addSecs(600);
    const QDateTime lockedAt = now;
    const QVariantMap settings = {
        {QStringLiteral("option"), QStringLiteral("value")}
    };
    BaseUser u1(1, BaseUser::Registered, QStringLiteral("user1"), QStringLiteral("user1@example.net"), created, updated, validUntil, lastSeen, lockedAt, 2, QStringLiteral("user2"), settings);

    QJsonObject json = u1.toJson();
    QCOMPARE(json.value(u"id"), QJsonValue(1));
    QCOMPARE(json.value(u"type"), QJsonValue(static_cast<int>(BaseUser::Registered)));
    QCOMPARE(json.value(u"username"), QJsonValue(QStringLiteral("user1")));
    QCOMPARE(json.value(u"email"), QJsonValue(QStringLiteral("user1@example.net")));
    QCOMPARE(json.value(u"created"), QJsonValue(created.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"updated"), QJsonValue(updated.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"validUntil"), QJsonValue(validUntil.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"lastSeen"), QJsonValue(lastSeen.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"lockedAt"), QJsonValue(lockedAt.toMSecsSinceEpoch()));
    QCOMPARE(json.value(u"lockedById"), QJsonValue(2));
    QCOMPARE(json.value(u"lockedByName"), QJsonValue(QStringLiteral("user2")));
    QCOMPARE(json.value(u"settings"), QJsonValue(QJsonObject::fromVariantMap(settings)));

    BaseUser u2(2, BaseUser::Administrator, QStringLiteral("user2"), QStringLiteral("user2@example.net"), created, updated, QDateTime(), QDateTime(), QDateTime(), 0, QString(), QVariantMap());

    json = u2.toJson();
    QCOMPARE(json.value(u"validUntil"), QJsonValue());
    QCOMPARE(json.value(u"lastSeen"), QJsonValue());
    QCOMPARE(json.value(u"lockedAt"), QJsonValue());

    BaseUser u3;
    json = u3.toJson();
    QVERIFY(json.isEmpty());
}

void BaseUserTest::testTypeStringToEnum()
{
    QCOMPARE(BaseUser::typeStringToEnum(QStringLiteral("DiSabled")), BaseUser::Disabled);
    QCOMPARE(BaseUser::typeStringToEnum(QStringLiteral("reGIstered")), BaseUser::Registered);
    QCOMPARE(BaseUser::typeStringToEnum(QStringLiteral("adminiStrator")), BaseUser::Administrator);
    QCOMPARE(BaseUser::typeStringToEnum(QStringLiteral("Superuser")), BaseUser::SuperUser);
    QCOMPARE(BaseUser::typeStringToEnum(QStringLiteral("invalid")), BaseUser::Invalid);
    QCOMPARE(BaseUser::typeStringToEnum(QStringLiteral("kackMIST")), BaseUser::Invalid);
}

void BaseUserTest::testTypeEnumToString()
{
    QCOMPARE(BaseUser::typeEnumToString(BaseUser::Disabled), QStringLiteral("Disabled"));
}

void BaseUserTest::testSupportedType()
{
    const QStringList supportedTypes = BaseUser::supportedTypes();
    QVERIFY(!supportedTypes.empty());
    QVERIFY(supportedTypes.contains(u"Administrator"));
    QVERIFY(!supportedTypes.contains(u"Invalid"));
}

void BaseUserTest::testTypeValues()
{
    const QStringList expected({QStringLiteral("0"), QStringLiteral("32"), QStringLiteral("64"), QStringLiteral("127")});
    const QStringList actual = BaseUser::typeValues();
    QCOMPARE(actual, expected);

    const QStringList belowSuperUesr = BaseUser::typeValues(BaseUser::SuperUser);
    QCOMPARE(belowSuperUesr, expected);

    const QStringList expectedAdministrator({QStringLiteral("0"), QStringLiteral("32")});
    const QStringList belowAdministrator = BaseUser::typeValues(BaseUser::Administrator);
    QCOMPARE(belowAdministrator, expectedAdministrator);
}

QTEST_MAIN(BaseUserTest)

#include "testbaseuser.moc"
