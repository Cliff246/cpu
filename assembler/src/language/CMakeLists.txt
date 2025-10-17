
set(ASM_LANG_SRC ${ASM_LANG_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/inter.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/encoder.c

)

add_library(ASM_LANG STATIC
	${ASM_LANG_SRC}
)


target_include_directories(ASM_LANG
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(ASM_LANG
	PUBLIC
	ASM_COMMONS
)

