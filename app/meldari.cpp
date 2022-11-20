/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldari.h"
#include "logging.h"

#include "controllers/root.h"
#include "controllers/controlcenter.h"

#include <QCoreApplication>

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(MEL_CORE, "meldari.core")
#else
Q_LOGGING_CATEGORY(MEL_CORE, "meldari.core", QtInfoMsg)
#endif

Meldari::Meldari(QObject *parent) : Application(parent)
{
    QCoreApplication::setApplicationName(QStringLiteral("Meldari"));
    QCoreApplication::setApplicationVersion(QStringLiteral(MELDARI_VERSION));
}

bool Meldari::init()
{
    qCDebug(MEL_CORE) << "Registering controllers";
    new Root(this);
    new ControlCenter(this);

    return true;
}

bool Meldari::postFork()
{
    return true;
}

#include "moc_meldari.cpp"
