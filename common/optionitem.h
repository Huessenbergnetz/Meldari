/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_OPTIONITEM_H
#define MELDARI_OPTIONITEM_H

#include <QSharedDataPointer>
#include <QObject>

class OptionItemData;

class OptionItem
{
    Q_GADGET
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString value READ value CONSTANT)
    Q_PROPERTY(bool selected READ selected CONSTANT)
public:
    OptionItem();
    OptionItem(const QString &name, const QString &value, bool selected = false);
    OptionItem(const QString &name, int value, bool selected = false);
    OptionItem(const OptionItem &other);
    OptionItem(OptionItem &&other) noexcept;
    OptionItem &operator=(const OptionItem &other);
    OptionItem &operator=(OptionItem &&other) noexcept;
    ~OptionItem();

    QString name() const;
    QString value() const;
    bool selected() const;

private:
    QSharedDataPointer<OptionItemData> data;
};

Q_DECLARE_METATYPE(OptionItem)
Q_DECLARE_TYPEINFO(OptionItem, Q_MOVABLE_TYPE);


#endif // MELDARI_OPTIONITEM_H
