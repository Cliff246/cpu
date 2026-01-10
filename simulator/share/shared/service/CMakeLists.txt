set (WS_SERVICE_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/SER_service.h
)


add_library(ws_service_shared INTERFACE
	${WS_SERVICE_SHARED}
)

target_include_directories(ws_service_shared INTERFACE

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/


)

target_link_libraries(ws_service_shared INTERFACE
	wcpu_commons

)
