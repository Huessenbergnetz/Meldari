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
    ErrorData(Error::Type _type, const QString &_text)
        : QSharedData(),
          text{_text},
          type{_type}
    {
        switch(_type) {
        case Error::NoError:
            status = Cutelyst::Response::OK;
            break;
        case Error::AuthenticationError:
            status = Cutelyst::Response::Unauthorized;
            break;
        case Error::AuthorizationError:
            status = Cutelyst::Response::Forbidden;
            break;
        case Error::NotFound:
            status = Cutelyst::Response::NotFound;
            break;
        case Error::InputError:
            status = Cutelyst::Response::BadRequest;
            break;
        default:
            status = Cutelyst::Response::InternalServerError;
            break;
        }
    }

    ErrorData(const QSqlError &_sqlError, const QString &_text)
        : QSharedData(),
          text{_text},
          sqlError{_sqlError},
          type{Error::SqlError},
          status{Cutelyst::Response::InternalServerError}
    {

    }

    QString text;
    QSqlError sqlError;
    Error::Type type = Error::NoError;
    Cutelyst::Response::HttpStatus status = Cutelyst::Response::OK;
};

Error::Error()
{

}

Error::Error(Error::Type type, const QString &text)
    : data{new ErrorData(type, text)}
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

Error::~Error() = default;

Error::Type Error::type() const
{
    return data ? data->type : Error::NoError;
}

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

    switch(data->type) {
    case NoError:
        //: Error title
        return c->translate("Error", "No error");
    case SqlError:
        //: Error title
        return c->translate("Error", "Database error");
    case ConfigError:
        //: Error title
        return c->translate("Error", "Configuration error");
    case ApplicationError:
        //: Error title
        return c->translate("Error", "Internal server error");
    case AuthenticationError:
    case AuthorizationError:
        //: Error title
        return c->translate("Error", "Access denied");
    case NotFound:
        //: Error title
        return c->translate("Error", "Not found");
    case InputError:
        //: Error title
        return c->translate("Error", "Invalid input data");
    default:
        //: Error title
        return c->translate("Error", "Unknown error");
    }
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

void Error::toStash(Cutelyst::Context *c, Error::Type type, const QString &text, bool detach)
{
    Error e(type, text);
    e.toStash(c, detach);
}

#include "moc_error.cpp"
