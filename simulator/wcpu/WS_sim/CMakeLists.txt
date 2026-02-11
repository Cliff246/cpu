

SET(WS_SIM_CFILES
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_simulator.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_transfer.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_channel.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_chnlcfg.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_graph.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_stage.c


	${CMAKE_CURRENT_SOURCE_DIR}/SIM_device.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_devcfg.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_handle.c


	${CMAKE_CURRENT_SOURCE_DIR}/SIM_mail.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_envelope.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_message.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_messageblock.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_vistable.c


	${CMAKE_CURRENT_SOURCE_DIR}/SIM_wire.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_wirering.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_wirecfg.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_wirerouter.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_port.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_commons.c
	${CMAKE_CURRENT_SOURCE_DIR}/SIM_mailbox.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_context.c

	${CMAKE_CURRENT_SOURCE_DIR}/SIM_packet.c

)


add_library(ws_sim STATIC ${WS_SIM_CFILES})


set(CMAKE_BUILD_TYPE Debug)
target_include_directories(ws_sim
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/entry
	ws_io
	ws_cfg

	ws_tag
	wcpu_commons

)
target_link_libraries(ws_sim PUBLIC
	ws_tag
	ws_cfg
	ws_io
	wcpu_commons
)

