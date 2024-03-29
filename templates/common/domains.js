/*
 * SPDX-FileCopyrightText: (C) 2022-2023 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

import * as MeldariUtils from './utils.min.js';

async function list(options = {}) {
    const hdrs = MeldariUtils.newXhrHeaders();

    const docUrl = new URL(document.URL);
    const url = new URL('/cc/domains/list', docUrl.origin);
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

async function add(formData) {
    const hdrs = MeldariUtils.newXhrHeaders();

    const res = await fetch('/cc/domains/add', {
        method: 'POST',
        headers: hdrs,
        body: formData
    });

    if (res.ok) {
        return res.json();
    } else {
        return Promise.reject(res);
    }
}

export { list, add };
