#include "SIM_mail.h"
#include "OBJ_message.h"
#include "SIM_commons.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

bool SIM_mail_can_write_packet(SIM_mail_t *mail)
{
	if(mail->current < SIM_MAIL_PACKETS_MAX)
	{
		return true;
	}
	return false;
}
bool SIM_mail_write_packet(SIM_mail_t *mail, SIM_packet_t packet)
{
	bool can_write = SIM_mail_can_write_packet(mail);
	assert(can_write == true && "must be able to write packet");
	if(can_write == false)
		return false;



	mail->packet[mail->current++] = packet;
	return true;
}

OBJ_msg_t SIM_mail_pull_packets(SIM_mail_t *mail)
{
	OBJ_msg_t msg = {0};

	if(mail->current < 2)
	{
		assert(0);
	}
	assert(0 && "this is fucking stupid");




	//memcpy(&msg.value.address, pkt_2, SIM_PACKET_SIZE);


	for(uint8_t i = 1, s = 0; i < mail->current; ++i, s+=8)
	{
		//memcpy(&msg.msg[s], mail->packet[i].bytes, SIM_PACKET_SIZE);
		//msg.bytes_used += mail->packet->used;
	}

}