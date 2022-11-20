# SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

add_executable(meldarictl)

target_sources(meldarictl
    PRIVATE
        main.cpp
        cli.cpp
        cli.h
        configuration.cpp
        configuration.h
        database.cpp
        database.h
        controller.cpp
        controller.h
)

add_subdirectory(../common common)
add_subdirectory(dbmigrations)
add_subdirectory(commands)

target_compile_features(meldarictl PRIVATE cxx_std_17)

target_link_libraries(meldarictl
    PRIVATE
        Qt6::Core
        Qt6::Sql
        FirfuoridaQt6::Core
)

GNUInstallDirs_get_absolute_install_dir(MELDARI_FULL_TRANSLATIONSDIR MELDARI_TRANSLATIONSDIR DATADIR)

target_compile_definitions(meldarictl
    PRIVATE
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_STRICT_ITERATORS
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_CAST_FROM_BYTEARRAY
        QT_USE_QSTRINGBUILDER
        MELDARI_VERSION="${PROJECT_VERSION}"
        MELDARI_TRANSLATIONSDIR="${MELDARI_FULL_TRANSLATIONSDIR}"
        MELDARI_CONFFILE="${CMAKE_INSTALL_FULL_SYSCONFDIR}/meldari.ini"
        $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
)

target_compile_options(meldarictl
    PRIVATE
        -Wall
        -Wcast-align
        -Wno-uninitialized
        -Wempty-body
        -Wformat-security
        -Wformat
        -Winit-self
)

install(TARGETS meldarictl DESTINATION ${CMAKE_INSTALL_BINDIR})

set(meldarictl_TS_FILES
    translations/meldarictl.ts
    translations/meldarictl.de.ts
)

set_source_files_properties(${meldarictl_TS_FILES}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/translations"
)

qt_add_lupdate(meldarictl
    TS_FILES ${meldarictl_TS_FILES}
    OPTIONS -no-obsolete -locations none -source-language en
)

qt_add_lrelease(meldarictl
    TS_FILES ${meldarictl_TS_FILES}
    QM_FILES_OUTPUT_VARIABLE meldarictl_QM_FILES
    OPTIONS -idbased
)

install(FILES ${meldarictl_QM_FILES} DESTINATION ${MELDARI_TRANSLATIONSDIR})
