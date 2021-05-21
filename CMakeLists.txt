cmake_minimum_required(VERSION 3.11)
project (wren-bind
    VERSION 0.1.0
    DESCRIPTION "wren-bind"
    HOMEPAGE_URL "https://github.com/robloach/wren-bind"
    LANGUAGES C
)

# Dependencies
find_package(wren QUIET)
if(NOT wren_FOUND)
    include("wren.cmake")
endif()

# Library: wren-bind
add_library(wren-bind INTERFACE)
target_include_directories(wren-bind INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

install(FILES
  wren-bind.h
  DESTINATION include
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
