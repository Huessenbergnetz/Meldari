/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

import * as MeldariUsers from '/common/js/users.min.js';
import * as Utils from './utils.min.js';
import initValidUntilInputs from './validuntilinputs.min.js';

initValidUntilInputs();

const rowTemplate = document.getElementById('user-row-template');
const typeTranslations = JSON.parse(document.getElementById('user-types-translated').textContent);
const tableBody = document.getElementById('usersTable').getElementsByTagName('tbody')[0];

function addTableRow(user, prepend) {
    const row = rowTemplate.content.cloneNode(true);
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

    const delBtn = actionsTd.getElementsByClassName('delete-user-btn')[0];
    delBtn.dataset.value = user.id;
    delBtn.dataset.bsToggle = 'modal';
    delBtn.dataset.bsTarget = '#removeUserModal';

    usernameTd.textContent = user.username;
    emailTd.textContent = user.email;
    typeTd.textContent = typeTranslations[user.type + ''];
    idTd.textContent = user.id;

    if (prepend) {
        tableBody.prepend(row);
    } else {
        tableBody.append(row);
    }
}

function updateTableRow(user) {
    const row = document.getElementById('user-' + user.id);
    const tds = row.getElementsByTagName('td');

    const emailTd = tds[2];
    const typeTd = tds[3];

    emailTd.textContent = user.email;
    typeTd.textContent = typeTranslations[user.type + ''];
}

function list() {
    Utils.togglePageSpinner();

    MeldariUsers.list({details:"full"})
    .then(users => {
        users.forEach(user => {
            addTableRow(user, false);
        });
    })
    .catch(error => {
        console.error(error);
        Utils.handleError(error);
    })
    .finally(() => {
        Utils.togglePageSpinner();
    });
}

list();

const addUserModal = document.getElementById('addUserModal');
const addUserForm = document.forms['addUserForm'];
const addUserFormSubmit = addUserForm.elements['addUserFormSubmit'];

function resetAddUserForm() {
    const elNames = ['username','email','password','password_confirmation','validUntil'];
    elNames.forEach((el) => {addUserForm.elements.namedItem(el).value = '';});
    addUserForm.elements['type'].value = 0;
}

function add() {
    Utils.switchButton(addUserFormSubmit);

    const fd = new FormData(addUserForm);

    MeldariUsers.add(fd)
    .then(json => {
        resetAddUserForm();
        const aum = bootstrap.Modal.getOrCreateInstance(addUserModal);
        aum.hide();
        addTableRow(json.data, true);
        Utils.createSuccess(json.message);
    })
    .catch(error => {
        console.error(error);
        Utils.handleError(error, addUserForm);
    })
    .finally(() => {
        Utils.switchButton(addUserFormSubmit);
    });
}

addUserForm.addEventListener('submit', (e) => { e.preventDefault(); add(); });

const removeUserModal = document.getElementById('removeUserModal');
const removeUserForm = document.forms['removeUserForm'];
const removeUserFormSubmit = removeUserForm.elements['removeUserFormSubmit'];

function loadForRemove(userId) {
    MeldariUsers.get(userId)
    .then(user => {
        document.getElementById('removeUserModalUsername').textContent = user.username;
        document.getElementById('removeUserModalType').textContent = typeTranslations[user.type + ''];
        document.getElementById('removeUserModalId').textContent = user.id + '';
        const rufu = removeUserForm.elements['removeUserFormUsername'];
        rufu.pattern = user.username;
        rufu.value = '';
    })
    .catch(error => {
        console.error(error);
        if (error instanceof Response) {
            error.json().then(json => {
                if (json.error) {
                    Utils.createError(json.error)
                } else if (json.fielderrors) {

                }
            });
        } else {
            Utils.createErrorFull(error.name, error.message);
        }
        bootstrap.Modal.getOrCreateInstance(removeUserModal).hide();
    })
    .finally(() => {

    });
}

removeUserModal.addEventListener('show.bs.modal', (e) => { loadForRemove(e.relatedTarget.dataset.value); });

function remove() {
    Utils.switchButton(removeUserFormSubmit);

    const userId = document.getElementById('removeUserModalId').textContent;
    const fd = new FormData(removeUserForm);

    MeldariUsers.remove(userId, fd)
    .then(json => {
        bootstrap.Modal.getOrCreateInstance(removeUserModal).hide();
        document.getElementById('user-' + json.data.id).remove();
        Utils.createSuccess(json.message);
    })
    .catch(error => {
        console.error(error);
        if (error instanceof Response) {
            error.json().then(json => {
                if (json.error) {
                    Utils.createError(json.error)
                    bootstrap.Modal.getOrCreateInstance(removeUserModal).hide();
                } else if (json.fielderrors) {
                    Utils.setFormFieldErrors(removeUserForm, json.fielderrors);
                }
            });
        } else {
            Utils.createErrorFull(error.name, error.message);
            bootstrap.Modal.getOrCreateInstance(removeUserModal).hide();
        }
    })
    .finally(() => {
        Utils.switchButton(removeUserFormSubmit);
    });
}

removeUserForm.addEventListener('submit', (e) => { e.preventDefault(); remove(); });

const editUserModal = document.getElementById('editUserModal');
const editUserForm = document.forms['editUserForm'];
const editUserFormSubmit = editUserForm.elements['editUserFormSubmit'];

function loadForEdit(userId) {
    MeldariUsers.get(userId)
    .then(user => {
        editUserForm.elements['userId'].value = user.id;
        editUserForm.elements['username'].value = user.username;
        editUserForm.elements['email'].value = user.email;
        editUserForm.elements['type'].value = user.type;
        if (user.validUntil) {
            editUserForm.elements['validUntil'].value = new Date(user.validUntil).toInputForm();
        } else {
            editUserForm.elements['validUntil'].value = '';
        }
        editUserForm.elements['timezone'].value = user.settings.timezone;
        editUserForm.elements['language'].value = user.settings.language;
    })
    .catch(error => {
        console.error(error);
        Utils.handleError(error);
        bootstrap.Modal.getOrCreateInstance(editUserModal).hide();
    })
    .finally(() => {});
}

editUserModal.addEventListener('show.bs.modal', (e) => { loadForEdit(e.relatedTarget.value); });

function edit() {
    const fd = new FormData(editUserForm);
    const userId = fd.get('userId');
    MeldariUsers.edit(userId, fd)
    .then(json => {
        updateTableRow(json.data);
        Utils.createSuccess(json.message);
        bootstrap.Modal.getOrCreateInstance(editUserModal).hide();
    })
    .catch(error => {
        console.error(error);
        Utils.handleError(error, editUserForm);
    })
    .finally(() => {

    });
}

editUserForm.addEventListener('submit', (e) => { e.preventDefault(); edit(); });