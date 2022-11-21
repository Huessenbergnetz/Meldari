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
)

add_subdirectory(../common common)
add_subdirectory(controllers)

target_link_libraries(meldari
    PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Sql
        Cutelyst::Core
        Cutelyst::Session
        Cutelyst::Authentication
        Cutelyst::StatusMessage
        Cutelyst::View::Cutelee
        Cutelyst::StaticSimple
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
