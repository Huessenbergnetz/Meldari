/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_OPTIONITEM_H
#define MELDARI_OPTIONITEM_H

#include "meldaricommon_export.h"
#include <QSharedDataPointer>
#include <QObject>
#include <QCollator>

class OptionItemData;

class MELDARICOMMON_EXPORT OptionItem
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
    ~OptionItem() noexcept;
    void swap(OptionItem &other) noexcept
    { data.swap(other.data); }

    bool operator==(const OptionItem &other) const noexcept;

    bool operator!=(const OptionItem &other) const noexcept
    { return !(*this == other); }

    QString name() const;
    QString value() const;
    bool selected() const;

private:
    QSharedDataPointer<OptionItemData> data;
};

class MELDARICOMMON_EXPORT OptionItemCollator : public QCollator
{
public:
    explicit OptionItemCollator(const QLocale &locale) : QCollator(locale) {}

    bool operator() (const OptionItem &left, const OptionItem &right) { return compare(left.name(), right.name()); }
};

Q_DECLARE_METATYPE(OptionItem)
Q_DECLARE_TYPEINFO(OptionItem, Q_MOVABLE_TYPE);

MELDARICOMMON_EXPORT void swap(OptionItem &a, OptionItem &b) noexcept;

#endif // MELDARI_OPTIONITEM_H
