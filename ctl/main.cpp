/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "cli.h"
#include "controller.h"

#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("Huessenbergnetz"));
    app.setOrganizationDomain(QStringLiteral("huessenbergnetz.de"));
    app.setApplicationName(QStringLiteral("meldarictl"));
    app.setApplicationVersion(QStringLiteral(MELDARI_VERSION));

    {
        const QLocale locale;
        auto trans = new QTranslator(&app);
        if (Q_LIKELY(trans->load(locale, QStringLiteral("meldarictl"), QStringLiteral("."), QStringLiteral(MELDARI_TRANSLATIONSDIR)))) {
            if (Q_UNLIKELY(!app.installTranslator(trans))) {
                qWarning("Failed to install translator for locale %s", qUtf8Printable(locale.name()));
            }
        } else {
            qWarning("Failed to load translations for locale %s from %s", qUtf8Printable(locale.name()), MELDARI_TRANSLATIONSDIR);
        }
    }

    if (argc < 2) {
        //: Hint given if meldarictl has been called without any option
        //% "No command given. Use -h/--help to show available options and usage information."
        qWarning("%s", qUtf8Printable(qtTrId("melctl-err-no-command")));
        return static_cast<int>(CLI::RC::InvalidOption);
    }

    auto c = new Controller(&app);

    return static_cast<int>(c->exec());
}
