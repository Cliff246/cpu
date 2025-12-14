

set(SIMULATOR_DEV_IMPL
	${CMAKE_CURRENT_SOURCE_DIR}/device_message.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_mailbox.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_command.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_flags.c
	${CMAKE_CURRENT_SOURCE_DIR}/device.c
)


add_library(sim_dev_impl SHARED ${SIMULATOR_DEV_IMPL})

target_include_directories(sim_dev_impl PUBLIC
   sim_dev_interface
   ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(sim_dev_impl

	wcpu_commons
   	sim_dev_interface
)