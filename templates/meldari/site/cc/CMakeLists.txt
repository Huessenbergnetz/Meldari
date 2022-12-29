# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

set(cc_FILES
    base.html
    wrapper.html
    index.html
    login.html
)

add_subdirectory(parts)
add_subdirectory(users)
add_subdirectory(usersettings)
add_subdirectory(domains)

target_sources(meldaritemplate
    PRIVATE
        ${cc_FILES}
)

install(FILES ${cc_FILES}
        DESTINATION ${MELDARI_TEMPLATESDIR}/meldari/site/cc
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
