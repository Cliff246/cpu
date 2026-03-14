set(WS_SYNTAX_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/SYNTAX_parser.c



)

add_library(ws_syntax STATIC
	${WS_SYNTAX_CFILES}


)

set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_syntax
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	wcpu_commons
	ws_io
	ws_sim
	ws_tag
	sim_shared_interface

)
target_link_libraries(ws_syntax PUBLIC
	wcpu_commons
	ws_io
	ws_sim
	ws_tag
	sim_shared_interface

)

