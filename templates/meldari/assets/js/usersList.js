/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {};

MeldariTmpl.Users = MeldariTmpl.Users || {}

MeldariTmpl.Users.List = MeldariTmpl.Users.List || {}

MeldariTmpl.Users.List.table = null;

MeldariTmpl.Users.List.exec = function() {
    Meldari.Users.list({details:"full"})
    .then(users => {
        users.forEach(user => {
            MeldariTmpl.Users.addTableRow(user, false);
        });
    })
    .catch(error => {
        console.error(error);
        MeldariTmpl.handleError(error);
    })
    .finally(() => {
    });
}

MeldariTmpl.Users.List.init = function() {
    const _table = document.getElementById('usersTable');
    if (_table) {
        MeldariTmpl.Users.List.table = _table;

        MeldariTmpl.Users.List.exec();
    }
}

MeldariTmpl.Users.List.init();
