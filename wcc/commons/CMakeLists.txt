
set(WCC_COMMONS_SRC ${WCC_COMMONS_SRC}

	${CMAKE_CURRENT_SOURCE_DIR}/wcc_map.c

)
set(CMAKE_BUILD_TYPE Debug)

add_library(WCC_COMMONS STATIC
	${WCC_COMMONS_SRC}
)


target_include_directories(WCC_COMMONS
	PUBLIC

)

target_link_libraries(WCC_COMMONS
	PUBLIC
)


