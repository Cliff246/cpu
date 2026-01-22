
set(WCC_DEBUG_SRC ${WCC_DEBUG_SRC}

	${CMAKE_CURRENT_SOURCE_DIR}/WCC_error.c


)
set(CMAKE_BUILD_TYPE Debug)

add_library(WCC_DEBUG STATIC
	${WCC_DEBUG_SRC}
)


target_include_directories(WCC_DEBUG
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/

)

target_link_libraries(WCC_DEBUG
	PUBLIC

)


