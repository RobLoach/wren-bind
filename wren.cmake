# Wren CMake
# TODO: Add CMake support to wren https://github.com/wren-lang/wren/pull/997

set(WREN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/wren)

# Options
option(WREN_META "Wren Meta Class" ON)
option(WREN_RANDOM "Wren Random Class" ON)
option(WREN_BUILD_STATIC "Wren Static Library" ON)
option(WREN_BUILD_SHARED "Wren Shared Library" OFF)
option(WREN_BUILD_TEST "Build Wren Tests" OFF)

# Source Files
set(WREN_SRC ${WREN_DIR}/src)
file(GLOB WREN_SRC_VM_FILES ${WREN_SRC}/vm/*.c)
file(GLOB WREN_SRC_OPTIONAL_FILES ${WREN_SRC}/optional/*.c)
set(WREN_SRC_FILES ${WREN_SRC_VM_FILES} ${WREN_SRC_OPTIONAL_FILES})
set(WREN_INCLUDE_DIRS
    ${WREN_SRC}/include
    ${WREN_SRC}/optional
    ${WREN_SRC}/vm
)

# Defines
set(WREN_DEFINES)
set(WREN_LIBRARIES m)
if (WREN_META)
    list(APPEND WREN_DEFINES WREN_OPT_META)
endif()
if (WREN_RANDOM)
    list(APPEND WREN_DEFINES WREN_OPT_RANDOM)
endif()

# Wren Libraries
if (WREN_BUILD_STATIC)
    add_library(wren STATIC ${WREN_SRC_FILES})
    target_link_libraries(wren ${WREN_LIBRARIES})
    target_include_directories(wren PUBLIC ${WREN_INCLUDE_DIRS})
    target_compile_definitions(wren PUBLIC ${WREN_DEFINES})
endif()
if (WREN_BUILD_SHARED)
    add_library(wren_shared SHARED ${WREN_SRC_FILES})
    target_link_libraries(wren_shared ${WREN_LIBRARIES})
    target_include_directories(wren PUBLIC ${WREN_INCLUDE_DIRS})
    target_compile_definitions(wren_shared PUBLIC ${WREN_DEFINES})
endif()
