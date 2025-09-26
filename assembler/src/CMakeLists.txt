
cmake_minimum_required(VERSION 3.15)

set(ASMFILES ${ASM_C_FILES}
	assembler.c
	main.c
	decoder.c
	commons.c
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(asm ${ASMFILES})

include_directories(asm
	inc
	../../c_cpu/src/inc/flags.h
)
