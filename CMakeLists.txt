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
pkg_search_module(Pwquality REQUIRED IMPORTED_TARGET pwquality>=1.2.2)

# Auto generate moc files
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)

set(MELDARI_APPDIR ${CMAKE_INSTALL_DATADIR}/meldari CACHE PATH "Directory to install Meldari data files into")
set(MELDARI_TEMPLATESDIR ${MELDARI_APPDIR}/templates CACHE PATH "Directory to install Meldari template files into")
set(MELDARI_TRANSLATIONSDIR ${MELDARI_APPDIR}/translations CACHE PATH "Directory to install Meldari translation files into")

option(BUILD_DOCS "Add the make docs target to build the documentation. Requires doxygen and dot" OFF)
option(BUILD_DOCS_QUIET "Tell doxygen to be quiet while building the documentation." OFF)

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
set(MELDARI_CONF_MEL_PWQUALITYSETTINGSFILE "pwqualitysettingsfile")
set(MELDARI_CONF_MEL_PWQUALITYSETTINGSFILE_DEFVAL "")

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
add_subdirectory(templates)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif (BUILD_TESTS)

if (BUILD_DOCS)
    find_package(Doxygen REQUIRED dot)

    if (BUILD_DOCS_QUIET)
        set(DOXYGEN_QUIET "YES")
        set(DOXYGEN_WARNINGS "NO")
        set(DOXYGEN_WARN_IF_UNDOCUMENTED "NO")
        set(DOXYGEN_WARN_IF_DOC_ERROR "NO")
    else(BUILD_DOCS_QUIET)
        set(DOXYGEN_QUIET "NO")
        set(DOXYGEN_WARNINGS "YES")
        set(DOXYGEN_WARN_IF_UNDOCUMENTED "YES")
        set(DOXYGEN_WARN_IF_DOC_ERROR "YES")
    endif(BUILD_DOCS_QUIET)

    set(DOXYGEN_STRIP_FROM_PATH ${CMAKE_SOURCE_DIR})
    set(DOXYGEN_BUILTIN_STL_SUPPORT "YES")
    set(DOXYGEN_VERBATIM_HEADERS "NO")
    set(DOXYGEN_ALWAYS_DETAILED_SEC "YES")
    set(DOXYGEN_HIDE_UNDOC_MEMBERS "YES")
    set(DOXYGEN_HIDE_UNDOC_CLASSES "YES")
#    set(DOXYGEN_LAYOUT_FILE ${CMAKE_SOURCE_DIR}/dox/DoxygenLayout.xml)
    set(DOXYGEN_COLLABORATION_GRAPH "NO")
#    set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR}/dox/examples)
    set(DOXYGEN_SORT_BRIEF_DOCS "YES")
    set(DOXYGEN_SORT_MEMBERS_CTORS_1ST "YES")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)
    set(DOXYGEN_SHOW_USED_FILES "NO")
    set(DOXYGEN_EXCLUDE_PATTERNS "*/node_modules/*")
    set(DOXYGEN_FILE_PATTERNS *.cpp *.h *.dox *.md)
#    set(DOXYGEN_EXTENSION_MAPPING js=C++)

    set(DOXYGEN_HTML_TIMESTAMP "YES")
#    set(DOXYGEN_TAGFILES ${ONLINETAGS})
    set(DOXYGEN_HTML_OUTPUT "webdocs")
    doxygen_add_docs(webdocs
        ${CMAKE_CURRENT_SOURCE_DIR}/app
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/commonjs
        ${CMAKE_CURRENT_SOURCE_DIR}/ctl
        COMMENT "Generate HTML documentation for online usage"
    )
    message(STATUS "Adding build target webdocs")

    set(DOXYGEN_HTML_TIMESTAMP "NO")
#    set(DOXYGEN_TAGFILES ${LOCALHTMLTAGS})
    set(DOXYGEN_HTML_OUTPUT "htmldocs")
    doxygen_add_docs(htmldocs
        ${CMAKE_CURRENT_SOURCE_DIR}/app
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/commonjs
        ${CMAKE_CURRENT_SOURCE_DIR}/ctl
        COMMENT "Generate HTML documentation for local usage"
    )
    message(STATUS "Adding build target htmldocs")

endif (BUILD_DOCS)
