/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_CLI_H
#define MELDARICTL_CLI_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <limits>
class QSqlError;
class QSqlQuery;
class QSqlDatabase;

class CLI : public QObject
{
    Q_OBJECT
public:
    explicit CLI(QObject *parent = nullptr);

    ~CLI() override = default;

    enum class RC : int {
        OK = 0,
        InvalidOption = 1,
        InputError = 2,
        ConfigError = 3,
        FileError = 4,
        DbError = 5,
        InternalError =6
    };

protected:
    void printError(const QString &error) const;

    void printError(const QStringList &errors) const;

    RC error(const QString &error, RC code) const;

    RC inputError(const QString &error) const;

    RC configError(const QString &error) const;

    RC fileError(const QString &error) const;

    RC dbError(const QString &error) const;

    RC dbError(const QSqlError &error) const;

    RC dbError(const QSqlQuery &query) const;

    RC dbError(const QSqlDatabase &db) const;

    RC internalError(const QString &error) const;

    void printWarning(const QString &warning) const;

    void printStatus(const QString &status) const;

    void printDone() const;

    void printDone(const QString &done) const;

    void printFailed() const;

    void printFailed(const QString &failed) const;

    void printMessage(const QByteArray &message) const;

    void printMessage(const QString &message) const;

    void printSuccess(const QString &message) const;

    void printDesc(const QString &description) const;

    void printDesc(const QStringList &description) const;

    QString readString(const QString &name, const QString &defaultValue = QString(), const QStringList &acceptableInput = QStringList()) const;

    int readInt(const QString &name, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max()) const;

    int readIntWithDef(const QString &name, int defaultValue, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max()) const;

    bool readBool(const QString &name, bool deaultValue = false) const;

    int readPort(const QString &name, int defaultValue) const;

    void setQuiet(bool quiet);

private:
    bool m_quiet = false;

    void printInputline(const QString &name, const QString &defaultValue = QString()) const;

    QString readInputline() const;

    Q_DISABLE_COPY(CLI)
};

#endif // MELDARICTL_CLI_H
