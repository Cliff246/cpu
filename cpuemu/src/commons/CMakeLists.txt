

SET(WCPU_COMMONS_CFILES ${WCPU_COMMONS_CFILES}
	${CMAKE_CURRENT_SOURCE_DIR}/src/common.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/token.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/loader.c

)


add_library(wcpu_commons ${WCPU_COMMONS_CFILES})

include_directories(wcpu_commons PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(wcpu_commons PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)
