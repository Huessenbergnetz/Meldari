# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

target_sources(meldarictl
    PRIVATE
        m0001_create_users_table.cpp
        m0001_create_users_table.h
        m0002_create_options_table.cpp
        m0002_create_options_table.h
        m0003_create_usersettings_table.cpp
        m0003_create_usersettings_table.h
        m0004_create_domains_table.cpp
        m0004_create_domains_table.h
)
