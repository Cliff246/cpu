set(WS_CORE_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/manager.c
	${CMAKE_CURRENT_SOURCE_DIR}/dynamic_lib.c
)

add_library(ws_core STATIC
	${WS_CORE_CFILES}


)

set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_core
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	sim_dev_impl
	wcpu_commons
	wcpu_emu_inputs
	ws_sim
)
target_link_libraries(ws_core PUBLIC
   	sim_dev_interface
	sim_dev_impl
	wcpu_commons
	wcpu_emu_inputs
	ws_sim
)

