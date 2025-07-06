cmake_minimum_required(VERSION 3.30)

# set project name
project(cchan
    VERSION 1.0.0
    DESCRIPTION "C language channel library"
    LANGUAGES C)

# add compile options
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# add option: build examples
option(BUILD_EXAMPLES "build examples" ON)
# add option: build tests
option(BUILD_TESTS "build tests" OFF)

# find source files
file(GLOB_RECURSE SOURCES src/*.c)

# set header file include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../uthash/src)

# add static library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# set library properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "src/cchan_broadcast.h;src/cchan_mpsc.h;src/cchan_oneshot.h;src/cchan_watch.h")

# add compile options to library
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /utf-8)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# if build examples
if(BUILD_EXAMPLES)
    message(STATUS "build examples")
    add_subdirectory(example)
endif()

# if build tests
if(BUILD_TESTS)
    message(STATUS "build tests")
    enable_testing()
    # here can add test subdirectory
    # add_subdirectory(tests)
endif()

# show build information
message(STATUS "CChan channel library configuration completed")
message(STATUS "  version: ${PROJECT_VERSION}")
message(STATUS "  build examples: ${BUILD_EXAMPLES}")
message(STATUS "  build tests: ${BUILD_TESTS}")
message(STATUS "  C standard: ${CMAKE_C_STANDARD}")
