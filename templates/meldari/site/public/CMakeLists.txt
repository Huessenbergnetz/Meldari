# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

set(public_FILES
    base.html
    wrapper.html
    index.html
)

target_sources(meldaritemplate
    PRIVATE
        ${public_FILES}
)

install(FILES ${public_FILES}
        DESTINATION ${MELDARI_TEMPLATESDIR}/meldari/site/public
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
