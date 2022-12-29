/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_CONTROLCENTERDOMAINS_H
#define MELDARI_CONTROLCENTERDOMAINS_H

#include <Cutelyst/Controller>

using namespace Cutelyst;

class ControlCenterDomains final : public Controller
{
    Q_OBJECT
    C_NAMESPACE("cc/domains")
public:
    explicit ControlCenterDomains(QObject *parent = nullptr);
    ~ControlCenterDomains() final = default;

    C_ATTR(index, :Path :Args(0))
    void index(Context *c);

    C_ATTR(list, :Local :Args(0))
    void list(Context *c);

    C_ATTR(add, :Local :Args(0))
    void add(Context *c);

private:
    C_ATTR(Auto, :Private)
    bool Auto(Context *c);
};

#endif // MELDARI_CONTROLCENTERDOMAINS_H
