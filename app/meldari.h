/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_MELDARI_H
#define MELDARI_MELDARI_H

#include <Cutelyst/Application>

using namespace Cutelyst;

class Meldari final : public Application
{
    Q_OBJECT
    CUTELYST_APPLICATION(IID "Meldari")
public:
    Q_INVOKABLE explicit Meldari(QObject *parent = nullptr);

    ~Meldari() final = default;

    bool init() final;

    bool postFork() final;
};

#endif // MELDARI_MELDARI_H
