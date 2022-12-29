/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

import * as MeldariGeneral from '/common/js/general.js';

MeldariGeneral.init();

if (document.getElementById('usersTable')) {
    import('./users.min.js');
}

if (document.forms['usersettingsForm']) {
    import('./usersettings.min.js');
}