/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {};

MeldariTmpl.Users = MeldariTmpl.Users || {}

MeldariTmpl.Users.rowTemplate = null;

MeldariTmpl.Users.tableBody = null;

MeldariTmpl.Users.typeTranslations = null;

MeldariTmpl.Users.addTableRow = function(user, prepend) {
    const row = MeldariTmpl.Users.rowTemplate.content.cloneNode(true);
    const tr = row.querySelector('tr');
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
    deleteBtn.dataset.bsToggle = 'modal';
    deleteBtn.dataset.bsTarget = '#removeUserModal';

    usernameTd.textContent = user.username;
    emailTd.textContent = user.email;
    typeTd.textContent = MeldariTmpl.Users.typeTranslations[user.type + ''];
    idTd.textContent = user.id;

    if (prepend) {
        MeldariTmpl.Users.tableBody.prepend(row)
    } else {
        MeldariTmpl.Users.tableBody.append(row);
    }
}

MeldariTmpl.Users.updateTableRow = function(user) {
    const row = document.getElementById('user-' + user.id);
    const tds = row.getElementsByTagName('td');

    const emailTd = tds[2];
    const typeTd = tds[3];

    emailTd.textContent = user.email;
    typeTd.textContent = MeldariTmpl.Users.typeTranslations[user.type + ''];
}

MeldariTmpl.Users.init = function() {
    const _table = document.getElementById('usersTable');
    if (_table) {
        MeldariTmpl.Users.rowTemplate = document.getElementById('user-row-template');
        MeldariTmpl.Users.tableBody = _table.getElementsByTagName('tbody')[0];
        MeldariTmpl.Users.typeTranslations = JSON.parse(document.getElementById('user-types-translated').textContent);
    }
}

MeldariTmpl.Users.init();
