SET(WS_IO_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/export.c
	${CMAKE_CURRENT_SOURCE_DIR}/WSio.c
	${CMAKE_CURRENT_SOURCE_DIR}/ws_sim_configure.c

)

add_library(ws_io STATIC ${WS_IO_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_io
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	sim_dev_impl
	wcpu_commons
	WEIRD_CPU
	wcpu
	ws_core

)

target_link_libraries(ws_io PUBLIC
   	sim_dev_interface
	sim_dev_impl
	wcpu_commons
	wcpu
	ws_core

)

