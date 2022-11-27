# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

set(users_FILES
    index.html
    addUserModal.html
    indexPageHeader.html
)

target_sources(meldaritemplate
    PRIVATE
        ${users_FILES}
)

install(FILES ${users_FILES}
        DESTINATION ${MELDARI_TEMPLATESDIR}/meldari/site/cc/users
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
