
SET(MODULE_TESTER_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/tester.c
)

add_library(module_tester SHARED ${MODULE_TESTER_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(module_tester
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	ws_module_shared
	sim_shared_interface

)
target_link_libraries(module_tester PUBLIC
  	ws_module_shared
	sim_shared_interface

)

