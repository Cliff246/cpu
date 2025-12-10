
SET(WCPU_CPU_INPUTS_CFILES ${WCPU_CPU_INPUTS_CFILES}
	${CMAKE_CURRENT_SOURCE_DIR}/src/cli.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cliparser.c
)


add_library(wcpu_emu_inputs ${WCPU_CPU_INPUTS_CFILES})

include_directories(wcpu_emu_inputs PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(wcpu_emu_inputs PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	wcpu_commons
)

target_link_libraries(wcpu_emu_inputs PUBLIC
	wcpu_commons
)
