SET(WS_SIM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/simulator.c
	${CMAKE_CURRENT_SOURCE_DIR}/simulator_bus_slot.c
	${CMAKE_CURRENT_SOURCE_DIR}/simulator_event.c
	${CMAKE_CURRENT_SOURCE_DIR}/simulator_port.c

)


add_library(ws_sim STATIC ${WS_SIM_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_sim
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	sim_dev_impl
	ws_io
)
target_link_libraries(ws_sim PUBLIC
   	sim_dev_interface
	sim_dev_impl
	ws_io
	wcpu_commons

)

