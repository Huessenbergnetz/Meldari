/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

function initValidUntilInputs() {
    const inputs = document.getElementsByClassName('valid-until-input');
    for (let i = 0; i < inputs.length; ++i) {
        const input = inputs[i].getElementsByTagName('input')[0];
        const as = inputs[i].getElementsByClassName('dropdown-item');
        for (let j = 0; j < as.length; ++j) {
            as[j].addEventListener('click', (e) => {
                e.preventDefault();
                const val = e.target.dataset.value;
                const count = parseInt(val.slice(0, val.length - 1));
                const now = new Date();
                let date;
                if (val.endsWith('d')) {
                    date = now.addDays(count);
                } else if (val.endsWith('w')) {
                    date = now.addWeeks(count);
                } else if (val.endsWith('m')) {
                    date = now.addMonths(count);
                } else if (val.endsWith('y')) {
                    date = now.addYears(count);
                }
                input.value = date.toInputForm();
            });
        }
    }
}

export default initValidUntilInputs;