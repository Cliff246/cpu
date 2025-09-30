
cmake_minimum_required(VERSION 3.15)

set(ASMFILES ${ASM_C_FILES}
	assembler.c
	main.c
	decoder.c
	commons.c
	hashmap.c
	error.c
	strtools.c
	fileio.c
)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(asm ${ASMFILES})

include_directories(asm
	inc
	../../c_cpu/src/inc/flags.h
)
