/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

const toastContainer = document.getElementById('toastContainer');
var currentToastId = 0;

function switchButton(button) {
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

function togglePageSpinner() {
    const spinner = document.getElementById('mel-page-spinner');
    if (spinner.classList.contains('d-flex')) {
        spinner.classList.replace('d-flex', 'd-none');
    } else {
        spinner.classList.replace('d-none', 'd-flex');
    }
}

function setFormFieldErrors(form, errors) {
    for (const field in errors) {
        if (errors.hasOwnProperty(field)) {
            const formField = form.elements.namedItem(field);
            const feedback = formField.nextElementSibling;
            feedback.innerHTML = errors[field].join('<br>');
            formField.classList.add('is-invalid');
        }
    }
}

function resetFormFieldErrors(form) {
    for (let i = 0; i < form.elements.length; ++i) {
        form.elements[i].classList.remove('is-invalid');
    }
}

function createToastFull(icon, iconColor, header, body) {
    const toast = document.createElement('div');
    toast.id = 'toast-' + currentToastId;
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
                               toastContainer.removeChild(toast);
                           });

    toastContainer.appendChild(toast);

    const bsToast = new bootstrap.Toast(toast);
    bsToast.show();

    currentToastId++;
}

function createToastSmall(icon, iconColor, body) {
    const toast = document.createElement('div');
    toast.id = 'toast-' + currentToastId;
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
                               toastContainer.removeChild(toast);
                           });

    toastContainer.appendChild(toast);

    const bsToast = new bootstrap.Toast(toast);
    bsToast.show();

    currentToastId++;
}

function createErrorFull(header, body) {
    createToastFull('exclamation-diamond', 'danger', header, body);
}

function createErrorSmall(body) {
    createToastSmall('exclamation-diamond', 'danger', body);
}

function createError(message) {
    if (message.title && message.text) {
        createErrorFull(message.title, message.text);
    } else {
        createErrorSmall(message.title);
    }
}

function handleError(error, form = null) {
    if (error instanceof Response) {
        error.json().then(json => {
            if (json.error) {
                createError(json.error);
            } else if (json.fielderrors) {
                if (form) {
                    setFormFieldErrors(form, json.fielderrors);
                } else {
                    console.error("Can not set field errors to a null form!");
                }
            }
        });
    } else {
        createErrorFull(error.name, error.message);
    }
}

function createSuccessFull(header, body) {
    createToastFull('check-square', 'success', header, body);
}

function createSuccessSmall(body) {
    createToastSmall('check-square', 'success', body);
}

function createSuccess(message) {
    if (message.title && message.text) {
        createSuccessFull(message.title, message.text);
    } else {
        createSuccessSmall(message.title);
    }
}

function setLang(lang) {
    const bcp47 = lang.replace('_', '-');
    document.documentElement.lang = bcp47;
}

function getLang() {
    return document.documentElement.lang;
}

function setTz(tz) {
    document.documentElement.dataset.timezone = tz;
}

function getTz() {
    return document.documentElement.dataset.timezone;
}

export { switchButton, togglePageSpinner, setFormFieldErrors, resetFormFieldErrors, createErrorSmall, createErrorFull, createError, handleError, createSuccess, setLang, getLang, setTz, getTz };