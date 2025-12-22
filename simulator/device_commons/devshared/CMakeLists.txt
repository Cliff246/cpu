set (SIMULATOR_DEV_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/device.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_command.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_list.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_mailbox.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_message.h
	#PARENT_SCOPE
)


add_library(sim_dev_interface INTERFACE
	${SIMULATOR_DEV_SHARED}
)

target_include_directories(sim_dev_interface INTERFACE

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons

)

target_link_libraries(sim_dev_interface INTERFACE
	wcpu_commons

)
