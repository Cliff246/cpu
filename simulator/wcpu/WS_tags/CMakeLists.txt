

SET(WS_TAG_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_tag.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_string.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_int.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_bool.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_list.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_resource.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_hook.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_map.c


)


add_library(ws_tag STATIC ${WS_TAG_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_tag
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ws_tag PUBLIC
	wcpu_commons

)

