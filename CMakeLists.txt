cmake_minimum_required(VERSION 3.15)

project(WEIRD_CPU)


add_subdirectory(assembler)
add_subdirectory(cpuemu)

add_custom_target(copy_stl ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/wcpu_stl
            ${CMAKE_BINARY_DIR}/stl
    COMMENT "Copying tests directory to build"
)
