cmake_minimum_required(VERSION 3.11)
project (wren-bind
    VERSION 0.0.1
    DESCRIPTION "wren-bind"
    HOMEPAGE_URL "https://github.com/robloach/wren-bind"
    LANGUAGES C
)

# Dependencies
find_package(wren QUIET)
if(NOT wren_FOUND)
    include("wren.cmake")
endif()

add_library(wren-bind STATIC
    wren-bind.c
    wren-bind.h
)
target_include_directories(wren-bind PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(wren-bind PUBLIC
    wren
)

# Testing
include(CTest)
enable_testing()
if(BUILD_TESTING)
    set(CTEST_CUSTOM_TESTS_IGNORE
        pkg-config--static
    )
    add_subdirectory(test)
endif()
