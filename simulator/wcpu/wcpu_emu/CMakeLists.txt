
set(WCPU_EMU_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/emulator.c
	${CMAKE_CURRENT_SOURCE_DIR}/emulator_configure.c
	${CMAKE_CURRENT_SOURCE_DIR}/emulator_device_argparser/emulator_device_argparser.c
)


add_library(wcpu_emu ${WCPU_EMU_CFILES})

include_directories(wcpu_emu PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/emulator_device_argparser

)

target_include_directories(wcpu_emu PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/emulator_device_argparser

	wcpu_commons
)

target_link_libraries(wcpu_emu PUBLIC
	wcpu_commons
	sim_dev_interface

	wcpu_emu_core
)
