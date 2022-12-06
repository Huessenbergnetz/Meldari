# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
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
find_package(PkgConfig REQUIRED)

# Auto generate moc files
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)

set(MELDARI_APPDIR ${CMAKE_INSTALL_DATADIR}/meldari CACHE PATH "Directory to install Meldari data files into")
set(MELDARI_TEMPLATESDIR ${MELDARI_APPDIR}/templates CACHE PATH "Directory to install Meldari template files into")
set(MELDARI_TRANSLATIONSDIR ${MELDARI_APPDIR}/translations CACHE PATH "Directory to install Meldari translation files into")
set(MELDARI_COMMONJSDIR ${MELDARI_APPDIR}/commonjs CACHE PATH "Directory to install common JS files into")

option(BUILD_TESTS "Build the tests" OFF)
if (BUILD_TESTS)
    find_package(Qt6 6.2.0 COMPONENTS Test REQUIRED)
    enable_testing()
endif (BUILD_TESTS)

# Set configuration key names and default values

set(MELDARI_CONF_DB "database")
set(MELDARI_CONF_DB_TYPE "type")
set(MELDARI_CONF_DB_TYPE_DEFVAL "QMYSQL")
set(MELDARI_CONF_DB_HOST "host")
set(MELDARI_CONF_DB_HOST_DEFVAL "localhost")
set(MELDARI_CONF_DB_USER "user")
set(MELDARI_CONF_DB_USER_DEFVAL "meldari")
set(MELDARI_CONF_DB_PASS "password")
set(MELDARI_CONF_DB_NAME "name")
set(MELDARI_CONF_DB_NAME_DEFVAL "meldaridb")
set(MELDARI_CONF_DB_PORT "port")
set(MELDARI_CONF_DB_PORT_DEFVAL 3306)

set(MELDARI_CONF_MEL "meldari")
set(MELDARI_CONF_MEL_TEMPLATE "template")
set(MELDARI_CONF_MEL_TEMPLATE_DEFVAL "meldari")
set(MELDARI_CONF_MEL_SITENAME "sitename")
set(MELDARI_CONF_MEL_SITENAME_DEFVAL "Meldari")
set(MELDARI_CONF_MEL_USEMEMCACHED "usememcached")
set(MELDARI_CONF_MEL_USEMEMCACHED_DEFVAL false)
set(MELDARI_CONF_MEL_USEMEMCACHEDSESSION "usememcachedsession")
set(MELDARI_CONF_MEL_USEMEMCACHEDSESSION_DEFVAL false)
set(MELDARI_CONF_MEL_STATICPLUGIN "staticplugin")
set(MELDARI_CONF_MEL_STATICPLUGIN_DEFVAL "simple")

set(MELDARI_CONF_MAIL "email")
SET(MELDARI_CONF_MAIL_HOST "host")
SET(MELDARI_CONF_MAIL_PORT "port")
SET(MELDARI_CONF_MAIL_PORT_DEFVAL 465)
SET(MELDARI_CONF_MAIL_FROM_MAIL "frommail")
SET(MELDARI_CONF_MAIL_FROM_NAME "fromname")
SET(MELDARI_CONF_MAIL_FROM_NAME_DEFVAL "Meldari")
SET(MELDARI_CONF_MAIL_USER "user")
SET(MELDARI_CONF_MAIL_PASS "password")
SET(MELDARI_CONF_MAIL_ENC "encryption")
SET(MELDARI_CONF_MAIL_ENC_DEFVAL "tls")

add_subdirectory(common)
add_subdirectory(app)
add_subdirectory(ctl)
add_subdirectory(commonjs)
add_subdirectory(templates)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif (BUILD_TESTS)
