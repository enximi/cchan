message(STATUS "configure example program build")

# set example program build options
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# basic example program
add_executable(cchan_example example.c)

# link to main library
target_link_libraries(cchan_example PRIVATE cchan)

# set compile options
if(MSVC)
    target_compile_options(cchan_example PRIVATE /W4 /utf-8)
else()
    target_compile_options(cchan_example PRIVATE -Wall -Wextra)
endif()

# set output directory
set_target_properties(cchan_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# show build information
message(STATUS "example program configuration completed:")
message(STATUS "  - cchan_example: basic usage example")

# optional: add custom target to run example
add_custom_target(run_example
    COMMAND cchan_example
    DEPENDS cchan_example
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "run CChan basic example")
