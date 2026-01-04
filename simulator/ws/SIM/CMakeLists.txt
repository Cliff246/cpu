


SET(WS_SIM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_sim.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_slot.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_event.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_device.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_handle.c

)

SET(WS_SIM_HND_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/handles/HND_bus.c
	${CMAKE_CURRENT_SOURCE_DIR}/handles/HND_service.c
	${CMAKE_CURRENT_SOURCE_DIR}/handles/HND_monolith.c

)

add_library(ws_simulator SHARED
	${WS_SIM_HND_CFILES}
	${WS_SIM_CFILES}
)

target_include_directories(ws_simulator PUBLIC
   sim_module_shared
   ${CMAKE_CURRENT_SOURCE_DIR}/
   ${CMAKE_CURRENT_SOURCE_DIR}/handles
)

target_link_libraries(ws_simulator
	ws_io
	wcpu_commons
   	sim_module_shared
)