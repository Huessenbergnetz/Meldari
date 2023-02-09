/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "error.h"
#include <Cutelyst/Context>
#include <QSqlError>
#include <QSqlQuery>

#define MELDARI_ERROR_STASH_KEY "_meldari_error"

class ErrorData : public QSharedData
{
public:
    ErrorData(Cutelyst::Response::HttpStatus _status, const QString &_text)
        : QSharedData(),
          text{_text},
          status{_status}
    {

    }

    ErrorData(const QSqlError &_sqlError, const QString &_text)
        : QSharedData(),
          text{_text},
          sqlError{_sqlError},
          status{Cutelyst::Response::InternalServerError}
    {

    }

    QString text;
    QSqlError sqlError;
    Cutelyst::Response::HttpStatus status = Cutelyst::Response::OK;
};

Error::Error()
{

}

Error::Error(Cutelyst::Response::HttpStatus status, const QString &text)
    : data{new ErrorData(status, text)}
{

}

Error::Error(const QSqlError &sqlError, const QString &text)
    : data{new ErrorData(sqlError, text)}
{

}

Error::Error(const QSqlQuery &query, const QString &text)
    : data{new ErrorData(query.lastError(), text)}
{

}

Error::Error(const Error &other)
    : data{other.data}
{

}

Error::Error(Error &&other) noexcept = default;

Error &Error::operator=(const Error &other)
{
    if (this != &other)
        data.operator=(other.data);
    return *this;
}

Error &Error::operator=(Error &&other) noexcept = default;

Error::~Error() noexcept = default;

Cutelyst::Response::HttpStatus Error::status() const
{
    return data ? data->status : Cutelyst::Response::OK;
}

QString Error::text() const
{
    return data ? data->text : QString();
}

QString Error::sqlErrorText() const
{
    return data ? data->sqlError.text() : QString();
}

QString Error::title(Cutelyst::Context *c) const
{
    Q_ASSERT(c);

    if (!data) {
        return QString();
    }

    switch(data->status) {
    case Cutelyst::Response::BadRequest:
        //: Error title
        return c->translate("Error", "Bad request");
    case Cutelyst::Response::Unauthorized:
    case Cutelyst::Response::Forbidden:
        //: Error title
        return c->translate("Error", "Access denied");
    case Cutelyst::Response::NotFound:
        //: Error title
        return c->translate("Error", "Not found");
    case Cutelyst::Response::MethodNotAllowed:
        //: Error title
        return c->translate("Error", "Method not allowed");
    default:
        return c->translate("Error", "Internal server error");
    }
}

bool Error::isError() const
{
    return data ? data->status >= Cutelyst::Response::BadRequest : false;
}

void Error::toStash(Cutelyst::Context *c, bool detach) const
{
    Q_ASSERT(c);
    c->setStash(QStringLiteral(MELDARI_ERROR_STASH_KEY), QVariant::fromValue<Error>(*this));
    if (detach) {
        if (c->ns().startsWith(u"cc")) {
            c->detach(c->getAction(QStringLiteral("error"), QStringLiteral("cc")));
        } else {
            c->detach(c->getAction(QStringLiteral("error")));
        }
    }
}

Error Error::fromStash(Cutelyst::Context *c)
{
    Q_ASSERT(c);
    return c->stash(QStringLiteral(MELDARI_ERROR_STASH_KEY)).value<Error>();
}

void Error::toStash(Cutelyst::Context *c, Cutelyst::Response::HttpStatus status, const QString &text, bool detach)
{
    Error e(status, text);
    e.toStash(c, detach);
}

QJsonObject Error::toJson(Cutelyst::Context *c) const
{
    QJsonObject o;

    QJsonObject e;

    e.insert(QStringLiteral("status"), static_cast<int>(status()));
    e.insert(QStringLiteral("text"), text());
    e.insert(QStringLiteral("title"), title(c));

    o.insert(QStringLiteral("error"), e);

    return o;
}

bool Error::operator==(const Error &other) const noexcept
{
    if (data == other.data) {
        return true;
    }

    if (status() != other.status()) {
        return false;
    }

    if (text() != other.text()) {
        return false;
    }

    if (sqlErrorText() != other.sqlErrorText()) {
        return false;
    }

    return true;
}

void swap(Error &a, Error &b) noexcept
{
    a.swap(b);
}

#include "moc_error.cpp"
