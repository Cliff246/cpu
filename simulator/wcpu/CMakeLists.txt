
add_subdirectory(inputs)

set(WS_CORE_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/WS_core/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_core/manager.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_core/module.c
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
	${CMAKE_CURRENT_SOURCE_DIR}/WS_sim/simulator.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_sim/simulator_bus_slot.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_sim/simulator_event.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_sim/simulator_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_sim/sim_actor.c

)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(wcpu
	${WS_CORE_CFILES}
	${WS_HELPERS_CFILES}
	${WS_CONSOLE_CFILES}
	${WS_IO_CFILES}
	${WS_SIM_CFILES}
)




set(CMAKE_BUILD_TYPE Debug)
target_link_libraries(wcpu
	wcpu_commons
	wcpu_emu_inputs
	sim_dev_interface
	sim_dev_impl
)

target_include_directories(wcpu PUBLIC
	wcpu_commons
	wcpu_emu_inputs
	sim_dev_interface
	sim_dev_impl
)

include_directories(wcpu PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/WS_console
	${CMAKE_CURRENT_SOURCE_DIR}/WS_core
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers
	${CMAKE_CURRENT_SOURCE_DIR}/WS_io
	${CMAKE_CURRENT_SOURCE_DIR}/WS_sim


)
