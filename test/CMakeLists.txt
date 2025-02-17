cmake_minimum_required(VERSION 3.28)
project(Taskify_tests)
# In locale ho Gtest su questa cartella "lib/googletest". Su git evito di caricare tutto Gtest.
add_subdirectory(./lib/googletest)
set(gtest_SOURCE_DIR, ./lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_package(wxWidgets REQUIRED)

set(TEST_SOURCE_FILES tests.cpp
        XMLParserTest.cpp)

add_executable(Taskify_tests ${TEST_SOURCE_FILES})

target_link_libraries(Taskify_tests PRIVATE
        core
        gtest
        gtest_main
        ${wxWidgets_LIBRARIES}
)