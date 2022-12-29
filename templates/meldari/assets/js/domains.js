/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

import * as MeldariDomains from '/common/js/domains.min.js';
import * as Utils from './utils.min.js';

const rowTemplate = document.getElementById('domain-row-template');
const statusTranslations = JSON.parse(document.getElementById('domain-stati-translated').textContent);
const tableBody = document.getElementById('domainsTable').getElementsByTagName('tbody')[0];

function addTableRow(domain, prepend) {
    const row = rowTemplate.content.cloneNode(true);
    const tr = row.querySelector('tr');
    tr.id = 'domain-' + domain.id;

    const tds = tr.getElementsByTagName('td');
    const actionsTd = tds[0];
    const nameTd = tds[1];
    const statusTd = tds[2];
    const idTd = tds[3];

    const editBtn = actionsTd.getElementsByClassName('edit-domain-btn')[0];
    editBtn.value = domain.id;

    const delBtn = actionsTd.getElementsByClassName('delete-domain-btn')[0];
    delBtn.dataset.value = domain.id;
    delBtn.dataset.bsToggle = 'modal';
    delBtn.dataset.bsTarget = '#removeDomainModal';

    nameTd.textContent = domain.name;
    statusTd.textContent = statusTranslations[domain.status + ''];
    idTd.textContent = domain.id;

    if (prepend) {
        tableBody.prepend(row);
    } else {
        tableBody.append(row);
    }
}

const addDomainModal = document.getElementById('addDomainModal');
const addDomainForm = document.forms['addDomainForm'];
const addDomainFormSubmit = addDomainForm.elements['addDomainFormSubmit'];

function resetAddDomainForm() {
    const elNames = ['name'];
    elNames.forEach((el) => {addDomainForm.elements.namedItem(el).value = ''});
    addDomainForm.elements['status'].value = 0;
}

function add() {
    Utils.switchButton(addDomainFormSubmit);

    const fd = new FormData(addDomainForm);

    MeldariDomains.add(fd)
    .then(json => {
        resetAddDomainForm();
        bootstrap.Modal.getOrCreateInstance(addDomainModal).hide();
        addTableRow(json.data, true);
        Utils.createSuccess(json.message);
    })
    .catch(error => {
        console.error(error);
        Utils.handleError(error, addDomainForm);
    })
    .finally(() => {
        Utils.switchButton(addDomainFormSubmit);
    })
}

addDomainForm.addEventListener('submit', (e) => { e.preventDefault(); add(); });