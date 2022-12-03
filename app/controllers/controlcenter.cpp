/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenter.h"
#include "logging.h"
#include "qtimezonevariant_p.h"
#include "meldariconfig.h"
#include "objects/menuitem.h"
#include "objects/user.h"
#include "objects/error.h"

#include <Cutelyst/Plugins/Authentication/authentication.h>
#include <Cutelyst/Plugins/Authentication/authenticationuser.h>
#include <Cutelyst/Plugins/Session/Session>

#include <QLocale>
#include <QTimeZone>

#include <vector>

ControlCenter::ControlCenter(QObject *parent)
    : Controller{parent}
{

}

void ControlCenter::index(Context *c)
{
    c->stash({
                 {QStringLiteral("site_title"), c->translate("ControlCenter", "Dashboard")},
                 {QStringLiteral("template"), QStringLiteral("index.html")}
             });
}

void ControlCenter::login(Context *c)
{
    const auto req = c->req();
    const auto params = req->bodyParams();
    const QString username = params.value(QStringLiteral("username"));

    if (req->isPost()) {
        const QString password = params.value(QStringLiteral("password"));

        if (!username.isEmpty() && !password.isEmpty()) {
            if (Authentication::authenticate(c, params, QStringLiteral("users"))) {
                AuthenticationUser authUser = Authentication::user(c);

                Error error;
                User user = User::get(c, error, authUser.id().toUInt());

                if (!user.isNull()) {
                    user.updateLastSeen(c, error);
                    user.toStash(c);
                }

                qCInfo(MEL_AUTHN) << user << "successfully logged in";

                const QVariantMap userSetings = user.settings();

                QLocale lang(userSetings.value(QStringLiteral("language"), MeldariConfig::defLanguage()).toString());
                if (lang.language() == QLocale::C) {
                    qCWarning(MEL_CORE) << "Invalid language" << userSetings.value(QStringLiteral("language")).toString() << "selected by" << user;
                }
                Session::setValue(c, QStringLiteral("lang"), QVariant::fromValue<QLocale>(lang));

                QTimeZone tz(userSetings.value(QStringLiteral("timezone"), MeldariConfig::defTimezone()).toString().toLatin1());
                if (!tz.isValid()) {
                    qCWarning(MEL_CORE) << "Invalid timezone" << userSetings.value(QStringLiteral("timezone")).toString() << "selected by" << user;
                    tz = QTimeZone::utc();
                }
                Session::setValue(c, QStringLiteral("tz"), QVariant::fromValue<QTimeZone>(tz));

                const QString redirectToQueryParam = req->queryParam(QStringLiteral("redirect_to"));
                const QUrl redirectToUrl = redirectToQueryParam.isEmpty() ? c->uriFor(QStringLiteral("/cc")) : QUrl::fromEncoded(redirectToQueryParam.toLatin1());
                c->res()->redirect(redirectToUrl);
                return;
            } else {
                qCWarning(MEL_AUTHN, "Login failed: bad password for user %s. IP: %s", qUtf8Printable(username), qUtf8Printable(req->addressString()));
                c->setStash(QStringLiteral("error_msg"), c->translate("ControlCenter", "Arrrgh, bad username and/or password!"));
                c->res()->setStatus(Response::Forbidden);
            }
        }
    }

    c->stash({
                 {QStringLiteral("no_wrapper"), true},
                 {QStringLiteral("template"), QStringLiteral("login.html")},
                 {QStringLiteral("site_title"), c->translate("ControlCenter", "Login", "site title")},
                 {QStringLiteral("login_field_label_username"), c->translate("ControlCenter", "Username")},
                 {QStringLiteral("login_field_label_password"), c->translate("ControlCenter", "Password")},
                 {QStringLiteral("login_button_label_login"), c->translate("ControlCenter", "Login", "button text")},
                 {QStringLiteral("username"), username}
             });
}

void ControlCenter::logout(Context *c)
{
    Authentication *auth = c->plugin<Authentication*>();

    const QString username = auth->user(c).value(QStringLiteral("username")).toString();
    auth->logout(c);

    qCInfo(MEL_AUTHN) << "User" << username << "logged out";

//    Session::deleteSession(c, QStringLiteral("User %1 logged out").arg(username));
    Authentication::logout(c);

    c->res()->redirect(c->uriFor(QStringLiteral("/")));
}

void ControlCenter::error(Context *c)
{
    Error e = Error::fromStash(c);
    c->res()->setStatus(e.status());

    if (c->req()->xhr()) {
        c->res()->setJsonObjectBody(e.toJson(c));
    }
    c->detach();
}

bool ControlCenter::Auto(Context *c)
{
    c->res()->setHeader(QStringLiteral("X-Robots-Tag"), QStringLiteral("none"));

    if (c->actionName() == u"login") {
        return true;
    }

    if (Q_UNLIKELY(!Authentication::userExists(c))) {
        ParamsMultiMap query;
        query.insert(QStringLiteral("redirect_to"), QString::fromLatin1(c->req()->uri().toEncoded()));
        c->res()->redirect(c->uriFor(QStringLiteral("/cc/login"), {}, query));
        return false;
    }

    return true;
}

void ControlCenter::End(Context *c)
{
    buildMainMenu(c);
    buildUserMenu(c);
}

void ControlCenter::buildMainMenu(Context *c)
{
    std::vector<MenuItem> mainMenu;

    mainMenu.emplace_back(c, QStringLiteral("mainMenuDashboard"), c->translate("ControlCenter", "Dashboard"), QStringLiteral("index"), QStringLiteral("cc"));

    if (User::fromStash(c).isAdmin()) {
        mainMenu.emplace_back(c, QStringLiteral("mainMenuUsers"), c->translate("ControlCenter", "Users"), QStringLiteral("index"), QStringLiteral("cc/users"));
    }

    c->setStash(QStringLiteral("main_menu"), QVariant::fromValue<std::vector<MenuItem>>(mainMenu));
}

void ControlCenter::buildUserMenu(Context *c)
{
    std::vector<MenuItem> userMenu;

    userMenu.emplace_back(c, QStringLiteral("userMenuSettings"), c->translate("ControlCenter", "Settings"), QStringLiteral("index"), QStringLiteral("cc/usersettings"));
    userMenu.emplace_back(c, QStringLiteral("userMenuLogout"), c->translate("ControlCenter", "Logout"), QStringLiteral("logout"), QStringLiteral("cc"));

    c->setStash(QStringLiteral("user_menu"), QVariant::fromValue<std::vector<MenuItem>>(userMenu));
}

#include "moc_controlcenter.cpp"
