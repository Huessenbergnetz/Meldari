# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

set(parts_FILES
    pageHeader.html
    modal.html
    inputWithLabel.html
    textInputWithLabel.html
    emailInputWithLabel.html
    passwordInputWithLabel.html
    selectWithLabel.html
    datetimeInputWithLabel.html
    validUntilInputWithLabel.html
    roPlaintextInputWithLabel.html
)

target_sources(meldaritemplate
    PRIVATE
        ${parts_FILES}
)

install(FILES ${parts_FILES}
        DESTINATION ${MELDARI_TEMPLATESDIR}/meldari/site/cc/parts
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
