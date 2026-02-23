SET(WS_LOGGER_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/LOG_logger.c
)

add_library(ws_logger STATIC ${WS_LOGGER_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_logger
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	wcpu_commons
	ws_core
	sim_shared_interface

)
target_link_libraries(ws_logger PUBLIC
	wcpu_commons
	ws_core
	sim_shared_interface

)

