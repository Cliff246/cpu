set (WS_MODULE_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/module_description.h

	#PARENT_SCOPE
)


add_library(ws_module_shared INTERFACE
	${WS_MODULE_SHARED}
)

target_include_directories(ws_module_shared INTERFACE

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/


)

target_link_libraries(ws_module_shared INTERFACE
	wcpu_commons

)
