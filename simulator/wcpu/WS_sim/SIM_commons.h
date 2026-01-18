#ifndef __WS_SIM_COMMONS_HEADER__
#define __WS_SIM_COMMONS_HEADER__

#include <stdint.h>
#include <stdbool.h>

#define SIM_PACKET_SIZE 8
#define SIM_PORT_SIZE 8

typedef uint32_t SIM_channel_global_t;
typedef uint8_t SIM_channel_local_t;
typedef uint8_t SIM_wire_channel_t;
typedef uint16_t SIM_object_global_t;
typedef uint64_t SIM_port_global_t;
typedef int32_t SIM_message_tag_t;

#endif
