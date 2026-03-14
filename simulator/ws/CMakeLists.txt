


set(WS_HELPERS_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/disassembler.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/code_decoder.c
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers/debug.c

)
set(WS_CONSOLE_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/WS_console/hydra.c
)



add_subdirectory(WS_resource)
add_subdirectory(WS_manifest)
add_subdirectory(WS_syntax)
add_subdirectory(WS_sim)
add_subdirectory(WS_logger)
add_subdirectory(WS_io)
add_subdirectory(WS_core)
add_subdirectory(WS_tags)
add_subdirectory(WS_config)
add_subdirectory(WS_runtime)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(ws
	${WS_HELPERS_CFILES}
	${WS_CONSOLE_CFILES}
)




set(CMAKE_BUILD_TYPE Debug)
target_link_libraries(ws
	wcpu_commons
	sim_shared_interface

	ws_io
	ws_sim
	ws_core
	ws_res
	ws_tag
	ws_cfg
	ws_run
	ws_manfst
	ws_syntax
	ws_logger

)

target_include_directories(ws PUBLIC
	wcpu_commons
	sim_shared_interface

	ws_io
	ws_sim
	ws_core
	ws_res
	ws_tag
	ws_cfg
	ws_run
	ws_manfst
	ws_syntax
	ws_logger

)

include_directories(ws PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/WS_console
	${CMAKE_CURRENT_SOURCE_DIR}/WS_core
	${CMAKE_CURRENT_SOURCE_DIR}/WS_helpers


)
