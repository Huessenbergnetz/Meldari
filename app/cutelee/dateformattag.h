/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_DATEFORMATTAG_H
#define MELDARI_DATEFORMATTAG_H

#include <cutelee/node.h>

class DateFormatTag final : public Cutelee::AbstractNodeFactory
{
    Q_OBJECT
public:
    explicit DateFormatTag(QObject *parent = nullptr);
    ~DateFormatTag() final = default;

    Cutelee::Node *getNode(const QString &tagContent, Cutelee::Parser *p) const final;
};

class DateFormat final : public Cutelee::Node
{
    Q_OBJECT
public:
    explicit DateFormat(const Cutelee::FilterExpression &dateTime, const Cutelee::FilterExpression &format, Cutelee::Parser *parser = nullptr);
    ~DateFormat() final = default;

    void render(Cutelee::OutputStream *stream, Cutelee::Context *gc) const final;

private:
    mutable QString m_cutelystContext = QStringLiteral("c");
    Cutelee::FilterExpression m_dateTime;
    Cutelee::FilterExpression m_format;
};

class DateFormatVarTag final : public Cutelee::AbstractNodeFactory
{
    Q_OBJECT
public:
    explicit DateFormatVarTag(QObject *parent = nullptr);
    ~DateFormatVarTag() final = default;

    Cutelee::Node *getNode(const QString &tagContent, Cutelee::Parser *p) const final;
};

class DateFormatVar final : public Cutelee::Node
{
    Q_OBJECT
public:
    explicit DateFormatVar(const Cutelee::FilterExpression &dateTime, const Cutelee::FilterExpression &format, const QString &resultName, Cutelee::Parser *parser = nullptr);
    ~DateFormatVar() final = default;

    void render(Cutelee::OutputStream *stream, Cutelee::Context *gc) const final;

private:
    mutable QString m_cutelystContext = QStringLiteral("c");
    Cutelee::FilterExpression m_dateTime;
    Cutelee::FilterExpression m_format;
    QString m_resultName;
};

#endif // MELDARI_DATEFORMATTAG_H
