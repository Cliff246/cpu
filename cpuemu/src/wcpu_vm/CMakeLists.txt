set(WCPU_CPU_VM_FILES ${WCPU_CPU_VM_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/vmhook.c

	${CMAKE_CURRENT_SOURCE_DIR}/src/core.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cpumemory.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/palu.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/psys.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/pmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/pjmp.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/debug.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/coreutils.c

)


add_library(wcpu_vm ${WCPU_CPU_VM_FILES})

include_directories(wcpu_vm PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/inc
	wcpu_commons

	wcpu_interface

)

target_include_directories(wcpu_vm PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/inc
	wcpu_commons
	wcpu_interface

)

target_link_libraries(wcpu_vm PUBLIC
	wcpu_commons
	wcpu_interface

)
