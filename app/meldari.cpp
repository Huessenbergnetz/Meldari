/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "meldari.h"
#include "logging.h"
#include "meldariconfig.h"
#include "userauthstoresql.h"

#include "common/confignames.h"

#include "controllers/root.h"
#include "controllers/controlcenter.h"
#include "controllers/controlcenterusers.h"

#include <Cutelyst/Engine>
#include <Cutelyst/Plugins/StaticSimple/StaticSimple>
#include <Cutelyst/Plugins/StaticCompressed/StaticCompressed>
#include <Cutelyst/Plugins/View/Cutelee/cuteleeview.h>
#include <Cutelyst/Plugins/Session/Session>
#include <Cutelyst/Plugins/Authentication/authentication.h>
#include <Cutelyst/Plugins/Authentication/credentialpassword.h>
#include <Cutelyst/Plugins/Authentication/authenticationrealm.h>
#include <Cutelyst/Plugins/Utils/Sql>
#include <Cutelyst/Plugins/StatusMessage>
#include <Cutelyst/Plugins/Memcached/Memcached>
#include <Cutelyst/Plugins/MemcachedSessionStore/MemcachedSessionStore>
#include <Cutelyst/Plugins/CSRFProtection/CSRFProtection>
#include <Cutelyst/Plugins/Utils/LangSelect>

#include <cutelee/engine.h>

#include <QCoreApplication>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#if defined(QT_DEBUG)
Q_LOGGING_CATEGORY(MEL_CORE, "meldari.core")
#else
Q_LOGGING_CATEGORY(MEL_CORE, "meldari.core", QtInfoMsg)
#endif

static QMutex mutex; // clazy:exclude=non-pod-global-static

Meldari::Meldari(QObject *parent) : Application(parent)
{
    QCoreApplication::setApplicationName(QStringLiteral("Meldari"));
    QCoreApplication::setApplicationVersion(QStringLiteral(MELDARI_VERSION));
}

bool Meldari::init()
{
    MeldariConfig::load(engine()->config(QStringLiteral(MELDARI_CONF_MEL)),
                        engine()->config(QStringLiteral(MELDARI_CONF_MAIL)));

#if defined(QT_DEBUG)
    const bool viewCache = false;
#else
    const bool viewCache = true;
#endif
    qCDebug(MEL_CORE) << "View cache enabled:" << viewCache;

    qCDebug(MEL_CORE) << "Registering view: cc";
    auto ccView = new CuteleeView(this, QStringLiteral("cc"));
    ccView->setCache(viewCache);
    ccView->setWrapper(QStringLiteral("wrapper.html"));
    ccView->setIncludePaths({MeldariConfig::tmplPath(u"site/cc")});
    ccView->engine()->addDefaultLibrary(QStringLiteral("cutelee_i18ntags"));
    qCDebug(MEL_CORE) << "Template include paths for view cc:" << ccView->includePaths();

    qCDebug(MEL_CORE) << "Registering view: public";
    auto publicView = new CuteleeView(this, QStringLiteral("public"));
    publicView->setCache(viewCache);
    publicView->setWrapper(QStringLiteral("wrapper.html"));
    publicView->setIncludePaths({MeldariConfig::tmplPath(u"site/public")});
    publicView->engine()->addDefaultLibrary(QStringLiteral("cutelee_i18ntags"));
    qCDebug(MEL_CORE) << "Template include paths for view public:" << publicView->includePaths();


    qCDebug(MEL_CORE) << "Registering controllers";
    new Root(this);
    new ControlCenter(this);
    new ControlCenterUsers(this);

    if (MeldariConfig::staticPlugin() == MeldariConfig::StaticSimple) {
        qCDebug(MEL_CORE) << "Registering StaticSimple plugin";
        auto statPlug = new StaticSimple(this);
        statPlug->setIncludePaths({MeldariConfig::tmplPath(u"static")});
    } else if (MeldariConfig::staticPlugin() == MeldariConfig::StaticCompressed) {
        qCDebug(MEL_CORE) << "Registering StaticCompressed plugin";
        auto statPlug = new StaticCompressed(this);
        statPlug->setIncludePaths({MeldariConfig::tmplPath(u"static")});
    }

    qCDebug(MEL_CORE) << "Registering Session plugin";
    auto sess = new Session(this);

    if (MeldariConfig::useMemcached()) {
        qCDebug(MEL_CORE) << "Registering Memcached plugin";
        auto memc = new Memcached(this);
        memc->setDefaultConfig({
                                   {QStringLiteral("binary_protocol"), true}
                               });
        if (MeldariConfig::useMemcachedSession()) {
            qCDebug(MEL_CORE) << "Using MemcachedSessionStore";
            sess->setStorage(new MemcachedSessionStore(this, this));
        }
    }

    qCDebug(MEL_CORE) << "Registering CSRFProtection plugin";
    auto csrf = new CSRFProtection(this);
    csrf->setDefaultDetachTo(QStringLiteral("/csrfDenied"));

    qCDebug(MEL_CORE) << "Registering StatusMessage plugin";
    new StatusMessage(this);

    qCDebug(MEL_CORE) << "Registering Authentication plugin";
    auto authn = new Authentication(this);
    auto userCreds = new CredentialPassword;
    userCreds->setPasswordType(CredentialPassword::Hashed);
    auto userStore = new UserAuthStoreSql;
    authn->addRealm(userStore, userCreds, QStringLiteral("users"));

    return true;
}

bool Meldari::postFork()
{
    QMutexLocker locker(&mutex);

    return initDb();
}

bool Meldari::initDb() const
{
    qCDebug(MEL_CORE) << "Establishing database connection";

    const QVariantMap dbconf = engine()->config(QStringLiteral(MELDARI_CONF_DB));
    const QString dbtype = dbconf.value(QStringLiteral(MELDARI_CONF_DB_TYPE), QStringLiteral(MELDARI_CONF_DB_TYPE_DEFVAL)).toString();
    const QString dbname = dbconf.value(QStringLiteral(MELDARI_CONF_DB_NAME), QStringLiteral(MELDARI_CONF_DB_NAME_DEFVAL)).toString();
    const QString dbuser = dbconf.value(QStringLiteral(MELDARI_CONF_DB_USER), QStringLiteral(MELDARI_CONF_DB_USER_DEFVAL)).toString();
    const QString dbpass = dbconf.value(QStringLiteral(MELDARI_CONF_DB_PASS)).toString();
    const QString dbhost = dbconf.value(QStringLiteral(MELDARI_CONF_DB_HOST), QStringLiteral(MELDARI_CONF_DB_HOST_DEFVAL)).toString();
    const int     dbport = dbconf.value(QStringLiteral(MELDARI_CONF_DB_PORT), MELDARI_CONF_DB_PORT_DEFVAL).toInt();

    QSqlDatabase db;
    const QString dbConName = Sql::databaseNameThread();
    if (dbtype == u"QMYSQL") {
        if (Q_UNLIKELY(dbhost.isEmpty())) {
            qCCritical(MEL_CORE) << "Can not establish database connection: missing database hostname";
            return false;
        }
        if (Q_UNLIKELY(dbname.isEmpty())) {
            qCCritical(MEL_CORE) << "Can not establish database connection: missing database name";
            return false;
        }

        db = QSqlDatabase::addDatabase(dbtype, dbConName);
        if (Q_LIKELY(db.isValid())) {
            db.setDatabaseName(dbname);
            if (!dbuser.isEmpty()) {
                db.setUserName(dbuser);
            }
            if (!dbpass.isEmpty()) {
                db.setPassword(dbpass);
            }
            if (dbhost[0] == QLatin1Char('/')) {
                db.setConnectOptions(QStringLiteral("UNIX_SOCKET=%1;MYSQL_OPT_RECONNECT=1;CLIENT_INTERACTIVE=1").arg(dbhost));
            } else {
                db.setConnectOptions(QStringLiteral("MYSQL_OPT_RECONNECT=1;CLIENT_INTERACTIVE=1"));
                db.setHostName(dbhost);
                db.setPort(dbport);
            }
        } else {
            qCCritical(MEL_CORE) << "Can not establish database connection: failed to obtain database object";
            return false;
        }
    } else {
        qCCritical(MEL_CORE) << "Can not establish database connection:" << dbtype << "is not a supported database type";
        return false;
    }

    if (Q_UNLIKELY(!db.open())) {
        qCCritical(MEL_CORE) << "Can not establish database connection:" << db.lastError().text();
        return false;
    }

    if (dbtype == u"QMYSQL") {
        QSqlQuery q(db);
        if (Q_UNLIKELY(!q.exec(QStringLiteral("SET time_zone = '+00:00'")))) {
            qCWarning(MEL_CORE) << "Failed to set database connection time zone to UTC:" << q.lastError().text();
        }
    }

    return true;
}

#include "moc_meldari.cpp"
