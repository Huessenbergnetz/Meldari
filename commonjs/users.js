/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var Meldari = Meldari || {};

Meldari.Users = Meldari.Users || {}

Meldari.Users.get = async (userId) => {
    const hdrs = MeldariTmpl.newXhrHeaders();

    const res = await fetch('/cc/users/get/' + userId, {
                                method: 'GET',
                                headers: hdrs
                            });

    if (res.ok) {
        const json = await res.json();
        return Promise.resolve(json);
    } else {
        return Promise.reject(res);
    }
};
