set(WS_CFG_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/CFG_link.c
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_map.c
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_context.c
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_node.c
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_edge.c
	${CMAKE_CURRENT_SOURCE_DIR}/CFG_commons.c

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
	ws_manfst
	sim_shared_interface
	ws_syntax

)
target_link_libraries(ws_cfg PUBLIC
	wcpu_commons
	wcpu_emu_inputs
	ws_io
	ws_sim
	ws_tag
	ws_manfst
	sim_shared_interface
	ws_syntax
)

