# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

set(site_FILES
    base.html
    wrapper.html
    index.html
)

target_sources(meldaritemplate
    PRIVATE
        ${site_FILES}
)

add_subdirectory(cc)

install(FILES ${site_FILES}
        DESTINATION ${MELDARI_TEMPLATESDIR}/meldari/site
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
