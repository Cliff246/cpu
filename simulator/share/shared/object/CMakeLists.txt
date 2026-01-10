set (WS_OBJECT_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_function.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_handle.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_config.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_object.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_global.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_context.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_commons.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_setting.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_pipeline.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_node.h
	${CMAKE_CURRENT_SOURCE_DIR}/OBJ_blueprint.h

	#PARENT_SCOPE
)


add_library(ws_object_shared INTERFACE
	${WS_OBJECT_SHARED}
)

target_include_directories(ws_object_shared INTERFACE

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/


)

target_link_libraries(ws_object_shared INTERFACE
	wcpu_commons

)
