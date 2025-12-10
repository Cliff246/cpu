
set(WCPU_TESTS ${WCPU_TESTS}
	${CMAKE_CURRENT_SOURCE_DIR}/test.c
	${CMAKE_CURRENT_SOURCE_DIR}/tester.c
	${CMAKE_CURRENT_SOURCE_DIR}/grader.c
	${CMAKE_CURRENT_SOURCE_DIR}/exporter.c
	${CMAKE_CURRENT_SOURCE_DIR}/rubrik.c
	${CMAKE_CURRENT_SOURCE_DIR}/main.c
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(wcpu_tests ${WCPU_TESTS})

target_link_libraries(wcpu_tests
	wcpu_emu
)


include_directories(wcpu_tests
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)


target_include_directories(wcpu_tests PRIVATE

	wcpu_emu
)

include_directories(wcput_tests
	tests
)