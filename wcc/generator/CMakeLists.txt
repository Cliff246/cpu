
set(WCC_GENERATOR_SRC ${WCC_GENERATOR_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/asmgen.c


)
set(CMAKE_BUILD_TYPE Debug)

add_library(WCC_GENERATOR STATIC
	${WCC_GENERATOR_SRC}
)


target_include_directories(WCC_GENERATOR
	PUBLIC

)

target_link_libraries(WCC_GENERATOR
	PUBLIC
)

