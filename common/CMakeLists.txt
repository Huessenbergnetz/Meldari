# SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

if (TARGET meldarictl)
    target_sources(meldarictl
        PRIVATE
            confignames.h.in
    )
endif (TARGET meldarictl)

if (TARGET meldari)
    target_sources(meldari
        PRIVATE
            confignames.h.in
    )
endif (TARGET meldarictl)

configure_file(confignames.h.in confignames.h)

if (TARGET meldarictl)
    target_include_directories(meldarictl
        PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif (TARGET meldarictl)

if (TARGET meldari)
    target_include_directories(meldari
        PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif (TARGET meldarictl)
