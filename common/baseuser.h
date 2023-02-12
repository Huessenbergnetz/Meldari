/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_BASEUSER_H
#define MELDARICOMMON_BASEUSER_H

#include "meldaricommon_export.h"
#include <QObject>
#include <QSharedDataPointer>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

class BaseUserData;

class MELDARICOMMON_EXPORT BaseUser
{
    Q_GADGET
public:
    using dbid_t = quint32;

    enum Type : int {
        Invalid = -1,
        Disabled = 0,
        Registered = 32,
        Administrator = 64,
        SuperUser = 127
    };
    Q_ENUM(Type)

    BaseUser();
    BaseUser(BaseUser::dbid_t id, BaseUser::Type type, const QString &username, const QString &email, const QDateTime &created, const QDateTime &updated, const QDateTime &validUntil, const QDateTime &lastSeen, const QDateTime &lockedAt, BaseUser::dbid_t lockedById, const QString &lockedByName, const QVariantMap &settings);
    BaseUser(const BaseUser &other);
    BaseUser(BaseUser &&other) noexcept;
    BaseUser &operator=(const BaseUser &other);
    BaseUser &operator=(BaseUser &&other) noexcept;
    ~BaseUser() noexcept;

    void swap(BaseUser &other) noexcept
    { data.swap(other.data); }

    dbid_t id() const;

    Type type() const;

    QString username() const;

    QString email() const;

    QDateTime created() const;

    QDateTime updated() const;

    QDateTime validUntil() const;

    QDateTime lastSeen() const;

    QDateTime lockedAt() const;

    dbid_t lockedById() const;

    QString lockedByName() const;

    QVariantMap settings() const;

    QString tz() const;

    QString lang() const;

    bool isAdmin() const;

    bool isValid() const;

    bool isNull() const;

    void clear();

    QString logInfo() const;

    QJsonObject toJson() const;

    bool operator==(const BaseUser &other) const noexcept;

    bool operator!=(const BaseUser &other) const noexcept
    { return !(*this == other); }

    static Type typeStringToEnum(const QString &str);

    static QString typeEnumToString(Type type);

    static QStringList supportedTypes();

    static QStringList typeValues();

    static QStringList typeValues(Type below);

protected:
    void setType(Type type);
    void setEmail(const QString &email);
    void setUpdated(const QDateTime &updated);
    void setValidUntil(const QDateTime &validUntil);
    void setLastSeen(const QDateTime &lastSeen);
    void setLockedAt(const QDateTime &lockedAt);
    void setLockedById(dbid_t id);
    void setLockedByName(const QString &username);
    void setSettings(const QVariantMap &settings);

private:
    QSharedDataPointer<BaseUserData> data;

    friend QDataStream &operator<<(QDataStream &stream, const BaseUser &user);
    friend QDataStream &operator>>(QDataStream &stream, BaseUser &user);
};

Q_DECLARE_METATYPE(BaseUser)
Q_DECLARE_TYPEINFO(BaseUser, Q_MOVABLE_TYPE);

inline void BaseUser::clear()
{ if (!isNull()) *this = BaseUser(); }

MELDARICOMMON_EXPORT QDebug operator<<(QDebug dbg, const BaseUser &user);

MELDARICOMMON_EXPORT QDataStream &operator<<(QDataStream &stream, const BaseUser &user);

MELDARICOMMON_EXPORT QDataStream &operator>>(QDataStream &stream, BaseUser &user);

MELDARICOMMON_EXPORT void swap(BaseUser &a, BaseUser &b) noexcept;

#endif // MELDARICOMMON_BASEUSER_H
