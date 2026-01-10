set (WS_MODULE_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_description.h
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_vtable.h
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_setup.h
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_binding.h
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_bindobj.h
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_bindser.h
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_function.h
	${CMAKE_CURRENT_SOURCE_DIR}/MOD_polling.h
	#PARENT_SCOPE
)


add_library(ws_module_shared INTERFACE
	${WS_MODULE_SHARED}
)

target_include_directories(ws_module_shared INTERFACE

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/
	ws_object_shared
	ws_service_shared

)

target_link_libraries(ws_module_shared INTERFACE
	wcpu_commons
	ws_object_shared
	ws_service_shared
)
