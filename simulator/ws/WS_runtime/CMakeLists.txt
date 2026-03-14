

SET(WS_RUN_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_graph.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_idmap.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_channel.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_object.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_pools.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_packet.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_commons.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_addrtbl.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_operation.c
	${CMAKE_CURRENT_SOURCE_DIR}/RUN_schedule.c


)


add_library(ws_run STATIC ${WS_RUN_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_run
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	wcpu_commons
	ws_io
	ws_sim
	ws_tag
	ws_res
	sim_shared_interface
)

target_link_libraries(ws_run PUBLIC
	wcpu_commons
	ws_io
	ws_sim
	ws_tag
	ws_res

	sim_shared_interface
)

