set(CPU_EMULATOR_FILES ${CPU_EMULATOR_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/src/core.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cpumemory.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/palu.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/psys.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/pmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/pjmp.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/debug.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/coreutils.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/code_decoder.c
)


add_library(wcpu_lib_emulator ${CPU_EMULATOR_FILES})

include_directories(wcpu_lib_emulator PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(wcpu_lib_emulator PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/inc
	wcpu_lib_commons
	wcpu_lib_inputs

)

target_link_libraries(wcpu_lib_emulator PUBLIC
	wcpu_lib_inputs

	wcpu_lib_commons
)
