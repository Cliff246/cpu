#include "SIM_mail.h"
#include "OBJ_message.h"
#include "SIM_commons.h"
#include "SIM_packet.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*

bool SIM_mail_can_write_packet(SIM_mail_t *mail)
{
	if(mail->current < SIM_MAIL_PACKETS_MAX && mail->done == false)
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
	if(packet.type == SIM_PACKET_FOOT)
		mail->done = true;
	return true;
}

OBJ_msg_t SIM_mail_pull_packets(SIM_mail_t *mail)
{
	OBJ_msg_t msg = {0};
	assert(mail->done == true);
	if(mail->current < 2)
	{
		assert(0);
	}
	assert(0 && "this is fucking stupid");

	int count = 0;

	if(mail->packet[count++].type != SIM_PACKET_HEADER)
	{
		assert(0);

	}

	if(mail->packet[count++].type != SIM_PACKET_VALUE)
	{
		assert(0);
	}
	if(mail->packet[count++].type != SIM_PACKET_TAG)
	{
		assert(0);
	}
	bool found_end = false;
	int body_count = 0;
	for(; count < mail->current; ++count)
	{
		if(mail->packet[count].type == SIM_PACKET_VALUE)
		{
			body_count++;
		}
		else if(mail->packet[count].type == SIM_PACKET_FOOT)
		{
			found_end = true;
			break;
		}
	}

	SIM_packet_t head = mail->packet[0];
	SIM_packet_t value = mail->packet[1];
	SIM_packet_t tag = mail->packet[2];


	msg.type = head.data.header.msgtype;
	msg.value = value.data.value.val;
	msg.tag = tag.data.tag.tag;

	if(body_count == 0)
	{
		assert(0);
	}
	else
	{
		int32_t curr = 3;
		assert(body_count < OBJ_MESSAGE_SIZE / 8);
		for(int i = 0; i < body_count; i+=8)
		{
			for(int k = 0; k < 8; ++k)
			{
				msg.msg[i + k] = mail->packet[curr].data.raw.bytes[k];

			}

			curr++;
			msg.bytes_used += 8;

		}
	}

	mail->current = 0;
	mail->done = false;

	return msg;


}


bool SIM_mail_can_read_packet(SIM_mail_t *mail)
{
	if(mail->current > 0 && mail->done == false)
	{
		return true;
	}
	return false;
}

bool SIM_mail_read_packet(SIM_mail_t *mail, SIM_packet_t *packet)
{
	bool can_read = SIM_mail_can_read_packet(mail);
	assert(can_read == true && "must be able to write packet");
	if(can_read == false)
		return false;



	*packet = mail->packet[mail->current--];
	if(mail->current == 0)
		return false;
	return true;
}

void SIM_mail_put_packets(SIM_mail_t *mail, OBJ_msg_t *msg)
{
	assert(mail->done == true);

	SIM_packet_t head = mail->packet[0];
	SIM_packet_t value = mail->packet[1];
	SIM_packet_t tag = mail->packet[2];

	msg->type = head.data.header.msgtype;
	msg->tag = tag.data.tag.tag;
	msg->value = value.data.value.val;
	bool finished = false;
	for(int i = 3, b = 0; i < SIM_MAIL_PACKETS_MAX; ++i, b+=8)
	{
		SIM_packet_t packet = mail->packet[i];


		if(packet.type == SIM_PACKET_FOOT)
		{
			finished = true;
			break;
		}
		else
		{
			for(int k = 0; k < 8; ++k)
			{
				msg->msg[b + k] = packet.data.raw.bytes[k];

			}
			msg->bytes_used += 8;

		}
	}
	assert(finished);

	mail->current = 0;
	mail->done = false;
}


*/