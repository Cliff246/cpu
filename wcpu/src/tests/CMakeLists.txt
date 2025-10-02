
set(WCPU_TESTS ${WCPU_TESTS}
	${CMAKE_CURRENT_SOURCE_DIR}/tests_entry.c
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(wcpu_tests ${WCPU_TESTS})

target_link_libraries(wcpu_tests
	wcpu_lib
)

target_include_directories(wcpu_tests PRIVATE
	wcpu_lib
)

include_directories(wcput_tests
	tests
)