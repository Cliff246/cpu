
cmake_minimum_required(VERSION 3.15)

add_subdirectory(commons)
add_subdirectory(language)




set(ASMFILES ${ASM_C_FILES}
	assembler.c
	main.c
)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(asm ${ASMFILES})

include_directories(asm
	inc
	${CMAKE_SOURCE_DIR}/flags/
)

target_link_libraries(asm
	PUBLIC
	ASM_LANG
	ASM_COMMONS
)

target_include_directories(asm
	PUBLIC
   	ASM_LANG
	ASM_COMMONS
)



