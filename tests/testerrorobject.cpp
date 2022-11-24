/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "objects/error.h"

#include <QTest>
#include <QSqlError>

class ErrorObjectTest : public QObject
{
    Q_OBJECT
public:
    explicit ErrorObjectTest(QObject *parent = nullptr) : QObject(parent) {

    }

    ~ErrorObjectTest() final = default;

private slots:
    void testDefaultConstructor();
    void testNormalConstructor();
    void testSqlErrorConstructor();
    void testCopy();
    void testMove();
    void testStatus();
    void testStatus_data();
};

void ErrorObjectTest::testDefaultConstructor()
{
    Error e;
    QCOMPARE(e.type(), Error::NoError);
    QVERIFY(e.text().isEmpty());
    QVERIFY(e.sqlErrorText().isEmpty());
}

void ErrorObjectTest::testNormalConstructor()
{
    Error e(Error::NotFound, QStringLiteral("Page not found"));
    QCOMPARE(e.type(), Error::NotFound);
    QCOMPARE(e.text(), QStringLiteral("Page not found"));
    QVERIFY(e.sqlErrorText().isEmpty());
}

void ErrorObjectTest::testSqlErrorConstructor()
{
    QSqlError sqlError(QStringLiteral("Little Driver Text"), QStringLiteral("Little Database Text"), QSqlError::StatementError);
    Error e(sqlError, QStringLiteral("Something failed in the database"));
    QCOMPARE(e.type(), Error::SqlError);
    QCOMPARE(e.text(), QStringLiteral("Something failed in the database"));
    QCOMPARE(e.status(), Cutelyst::Response::InternalServerError);
}

void ErrorObjectTest::testCopy()
{
    // test copy constructor
    {
        Error e1(Error::NotFound, QStringLiteral("Page not found"));
        Error e2(e1);

        QCOMPARE(e1.type(), e2.type());
        QCOMPARE(e1.status(), e2.status());
        QCOMPARE(e1.text(), e2.text());
    }

    // test copy assignment
    {
        Error e1(Error::NotFound, QStringLiteral("Page not found"));
        Error e2 = e1;

        QCOMPARE(e1.type(), e2.type());
        QCOMPARE(e1.status(), e2.status());
        QCOMPARE(e1.text(), e2.text());
    }
}

void ErrorObjectTest::testMove()
{
    // test move constructor
    {
        Error e1(Error::NotFound, QStringLiteral("Page not found"));
        Error e2(std::move(e1));

        QCOMPARE(e2.type(), Error::NotFound);
        QCOMPARE(e2.status(), Cutelyst::Response::NotFound);
        QCOMPARE(e2.text(), QStringLiteral("Page not found"));
    }

    // test move assignment
    {
        Error e1(Error::NotFound, QStringLiteral("Page not found"));
        Error e2(Error::ConfigError, QStringLiteral("Invalid config value"));
        e2 = std::move(e1);

        QCOMPARE(e2.type(), Error::NotFound);
        QCOMPARE(e2.status(), Cutelyst::Response::NotFound);
        QCOMPARE(e2.text(), QStringLiteral("Page not found"));
    }
}

void ErrorObjectTest::testStatus()
{
    QFETCH(Error::Type, type);
    QFETCH(Cutelyst::Response::HttpStatus, status);

    Error e(type, QStringLiteral("error text"));
    QCOMPARE(e.status(), status);
}

void ErrorObjectTest::testStatus_data()
{
    QTest::addColumn<Error::Type>("type");
    QTest::addColumn<Cutelyst::Response::HttpStatus>("status");

    QTest::newRow("NoError") << Error::NoError << Cutelyst::Response::OK;
    QTest::newRow("AuthnError") << Error::AuthenticationError << Cutelyst::Response::Unauthorized;
    QTest::newRow("AuthzError") << Error::AuthorizationError << Cutelyst::Response::Forbidden;
    QTest::newRow("NotFound") << Error::NotFound << Cutelyst::Response::NotFound;
    QTest::newRow("InputError") << Error::InputError << Cutelyst::Response::BadRequest;
    QTest::newRow("SqlError") << Error::SqlError << Cutelyst::Response::InternalServerError;
    QTest::newRow("ConfigError") << Error::ConfigError << Cutelyst::Response::InternalServerError;
    QTest::newRow("ApplicationError") << Error::ApplicationError << Cutelyst::Response::InternalServerError;
    QTest::newRow("UnknownError") << Error::UnknownError << Cutelyst::Response::InternalServerError;
}

QTEST_MAIN(ErrorObjectTest)

#include "testerrorobject.moc"
