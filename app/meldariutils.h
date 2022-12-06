/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARIUTILS_H
#define MELDARIUTILS_H

#include <QStringView>

namespace Cutelyst {
class Context;
}

class MeldariUtils
{
public:
    static bool checkAllowedMethod(Cutelyst::Context *c, QStringView allowedMethod, bool detachOnError = true);
};

#endif // MELDARIUTILS_H
