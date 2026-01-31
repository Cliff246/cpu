

SET(WS_SIM_TAG_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_prototype.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_string.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_int.c
	${CMAKE_CURRENT_SOURCE_DIR}/TAG_bool.c


)


add_library(ws_sim_tag STATIC ${WS_SIM_TAG_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_sim_tag
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ws_sim_tag PUBLIC
	wcpu_commons

)

