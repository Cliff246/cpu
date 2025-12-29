set(SIMULATOR_DEVICE_WCPU_VM
	${CMAKE_CURRENT_SOURCE_DIR}/vm.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_path.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_regs.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_utils.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_op.c
	${CMAKE_CURRENT_SOURCE_DIR}/vm_bus.c

	${CMAKE_CURRENT_SOURCE_DIR}/vm_device_config_setting.c
)

SET(SIMULATOR_DEVICE_WCPU_VM_PATHUNITS
	${CMAKE_CURRENT_SOURCE_DIR}/pathunits/vm_alu.c
	${CMAKE_CURRENT_SOURCE_DIR}/pathunits/vm_sys.c
	${CMAKE_CURRENT_SOURCE_DIR}/pathunits/vm_mem.c
	${CMAKE_CURRENT_SOURCE_DIR}/pathunits/vm_jmp.c
)

set(SIMULATOR_DEVICE_WCPU_VM_EVAL
	${CMAKE_CURRENT_SOURCE_DIR}/eval/vm_txn.c
)


add_library(sim_dev_wcpu_vm SHARED
	${SIMULATOR_DEVICE_WCPU_VM}
	${SIMULATOR_DEVICE_WCPU_VM_EVAL}
	${SIMULATOR_DEVICE_WCPU_VM_PATHUNITS}
)

include_directories(sim_dev_wcpu_vm PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/pathunits
	${CMAKE_CURRENT_SOURCE_DIR}/eval
	wcpu_commons


)

target_include_directories(sim_dev_wcpu_vm PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/eval
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/pathunits

	wcpu_commons
	sim_dev_interface
	sim_dev_impl
)

target_link_libraries(sim_dev_wcpu_vm PUBLIC
	wcpu_commons
	sim_dev_interface
	sim_dev_impl
)
