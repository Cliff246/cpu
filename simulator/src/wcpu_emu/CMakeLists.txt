
add_subdirectory(emulator)



SET(WCPU_EMU_INTERFACE ${WCPU_EMU_INTERFACE}
	${CMAKE_CURRENT_SOURCE_DIR}/emuhook.c
)


add_library(wcpu_emu ${WCPU_EMU_INTERFACE})

include_directories(wcpu_emu PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(wcpu_emu PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}

	wcpu_commons
)

target_link_libraries(wcpu_emu PUBLIC
	wcpu_commons

	wcpu_emu_core
)
