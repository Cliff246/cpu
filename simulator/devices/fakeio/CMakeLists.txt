

set(DEVICE_FAKEIO_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/fakeio.c
	${CMAKE_CURRENT_SOURCE_DIR}/fakeio_device_config_setting.c
)

set(SIM_DEV_FAKEIO_VERSION 1)




add_library(sim_dev_fakeio SHARED ${DEVICE_FAKEIO_CFILES})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/fakeio_version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/fakeio_version.h
)




target_include_directories(sim_dev_fakeio
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	sim_dev_impl

)
target_link_libraries(sim_dev_fakeio PUBLIC
   	sim_dev_interface
	sim_dev_impl
)

