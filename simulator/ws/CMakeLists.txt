
add_subdirectory(inputs)

set(WS_CORE_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/CORE/CORE_manager.c
	${CMAKE_CURRENT_SOURCE_DIR}/CORE/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/CORE/CORE_dlhnd.c
)
set(WS_HELPERS_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/disassembler.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/code_decoder.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/debug.c

)
set(WS_CONSOLE_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/WS_console/hydra.c
)



add_subdirectory(IO)
add_subdirectory(SIM)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(ws
	${WS_CORE_CFILES}
	${WS_HELPERS_CFILES}
	${WS_CONSOLE_CFILES}
	${WS_IO_CFILES}
)
set(CMAKE_BUILD_TYPE Debug)




target_link_libraries(ws
	sim_module_shared

	wcpu_commons
	wcpu_emu_inputs
	sim_module_impl
	ws_simulator
)

target_include_directories(ws PUBLIC
	sim_module_impl

	wcpu_commons
	wcpu_emu_inputs
	sim_module_shared
	ws_simulator

)

