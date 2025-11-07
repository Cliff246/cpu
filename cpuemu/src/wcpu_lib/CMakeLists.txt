
add_subdirectory(commons)
add_subdirectory(inputs)
add_subdirectory(emulator)

set(CPU_C_FILES ${CPU_C_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/manager.c
)


add_library(wcpu_lib ${CPU_C_FILES})


target_link_libraries(wcpu_lib PUBLIC
	wcpu_lib_inputs
	wcpu_lib_commons
	wcpu_lib_emulator
)

include_directories(wcpu_lib
	${CMAKE_SOURCE_DIR}/flags/
	${CMAKE_CURRENT_SOURCE_DIR}/inc

)

target_include_directories(wcpu_lib PUBLIC
	${CMAKE_SOURCE_DIR}/flags/
	${CMAKE_CURRENT_SOURCE_DIR}/inc

	wcpu_lib_commons
	wcpu_lib_inputs
	wcpu_lib_emulator
)
