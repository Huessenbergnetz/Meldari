/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {};

MeldariTmpl.validUntilInput = MeldariTmpl.validUntilInput || {};

MeldariTmpl.validUntilInput.init = function() {
    const inputs = document.getElementsByClassName('valid-until-input');
    for (let i = 0; i < inputs.length; ++i) {
        const input = inputs[i].getElementsByTagName('input')[0];
        const as = inputs[i].getElementsByClassName('dropdown-item');
        for (let j = 0; j < as.length; ++j) {
            as[j].addEventListener('click', (e) => {
                                       e.preventDefault();
                                       const date = new Date(Date.now() + parseInt(e.target.dataset.value) * 1000);
                                       input.value = MeldariTmpl.createInputDate(date);
                                   });
        }
    }
}

MeldariTmpl.validUntilInput.init();
