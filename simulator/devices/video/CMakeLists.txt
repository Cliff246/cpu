

set(DEVICE_VIDEO_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/video.c
)

set(SIM_DEV_VIDEO_VERSION 0.1)




add_library(dev_video SHARED ${DEVICE_VIDEO_CFILES})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/video_version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/video_version.h
)




target_include_directories(dev_video
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	ws_module_shared
	ws_object_shared

)
target_link_libraries(dev_video PUBLIC
	ws_module_shared
	ws_object_shared
)

