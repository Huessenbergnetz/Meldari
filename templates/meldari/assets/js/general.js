/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {};

MeldariTmpl.newXhrHeaders = function() {
    return new Headers({'X-Requested-With': 'XMLHttpRequest'});
}

MeldariTmpl.createInputDate = function(date) {
    if (date.constructor.name !== 'Date') {
        console.error('createInputDate expects a Date object as input value!');
        return '';
    }

    const y = date.getFullYear();
    const m = date.getMonth() + 1;
    const d = date.getDate();
    const hrs = date.getHours();
    const min = date.getMinutes();
    const rv = y + '-' + (m < 10 ? '0' : '') + m + '-' + (d < 10 ? '0' : '') + d + 'T' + (hrs < 10 ? '0' : '') + hrs + ':' + (min < 10 ? '0' : '') + min;
    return rv;
}

MeldariTmpl.switchButton = function(button) {
    if (button.disabled) {
        button.disabled = false;
        const spinner = button.getElementsByTagName('span')[0];
        spinner.classList.remove('d-inline-block');
        spinner.classList.add('d-none');
        const icon = button.getElementsByTagName('i')[0];
        icon.classList.remove('d-none');
    } else {
        button.disabled = true;
        const spinner = button.getElementsByTagName('span')[0];
        spinner.classList.remove('d-none');
        spinner.classList.add('d-inline-block');
        const icon = button.getElementsByTagName('i')[0];
        icon.classList.add('d-none');
    }
}

MeldariTmpl.setFormFieldErrors = function(form, errors) {
    for (const field in errors) {
        if (errors.hasOwnProperty(field)) {
            const formField = form.elements.namedItem(field);
            const feedback = formField.nextElementSibling;
            feedback.innerHTML = errors[field].join('<br>');
            formField.classList.add('is-invalid');
        }
    }
}

MeldariTmpl.resetFormFieldErrors = function(form) {
    for (let i = 0; i < form.elements.length; ++i) {
        form.elements[i].classList.remove('is-invalid');
    }
}
