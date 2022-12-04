/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {}

MeldariTmpl.Users = MeldariTmpl.Users || {}

MeldariTmpl.Users.Remove = MeldariTmpl.Users.Remove || {}

MeldariTmpl.Users.Remove.modal = null;

MeldariTmpl.Users.Remove.form = null;

MeldariTmpl.Users.Remove.load = function(userId) {
    const hdrs = MeldariTmpl.newXhrHeaders();

    fetch('/cc/users/get/' + userId, {
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
    .then(user => {
              document.getElementById('removeUserModalUsername').textContent = user.username;
              document.getElementById('removeUserModalType').textContent = MeldariTmpl.Users.typeTranslations[user.type + ''];
              document.getElementById('removeUserModalId').textContent = user.id + '';
              MeldariTmpl.Users.Remove.form.elements['removeUserFormUsername'].pattern = user.username;
          })
    .catch(error => {
               console.error(error);
               if (error instanceof Response) {
                   error.json().then(json => {
                                     });
               } else {

               }
           })
    .finally(() => {

             });
}

MeldariTmpl.Users.Remove.exec = function() {

}

MeldariTmpl.Users.Remove.init = function() {
    const rum = document.getElementById('removeUserModal');
    if (rum) {
        MeldariTmpl.Users.Remove.modal = bootstrap.Modal.getOrCreateInstance(rum);
        rum.addEventListener('show.bs.modal', (e) => { MeldariTmpl.Users.Remove.load(e.relatedTarget.dataset.value); console.log(e.relatedTarget); });
        MeldariTmpl.Users.Remove.form = document.forms['removeUserForm'];
        MeldariTmpl.Users.Remove.form.addEventListener('submit', (e) => { e.preventDefault(); MeldariTmpl.Users.Remove.exec(); });
    }
}

MeldariTmpl.Users.Remove.init();
