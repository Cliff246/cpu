


set(WCPU_INTERFACE ${WCPU_INTERFACE}
	${CMAKE_CURRENT_SOURCE_DIR}/interface.c
	${CMAKE_CURRENT_SOURCE_DIR}/message.c

)


add_library(wcpu_interface ${WCPU_INTERFACE})

target_link_libraries(wcpu_interface
	wcpu_commons
)

target_include_directories(wcpu_interface PUBLIC
	wcpu_commons
	${CMAKE_CURRENT_SOURCE_DIR}/

)

include_directories(wcpu_interface PUBLIC

	${CMAKE_CURRENT_SOURCE_DIR}/


)
