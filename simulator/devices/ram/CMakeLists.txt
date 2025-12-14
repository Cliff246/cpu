
SET(DEVICE_RAM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/ram.c
	${CMAKE_CURRENT_SOURCE_DIR}/ram_device_config_setting.c
)

add_library(sim_dev_ram SHARED ${DEVICE_RAM_CFILES})


target_include_directories(sim_dev_ram
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	sim_dev_impl

)
target_link_libraries(sim_dev_ram PUBLIC
   	sim_dev_interface
	sim_dev_impl
)

