/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_ERROR_H
#define MELDARI_ERROR_H

#include <Cutelyst/Response>

#include <QSharedDataPointer>
#include <QObject>
#include <QJsonObject>

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
    Error();
    Error(Cutelyst::Response::HttpStatus status, const QString &text);
    Error(const QSqlError &sqlError, const QString &text);
    Error(const QSqlQuery &query, const QString &text);
    Error(const Error &other);
    Error(Error && other) noexcept;
    Error &operator=(const Error &other);
    Error &operator=(Error && other) noexcept;
    ~Error() noexcept;

    void swap(Error &other) noexcept
    { data.swap(other.data); }

    Cutelyst::Response::HttpStatus status() const;

    QString text() const;

    QString sqlErrorText() const;

    QString title(Cutelyst::Context *c) const;

    bool isError() const;

    explicit operator bool() const {
        return isError();
    }

    void toStash(Cutelyst::Context *c, bool detach = false) const;

    static Error fromStash(Cutelyst::Context *c);

    static void toStash(Cutelyst::Context *c, Cutelyst::Response::HttpStatus status, const QString &text, bool detach = false);

    QJsonObject toJson(Cutelyst::Context *c) const;

    bool operator==(const Error &other) const noexcept;

    bool operator!=(const Error &other) const noexcept
    { return !(*this == other); }

private:
    QSharedDataPointer<ErrorData> data;
};

Q_DECLARE_METATYPE(Error)
Q_DECLARE_TYPEINFO(Error, Q_MOVABLE_TYPE);

void swap(Error &a, Error &b) noexcept;

#endif // MELDARI_ERROR_H
