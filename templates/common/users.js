/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

import * as MeldariUtils from './utils.min.js';

async function get(userId) {
    const hdrs = MeldariUtils.newXhrHeaders();

    const res = await fetch('/cc/users/get/' + userId, {
        method: 'GET',
        headers: hdrs
    });

    if (res.ok) {
        return res.json();
    } else {
        return Promise.reject(res);
    }
}

async function list(options = {}) {
    const hdrs = MeldariUtils.newXhrHeaders();

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

async function remove (userId, formData) {
    const hdrs = MeldariUtils.newXhrHeaders();

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

async function add(formData) {
    const hdrs = MeldariUtils.newXhrHeaders();

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

async function edit(userId, formData) {
    const hdrs = MeldariUtils.newXhrHeaders();

    const res = await fetch('/cc/users/edit/' + userId, {
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

export { get, list, remove, add, edit };