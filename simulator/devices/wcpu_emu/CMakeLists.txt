
set(WCPU_CPU_CORE_EMULATOR ${CPU_CORE_EMULATOR_FILES}



	${CMAKE_CURRENT_SOURCE_DIR}/dev_wcpu.c
	${CMAKE_CURRENT_SOURCE_DIR}/dev_wcpu_device_config_setting.c
	${CMAKE_CURRENT_SOURCE_DIR}/wcpu_commons/wcpu_part.c
	${CMAKE_CURRENT_SOURCE_DIR}/wcpu_commons/wcpu_part_bus.c
	${CMAKE_CURRENT_SOURCE_DIR}/wcpu_commons/wcpu_part_signal.c
	${CMAKE_CURRENT_SOURCE_DIR}/core/core.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/deployer/deployer.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/cache/cache.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/cache/cache_line.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/aggregator/aggregator.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/aggregator/aggregator_command.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/ledger/ledger.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/ledger/ledger_operation.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/ledger/ledger_tensor.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mau/mau.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mau/mau_operation.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/regfile/register_file.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/lsu/lsu.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/mattress.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/mattress_interface.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/mattress_packet.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/mattress_unit.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/fetcher.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/fetcher_entry.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/fetcher_controller.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/fetcher_interface.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/fetcher_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/fetcher_port_order.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_code_descriptor/fetcher_code_descriptor_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_code_descriptor/fetcher_code_descriptor_port_context.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_code_descriptor/fetcher_code_descriptor_port_order.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_immediate/fetcher_immediate_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_instruction/fetcher_instruction_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_code_table/fetcher_code_table_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric_part.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric_parts/fabric_regfile.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric_parts/fabric_bus.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric_parts/fabric_executer.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric_parts/fabric_controller.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric_parts/fabric_lane.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric_parts/fabric_alu.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/shared/fabric_uop.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/shared/fabric_internals.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/shared/fabric_shared.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/old/pjmp.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/old/pmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/old/psys.c
)


set(CMAKE_BUILD_TYPE Debug)
add_library(wcpu_emu_core SHARED ${WCPU_CPU_CORE_EMULATOR} )

include_directories(wcpu_emu_core PUBLIC
	${CMAKE_SOURCE_DIR}/flags/
	${CMAKE_CURRENT_SOURCE_DIR}/

	${CMAKE_CURRENT_SOURCE_DIR}/parts
	${CMAKE_CURRENT_SOURCE_DIR}/core
	${CMAKE_CURRENT_SOURCE_DIR}/wcpu_commons
	${CMAKE_CURRENT_SOURCE_DIR}/wcpu_commons/signal_types
	${CMAKE_CURRENT_SOURCE_DIR}/parts/lsu
	${CMAKE_CURRENT_SOURCE_DIR}/parts/cache
	${CMAKE_CURRENT_SOURCE_DIR}/parts/deployer
	${CMAKE_CURRENT_SOURCE_DIR}/parts/ledger
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_code_descriptor/
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_immediate/
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_instruction/
	${CMAKE_CURRENT_SOURCE_DIR}/parts/fetcher/ports/port_code_table/
	${CMAKE_CURRENT_SOURCE_DIR}/parts/regfile
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mau
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress
	${CMAKE_CURRENT_SOURCE_DIR}/parts/aggregator
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/fabric_parts
	${CMAKE_CURRENT_SOURCE_DIR}/parts/mattress/fabric/shared
	${CMAKE_CURRENT_SOURCE_DIR}io
	${CMAKE_CURRENT_SOURCE_DIR}

)

target_include_directories(wcpu_emu_core PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/emulator_device_argparser
	${CMAKE_CURRENT_SOURCE_DIR}/device_system
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/core
#commons
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/wcpu_commons
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/wcpu_commons/signal_types
#parts
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/lsu
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/port_code_descriptor/
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/port_immediate/
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/port_instruction/
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/port_code_table/

#cache
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/cache

#deployer
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/deployer


	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/ledger
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/regfile
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mau
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/aggregator
#fabric on mattress
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric_parts
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/shared


#device fakeio
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/fakeio
#device ram

	${CMAKE_CURRENT_SOURCE_DIR}/helpers
	wcpu_commons

)

target_link_libraries(wcpu_emu_core PUBLIC
	sim_dev_interface

	wcpu_commons
)
