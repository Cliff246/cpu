
set(CPU_CORE_EMULATOR ${CPU_CORE_EMULATOR_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/emucore.c
	${CMAKE_CURRENT_SOURCE_DIR}/emulator_configure.c
#bus

#device core
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device.c

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device_message.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device_mailbox.c
#wcpu device
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/dev_wcpu.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/fetcher.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/aggregator.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/register_file.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/lsu.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/wcpu_part.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/cache.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/wcpu_isa.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/ledger.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/alu.c

#mattress
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/mattress.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/mattress_interface.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/mattress_packet.c


#fabric details
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric/fabric.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric/fabric_lane.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric/fabric_register.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric/fabric_bus.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric/fabric_execution.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric/pjmp.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric/pmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric/psys.c
#device fakeio

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/fakeio/dev_fakeio.c
#device fakeram
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/ram/dev_ram.c

	${CMAKE_CURRENT_SOURCE_DIR}/helpers/debug.c
	${CMAKE_CURRENT_SOURCE_DIR}/helpers/code_decoder.c
)

add_library(wcpu_lib_emulator ${CPU_CORE_EMULATOR} )

include_directories(wcpu_lib_emulator PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/bus
	${CMAKE_CURRENT_SOURCE_DIR}/device_system
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/fakeio
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/ram
	${CMAKE_CURRENT_SOURCE_DIR}/helpers


)

target_include_directories(wcpu_lib_emulator PUBLIC
	${CMAKE_SOURCE_DIR}/flags/

	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/bus
	${CMAKE_CURRENT_SOURCE_DIR}/device_system
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress/fabric
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/mattress

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/fakeio
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/ram
	${CMAKE_CURRENT_SOURCE_DIR}/helpers
	wcpu_lib_commons
	wcpu_lib_inputs

)

target_link_libraries(wcpu_lib_emulator PUBLIC
	wcpu_lib_inputs

	wcpu_lib_commons
)
