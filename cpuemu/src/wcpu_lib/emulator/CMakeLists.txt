
set(CPU_CORE_EMULATOR ${CPU_CORE_EMULATOR_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/emucore.c
	${CMAKE_CURRENT_SOURCE_DIR}/emulator_configure.c
#emulator device argparser
	${CMAKE_CURRENT_SOURCE_DIR}/emulator_device_argparser/emulator_device_argparser.c
#bus

#device core
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device.c

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device_message.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device_mailbox.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device_command.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device_flags.c




#wcpu device
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/dev_wcpu.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/dev_wcpu_device_config_setting.c

#wcpu commmons
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/wcpu_commons/wcpu_isa.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/wcpu_commons/wcpu_part.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/wcpu_commons/wcpu_part_bus.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/wcpu_commons/wcpu_part_signal.c



#wcpu core
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/core/core.c


#wcpu parts
#cache
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/cache/cache.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/cache/cache_line.c
#aggregator part
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/aggregator/aggregator.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/aggregator/aggregator_command.c


#ledger
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/ledger/ledger.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/ledger/ledger_operation.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/ledger/ledger_tensor.c

#mau
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mau/mau.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mau/mau_operation.c

#regfile
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/regfile/register_file.c

#lsu
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/lsu/lsu.c

#mattress
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/mattress.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/mattress_interface.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/mattress_packet.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/mattress_unit.c
#fetcher
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/fetcher.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/fetcher_entry.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/fetcher_controller.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/fetcher_interface.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/fetcher_code_descriptor_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/fetcher_immediate_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/fetcher_instruction_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/fetcher_code_table_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/fetcher_port.c

#fabric details
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric_part.c


#fabirc_parts
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric_parts/fabric_regfile.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric_parts/fabric_bus.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric_parts/fabric_executer.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric_parts/fabric_controller.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric_parts/fabric_lane.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/fabric_parts/fabric_alu.c


#fabric_shared
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/shared/fabric_uop.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/shared/fabric_internals.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/shared/fabric_shared.c


#fabric_old

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/old/pjmp.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/old/pmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/mattress/fabric/old/psys.c
#device fakeio

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/fakeio/dev_fakeio.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/fakeio/dev_fakeio_device_config_setting.c
#device fakeram
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/ram/dev_ram.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/ram/dev_ram_device_config_setting.c

	${CMAKE_CURRENT_SOURCE_DIR}/helpers/debug.c
	${CMAKE_CURRENT_SOURCE_DIR}/helpers/code_decoder.c
)


set(CMAKE_BUILD_TYPE Debug)
add_library(wcpu_lib_emulator ${CPU_CORE_EMULATOR} )

include_directories(wcpu_lib_emulator PUBLIC
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
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/cache

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/ledger
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/fetcher/ports/

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

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/ram
	${CMAKE_CURRENT_SOURCE_DIR}/helpers

)

target_include_directories(wcpu_lib_emulator PUBLIC
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
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/parts/cache

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

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/ram
	${CMAKE_CURRENT_SOURCE_DIR}/helpers
	wcpu_lib_commons
	wcpu_lib_inputs

)

target_link_libraries(wcpu_lib_emulator PUBLIC
	wcpu_lib_inputs

	wcpu_lib_commons
)
