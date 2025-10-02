set(CPU_C_FILES ${CPU_C_FILES}

	core.c
	cpumemory.c
	palu.c
	psys.c
	pmem.c
	pjmp.c
	common.c
	debug.c
	coreutils.c
)


add_library(wcpu_lib ${CPU_C_FILES})

include_directories(wcpu_lib
	${CMAKE_SOURCE_DIR}/flags/
	inc
)
target_include_directories(wcpu_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${CMAKE_SOURCE_DIR}/flags/
)