add_subdirectory(object)
add_subdirectory(module)

set (SIMULATOR_DEV_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/device_list.h
	#PARENT_SCOPE
)


add_library(sim_shared_interface INTERFACE
	${SIMULATOR_DEV_SHARED}
)

target_include_directories(sim_shared_interface INTERFACE
	ws_module_shared
	ws_object_shared

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/


)

target_link_libraries(sim_shared_interface INTERFACE
	wcpu_commons
	ws_module_shared
	ws_object_shared
)
