cmake_minimum_required(VERSION 3.1)

enable_testing()

set(UNIT_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/Main.cpp

    # Common
    ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/common/Utils.cpp

    # engine

    # parser
)

# Build test executable
add_executable(unit-tests ${UNIT_TEST_SOURCES})
target_link_libraries(unit-tests common-static parser-static engine-static)
add_test(unit-tests unit-tests)
