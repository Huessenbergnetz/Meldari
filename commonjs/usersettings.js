/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var Meldari = Meldari || {};

Meldari.Usersettings = Meldari.Usersettings || {};

Meldari.Usersettings.update = async (formData) => {
    const hdrs = Meldari.newXhrHeaders();

    const res = await fetch('/cc/usersettings/update', {
        method: 'POST',
        headers: hdrs,
        body: formData
    });

    if (res.ok) {
        const json = await res.json();
        return Promise.resolve(json);
    } else {
        return Promise.reject(res);
    }
};
