SET(C_CPU_COMMONS_CFILES ${C_CPU_COMMONS_CFILES}
	${CMAKE_CURRENT_SOURCE_DIR}/src/common.c	
)


add_library(wcpu_lib_commons ${C_CPU_COMMONS_CFILES})

include_directories(wcpu_lib_commons PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(wcpu_lib_commons PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)
