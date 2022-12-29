/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

import * as Utils from './utils.min.js';

async function update(formData) {
    const hdrs = Utils.newXhrHeaders();

    const res = await fetch('/cc/usersettings/update', {
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

export { update };