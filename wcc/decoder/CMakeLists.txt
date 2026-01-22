
set(WCC_DECODER_SRC ${WCC_DECODER_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/wcc_dtree.c


)
set(CMAKE_BUILD_TYPE Debug)

add_library(WCC_DECODER STATIC
	${WCC_DECODER_SRC}
)


target_include_directories(WCC_DECODER
	PUBLIC

)

target_link_libraries(WCC_DECODER
	PUBLIC
)


