set(WS_RES_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/RES_commons.c

)

add_library(ws_res STATIC
	${WS_RES_CFILES}


)

set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_res
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	wcpu_commons
	wcpu_emu_inputs
	ws_io
	ws_sim
	ws_tag
	sim_shared_interface

)
target_link_libraries(ws_res PUBLIC
	wcpu_commons
	wcpu_emu_inputs
	ws_io
	ws_sim
	ws_tag
	sim_shared_interface

)

