
add_subdirectory(backend)
add_subdirectory(internals)
add_subdirectory(frontend)


set(CMAKE_BUILD_TYPE Debug)

add_library(ASM_LANG STATIC lang.c)


target_include_directories(ASM_LANG
	PUBLIC

		ASM_COMMONS
	ASM_INTERNALS
	ASM_FRONTEND
	ASM_BACKEND
)

target_link_libraries(ASM_LANG
	PUBLIC
	ASM_COMMONS
	ASM_INTERNALS
	ASM_FRONTEND
	ASM_BACKEND
)

