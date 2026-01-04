SET(WS_IO_CFILES

	${CMAKE_CURRENT_SOURCE_DIR}/export.c
	${CMAKE_CURRENT_SOURCE_DIR}/WSio.c
	${CMAKE_CURRENT_SOURCE_DIR}/IO_configure.c

)






add_library(ws_io SHARED
	${WS_IO_CFILES}
)

target_include_directories(ws_io PUBLIC
   sim_module_shared
   ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(ws_io

	wcpu_commons
   	sim_module_shared
)