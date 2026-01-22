
set(WCC_CLI_SRC ${WCC_CLI_SRC}

	${CMAKE_CURRENT_SOURCE_DIR}/cli.c
)
set(CMAKE_BUILD_TYPE Debug)

add_library(WCC_CLI STATIC
	${WCC_CLI_SRC}
)


target_include_directories(WCC_CLI
	PUBLIC

)

target_link_libraries(WCC_CLI
	PUBLIC
)

