

set(DEVICE_DISK_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/disk.c
)

set(SIM_DEV_DISK_VERSION 0.1)




add_library(dev_disk SHARED ${DEVICE_DISK_CFILES})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/disk_version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/disk_version.h
)




target_include_directories(dev_disk
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	ws_module_shared
)
target_link_libraries(dev_disk PUBLIC
	ws_module_shared
)

