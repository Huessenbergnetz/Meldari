# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

add_custom_target(meldaritemplate
    SOURCES
        package.json
        webpack.mix.js
)

add_subdirectory(assets)
add_subdirectory(site)

install(DIRECTORY static
        DESTINATION ${MELDARI_TEMPLATESDIR}/meldari
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
        DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
