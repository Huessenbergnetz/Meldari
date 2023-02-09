/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "optionitem.h"

class OptionItemData : public QSharedData
{
public:
    OptionItemData() noexcept = default;
    OptionItemData(const QString &_name, const QString &_value, bool _selected) noexcept
        : QSharedData(),
          name{_name},
          value{_value},
          selected{_selected}
    {}
    OptionItemData(const OptionItemData &) noexcept = default;
    ~OptionItemData() noexcept = default;
    OptionItemData &operator=(const OptionItemData &) = delete;

    QString name;
    QString value;
    bool selected {false};
};

OptionItem::OptionItem() : data(new OptionItemData)
{

}

OptionItem::OptionItem(const QString &name, const QString &value, bool selected)
    : data{new OptionItemData{name, value, selected}}
{

}

OptionItem::OptionItem(const QString &name, int value, bool selected)
    : data{new OptionItemData{name, QString::number(value), selected}}
{

}

OptionItem::OptionItem(const OptionItem &other) = default;

OptionItem::OptionItem(OptionItem &&other) noexcept = default;

OptionItem &OptionItem::operator=(const OptionItem &other) = default;

OptionItem &OptionItem::operator=(OptionItem &&other) noexcept = default;

OptionItem::~OptionItem() noexcept = default;

QString OptionItem::name() const
{
    return data->name;
}

QString OptionItem::value() const
{
    return data->value;
}

bool OptionItem::selected() const
{
    return data->selected;
}

void swap(OptionItem &a, OptionItem &b) noexcept
{
    a.swap(b);
}

bool OptionItem::operator==(const OptionItem &other) const noexcept
{
    if (data == other.data) {
        return true;
    }

    if (name() != other.name()) {
        return false;
    }

    if (value() != other.value()) {
        return false;
    }

    if (selected() != other.selected()) {
        return false;
    }

    return true;
}

#include "moc_optionitem.cpp"
