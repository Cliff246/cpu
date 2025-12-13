set(WCPU_CPU_VM_FILES ${WCPU_CPU_VM_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/vmhook.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_cpu_path.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_cpu.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_alu.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_sys.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_mem.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_jmp.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_cpu_utils.c

)


add_library(wcpu_vm ${WCPU_CPU_VM_FILES})

include_directories(wcpu_vm PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/
	wcpu_commons


)

target_include_directories(wcpu_vm PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/
	wcpu_commons

)

target_link_libraries(wcpu_vm PUBLIC
	wcpu_commons

)
