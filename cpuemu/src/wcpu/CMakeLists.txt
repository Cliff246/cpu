
add_subdirectory(inputs)

set(WCPU_EXE ${WCPU_EXE}
	${CMAKE_CURRENT_SOURCE_DIR}/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/general/manager.c
	${CMAKE_CURRENT_SOURCE_DIR}/general/hydra.c
	${CMAKE_CURRENT_SOURCE_DIR}/general/disassembler.c
	${CMAKE_CURRENT_SOURCE_DIR}/general/export.c
	${CMAKE_CURRENT_SOURCE_DIR}/helpers/debug.c
	${CMAKE_CURRENT_SOURCE_DIR}/helpers/code_decoder.c
)



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(wcpu ${WCPU_EXE})

target_link_libraries(wcpu
	wcpu_commons
	wcpu_interface
	wcpu_emu_inputs
	wcpu_emu
	wcpu_vm
)

target_include_directories(wcpu PUBLIC
	wcpu_commons
	wcpu_interface

	wcpu_emu
	wcpu_emu_inputs
	wcpu_vm
)

include_directories(wcpu PUBLIC

	${CMAKE_CURRENT_SOURCE_DIR}/inputs
	${CMAKE_CURRENT_SOURCE_DIR}/general
	${CMAKE_CURRENT_SOURCE_DIR}/flags
	${CMAKE_CURRENT_SOURCE_DIR}/helpers


)
