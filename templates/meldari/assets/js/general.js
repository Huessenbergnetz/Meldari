/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

var MeldariTmpl = MeldariTmpl || {};

MeldariTmpl.currentToastId = 0;

MeldariTmpl.toastContainer = null;

MeldariTmpl.lang = null;

MeldariTmpl.tz = null;

MeldariTmpl.newXhrHeaders = function() {
    return new Headers({'X-Requested-With': 'XMLHttpRequest'});
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

MeldariTmpl.createToastFull = function(icon, iconColor, header, body) {
    const toast = document.createElement('div');
    toast.id = 'toast-' + MeldariTmpl.currentToastId;
    toast.className = 'toast align-items-center';
    toast.setAttribute('role', 'alert');
    toast.ariaLive = 'assertive';
    toast.ariaAtomic = 'true';
    const toastHeader = toast.appendChild(document.createElement('div'));
    toastHeader.className = 'toast-header';
    const i = toastHeader.appendChild(document.createElement('i'));
    i.className = 'bi bi-' + icon + ' text-' + iconColor + ' me-2';
    const strong = toastHeader.appendChild(document.createElement('strong'));
    strong.className = 'me-auto';
    strong.appendChild(document.createTextNode(header));
    const button = toastHeader.appendChild(document.createElement('button'));
    button.type = 'button';
    button.className = 'btn-close';
    button.ariaLabel = 'Close';
    button.dataset.bsDismiss = 'toast';
    const toastBody = toast.appendChild(document.createElement('div'));
    toastBody.className = 'toast-body';
    toastBody.appendChild(document.createTextNode(body));

    toast.addEventListener('hidden.bs.toast', () => {
                               MeldariTmpl.toastContainer.removeChild(toast);
                           });

    MeldariTmpl.toastContainer.appendChild(toast);

    const bsToast = new bootstrap.Toast(toast);
    bsToast.show();

    MeldariTmpl.currentToastId++;
}

MeldariTmpl.createToastSmall = function(icon, iconColor, body) {
    const toast = document.createElement('div');
    toast.id = 'toast-' + MeldariTmpl.currentToastId;
    toast.className = 'toast align-items-center';
    toast.setAttribute('role', 'alert');
    toast.ariaLive = 'assertive';
    toast.ariaAtomic = 'true';
    const flex = toast.appendChild(document.createElement('div'));
    flex.className = 'd-flex';
    const toastBody = flex.appendChild(document.createElement('div'));
    toastBody.className = 'toast-body';
    const i = toastBody.appendChild(document.createElement('i'));
    i.className = 'bi bi-' + icon + ' text-' + iconColor + ' me-2';
    const span = toastBody.appendChild(document.createElement('span'));
    span.appendChild(document.createTextNode(body));
    const button = flex.appendChild(document.createElement('button'));
    button.type = 'button';
    button.className = 'btn-close me-2 m-auto';
    button.ariaLabel = 'Close';
    button.dataset.bsDismiss = 'toast';

    toast.addEventListener('hidden.bs.toast', () => {
                               MeldariTmpl.toastContainer.removeChild(toast);
                           });

    MeldariTmpl.toastContainer.appendChild(toast);

    const bsToast = new bootstrap.Toast(toast);
    bsToast.show();

    MeldariTmpl.currentToastId++;
}

MeldariTmpl.createErrorFull = function(header, body) {
    MeldariTmpl.createToastFull('exclamation-diamond', 'danger', header, body);
}

MeldariTmpl.createErrorSmall = function(body) {
    MeldariTmpl.createToastSmall('exclamation-diamond', 'danger', body);
}

MeldariTmpl.createError = function(message) {
    if (message.title && message.text) {
        MeldariTmpl.createErrorFull(message.title, message.text);
    } else {
        MeldariTmpl.createErrorSmall(message.title);
    }
}

MeldariTmpl.handleError = function(error, form = null) {
    if (error instanceof Response) {
        error.json().then(json => {
            if (json.error) {
                MeldariTmpl.createError(json.error);
            } else if (json.fielderrors) {
                if (form) {
                    MeldariTmpl.setFormFieldErrors(form, json.fielderrors);
                } else {
                    console.error("Can not set field errors to a null form!");
                }
            }
        });
    } else {
        MeldariTmpl.createErrorFull(error.name, error.message);
    }
}

MeldariTmpl.createSuccessFull = function(header, body) {
    MeldariTmpl.createToastFull('check-square', 'success', header, body);
}

MeldariTmpl.createSuccessSmall = function(body) {
    MeldariTmpl.createToastSmall('check-square', 'success', body);
}

MeldariTmpl.createSuccess = function(message) {
    if (message.title && message.text) {
        MeldariTmpl.createSuccessFull(message.title, message.text);
    } else {
        MeldariTmpl.createSuccessSmall(message.title);
    }
}

MeldariTmpl.setLang = function(lang) {
    const bcp47 = lang.replace('_', '-');
    MeldariTmpl.lang = bcp47;
    document.documentElement.lang = bcp47;
}

MeldariTmpl.init = function() {
    MeldariTmpl.toastContainer = document.getElementById('toastContainer');
    MeldariTmpl.lang = document.documentElement.lang;
    MeldariTmpl.tz = document.documentElement.dataset.timezone;
}

MeldariTmpl.init();
