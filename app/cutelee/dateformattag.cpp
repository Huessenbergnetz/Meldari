/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "dateformattag.h"
#include "qtimezonevariant_p.h"
#include "logging.h"
#include "meldariconfig.h"
#include <cutelee/exception.h>
#include <cutelee/parser.h>
#include <Cutelyst/Context>
#include <Cutelyst/Plugins/Session/Session>

QString getFormat(const Cutelee::FilterExpression &f, Cutelee::Context *cc)
{
    QString format;

    if (f.isValid()) {
        const QVariant formatVar = f.resolve(cc);
        const int formatVarType = formatVar.userType();
        if (formatVarType == qMetaTypeId<Cutelee::SafeString>()) {
            format = formatVar.value<Cutelee::SafeString>().get();
        } else {
            format = formatVar.toString();
        }
    } else {
        format = QStringLiteral("short");
    }

    return format;
}

QString formatDateTime(Cutelyst::Context *c, const QVariant &dt, const QString &format)
{
    QString formattedDateTime;

    QDateTime _dt = dt.toDateTime();

    if (_dt.isNull() || !_dt.isValid()) {
        return formattedDateTime;
    }

    QTimeZone userTz = Cutelyst::Session::value(c, QStringLiteral("tz")).value<QTimeZone>();
    if (!userTz.isValid()) {
        QTimeZone defTz(MeldariConfig::defTimezone().toLatin1());
        if (defTz.isValid()) {
            userTz = defTz;
        } else {
            userTz = QTimeZone::utc();
        }
    }

    if (userTz != QTimeZone::utc()) {
        _dt.setTimeSpec(Qt::UTC);
        _dt = _dt.toTimeZone(userTz);
    }

    if (format.compare(u"short") == 0) {
        //: short format string for a QDateTime value, should fit to the output of JavaScript’s Intl.DateTimeFormat
        //: with dateStyle: short and timeStyle: short
        formattedDateTime = c->locale().toString(_dt, c->translate("DateFormat", "M/d/yy, h:mm AP"));
    } else if (format.compare(u"long") == 0) {
        //: long format string for a QDateTime value, should fit to the output of JavaScript’s Intl.DateTimeFormat
        //: with dateStyle: long and timeStyle: medium
        formattedDateTime = c->locale().toString(_dt, c->translate("DateFormat", "MMMM d, yyyy 'at' h:mm:ss AP"));
    } else {
        formattedDateTime = c->locale().toString(_dt, format);
    }

    return formattedDateTime;
}

QString formatDate(Cutelyst::Context *c, const QVariant &date, const QString &format)
{
    QString formattedDate;

    const QDate _date = date.toDate();

    if (_date.isNull() || !_date.isValid()) {
        return formattedDate;
    }

    if (format.compare(u"short") == 0) {
        //: short format string for a QDate value, should fit to the output of JavaScript’s Intl.DateTimeFormat
        //: with dateStyle: short
        formattedDate = c->locale().toString(_date, c->translate("DateFormat", "M/d/yy"));
    } else if (format.compare(u"long") == 0) {
        //: short format string for a QDate value, should fit to the output of JavaScript’s Intl.DateTimeFormat
        //: with dateStyle: long
        formattedDate = c->locale().toString(_date, c->translate("DateFormat", "MMMM d, yyyy"));
    } else {
        formattedDate = c->locale().toString(_date, format);
    }

    return formattedDate;
}

QString formatTime(Cutelyst::Context *c, const QVariant &time, const QString &format)
{
    QString formattedTime;

    const QTime _time = time.toTime();

    if (_time.isNull() || !_time.isValid()) {
        return formattedTime;
    }

    if (format.compare(u"short") == 0) {
        //: short format string for a QDate value, should fit to the output of JavaScript’s Intl.DateTimeFormat
        //: with timeStyle: short
        formattedTime = c->locale().toString(_time, c->translate("DateFormat", "h:mm AP"));
    } else if (format.compare(u"long") == 0) {
        //: short format string for a QDate value, should fit to the output of JavaScript’s Intl.DateTimeFormat
        //: with timeStyle: medium
        formattedTime = c->locale().toString(_time, c->translate("DateFormat", "h:mm:ss AP"));
    } else {
        formattedTime = c->locale().toString(_time, format);
    }

    return formattedTime;
}

DateFormatTag::DateFormatTag(QObject *parent)
    : Cutelee::AbstractNodeFactory{parent}
{

}

Cutelee::Node *DateFormatTag::getNode(const QString &tagContent, Cutelee::Parser *p) const
{
    QStringList parts = smartSplit(tagContent);
    parts.removeFirst();
    if (parts.empty()) {
        throw Cutelee::Exception(Cutelee::TagSyntaxError, QStringLiteral("mel_dateformat requires at least the datetime, date or time"));
    }

    Cutelee::FilterExpression dateTime(parts.at(0), p);

    Cutelee::FilterExpression format;
    if (parts.size() > 1) {
        format = Cutelee::FilterExpression(parts.at(1), p);
    }

    return new DateFormat(dateTime, format, p);
}

DateFormat::DateFormat(const Cutelee::FilterExpression &dateTime, const Cutelee::FilterExpression &format, Cutelee::Parser *parser)
    : Cutelee::Node{parser}, m_dateTime{dateTime}, m_format{format}
{

}

void DateFormat::render(Cutelee::OutputStream *stream, Cutelee::Context *gc) const
{
    const QVariant dtVar = m_dateTime.resolve(gc);
    const int dtVarType = dtVar.userType();
    if (dtVarType == qMetaTypeId<Cutelee::SafeString>()) {
        *stream << dtVar.value<Cutelee::SafeString>().get();
        return;
    } else if (dtVarType == QMetaType::QString) {
        *stream << dtVar.toString();
        return;
    } else {
        switch(dtVarType) {
        case QMetaType::QDateTime:
        case QMetaType::QDate:
        case QMetaType::QTime:
            break;
        default:
            qCWarning(MEL_CUTELEE) << "mel_dateformat can only operate on QDateTime, QDate or QTime values, but not on" << dtVar.metaType().name() << "(ID:" << dtVarType << "):" << dtVar;
            return;
        }
    }

    auto c = gc->lookup(m_cutelystContext).value<Cutelyst::Context*>();
    if (!c) {
        const QVariantHash hash = gc->stackHash(0);
        auto it = hash.constBegin();
        while (it != hash.constEnd()) {
            if (it.value().userType() == qMetaTypeId<Cutelyst::Context*>()) {
                c = it.value().value<Cutelyst::Context*>();
                if (c) {
                    m_cutelystContext = it.key();
                    break;
                }
            }
            ++it;
        }
    }

    if (!c) {
        qCWarning(MEL_CUTELEE) << "mel_dateformat can not find the Cutelyst context";
        return;
    }

    const QString format = getFormat(m_format, gc);

    if (dtVarType == QMetaType::QDateTime) {
        *stream << formatDateTime(c, dtVar, format);
    } else if (dtVarType == QMetaType::QDate) {
        *stream << formatDate(c, dtVar, format);
    } else if (dtVarType == QMetaType::QTime) {
        *stream << formatTime(c, dtVar, format);
    }
}

DateFormatVarTag::DateFormatVarTag(QObject *parent)
    : Cutelee::AbstractNodeFactory{parent}
{

}

Cutelee::Node *DateFormatVarTag::getNode(const QString &tagContent, Cutelee::Parser *p) const
{
    QStringList parts = smartSplit(tagContent);
    parts.takeFirst();

    if (parts.size() < 3) {
        throw Cutelee::Exception(Cutelee::TagSyntaxError, QStringLiteral("mel_dateformat_var requires at least three arguments"));
    }

    Cutelee::FilterExpression dateTime(parts.at(0), p);

    Cutelee::FilterExpression format;
    if (parts.size() > 3) {
        format = Cutelee::FilterExpression(parts.at(1), p);
    }

    QString resultName = parts.last();

    return new DateFormatVar(dateTime, format, resultName, p);
}

DateFormatVar::DateFormatVar(const Cutelee::FilterExpression &dateTime, const Cutelee::FilterExpression &format, const QString &resultName, Cutelee::Parser *parser)
    : Cutelee::Node{parser}, m_dateTime{dateTime}, m_format{format}, m_resultName{resultName}
{

}

void DateFormatVar::render(Cutelee::OutputStream *stream, Cutelee::Context *gc) const
{
    const QVariant dtVar = m_dateTime.resolve(gc);
    const int dtVarType = dtVar.userType();
    if (dtVarType == qMetaTypeId<Cutelee::SafeString>()) {
        *stream << dtVar.value<Cutelee::SafeString>().get();
        return;
    } else if (dtVarType == QMetaType::QString) {
        *stream << dtVar.toString();
        return;
    } else {
        switch(dtVarType) {
        case QMetaType::QDateTime:
        case QMetaType::QDate:
        case QMetaType::QTime:
            break;
        default:
            qCWarning(MEL_CUTELEE) << "mel_dateformat_var can only operate on QDateTime, QDate or QTime values, but not on" << dtVar.metaType().name() << "(ID:" << dtVarType << "):" << dtVar;
            return;
        }
    }

    auto c = gc->lookup(m_cutelystContext).value<Cutelyst::Context*>();
    if (!c) {
        const QVariantHash hash = gc->stackHash(0);
        auto it = hash.constBegin();
        while (it != hash.constEnd()) {
            if (it.value().userType() == qMetaTypeId<Cutelyst::Context*>()) {
                c = it.value().value<Cutelyst::Context*>();
                if (c) {
                    m_cutelystContext = it.key();
                    break;
                }
            }
            ++it;
        }
    }

    if (!c) {
        qCWarning(MEL_CUTELEE) << "mel_dateformat_var can not find the Cutelyst context";
        return;
    }

    const QString format = getFormat(m_format, gc);

    QString resultString;
    if (dtVarType == QMetaType::QDateTime) {
        resultString = formatDateTime(c, dtVar, format);
    } else if (dtVarType == QMetaType::QDate) {
        resultString = formatDate(c, dtVar, format);
    } else if (dtVarType == QMetaType::QTime) {
        resultString = formatTime(c, dtVar, format);
    }

    gc->insert(m_resultName, resultString);
}
