
SET(C_CPU_INPUTS_CFILES ${C_CPU_INPUTS_CFILES}
	${CMAKE_CURRENT_SOURCE_DIR}/src/cli.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/token.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cmdsys.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cliparser.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/manager.c
)


add_library(wcpu_lib_inputs ${C_CPU_INPUTS_CFILES})

include_directories(wcpu_lib_inputs PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(wcpu_lib_inputs PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	wcpu_lib_commons
)

target_link_libraries(wcpu_lib_inputs PUBLIC
	wcpu_lib_commons
)
