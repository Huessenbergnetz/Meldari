/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldaricutelee.h"
#include "logging.h"
#include "dateformattag.h"

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(MEL_CUTELEE, "meldari.cutelee")
#else
Q_LOGGING_CATEGORY(MEL_CUTELEE, "meldari.cutelee", QtInfoMsg)
#endif

MeldariCutelee::MeldariCutelee(QObject *parent)
    : QObject{parent}
{

}

QHash<QString, Cutelee::AbstractNodeFactory*> MeldariCutelee::nodeFactories(const QString &name)
{
    Q_UNUSED(name)

    QHash<QString, Cutelee::AbstractNodeFactory*> ret;
    ret.insert(QStringLiteral("mel_dateformat"), new DateFormatTag);
    ret.insert(QStringLiteral("mel_dateformat_var"), new DateFormatVarTag);

    return ret;
}

QHash<QString, Cutelee::Filter*> MeldariCutelee::filters(const QString &name)
{
    Q_UNUSED(name)

    QHash<QString, Cutelee::Filter*> ret;

    return ret;
}

#include "moc_meldaricutelee.cpp"
