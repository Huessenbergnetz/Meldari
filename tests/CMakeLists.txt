# SPDX-FileCopyrightText: (C) 2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: AGPL-3.0-or-later

function (meldari_test_common _testname)
    add_executable(${_testname}_exec ${_testname}.cpp)
    add_test(NAME ${_testname} COMMAND ${_testname}_exec)
    target_compile_features(${_testname}_exec PRIVATE cxx_std_17)
    target_link_libraries(${_testname}_exec Qt6::Core Qt6::Test meldaricommon)
endfunction (meldari_test_common _testname)

meldari_test_common(testuser)