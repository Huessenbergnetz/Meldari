/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_ERROR_H
#define MELDARI_ERROR_H

#include <Cutelyst/Response>

#include <QSharedDataPointer>
#include <QObject>

class ErrorData;
class QSqlError;
class QSqlQuery;

namespace Cutelyst {
class Context;
}

class Error
{
    Q_GADGET
public:
    enum Type : int {
        NoError = 0,
        SqlError,
        ConfigError,
        ApplicationError,
        AuthenticationError,
        AuthorizationError,
        NotFound,
        InputError,
        UnknownError
    };
    Q_ENUM(Type)

    Error();
    Error(Type type, const QString &text);
    Error(const QSqlError &sqlError, const QString &text);
    Error(const QSqlQuery &query, const QString &text);
    Error(const Error &other);
    Error(Error && other) noexcept;
    Error &operator=(const Error &other);
    Error &operator=(Error && other) noexcept;
    ~Error();

    Type type() const;

    Cutelyst::Response::HttpStatus status() const;

    QString text() const;

    QString sqlErrorText() const;

    QString title(Cutelyst::Context *c) const;

    void toStash(Cutelyst::Context *c, bool detach = false) const;

    static Error fromStash(Cutelyst::Context *c);

    static void toStash(Cutelyst::Context *c, Type type, const QString &text, bool detach = false);

private:
    QSharedDataPointer<ErrorData> data;
};

Q_DECLARE_METATYPE(Error)
Q_DECLARE_TYPEINFO(Error, Q_MOVABLE_TYPE);

#endif // MELDARI_ERROR_H
