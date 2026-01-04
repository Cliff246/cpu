

set(SIMULATOR_MODULE_IMPL
	${CMAKE_CURRENT_SOURCE_DIR}/device_message.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_mailbox.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_command.c
	${CMAKE_CURRENT_SOURCE_DIR}/device.c
)


add_library(sim_module_impl SHARED ${SIMULATOR_MODULE_IMPL})

target_include_directories(sim_module_impl PUBLIC
   sim_module_shared
   ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(sim_module_impl

	wcpu_commons
   	sim_module_shared
)