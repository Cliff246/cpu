

set (SIMULATOR_MODULE_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/module_description.h
	${CMAKE_CURRENT_SOURCE_DIR}/module_vtable.h
)


add_library(sim_module_shared INTERFACE
	${SIMULATOR_MODULE_SHARED}
)

target_include_directories(sim_module_shared INTERFACE
	sim_monolith_shared

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/
)

target_link_libraries(sim_module_shared INTERFACE
	wcpu_commons
	sim_monolith_shared

)
