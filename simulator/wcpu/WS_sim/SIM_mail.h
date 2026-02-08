#ifndef __WS_SIM_MAIL_HEADER__
#define __WS_SIM_MAIL_HEADER__

#include "OBJ_message.h"
#include "SIM_message.h"
#include "SIM_envelope.h"
#include "SIM_packet.h"

#include <stdint.h>
#include <stdbool.h>

#define SIM_MAIL_PACKETS_BUFFER 16
#define SIM_MAIL_PACKETS_MAX 12

typedef struct WS_SIM_mail
{
	SIM_envelope_t envelope;

	SIM_message_t message;
}SIM_mail_t;

/*

bool SIM_mail_can_write_packet(SIM_mail_t *mail);
bool SIM_mail_write_packet(SIM_mail_t *mail, SIM_packet_t packet);

OBJ_msg_t SIM_mail_pull_packets(SIM_mail_t *mail);

bool SIM_mail_can_read_packet(SIM_mail_t *mail);
bool SIM_mail_read_packet(SIM_mail_t *mail, SIM_packet_t *packet);

void SIM_mail_put_packets(SIM_mail_t *mail, OBJ_msg_t *msg);

*/

#endif
