SET(WCPU_CONFIGURER_CFILES ${WCPU_CONFIGURER_CFILES}
	${CMAKE_CURRENT_SOURCE_DIR}/src/cli.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cliparser.c
)


add_library(wcpu_configurer ${WCPU_CONFIGURER_CFILES})

include_directories(wcpu_configurer PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
)

target_include_directories(wcpu_configurer PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	wcpu_commons
)

target_link_libraries(wcpu_configurer PUBLIC
	wcpu_commons
)
