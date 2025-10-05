
set(ASM_COMMONS_SRC ${ASM_COMMONS_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/src/commons.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/hashmap.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/error.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/strtools.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/fileio.c

)

add_library(ASM_COMMONS STATIC 
	${ASM_COMMONS_SRC}
)


target_include_directories(ASM_COMMONS 
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(ASM_COMMONS
)

