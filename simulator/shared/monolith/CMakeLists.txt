

set (SIMULATOR_MODULE_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/monolith_tag.h
	${CMAKE_CURRENT_SOURCE_DIR}/monolith_instance.h
)


add_library(sim_monolith_shared INTERFACE
	${SIMULATOR_MODULE_SHARED}
)

target_include_directories(sim_monolith_shared INTERFACE

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/
)

target_link_libraries(sim_monolith_shared INTERFACE
	wcpu_commons
	wcpu_commons

)
