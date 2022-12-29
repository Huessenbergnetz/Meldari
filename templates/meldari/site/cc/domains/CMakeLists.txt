# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

set(domains_FILES
    index.html
    indexPageHeader.html
    addDomainModal.html
)

target_sources(meldaritemplate
    PRIVATE
        ${domains_FILES}
)

install(FILES ${domains_FILES}
        DESTINATION ${MELDARI_TEMPLATESDIR}/meldari/site/cc/domains
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
