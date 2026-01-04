
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


SET(WS_IO_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/WS_io/export.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_io/WSio.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_io/ws_sim_configure.c

)

SET(WS_SIM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/SIM/SIM_sim.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM/SIM_slot.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM/SIM_event.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM/SIM_port.c

)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(ws
	${WS_CORE_CFILES}
	${WS_HELPERS_CFILES}
	${WS_CONSOLE_CFILES}
	${WS_IO_CFILES}
	${WS_SIM_CFILES}
)
set(CMAKE_BUILD_TYPE Debug)




target_link_libraries(ws
	sim_module_shared

	wcpu_commons
	wcpu_emu_inputs
	sim_module_impl
)

target_include_directories(ws PUBLIC
	sim_module_impl

	wcpu_commons
	wcpu_emu_inputs
	sim_module_shared
)

