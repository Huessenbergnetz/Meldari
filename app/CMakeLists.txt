# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

add_library(meldari SHARED)

target_sources(meldari
    PRIVATE
        meldari.h
        meldari.cpp
        logging.h
        meldariconfig.cpp
        meldariconfig.h
        userauthstoresql.cpp
        userauthstoresql.h
        qtimezonevariant_p.h
        meldariutils.cpp
        meldariutils.h
)

add_subdirectory(controllers)
add_subdirectory(objects)
add_subdirectory(cutelee)
add_subdirectory(validators)

target_link_libraries(meldari
    PRIVATE
        meldaricommon
        Qt6::Core
        Qt6::Network
        Qt6::Sql
        Cutelyst::Core
        Cutelyst::Session
        Cutelyst::Authentication
        Cutelyst::StatusMessage
        Cutelyst::View::Cutelee
        Cutelyst::StaticSimple
        Cutelyst::StaticCompressed
        Cutelyst::Utils::Validator
        Cutelyst::Utils::Sql
        Cutelyst::Utils::Pagination
        Cutelyst::Utils::LangSelect
        Cutelyst::Memcached
        Cutelyst::MemcachedSessionStore
        Cutelyst::CSRFProtection
        Cutelee::Templates
)

target_compile_features(meldari PRIVATE cxx_std_17)

GNUInstallDirs_get_absolute_install_dir(MELDARI_FULL_TRANSLATIONSDIR MELDARI_TRANSLATIONSDIR DATADIR)
GNUInstallDirs_get_absolute_install_dir(MELDARI_FULL_TEMPLATESDIR MELDARI_TEMPLATESDIR DATADIR)

target_compile_definitions(meldari
    PRIVATE
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_STRICT_ITERATORS
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_CAST_FROM_BYTEARRAY
        QT_USE_QSTRINGBUILDER
        MELDARI_VERSION="${PROJECT_VERSION}"
        MELDARI_TRANSLATIONSDIR="${MELDARI_FULL_TRANSLATIONSDIR}"
        MELDARI_TEMPLATESDIR="${MELDARI_FULL_TEMPLATESDIR}"
)

target_compile_options(meldari
    PRIVATE
        -Wall
        -Wcast-align
        -Wno-uninitialized
        -Wempty-body
        -Wformat-security
        -Wformat
        -Winit-self
)

install(TARGETS meldari DESTINATION ${CMAKE_INSTALL_LIBDIR}/cutelyst${Cutelyst3Qt6_VERSION_MAJOR}-qt6-apps)

set(meldari_TS_FILES
    translations/meldari.en.ts
    translations/meldari.de.ts
)

set_source_files_properties(${meldari_TS_FILES}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/translations"
)

qt_add_lupdate(meldari
    TS_FILES ${meldari_TS_FILES}
    OPTIONS -no-obsolete -locations none -source-language en
)

qt_add_lrelease(meldari
    TS_FILES ${meldari_TS_FILES}
    QM_FILES_OUTPUT_VARIABLE meldari_QM_FILES
)

install(FILES ${meldari_QM_FILES} DESTINATION ${MELDARI_TRANSLATIONSDIR})
