/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

import * as MeldariUsersettings from '/common/js/usersettings.min.js';
import * as Utils from './utils.min.js';

const form = document.forms['usersettingsForm'];
const button = form.elements['userSettingsFormSubmit'];

function update() {
    Utils.switchButton(button);
    Utils.resetFormFieldErrors(form);
    const fd = new FormData(form);

    MeldariUsersettings.update(fd)
    .then(json => {
        Utils.setLang(json.data.settings.language);
        const lang = Utils.getLang();
        
        const tz = json.data.settings.timezone;
        Utils.setTz(tz);
        
        const localeDtOptions = {dateStyle: "long", timeStyle: "medium", timeZone: tz};
        
        const created = new Date(json.data.created);
        form.elements['created'].value = created.toLocaleString(lang, localeDtOptions);

        const updated = new Date(json.data.updated);
        form.elements['updated'].value = updated.toLocaleString(lang, localeDtOptions);

        if (json.data.lastSeen) {
            const lastSeen = new Date(json.data.lastSeen);
            form.elements['lastSeen'].value = lastSeen.toLocaleString(lang, localeDtOptions);
        } else {
            form.elements['lastSeen'].value = '';
        }

        if (json.data.validUntil) {
            const validUntil = new Date(json.data.validUntil);
            form.elements['validUntil'].value = validUntil.toLocaleString(lang, localeDtOptions);
        } else {
            form.elements['validUntil'].value = '';
        }

        form.elements['password'].value = '';
        form.elements['newpassword'].value = '';
        form.elements['newpassword_confirmation'].value = '';

        Utils.createSuccess(json.message);
    })
    .catch(error => {
        console.log(error);
        Utils.handleError(error, form);
    })
    .finally(() => {
        Utils.switchButton(button);
    });
}

form.addEventListener('submit', (e) => { e.preventDefault(); update(); });
