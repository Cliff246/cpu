
add_subdirectory(commons)
add_subdirectory(emulator)
add_subdirectory(inputs)

set(CPU_C_FILES ${CPU_C_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/manager.c
)


add_library(wcpu_lib ${CPU_C_FILES})


target_link_libraries(wcpu_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	wcpu_lib_inputs
	wcpu_lib_commons
	wcpu_lib_emulator
)

include_directories(wcpu_lib
	${CMAKE_SOURCE_DIR}/flags/
	${CMAKE_CURRENT_SOURCE_DIR}/
)

target_include_directories(wcpu_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/flags/
	wcpu_lib_commons
	wcpu_lib_inputs
	wcpu_lib_emulator
)
