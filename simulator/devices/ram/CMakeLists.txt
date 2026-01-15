
SET(DEVICE_RAM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/ram.c
	${CMAKE_CURRENT_SOURCE_DIR}/ram_device_config_setting.c
)

set(SIM_DEV_RAM_VERSION 1)


add_library(sim_dev_ram SHARED ${DEVICE_RAM_CFILES})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/ram_version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/ram_version.h
)



set(CMAKE_BUILD_TYPE Debug)
target_include_directories(sim_dev_ram
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	ws_module_shared


)
target_link_libraries(sim_dev_ram PUBLIC
	ws_module_shared

)

