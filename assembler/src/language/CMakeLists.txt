
set(ASM_LANG_SRC ${ASM_LANG_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/frontend/src/lexer.c
	${CMAKE_CURRENT_SOURCE_DIR}/frontend/src/parser.c
	${CMAKE_CURRENT_SOURCE_DIR}/frontend/src/inter.c
	${CMAKE_CURRENT_SOURCE_DIR}/backend/src/encoder.c
	${CMAKE_CURRENT_SOURCE_DIR}/backend/src/objfile.c
	${CMAKE_CURRENT_SOURCE_DIR}/backend/src/linker.c
	${CMAKE_CURRENT_SOURCE_DIR}/backend/src/outputer.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/entry.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/instruction.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/metaop.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/decoder.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/reference.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/segment.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/scope.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/symbol.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/src/directive.c

)

add_library(ASM_LANG STATIC
	${ASM_LANG_SRC}
)


target_include_directories(ASM_LANG
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/frontend/inc
	${CMAKE_CURRENT_SOURCE_DIR}/backend/inc
	${CMAKE_CURRENT_SOURCE_DIR}/internals/inc
)

target_link_libraries(ASM_LANG
	PUBLIC
	ASM_COMMONS
)

