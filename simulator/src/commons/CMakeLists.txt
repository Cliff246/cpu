

SET(WCPU_COMMONS_CFILES ${WCPU_COMMONS_CFILES}
	${CMAKE_CURRENT_SOURCE_DIR}/commons.c
	${CMAKE_CURRENT_SOURCE_DIR}/token.c
	${CMAKE_CURRENT_SOURCE_DIR}/loader.c
	${CMAKE_CURRENT_SOURCE_DIR}/isa.c

)


add_library(wcpu_commons ${WCPU_COMMONS_CFILES})

include_directories(wcpu_commons PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
)

target_include_directories(wcpu_commons PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
)
