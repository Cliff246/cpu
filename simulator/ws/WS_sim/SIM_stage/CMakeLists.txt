SET(WS_SIM_STAGE_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/STAGE_stage.c	# main stage
	${CMAKE_CURRENT_SOURCE_DIR}/STAGE_actor.c	# actor
	${CMAKE_CURRENT_SOURCE_DIR}/STAGE_role.c	# format of actor, costume's must be implemented (similar to components)
	${CMAKE_CURRENT_SOURCE_DIR}/STAGE_commons.c	# commons

)

add_library(ws_sim_stage STATIC ${WS_SIM_STAGE_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_sim_stage
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/entry
	ws_io
	ws_cfg

	ws_tag
	wcpu_commons
	ws_run
	ws_manfst
	ws_sim
	ws_sim_scene
)
target_link_libraries(ws_sim_stage PUBLIC
	ws_tag
	ws_cfg
	ws_io
	wcpu_commons
	ws_run
	ws_manfst
	ws_sim
	ws_sim_scene

)

