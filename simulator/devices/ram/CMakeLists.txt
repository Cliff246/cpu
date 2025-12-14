
SET(DEVICE_RAM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/dev_ram.c
	${CMAKE_CURRENT_SOURCE_DIR}/dev_ram_device_config_setting.c
)

add_library(sim_dev_ram SHARED ${DEVICE_RAM_CFILES})


target_include_directories(sim_dev_ram
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/

)
target_link_libraries(sim_dev_ram PUBLIC
   	sim_dev_interface

)

