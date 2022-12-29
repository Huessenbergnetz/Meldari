/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

function init() {
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
    
    Date.prototype.toInputForm = function() {
        const y = this.getFullYear();
        const m = this.getMonth() + 1;
        const d = this.getDate();
        const hrs = this.getHours();
        const min = this.getMinutes();
        const rv = y + '-' + (m < 10 ? '0' : '') + m + '-' + (d < 10 ? '0' : '') + d + 'T' + (hrs < 10 ? '0' : '') + hrs + ':' + (min < 10 ? '0' : '') + min;
        return rv;
    }
}

export { init };
