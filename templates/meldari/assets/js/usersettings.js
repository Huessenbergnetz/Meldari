/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {}

MeldariTmpl.UserSettings = MeldariTmpl.UserSettings || {}

MeldariTmpl.UserSettings.form = null;

MeldariTmpl.UserSettings.button = null;

MeldariTmpl.UserSettings.exec = function() {
    MeldariTmpl.switchButton(MeldariTmpl.UserSettings.button);
    MeldariTmpl.resetFormFieldErrors(MeldariTmpl.UserSettings.form);
    const fd = new FormData(MeldariTmpl.UserSettings.form);

    Meldari.Usersettings.update(fd)
    .then(json => {
              MeldariTmpl.setLang(json.data.settings.language);

              const f = MeldariTmpl.UserSettings.form;

              MeldariTmpl.tz = json.data.settings.timezone;
              document.documentElement.dataset.timezone = MeldariTmpl.tz;

              const localeDtOptions = {dateStyle: "long", timeStyle: "medium", timeZone: MeldariTmpl.tz}

              const created = new Date(json.data.created);
              f.elements['created'].value = created.toLocaleString(MeldariTmpl.lang, localeDtOptions);

              const updated = new Date(json.data.updated);
              f.elements['updated'].value = updated.toLocaleString(MeldariTmpl.lang, localeDtOptions);

              if (json.data.lastSeen) {
                  const lastSeen = new Date(json.data.lastSeen);
                  f.elements['lastSeen'].value = lastSeen.toLocaleString(MeldariTmpl.lang, localeDtOptions);
              } else {
                  f.elements['lastSeen'].value = '';
              }

              if (json.data.validUntil) {
                  const validUntil = new Date(json.data.validUntil);
                  f.elements['validUntil'].value = validUntil.toLocaleString(MeldariTmpl.lang, localeDtOptions);
              } else {
                  f.elements['validUntil'].value = '';
              }

              f.elements['password'].value = '';
              f.elements['newpassword'].value = '';
              f.elements['newpassword_confirmation'].value = '';

              MeldariTmpl.createSuccess(json.message);
          })
    .catch(error => {
        console.log(error);
        MeldariTmpl.handleError(error, MeldariTmpl.UserSettings.form);
    })
    .finally(() => {
                 MeldariTmpl.switchButton(MeldariTmpl.UserSettings.button);
             });
}

MeldariTmpl.UserSettings.init = function() {
    MeldariTmpl.UserSettings.form = document.forms['usersettingsForm'];
    if (MeldariTmpl.UserSettings.form) {
        MeldariTmpl.UserSettings.form.addEventListener('submit', (e) => { e.preventDefault(); MeldariTmpl.UserSettings.exec(); });
        MeldariTmpl.UserSettings.button = MeldariTmpl.UserSettings.form.elements['userSettingsFormSubmit'];
    }
}

MeldariTmpl.UserSettings.init();
