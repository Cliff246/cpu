

SET(WS_SIM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_simulator.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_stage.c


	${CMAKE_CURRENT_SOURCE_DIR}/SIM_device.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_handle.c






	${CMAKE_CURRENT_SOURCE_DIR}/SIM_commons.c



)


add_library(ws_sim STATIC ${WS_SIM_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_sim
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/entry
	ws_io
	ws_cfg

	ws_tag
	wcpu_commons
	ws_run
)
target_link_libraries(ws_sim PUBLIC
	ws_tag
	ws_cfg
	ws_io
	wcpu_commons
	ws_run
)

