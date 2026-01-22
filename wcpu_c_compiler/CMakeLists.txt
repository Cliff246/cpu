cmake_minimum_required(VERSION 3.15)
#weirdo cpu c compiler
add_subdirectory(constants)
add_subdirectory(debug)
add_subdirectory(commons)
add_subdirectory(parser)
add_subdirectory(decoder)
add_subdirectory(generator)
add_subdirectory(cli)


set(WCC_C_FILES ${WCC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(wcc ${WCC_C_FILES})



target_link_libraries(wcc
	PUBLIC
	WCC_DEBUG
	WCC_GENERATOR
	WCC_CLI
	WCC_DECODER
	WCC_COMMONS
	WCC_PARSER

)

target_include_directories(wcc
	PUBLIC
	WCC_DEBUG

	WCC_GENERATOR
	WCC_CLI
	WCC_DECODER
	WCC_COMMONS
	WCC_PARSER
)


