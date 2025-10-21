
SET(C_CPU_CLI_CFILES ${C_CPU_CLI_CFILES}
	${CMAKE_CURRENT_SOURCE_DIR}/src/cli.c	
	${CMAKE_CURRENT_SOURCE_DIR}/src/token.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cmdsys.c
)


add_library(wcpu_lib_cli ${C_CPU_CLI_CFILES})

include_directories(wcpu_lib_cli PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(wcpu_lib_cli PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	wcpu_lib_commons
)	

target_link_libraries(wcpu_lib_cli PUBLIC
	wcpu_lib_commons
)
