/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "menuitem.h"

#include <Cutelyst/Context>

class MenuItemData : public QSharedData
{
public:
    std::vector<MenuItem> children;
    QString name;
    QString title;
    QUrl url;
    bool isActive = false;
    bool isExpanded = false;
};

MenuItem::MenuItem()
{

}

MenuItem::MenuItem(Cutelyst::Context *c, const QString &name, const QString &title, const QString &action, const QString &ns, const QStringList &captures, const QStringList &args, const Cutelyst::ParamsMultiMap &queryValues)
    : data(new MenuItemData)
{
    data->name = name;
    data->title = title;
    auto _action = c->getAction(action, ns);
    Q_ASSERT_X(_action, "create new MenuItem", "can not find action");
    data->url = c->uriFor(_action, captures, args, queryValues);
    data->isActive = _action == c->action();
}

MenuItem::MenuItem(const MenuItem &other)
    : data{other.data}
{

}

MenuItem::MenuItem(MenuItem &&other) noexcept = default;

MenuItem &MenuItem::operator=(const MenuItem &other)
{
    if (this != &other)
        data.operator=(other.data);
    return *this;
}

MenuItem &MenuItem::operator=(MenuItem &&other) noexcept = default;

MenuItem::~MenuItem() = default;

std::vector<MenuItem> MenuItem::children() const
{
    return data ? data->children : std::vector<MenuItem>();
}

QString MenuItem::name() const
{
    return data ? data->name : QString();
}

QString MenuItem::title() const
{
    return data ? data->title : QString();
}

QUrl MenuItem::url() const
{
    return data ? data->url : QUrl();
}

bool MenuItem::isActive() const
{
    return data ? data->isActive : false;
}

bool MenuItem::hasChildren() const
{
    return data ? !data->children.empty() : false;
}

bool MenuItem::isExpanded() const
{
    return data ? data->isExpanded : false;
}

void MenuItem::addChildItem(const MenuItem &child)
{
    data->children.push_back(child);
    data->isExpanded = child.isActive();
}

#include "moc_menuitem.cpp"
