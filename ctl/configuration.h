/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_CONFIGURATION_H
#define MELDARICTL_CONFIGURATION_H

#include "cli.h"

#include <QVariantMap>

class Configuration : public CLI
{
    Q_OBJECT
public:
    explicit Configuration(QObject *parent = nullptr);

    ~Configuration() override = default;

    QVariant value(const QString &group, const QString &key, const QVariant &defaultValue = QVariant()) const;

protected:
    int loadConfig(const QString &iniPath = QString());
    void setIniPath(const QString &iniPath);

private:
    QString m_iniPath;
    QVariantMap m_config;
    bool m_loaded = false;
};

#endif // MELDARICTL_CONFIGURATION_H
