SET(WS_SIM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/simulator.c
	${CMAKE_CURRENT_SOURCE_DIR}/simulator_bus_slot.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_packet.c
	${CMAKE_CURRENT_SOURCE_DIR}/entry/SIM_object.c
	${CMAKE_CURRENT_SOURCE_DIR}/entry/SIM_handle.c
	${CMAKE_CURRENT_SOURCE_DIR}/entry/SIM_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/entry/SIM_tag.c

)


add_library(ws_sim STATIC ${WS_SIM_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_sim
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/entry
	sim_dev_impl
	ws_io
)
target_link_libraries(ws_sim PUBLIC
   	sim_dev_interface
	sim_dev_impl
	ws_io
	wcpu_commons

)

