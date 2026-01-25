#ifndef __WS_SIM_MAIL_HEADER__
#define __WS_SIM_MAIL_HEADER__

#include "OBJ_message.h"
#include "SIM_packet.h"

#include <stdint.h>
#include <stdbool.h>

#define SIM_MAIL_PACKETS_BUFFER 16
#define SIM_MAIL_PACKETS_MAX 11

typedef struct WS_SIM_mail
{
	uint8_t current;

	SIM_packet_t packet[SIM_MAIL_PACKETS_BUFFER];
}SIM_mail_t;

bool SIM_mail_can_write_packet(SIM_mail_t *mail);
bool SIM_mail_write_packet(SIM_mail_t *mail, SIM_packet_t packet);

OBJ_msg_t SIM_mail_pull_packets(SIM_mail_t *mail);




#endif
