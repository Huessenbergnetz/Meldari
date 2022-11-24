/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {};

MeldariTmpl.Users = MeldariTmpl.Users || {}

MeldariTmpl.Users.List = MeldariTmpl.Users.List || {}

MeldariTmpl.Users.List.table = null;

MeldariTmpl.Users.List.exec = function() {
    const hdrs = MeldariTmpl.newXhrHeaders();

    fetch('/cc/users/list?details=full', {
        method: 'GET',
        headers: hdrs
    })
    .then(response => {
        if (response.ok) {
            return response.json();
        } else {
            return Promise.reject(response);
        }
    })
    .then(users => {
        console.log(users);
        const template = document.getElementById('user-row-template');
        const tbody = MeldariTmpl.Users.List.table.getElementsByTagName('tbody')[0];
        const typeTrans = JSON.parse(document.getElementById('user-types-translated').textContent);

        users.forEach(user => {
            const clone = template.content.cloneNode(true);
            const tr = clone.querySelector('tr');
            tr.id = 'user-' + user.id;

            const tds = tr.getElementsByTagName('td');
            const actionsTd = tds[0];
            const usernameTd = tds[1];
            const emailTd = tds[2];
            const typeTd = tds[3];
            const idTd = tds[4];

            const editBtn = actionsTd.getElementsByClassName('edit-user-btn')[0];
            editBtn.value = user.id;

            const deleteBtn = actionsTd.getElementsByClassName('delete-user-btn')[0];
            deleteBtn.dataset.value = user.id;

            usernameTd.textContent = user.username;
            emailTd.textContent = user.email;
            typeTd.textContent = typeTrans[user.type + ''];
            idTd.textContent = user.id;

            tbody.appendChild(clone);
        });
    })
    .catch(error => {
        console.log(error);
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
