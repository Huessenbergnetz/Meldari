/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "optionitem.h"

class OptionItemData : public QSharedData
{
public:
    QString name;
    QString value;
    bool selected = false;
};

OptionItem::OptionItem() : data(new OptionItemData)
{

}

OptionItem::OptionItem(const QString &name, const QString &value, bool selected)
    : data{new OptionItemData}
{
    data->name = name;
    data->value = value;
    data->selected = selected;
}

OptionItem::OptionItem(const QString &name, int value, bool selected)
    : data{new OptionItemData}
{
    data->name = name;
    data->value = QString::number(value);
    data->selected = selected;
}

OptionItem::OptionItem(const OptionItem &other)
    : data{other.data}
{

}

OptionItem::OptionItem(OptionItem &&other) noexcept = default;

OptionItem &OptionItem::operator=(const OptionItem &other)
{
    if (this != &other)
        data.operator=(other.data);
    return *this;
}

OptionItem &OptionItem::operator=(OptionItem &&other) noexcept = default;

OptionItem::~OptionItem() = default;

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

#include "moc_optionitem.cpp"
