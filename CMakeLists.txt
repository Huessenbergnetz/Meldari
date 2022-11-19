# SPDX-FileCopyrightText: (C) 2019 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

cmake_minimum_required(VERSION 3.20)

project(Meldari
    VERSION 0.0.1
    DESCRIPTION "Security reporting endpoint based on Cutelyst"
    HOMEPAGE_URL https://github.com/Huessenbergnetz/Meldari
    LANGUAGES CXX
)

if (POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW)
endif()

find_package(Qt6 6.2.0 COMPONENTS Core Network Sql LinguistTools REQUIRED)
find_package(Cutelyst3Qt6 REQUIRED)
find_package(Cutelee6Qt6 REQUIRED)
find_package(FirfuoridaQt6 REQUIRED)

# Auto generate moc files
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)

set(MELDARI_APPDIR ${CMAKE_INSTALL_DATADIR}/meldari CACHE PATH "Directory to install Meldari data files into")
set(MELDARI_TEMPLATESDIR ${MELDARI_APPDIR}/templates CACHE PATH "Directory to install Meldari template files into")
set(MELDARI_TRANSLATIONSDIR ${MELDARI_APPDIR}/translations CACHE PATH "Directory to install Meldari translation files into")

add_subdirectory(app)
add_subdirectory(ctl)
