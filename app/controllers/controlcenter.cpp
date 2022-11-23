/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "controlcenter.h"
#include "logging.h"
#include "objects/menuitem.h"
#include "objects/user.h"

#include <Cutelyst/Plugins/Authentication/authentication.h>
#include <Cutelyst/Plugins/Authentication/authenticationuser.h>
#include <Cutelyst/Plugins/Session/Session>

#include <vector>

ControlCenter::ControlCenter(QObject *parent)
    : Controller{parent}
{

}

void ControlCenter::index(Context *c)
{
    c->stash({
                 {QStringLiteral("site_title"), c->translate("ControlCenter", "Control Center")}
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
                qCInfo(MEL_AUTHN, "User %s successfully logged in", qUtf8Printable(username));
                const QString redirectToQueryParam = params.value(QStringLiteral("redirect_to"));
                const QUrl redirectToUrl = redirectToQueryParam.isEmpty() ? c->uriFor(QStringLiteral("/cc")) : QUrl::fromEncoded(redirectToQueryParam.toLatin1());
                c->res()->redirect(redirectToUrl);
                return;
            } else {
                qCWarning(MEL_AUTHN, "Login failed: bad password for user %s. IP: %s", qUtf8Printable(username), qUtf8Printable(req->addressString()));
                c->setStash(QStringLiteral("error_msg"), c->translate("Loging", "Arrrgh, bad username and/or password!"));
                c->res()->setStatus(Response::Forbidden);
            }
        }
    }

    c->stash({
                 {QStringLiteral("no_wrapper"), true},
                 {QStringLiteral("template"), QStringLiteral("cc/login.html")},
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

    Session::deleteSession(c, QStringLiteral("User %1 logged out").arg(username));

    c->res()->redirect(c->uriFor(QStringLiteral("/")));
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

    mainMenu.emplace_back(c, QStringLiteral("dashboard"), c->translate("ControlCenter", "Dashboard"), QStringLiteral("index"), QStringLiteral("cc"));

    if (User::fromStash(c).isAdmin()) {

    }

    c->setStash(QStringLiteral("main_menu"), QVariant::fromValue<std::vector<MenuItem>>(mainMenu));
}

void ControlCenter::buildUserMenu(Context *c)
{
    std::vector<MenuItem> userMenu;

    userMenu.emplace_back(c, QStringLiteral("logout"), c->translate("ControlCenter", "Logout"), QStringLiteral("logout"), QStringLiteral("cc"));

    c->setStash(QStringLiteral("user_menu"), QVariant::fromValue<std::vector<MenuItem>>(userMenu));
}

#include "moc_controlcenter.cpp"
