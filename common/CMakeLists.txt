# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

pkg_search_module(Botan REQUIRED IMPORTED_TARGET botan-2)

add_library(meldaricommon SHARED)

target_sources(meldaricommon
    PRIVATE
        confignames.h.in
        baseuser.cpp
        baseuser.h
        baseuser_p.h
        meldaricommon_global.h
        credentialbotan.cpp
        credentialbotan.h
        credentialbotan_p.h
        utils.cpp
        utils.h
)

configure_file(confignames.h.in confignames.h)

target_compile_features(meldaricommon PRIVATE cxx_std_17)

target_include_directories(meldaricommon
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(meldaricommon
    PUBLIC
        Qt6::Core
        Cutelyst::Core
        Cutelyst::Authentication
    PRIVATE
        PkgConfig::Botan
)

target_compile_definitions(meldaricommon
    PRIVATE
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_STRICT_ITERATORS
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_CAST_FROM_BYTEARRAY
        QT_USE_QSTRINGBUILDER
)

target_compile_options(meldaricommon
    PRIVATE
        -Wall
        -Wcast-align
        -Wno-uninitialized
        -Wempty-body
        -Wformat-security
        -Wformat
        -Winit-self
)

install(TARGETS meldaricommon DESTINATION ${CMAKE_INSTALL_LIBDIR})
