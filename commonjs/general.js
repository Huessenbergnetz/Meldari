/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

Date.prototype.addDays = function(days) {
    let date = new Date(this.valueOf());
    date.setDate(date.getDate() + days);
    return date;
}

Date.prototype.addWeeks = function(weeks) {
    let date = new Date(this.valueOf());
    return date.addDays(weeks * 7);
}

Date.prototype.addMonths = function(months) {
    let date = new Date(this.valueOf());
    date.setMonth(date.getMonth() + months);
    return date;
}

Date.prototype.addYears = function(years) {
    let date = new Date(this.valueOf());
    date.setFullYear(date.getFullYear() + years);
    return date;
}

var Meldari = Meldari || {};

Meldari.newXhrHeaders = function() {
    return new Headers({'X-Requested-With': 'XMLHttpRequest'});
}
