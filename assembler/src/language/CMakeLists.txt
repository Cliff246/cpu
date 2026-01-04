
set(ASM_LANG_SRC ${ASM_LANG_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/frontend/lexer.c
	${CMAKE_CURRENT_SOURCE_DIR}/frontend/parser.c
	${CMAKE_CURRENT_SOURCE_DIR}/frontend/inter.c
	${CMAKE_CURRENT_SOURCE_DIR}/backend/objfile.c
	${CMAKE_CURRENT_SOURCE_DIR}/backend/linker.c
	${CMAKE_CURRENT_SOURCE_DIR}/backend/emiter.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/entry.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/instruction.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/metaop.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/decoder.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/reference.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/segment.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/scope.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/symbol.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/directive.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/module.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/psudeoop.c
	${CMAKE_CURRENT_SOURCE_DIR}/internals/keywords.c

)
set(CMAKE_BUILD_TYPE Debug)

add_library(ASM_LANG STATIC
	${ASM_LANG_SRC}
)


target_include_directories(ASM_LANG
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/frontend
	${CMAKE_CURRENT_SOURCE_DIR}/backend
	${CMAKE_CURRENT_SOURCE_DIR}/internals
)

target_link_libraries(ASM_LANG
	PUBLIC
	ASM_COMMONS
)

