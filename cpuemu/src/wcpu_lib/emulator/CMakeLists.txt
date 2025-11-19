
set(CPU_CORE_EMULATOR ${CPU_CORE_EMULATOR_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/emucore.c
#bus
	${CMAKE_CURRENT_SOURCE_DIR}/bus/bus.c
#device core
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device.c

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/device_configurer.c
#wcpu device
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/core.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/coreutils.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/dev_wcpu.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/palu.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/pjmp.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/pmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_system/wcpu/psys.c

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/fakeio/dev_fakeio.c

	${CMAKE_CURRENT_SOURCE_DIR}/device_system/ram/dev_ram.h

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
