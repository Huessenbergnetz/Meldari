/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_CONTROLLER_H
#define MELDARICTL_CONTROLLER_H

#include "cli.h"

#include <QCommandLineOption>

class QCommandLineParser;

class Controller final : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller() final = default;

    CLI::RC exec();

    QList<QCommandLineOption> globalOptions() const;

private:
    void init();
    void showHelp() const;
    QList<QCommandLineOption> m_globalOptions;
};

#endif // MELDARICTL_CONTROLLER_H
