/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_ROOT_H
#define MELDARI_ROOT_H

#include <Cutelyst/Controller>

using namespace Cutelyst;

class Root final : public Controller
{
    Q_OBJECT
    C_NAMESPACE("")
public:
    explicit Root(QObject *parent = nullptr);
    ~Root() final = default;

    C_ATTR(index, :Path :Args(0))
    void index(Context *c);

    C_ATTR(pageNotFound, :Path)
    void pageNotFound(Context *c);

private:
    C_ATTR(End, :ActionClass("RenderView"))
    void End(Context *c);

    C_ATTR(Auto, :Private)
    bool Auto(Context *c);

    C_ATTR(csrfDenied, :Private)
    void csrfDenied(Context *c);

    C_ATTR(error, :Private)
    void error(Context *c);
};

#endif // MELDARI_ROOT_H
