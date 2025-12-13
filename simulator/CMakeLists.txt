cmake_minimum_required(VERSION 3.15)
project(weirdo_cpu)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add subdirectories
add_subdirectory(src)


add_custom_target(copy_simulator_config_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/configfiles
            ${CMAKE_BINARY_DIR}/configfiles
    COMMENT "Copying tests directory to build"
)