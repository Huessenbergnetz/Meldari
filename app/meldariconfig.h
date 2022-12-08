/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICONFIG_H
#define MELDARICONFIG_H

#include "optionitem.h"
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <vector>

namespace Cutelyst {
class Context;
}

class MeldariConfig
{
public:
    enum StaticPlugin : int {
        StaticNone = 0,
        StaticSimple,
        StaticCompressed
    };

    static void load(const QVariantMap &meldari, const QVariantMap &email);
    static void loadSupportedLocales(const QVector<QLocale> &locales);
    static bool loaded();

    static QString tmpl();
    static QString tmplPath();
    static QString tmplPath(QStringView path);
    static QString tmplPath(const QStringList &pathParts);
    static QString tmplIcon(const QString &name);

    static QString siteName();

    static StaticPlugin staticPlugin();

    static bool useMemcached();
    static bool useMemcachedSession();

    static QString defTimezone();
    static QString defLanguage();

    static QStringList supportedLocaleNames();
    static std::vector<OptionItem> supportedLocaleOptionItems(Cutelyst::Context *c, const QLocale &selected = QLocale());
    static std::vector<OptionItem> supportedLocaleOptionItems(Cutelyst::Context *c, const QString &selected);

    static QString pwQualitySettingsFile();
    static int pwMinLength();

private:
    template< typename T >
    static T getDbOption(const QString &option, const T &defVal);

    template< typename T >
    static bool setDbOption(const QString &option, const T &value);
};

#endif // MELDARICONFIG_H
