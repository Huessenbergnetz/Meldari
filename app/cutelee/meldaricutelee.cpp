/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldaricutelee.h"

MeldariCutelee::MeldariCutelee(QObject *parent)
    : QObject{parent}
{

}

QHash<QString, Cutelee::AbstractNodeFactory*> MeldariCutelee::nodeFactories(const QString &name)
{
    Q_UNUSED(name)

    QHash<QString, Cutelee::AbstractNodeFactory*> ret;

    return ret;
}

QHash<QString, Cutelee::Filter*> MeldariCutelee::filters(const QString &name)
{
    Q_UNUSED(name)

    QHash<QString, Cutelee::Filter*> ret;

    return ret;
}

#include "moc_meldaricutelee.cpp"
