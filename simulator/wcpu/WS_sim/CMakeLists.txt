SET(WS_SIM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_simulator.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_wireslot.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_packet.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_channel.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_object.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_graph.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_routetable.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_device.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_handle.c


	${CMAKE_CURRENT_SOURCE_DIR}/SIM_wire.c


)


add_library(ws_sim STATIC ${WS_SIM_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_sim
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/entry
	ws_io
)
target_link_libraries(ws_sim PUBLIC
	ws_io
	wcpu_commons

)

