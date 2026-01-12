SET(WS_IO_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/export.c
	${CMAKE_CURRENT_SOURCE_DIR}/WSio.c
	${CMAKE_CURRENT_SOURCE_DIR}/IO_configure.c
	${CMAKE_CURRENT_SOURCE_DIR}/IO_graph.c
	${CMAKE_CURRENT_SOURCE_DIR}/IO_parser.c

)

add_library(ws_io STATIC ${WS_IO_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_io
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	wcpu_commons
	ws_core
	sim_shared_interface

)
target_link_libraries(ws_io PUBLIC
	wcpu_commons
	ws_core
	sim_shared_interface

)

