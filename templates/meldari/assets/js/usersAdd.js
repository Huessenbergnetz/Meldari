/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {}

MeldariTmpl.Users = MeldariTmpl.Users || {}

MeldariTmpl.Users.Add = MeldariTmpl.Users.Add || {}

MeldariTmpl.Users.Add.modal = null;

MeldariTmpl.Users.Add.form = null;

MeldariTmpl.Users.Add.button = null;

MeldariTmpl.Users.Add.resetForm = function() {
    const elNames = ['username','email','password','password_confirmation','validUntil'];
    elNames.forEach((el) => {MeldariTmpl.Users.Add.form.elements.namedItem(el).value = '';});
    MeldariTmpl.Users.Add.form.elements['type'].value = 0;
}

MeldariTmpl.Users.Add.exec = function() {
    MeldariTmpl.switchButton(MeldariTmpl.Users.Add.button);

    const fd = new FormData(MeldariTmpl.Users.Add.form);
    const hdrs = MeldariTmpl.newXhrHeaders();

    fetch('/cc/users/add', {
              method: 'POST',
              headers: hdrs,
              body: fd
          })
    .then(response => {
              if (response.ok) {
                  return response.json();
              } else {
                  return Promise.reject(response);
              }
          })
    .then(json => {
              MeldariTmpl.Users.Add.resetForm();
              MeldariTmpl.Users.Add.modal.hide();
              MeldariTmpl.Users.addTableRow(json.data, true);
              MeldariTmpl.createSuccess(json.message);
          })
    .catch(error => {
               console.error(error);
               if (error instanceof Response) {
                   error.json().then(json => {
                                         if (json.error) {
                                             MeldariTmpl.createError(json.error)
                                         } else if (json.fielderrors) {
                                             MeldariTmpl.setFormFieldErrors(MeldariTmpl.Users.Add.form, json.fielderrors);
                                         }
                                     });
               } else {
                   MeldariTmpl.createErrorFull(error.name, error.message);
               }
           })
    .finally(() => {
                 MeldariTmpl.switchButton(MeldariTmpl.Users.Add.button);
             });
}

MeldariTmpl.Users.Add.init = function() {
    const aum = document.getElementById('addUserModal');
    if (aum) {
        MeldariTmpl.Users.Add.modal = bootstrap.Modal.getOrCreateInstance(aum);
        MeldariTmpl.Users.Add.form = document.forms['addUserForm'];
        MeldariTmpl.Users.Add.form.addEventListener('submit', (e) => { e.preventDefault(); MeldariTmpl.Users.Add.exec(); });
        MeldariTmpl.Users.Add.button = MeldariTmpl.Users.Add.form.elements['addUserFormSubmit'];
    }
}

MeldariTmpl.Users.Add.init();
