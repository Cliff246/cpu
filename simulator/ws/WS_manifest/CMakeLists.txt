set(WS_MANFST_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/MANFST_manifest.c
	${CMAKE_CURRENT_SOURCE_DIR}/MANFST_prototag.c
	${CMAKE_CURRENT_SOURCE_DIR}/MANFST_tag.c
	${CMAKE_CURRENT_SOURCE_DIR}/MANFST_entry.c
	${CMAKE_CURRENT_SOURCE_DIR}/MANFST_scope.c


)

add_library(ws_manfst STATIC
	${WS_MANFST_CFILES}


)

set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_manfst
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	wcpu_commons
	wcpu_emu_inputs
	ws_io
	ws_sim
	ws_tag
	sim_shared_interface
	ws_syntax
)
target_link_libraries(ws_manfst PUBLIC
	wcpu_commons
	wcpu_emu_inputs
	ws_io
	ws_sim
	ws_tag
	sim_shared_interface
	ws_syntax

)

