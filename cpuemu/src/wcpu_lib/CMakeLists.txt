
add_subdirectory(commons)
add_subdirectory(cli)

set(CPU_C_FILES ${CPU_C_FILES}

	core.c
	cpumemory.c
	palu.c
	psys.c
	pmem.c
	pjmp.c
	debug.c
	coreutils.c
)


add_library(wcpu_lib ${CPU_C_FILES})


target_link_libraries(wcpu_lib PUBLIC
	wcpu_lib_cli	
	wcpu_lib_commons
)	

include_directories(wcpu_lib
	${CMAKE_SOURCE_DIR}/flags/
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(wcpu_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${CMAKE_SOURCE_DIR}/flags/
	wcpu_lib_commons
	wcpu_lib_cli
)
