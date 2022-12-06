/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var Meldari = Meldari || {};

Meldari.Users = Meldari.Users || {}

Meldari.Users.get = async (userId) => {
    const hdrs = Meldari.newXhrHeaders();

    const res = await fetch('/cc/users/get/' + userId, {
        method: 'GET',
        headers: hdrs
    });

    if (res.ok) {
        return res.json();
    } else {
        return Promise.reject(res);
    }
};

Meldari.Users.list = async (options = {}) => {
    const hdrs = Meldari.newXhrHeaders();

    const docUrl = new URL(document.URL);
    const url = new URL('/cc/users/list', docUrl.origin);
    const params = new URLSearchParams(options);
    url.search = params.toString();

    const res = await fetch(url.href, {
        method: 'GET',
        headers: hdrs
    });

    if (res.ok) {
        return res.json();
    } else {
        return Promise.reject(res);
    }
};

Meldari.Users.remove = async (userId, formData) => {
    const hdrs = Meldari.newXhrHeaders();

    const res = await fetch('/cc/users/remove/' + userId, {
        method: 'POST',
        headers: hdrs,
        body: formData
    });

    if (res.ok) {
        return res.json();
    } else {
        return Promise.reject(res);
    }
};

Meldari.Users.add = async (formData) => {
    const hdrs = Meldari.newXhrHeaders();

    const res = await fetch('/cc/users/add', {
        method: 'POST',
        headers: hdrs,
        body: formData
    });

    if (res.ok) {
        return res.json();
    } else {
        return Promise.reject(res);
    }
};
