
set(WCC_PARSER_SRC ${WCC_PARSER_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/wcc_parser.c


)
set(CMAKE_BUILD_TYPE Debug)

add_library(WCC_PARSER STATIC
	${WCC_PARSER_SRC}
)


target_include_directories(WCC_PARSER
	PUBLIC

)

target_link_libraries(WCC_PARSER
	PUBLIC
)

