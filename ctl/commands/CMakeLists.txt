# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

target_sources(meldarictl
    PRIVATE
        command.cpp
        command.h
        databasecommand.cpp
        databasecommand.h
        databasemigrationcommand.cpp
        databasemigrationcommand.h
        databaserollbackcommand.cpp
        databaserollbackcommand.h
        databaserefreshcommand.cpp
        databaserefreshcommand.h
        databaseresetcommand.cpp
        databaseresetcommand.h
        usercommand.cpp
        usercommand.h
        useraddcommand.cpp
        useraddcommand.h
)

