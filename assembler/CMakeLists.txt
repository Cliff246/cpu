cmake_minimum_required(VERSION 3.15)
project(asm C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)




add_custom_target(copy_src ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/asmtests
            ${CMAKE_BINARY_DIR}/asmtests
    COMMENT "Copying tests directory to build"
)
