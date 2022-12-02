/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICUTELEE_H
#define MELDARICUTELEE_H

#include <QObject>
#include <cutelee/taglibraryinterface.h>

class MeldariCutelee final : public QObject, public Cutelee::TagLibraryInterface
{
    Q_OBJECT
    Q_INTERFACES(Cutelee::TagLibraryInterface)
public:
    explicit MeldariCutelee(QObject *parent = nullptr);
    ~MeldariCutelee() final = default;

    QHash<QString, Cutelee::AbstractNodeFactory*> nodeFactories(const QString &name = QString()) final;

    QHash<QString, Cutelee::Filter*> filters(const QString &name = QString()) final;
};

#endif // MELDARICUTELEE_H
