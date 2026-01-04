

set (SIMULATOR_MODULE_SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/object_tag.h
	${CMAKE_CURRENT_SOURCE_DIR}/object_instance.h
	${CMAKE_CURRENT_SOURCE_DIR}/object_packet.h
	${CMAKE_CURRENT_SOURCE_DIR}/object_vtable.h
)


add_library(sim_object_shared INTERFACE
	${SIMULATOR_MODULE_SHARED}
)

target_include_directories(sim_object_shared INTERFACE

	${CMAKE_CURRENT_SOURCE_DIR}/
	private wcpu_commons
	${CMAKE_SOURCE_DIR}/flags/
)

target_link_libraries(sim_object_shared INTERFACE
	wcpu_commons
	wcpu_commons

)
