
add_subdirectory(inputs)
add_subdirectory(emulator)

set(EMU_CPU_C_FILES ${EMU_CPU_C_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/manager.c
	${CMAKE_CURRENT_SOURCE_DIR}/hydra.c
	${CMAKE_CURRENT_SOURCE_DIR}/disassembler.c
	${CMAKE_CURRENT_SOURCE_DIR}/export.c
)


add_library(wcpu_emu ${EMU_CPU_C_FILES})


target_link_libraries(wcpu_emu PUBLIC
	wcpu_emu_inputs
	wcpu_commons
	wcpu_emu_core
)

include_directories(wcpu_emu
	${CMAKE_SOURCE_DIR}/flags/
	${CMAKE_CURRENT_SOURCE_DIR}/inc

)

target_include_directories(wcpu_emu PUBLIC
	${CMAKE_SOURCE_DIR}/flags/
	${CMAKE_CURRENT_SOURCE_DIR}/inc

	wcpu_commons
	wcpu_emu_inputs
	wcpu_emu_core
)
