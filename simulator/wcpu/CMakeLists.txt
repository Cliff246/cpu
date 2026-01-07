
add_subdirectory(inputs)


set(WS_HELPERS_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/disassembler.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/code_decoder.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/debug.c

)
set(WS_CONSOLE_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/WS_console/hydra.c
)




add_subdirectory(WS_sim)
add_subdirectory(WS_io)
add_subdirectory(WS_core)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(wcpu
	${WS_HELPERS_CFILES}
	${WS_CONSOLE_CFILES}
)




set(CMAKE_BUILD_TYPE Debug)
target_link_libraries(wcpu
	wcpu_commons
	wcpu_emu_inputs
	sim_dev_interface
	sim_dev_impl
	ws_io
	ws_sim
	ws_core
)

target_include_directories(wcpu PUBLIC
	wcpu_commons
	wcpu_emu_inputs
	sim_dev_interface
	sim_dev_impl
	ws_io
	ws_sim
	ws_core

)

include_directories(wcpu PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/WS_console
	${CMAKE_CURRENT_SOURCE_DIR}/WS_core
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers


)
