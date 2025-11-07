
set(WCPU_EXE ${WCPU_EXE}
	${CMAKE_CURRENT_SOURCE_DIR}/main.c
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(wcpu ${WCPU_EXE})

target_link_libraries(wcpu

	wcpu_lib_inputs
	wcpu_lib

)

target_include_directories(wcpu PUBLIC
	wcpu_lib_inputs
	wcpu_lib
)
