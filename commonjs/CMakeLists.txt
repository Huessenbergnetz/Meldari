# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

add_custom_target(commonjs
    SOURCES
        package.json
        webpack.mix.js
        general.js
        users.js
        usersettings.js
)
