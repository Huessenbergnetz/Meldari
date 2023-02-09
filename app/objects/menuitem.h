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
    Q_PROPERTY(std::vector<MenuItem> children READ children CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString icon READ icon CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)
    Q_PROPERTY(bool isActive READ isActive CONSTANT)
    Q_PROPERTY(bool hasChildren READ hasChildren CONSTANT)
    Q_PROPERTY(bool isExpanded READ isExpanded CONSTANT)
public:
    MenuItem();
    MenuItem(Cutelyst::Context *c, const QString &name, const QString &title, const QString &action, const QString &ns = QString(), const QStringList &captures = QStringList(), const QStringList &args = QStringList(), const Cutelyst::ParamsMultiMap &queryValues = Cutelyst::ParamsMultiMap());
    MenuItem(const QString &name, const QString &title, const QUrl &url, bool isActive = false);
    MenuItem(const QString &name, const QString &title, const QString &url, bool isActive = false);
    MenuItem(const MenuItem &other);
    MenuItem(MenuItem &&other) noexcept;
    MenuItem &operator=(const MenuItem &other);
    MenuItem &operator=(MenuItem &&other) noexcept;
    ~MenuItem() noexcept;

    void swap(MenuItem &other) noexcept
    { data.swap(other.data); }

    std::vector<MenuItem> children() const;

    QString name() const;

    QString title() const;

    QString icon() const;

    QUrl url() const;

    bool isActive() const;

    bool hasChildren() const;

    bool isExpanded() const;

    void addChildItem(const MenuItem &child);

    bool operator==(const MenuItem &other) const noexcept;

    bool operator!=(const MenuItem &other) const noexcept
    { return !(*this == other); }

private:
    QSharedDataPointer<MenuItemData> data;
};

Q_DECLARE_METATYPE(MenuItem)
Q_DECLARE_TYPEINFO(MenuItem, Q_MOVABLE_TYPE);

void swap(MenuItem &a, MenuItem &b) noexcept;

#endif // MELDARI_MENUITEM_H
