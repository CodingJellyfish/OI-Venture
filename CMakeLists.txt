cmake_minimum_required(VERSION 3.14)
project(OI-Venture)

# flecs
find_package(flecs QUIET)
if (NOT flecs_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
        GIT_TAG v3.0.4
    )
    FetchContent_MakeAvailable(flecs)
endif()

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/CodingJellyfish/raylib.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(raylib)
endif()

# This is the main part:

include_directories(src)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.c")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.h")

set(save_src_dir ${CMAKE_CURRENT_BINARY_DIR})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${save_src_dir})

add_executable(OI-Venture ${SOURCES} ${HEADERS})

set(CMAKE_C_FLAGS "-O2 --std=c11 -Wall")

target_link_libraries(${PROJECT_NAME} PUBLIC raylib flecs_static)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()