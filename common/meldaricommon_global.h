/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef MELDARICOMMON_GLOBAL_H
#define MELDARICOMMON_GLOBAL_H

#include <QtGlobal>

#if defined(meldaricommon_EXPORTS)
#define MELDARICOMMON_LIBRARY Q_DECL_EXPORT
#else
#define MELDARICOMMON_LIBRARY Q_DECL_IMPORT
#endif

#endif // MELDARICOMMON_GLOBAL_H
