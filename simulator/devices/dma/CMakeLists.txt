

set(DEVICE_DMA_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/dma.c
)

set(SIM_DEV_DMA_VERSION 0.1)




add_library(dev_dma SHARED ${DEVICE_DMA_CFILES})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/dma_version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/dma_version.h
)




target_include_directories(dev_dma
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	ws_module_shared
)
target_link_libraries(dev_dma PUBLIC
	ws_module_shared
)

