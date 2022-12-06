/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {}

MeldariTmpl.Users = MeldariTmpl.Users || {}

MeldariTmpl.Users.Remove = MeldariTmpl.Users.Remove || {}

MeldariTmpl.Users.Remove.modal = null;

MeldariTmpl.Users.Remove.form = null;

MeldariTmpl.Users.Remove.button = null;

MeldariTmpl.Users.Remove.load = function(userId) {
    const hdrs = MeldariTmpl.newXhrHeaders();

    Meldari.Users.get(userId)
    .then(user => {
              document.getElementById('removeUserModalUsername').textContent = user.username;
              document.getElementById('removeUserModalType').textContent = MeldariTmpl.Users.typeTranslations[user.type + ''];
              document.getElementById('removeUserModalId').textContent = user.id + '';
              const rufu = MeldariTmpl.Users.Remove.form.elements['removeUserFormUsername'];
              rufu.pattern = user.username;
              rufu.value = '';
          })
    .catch(error => {
               console.error(error);
               if (error instanceof Response) {
                   error.json().then(json => {
                                         if (json.error) {
                                             MeldariTmpl.createError(json.error)
                                         } else if (json.fielderrors) {

                                         }
                                     });
               } else {
                   MeldariTmpl.createErrorFull(error.name, error.message);
               }
               MeldariTmpl.Users.Remove.modal.hide();
           })
    .finally(() => {

             });
}

MeldariTmpl.Users.Remove.exec = function() {
    MeldariTmpl.switchButton(MeldariTmpl.Users.Remove.button);

    const hdrs = MeldariTmpl.newXhrHeaders();
    const userId = document.getElementById('removeUserModalId').textContent;
    const fd = new FormData(MeldariTmpl.Users.Remove.form);

    Meldari.Users.remove(userId, fd)
    .then(json => {
              MeldariTmpl.Users.Remove.modal.hide();
              document.getElementById('user-' + json.data.id).remove();
              MeldariTmpl.createSuccess(json.message);
          })
    .catch(error => {
               console.error(error);
               if (error instanceof Response) {
                   error.json().then(json => {
                                         if (json.error) {
                                             MeldariTmpl.createError(json.error)
                                             MeldariTmpl.Users.Remove.modal.hide();
                                         } else if (json.fielderrors) {
                                             MeldariTmpl.setFormFieldErrors(MeldariTmpl.Users.Remove.form, json.fielderrors);
                                         }
                                     });
               } else {
                   MeldariTmpl.createErrorFull(error.name, error.message);
                   MeldariTmpl.Users.Remove.modal.hide();
               }
          })
    .finally(() => {
                 MeldariTmpl.switchButton(MeldariTmpl.Users.Remove.button);
             });
}

MeldariTmpl.Users.Remove.init = function() {
    const rum = document.getElementById('removeUserModal');
    if (rum) {
        MeldariTmpl.Users.Remove.modal = bootstrap.Modal.getOrCreateInstance(rum);
        rum.addEventListener('show.bs.modal', (e) => { MeldariTmpl.Users.Remove.load(e.relatedTarget.dataset.value); });
        MeldariTmpl.Users.Remove.form = document.forms['removeUserForm'];
        MeldariTmpl.Users.Remove.form.addEventListener('submit', (e) => { e.preventDefault(); MeldariTmpl.Users.Remove.exec(); });
        MeldariTmpl.Users.Remove.button = MeldariTmpl.Users.Remove.form.elements['removeUserFormSubmit'];
    }
}

MeldariTmpl.Users.Remove.init();
