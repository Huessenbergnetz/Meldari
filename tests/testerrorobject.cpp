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
};

void ErrorObjectTest::testDefaultConstructor()
{
    Error e;
    QCOMPARE(e.status(), Cutelyst::Response::OK);
    QVERIFY(e.text().isEmpty());
    QVERIFY(e.sqlErrorText().isEmpty());
}

void ErrorObjectTest::testNormalConstructor()
{
    Error e1(Cutelyst::Response::MethodNotAllowed, QStringLiteral("Only POST is allowed"));
    QCOMPARE(e1.status(), Cutelyst::Response::MethodNotAllowed);
    QCOMPARE(e1.text(), QStringLiteral("Only POST is allowed"));
}

void ErrorObjectTest::testSqlErrorConstructor()
{
    QSqlError sqlError(QStringLiteral("Little Driver Text"), QStringLiteral("Little Database Text"), QSqlError::StatementError);
    Error e(sqlError, QStringLiteral("Something failed in the database"));
    QCOMPARE(e.text(), QStringLiteral("Something failed in the database"));
    QCOMPARE(e.status(), Cutelyst::Response::InternalServerError);
}

void ErrorObjectTest::testCopy()
{
    // test copy constructor
    {
        Error e1(Cutelyst::Response::NotFound, QStringLiteral("Page not found"));
        Error e2(e1);

        QCOMPARE(e1.status(), e2.status());
        QCOMPARE(e1.text(), e2.text());
    }

    // test copy assignment
    {
        Error e1(Cutelyst::Response::NotFound, QStringLiteral("Page not found"));
        Error e2 = e1;

        QCOMPARE(e1.status(), e2.status());
        QCOMPARE(e1.text(), e2.text());
    }
}

void ErrorObjectTest::testMove()
{
    // test move constructor
    {
        Error e1(Cutelyst::Response::NotFound, QStringLiteral("Page not found"));
        Error e2(std::move(e1));

        QCOMPARE(e2.status(), Cutelyst::Response::NotFound);
        QCOMPARE(e2.text(), QStringLiteral("Page not found"));
    }

    // test move assignment
    {
        Error e1(Cutelyst::Response::NotFound, QStringLiteral("Page not found"));
        Error e2(Cutelyst::Response::InternalServerError, QStringLiteral("Invalid config value"));
        e2 = std::move(e1);

        QCOMPARE(e2.status(), Cutelyst::Response::NotFound);
        QCOMPARE(e2.text(), QStringLiteral("Page not found"));
    }
}

QTEST_MAIN(ErrorObjectTest)

#include "testerrorobject.moc"
