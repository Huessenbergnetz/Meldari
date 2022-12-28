/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "menuitem.h"
#include "meldariconfig.h"

#include <Cutelyst/Context>

class MenuItemData : public QSharedData
{
public:
    std::vector<MenuItem> children;
    QString name;
    QString title;
    QString icon;
    QUrl url;
    bool isActive = false;
    bool isExpanded = false;
};

MenuItem::MenuItem()
{

}

MenuItem::MenuItem(Cutelyst::Context *c, const QString &name, const QString &title, const QString &action, const QString &ns, const QStringList &captures, const QStringList &args, const Cutelyst::ParamsMultiMap &queryValues)
    : data{new MenuItemData}
{
    data->name = name;
    data->title = title;
    data->icon = MeldariConfig::tmplIcon(name);
    auto _action = c->getAction(action, ns);
    Q_ASSERT_X(_action, "create new MenuItem", "can not find action");
    data->url = c->uriFor(_action, captures, args, queryValues);
    data->isActive = _action == c->action();
}

MenuItem::MenuItem(const QString &name, const QString &title, const QUrl &url, bool isActive)
    : data{new MenuItemData}
{
    data->name = name;
    data->title = title;
    data->icon = MeldariConfig::tmplIcon(name);
    data->url = url;
    data->isActive = isActive;
}

MenuItem::MenuItem(const QString &name, const QString &title, const QString &url, bool isActive)
    : data{new MenuItemData}
{
    data->name = name;
    data->title = title;
    data->icon = MeldariConfig::tmplIcon(name);
    data->url.setUrl(url);
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

QString MenuItem::icon() const
{
    return data ? data->icon : QString();
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

bool MenuItem::operator==(const MenuItem &other) const noexcept
{
    if (data == other.data) {
        return true;
    }

    if (name() != other.name()) {
        return false;
    }

    if (title() != other.title()) {
        return false;
    }

    if (icon() != other.icon()) {
        return false;
    }

    if (url() != other.url()) {
        return false;
    }

    if (isActive() != other.isActive()) {
        return false;
    }

    if (hasChildren() != other.hasChildren()) {
        return false;
    }

    if (isExpanded() != other.isExpanded()) {
        return false;
    }

    if (children() != other.children()) {
        return false;
    }

    return true;
}

void swap(MenuItem &a, MenuItem &b) noexcept
{
    a.swap(b);
}

#include "moc_menuitem.cpp"
