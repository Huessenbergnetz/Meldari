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
