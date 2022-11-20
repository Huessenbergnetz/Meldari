/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenter.h"
#include "../logging.h"

ControlCenter::ControlCenter(QObject *parent)
    : Controller{parent}
{

}

void ControlCenter::index(Context *c)
{

}

void ControlCenter::login(Context *c)
{

}

void ControlCenter::logout(Context *c)
{

}

bool ControlCenter::Auto(Context *c)
{
    c->res()->setHeader(QStringLiteral("X-Robots-Tag"), QStringLiteral("none"));

    return true;
}
