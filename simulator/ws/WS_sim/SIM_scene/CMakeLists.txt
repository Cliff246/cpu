SET(WS_SIM_SCENE_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/SCENE_device.c
	${CMAKE_CURRENT_SOURCE_DIR}/SCENE_anchor.c
	${CMAKE_CURRENT_SOURCE_DIR}/SCENE_wire.c
	${CMAKE_CURRENT_SOURCE_DIR}/SCENE_scene.c
	${CMAKE_CURRENT_SOURCE_DIR}/SCENE_port.c
)

add_library(ws_sim_scene STATIC ${WS_SIM_SCENE_CFILES})
target_include_directories(ws_sim_scene
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/entry
	ws_io
	ws_cfg

	ws_tag
	wcpu_commons
	ws_run
	ws_manfst
	ws_sim_stage

	ws_sim
)

target_link_libraries(ws_sim_scene PUBLIC
	ws_tag
	ws_cfg
	ws_io
	wcpu_commons
	ws_run
	ws_manfst
	ws_sim_stage
	ws_sim
)
