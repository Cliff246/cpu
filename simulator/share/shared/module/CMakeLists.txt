set (WS_MODULE_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_description.h

)


add_library(ws_module_shared INTERFACE
	${WS_MODULE_SHARED}
)

target_include_directories(ws_module_shared INTERFACE

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/
	ws_object_shared

)

target_link_libraries(ws_module_shared INTERFACE
	wcpu_commons
	ws_object_shared
)
