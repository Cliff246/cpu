
SET(DEVICE_ECHO_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/echo.c
)

set(SIM_DEV_ECHO_VERSION 1)


add_library(dev_echo SHARED ${DEVICE_ECHO_CFILES})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/echo_version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/echo_version.h
)



set(CMAKE_BUILD_TYPE Debug)
target_include_directories(dev_echo
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	ws_module_shared


)
target_link_libraries(dev_echo PUBLIC
	ws_module_shared

)

