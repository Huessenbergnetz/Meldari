/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

import * as MeldariUtils from './utils.min.js';

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

export { add };