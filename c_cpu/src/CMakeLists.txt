set(CPU_C_FILES ${CPU_C_FILES}
	main.c
	core.c
	memory.c
	alu.c
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(c_cpu ${CPU_C_FILES})

include_directories(c_cpu
	inc
)
