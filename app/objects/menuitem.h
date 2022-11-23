/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARI_MENUITEM_H
#define MELDARI_MENUITEM_H

#include <Cutelyst/ParamsMultiMap>

#include <QObject>
#include <QSharedDataPointer>
#include <QUrl>

#include <vector>

class MenuItemData;
namespace Cutelyst {
    class Context;
}

class MenuItem
{
    Q_GADGET
public:
    MenuItem();
    MenuItem(Cutelyst::Context *c, const QString &name, const QString &title, const QString &action, const QString &ns = QString(), const QStringList &captures = QStringList(), const QStringList &args = QStringList(), const Cutelyst::ParamsMultiMap &queryValues = Cutelyst::ParamsMultiMap());
    MenuItem(const MenuItem &other);
    MenuItem(MenuItem &&other) noexcept;
    MenuItem &operator=(const MenuItem &other);
    MenuItem &operator=(MenuItem &&other) noexcept;
    ~MenuItem();

    std::vector<MenuItem> children() const;

    QString name() const;

    QString title() const;

    QUrl url() const;

    bool isActive() const;

    bool hasChildren() const;

    bool isExpanded() const;

    void addChildItem(const MenuItem &child);

private:
    QSharedDataPointer<MenuItemData> data;
};

#endif // MELDARI_MENUITEM_H
