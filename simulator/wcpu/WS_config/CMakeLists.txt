set(WS_CFG_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_manifest.c
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_prototag.c
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_entry.c
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_setting.c

)

add_library(ws_cfg STATIC
	${WS_CFG_CFILES}


)

set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_cfg
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	wcpu_commons
	wcpu_emu_inputs
	ws_io
	ws_sim
	ws_tag
	sim_shared_interface

)
target_link_libraries(ws_cfg PUBLIC
	wcpu_commons
	wcpu_emu_inputs
	ws_io
	ws_sim
	ws_tag
	sim_shared_interface

)

