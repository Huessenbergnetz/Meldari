/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {};

MeldariTmpl.Users = MeldariTmpl.Users || {};

MeldariTmpl.Users.Edit = MeldariTmpl.Users.Edit || {};

MeldariTmpl.Users.Edit.modal = null;

MeldariTmpl.Users.Edit.form = null;

MeldariTmpl.Users.Edit.button = null;

MeldariTmpl.Users.Edit.load = function(userId) {
    Meldari.Users.get(userId)
    .then(user => {
        const f = MeldariTmpl.Users.Edit.form;
        f.elements['userId'].value = user.id;
        f.elements['username'].value = user.username;
        f.elements['email'].value = user.email;
        f.elements['type'].value = user.type;
        f.elements['validUntil'].value = user.validUntil;
        f.elements['timezone'].value = user.settings.timezone;
        f.elements['language'].value = user.settings.language;
    })
    .catch(error => {
        console.error(error);
        MeldariTmpl.handleError(error);
        MeldariTmpl.Users.Edit.modal.hide()
    })
    .finally(() => {});
}

MeldariTmpl.Users.Edit.exec = function() {
    const fd = new FormData(MeldariTmpl.Users.Edit.form);
    const userId = fd.get('userId');
    Meldari.Users.edit(userId, fd).
    .then(json => {
        console.log(json);
    })
    .catch(error => {
        console.error(error);
        MeldariTmpl.handleError(error, MeldariTmpl.Users.Edit.form);
    })
    .finally(() => {

    });
}

MeldariTmpl.Users.Edit.init = function() {
    const eum = document.getElementById('editUserModal');
    if (eum) {
        MeldariTmpl.Users.Edit.modal = bootstrap.Modal.getOrCreateInstance(eum);
        eum.addEventListener('show.bs.modal', (e) => { MeldariTmpl.Users.Edit.load(e.relatedTarget.value); });
        MeldariTmpl.Users.Edit.form = document.forms['editUserForm'];
        MeldariTmpl.Users.Edit.form.addEventListener('submit', (e) => { e.preventDefault(); MeldariTmpl.Users.Edit.exec(); });
        MeldariTmpl.Users.Edit.button = MeldariTmpl.Users.Edit.form.elements['editUserFormSubmit'];
    }
}

MeldariTmpl.Users.Edit.init();
