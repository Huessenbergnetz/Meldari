# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

set(usersettings_FILES
    index.html
)

target_sources(meldaritemplate
    PRIVATE
        ${usersettings_FILES}
)

install(FILES ${usersettings_FILES}
        DESTINATION ${MELDARI_TEMPLATESDIR}/meldari/site/cc/usersettings
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
