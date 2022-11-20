/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "cli.h"
#include <QCoreApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTextStream>
#include <cstdio>

CLI::CLI(QObject *parent) : QObject(parent)
{

}

void CLI::printError(const QString &error) const
{
    if (!m_quiet) {
        std::fprintf(stderr, "\x1b[31m%s\x1b[0m\n", error.toUtf8().constData());
    }
}

void CLI::printError(const QStringList &errors) const
{
    if (!m_quiet && !errors.empty()) {
        for (const QString &error : errors) {
            printError(error);
        }
    }
}

CLI::RC CLI::error(const QString &error, CLI::RC code) const
{
    printError(error);
    return code;
}

CLI::RC CLI::inputError(const QString &error) const
{
    return this->error(error, RC::InputError);
}

CLI::RC CLI::configError(const QString &error) const
{
    return this->error(error, RC::ConfigError);
}

CLI::RC CLI::fileError(const QString &error) const
{
    return this->error(error, RC::FileError);
}

CLI::RC CLI::dbError(const QString &error) const
{
    return this->error(error, RC::DbError);
}

CLI::RC CLI::dbError(const QSqlError &error) const
{
    return dbError(error.text());
}

CLI::RC CLI::dbError(const QSqlQuery &query) const
{
    return dbError(query.lastError());
}

CLI::RC CLI::dbError(const QSqlDatabase &db) const
{
    return dbError(db.lastError());
}

CLI::RC CLI::internalError(const QString &error) const
{
    return this->error(error, RC::InternalError);
}

void CLI::printWarning(const QString &warning) const
{
    if (!m_quiet) {
        std::printf("\x1b[33m%s\x1b[0m\n", qUtf8Printable(warning));
    }
}

void CLI::printStatus(const QString &status) const
{
    if (!m_quiet) {
        std::printf("%-100s", status.toUtf8().constData());
    }
}

void CLI::printDone() const
{
    //% "Done"
    printDone(qtTrId("melctl-msg-done"));
}

void CLI::printDone(const QString &done) const
{
    if (!m_quiet) {
        std::printf("\x1b[32m%s\x1b[0m\n", qUtf8Printable(done));
    }
}

void CLI::printFailed() const
{
    //% "Failed"
    printFailed(qtTrId("melctl-msg-failed"));
}

void CLI::printFailed(const QString &failed) const
{
    if (!m_quiet) {
        printf("\x1b[31m%s\x1b[0m\n", qUtf8Printable(failed));
    }
}

void CLI::printMessage(const QByteArray &message) const
{
    if (!m_quiet) {
        std::printf("%s\n", message.constData());
    }
}

void CLI::printMessage(const QString &message) const
{
    if (!m_quiet) {
        printMessage(message.toUtf8());
    }
}

void CLI::printSuccess(const QString &message) const
{
    if (!m_quiet) {
        std::printf("\x1b[32m%s\x1b[0m\n", qUtf8Printable(message));
    }
}

void CLI::printDesc(const QString &description) const
{
    if (!m_quiet) {
        if (description.length() <= 95) {
            std::printf("# %s\n", qUtf8Printable(description));
        } else {
            const QStringList parts = description.split(QChar(QChar::Space));
            QStringList out;
            int outSize = 0;
            for (const QString &part : parts) {
                if ((outSize + part.length() + 1) <= 95) {
                    out << part;
                    outSize += (part.length() + 1);
                } else {
                    printDesc(out.join(QChar(QChar::Space)));
                    out.clear();
                    out << part;
                    outSize = (part.size() + 1);
                }
            }
            if (!out.empty()) {
                printDesc(out.join(QChar(QChar::Space)));
            }
        }
    }
}

void CLI::printDesc(const QStringList &description) const
{
    if (!m_quiet && !description.empty()) {
        for (const QString &desc : description) {
            printDesc(desc);
        }
    }
}

void CLI::printInputline(const QString &name, const QString &defaultValue) const
{
    if (defaultValue.isEmpty()) {
        std::printf("%s: ", qUtf8Printable(name));
    } else {
        std::printf("%s [%s]: ", qUtf8Printable(name), qUtf8Printable(defaultValue));
    }
}

QString CLI::readInputline() const
{
    QString line;
    QTextStream stream(stdin);
    stream.readLineInto(&line);
    return line;
}

QString CLI::readString(const QString &name, const QString &defaultValue, const QStringList &acceptableInput) const
{
    QString input;

    if (acceptableInput.empty()) {
        while (input.isEmpty()) {
            printInputline(name, defaultValue);
            input = readInputline();
            if (input.isEmpty() && !defaultValue.isEmpty()) {
                input = defaultValue;
            }
        }
    } else {
        while (!acceptableInput.contains(input)) {
            printInputline(name, defaultValue);
            input = readInputline();
            if (input.isEmpty() && !defaultValue.isEmpty()) {
                input = defaultValue;
            }
        }
    }

    return input;
}

int CLI::readInt(const QString &name, int min, int max) const
{
    int val;

    bool ok = false;
    while (!ok) {
        printInputline(name);
        QString line;
        QTextStream stream(stdin);
        stream.readLineInto(&line);
        if (!line.isEmpty()) {
            val = line.toInt(&ok);
            if (ok && (val < min || val > max)) {
                ok = false;
            }
        }
    }

    return val;
}

int CLI::readIntWithDef(const QString &name, int defaultValue, int min, int max) const
{
    Q_ASSERT_X(defaultValue >= min && defaultValue <= max, "read int value", "default value is out of range of min and max values");

    int val;

    bool ok = false;
    while (!ok) {
        printInputline(name, QString::number(defaultValue));
        QString line;
        QTextStream stream(stdin);
        stream.readLineInto(&line);
        if (line.isEmpty()) {
            val = defaultValue;
            ok = true;
        } else {
            val = line.toInt(&ok);
            if (ok && (val < min || val > max)) {
                ok = false;
            }
        }
    }

    return val;
}

bool CLI::readBool(const QString &name, bool defaultValue) const
{
    bool val = false;

    static const QStringList possibleValues({
                                                //: short for yes, answer for a boolean question
                                                //% "Y"
                                                qtTrId("melctl-cli-read-bool-short-yes"),
                                                //: short for no, answer for a boolean question
                                                //% "N"
                                                qtTrId("melctl-cli-read-bool-short-no"),
                                                //: answer for a boolean question
                                                //% "Yes"
                                                qtTrId("melctl-cli-read-bool-long-yes"),
                                                //: answer for a boolean question
                                                //% "No"
                                                qtTrId("melctl-cli-read-bool-long-no")
                                            });
    QString line;
    const QString defAnswer = defaultValue ? qtTrId("melctl-cli-read-bool-long-yes") : qtTrId("melctl-cli-read-bool-long-no");
    while (!possibleValues.contains(line.trimmed(), Qt::CaseInsensitive)) {
        printInputline(name, defAnswer);
        QTextStream stream(stdin);
        stream.readLineInto(&line);
        if (line.isEmpty()) {
            line = defAnswer;
        }
    }

    val = line.compare(qtTrId("melctl-cli-read-bool-short-yes"), Qt::CaseInsensitive) == 0 || line.compare(qtTrId("melctl-cli-read-bool-long-yes"), Qt::CaseInsensitive) == 0;

    return val;
}

int CLI::readPort(const QString &name, int defaultValue) const
{
    return readIntWithDef(name, defaultValue, 0, 65535);
}

void CLI::setQuiet(bool quiet)
{
    m_quiet = quiet;
}

#include "moc_cli.cpp"
