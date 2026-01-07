set (WS_OBJECT_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/WS_OBJ_object.h
	${CMAKE_CURRENT_SOURCE_DIR}/WS_OBJ_message.h
	${CMAKE_CURRENT_SOURCE_DIR}/WS_OBJ_port.h
	${CMAKE_CURRENT_SOURCE_DIR}/WS_OBJ_flag.h
	${CMAKE_CURRENT_SOURCE_DIR}/WS_OBJ_tag.h
	${CMAKE_CURRENT_SOURCE_DIR}/WS_OBJ_value.h

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
