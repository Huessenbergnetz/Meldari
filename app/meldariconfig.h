/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICONFIG_H
#define MELDARICONFIG_H

#include <QString>
#include <QStringList>
#include <QVariantMap>

class MeldariConfig
{
public:
    static void load(const QVariantMap &meldari, const QVariantMap &email);
    static bool loaded();

    static QString tmpl();
    static QString tmplPath();
    static QString tmplPath(QStringView path);
    static QString tmplPath(const QStringList &pathParts);
};

#endif // MELDARICONFIG_H
