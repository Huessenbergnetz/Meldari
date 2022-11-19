/*
 * SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICTL_RETURNCODES_H
#define MELDARICTL_RETURNCODES_H

namespace RC {
    enum ReturnCodes : int {
        OK = 0,
        InvalidOption = 1
    };
}

#endif // MELDARICTL_RETURNCODES_H
