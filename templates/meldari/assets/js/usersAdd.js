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
    const elNames = ['username','email','password','password_confirmed','validUntil'];
    elNames.foreEach((el) => {MeldariTmpl.Users.Add.form.elements.namedItem(el).value = '';});
    MeldariTmpl.Users.Add.form.elements.namedItem('type').value = 0;
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
    .then(user => {
              console.log(user);
              MeldariTmpl.Users.Add.resetForm();
          })
    .catch(error => {
               if (error instanceof Response) {
                   error.json().then(json => {
                                         console.log(json);
                                         if (json.error) {

                                         } else if (json.fielderrors) {
                                             MeldariTmpl.setFormFieldErrors(MeldariTmpl.Users.Add.form, json.fielderrors);
                                         }
                                     });
               } else {
                   console.log(error);
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
