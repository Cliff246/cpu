#include "SIM_message.h"
#include "OBJ_message.h"
#include <string.h>
#include <assert.h>

bool SIM_message_get_open(SIM_message_t *smsg)
{
	if(smsg->used > 0)
		return false;
	for(uint8_t i = 0; i < OBJ_MESSAGE_KEY_SIZE; ++i)
	{
		if(smsg->key[i] != 0)
			return false;
	}
	return true;
}

bool SIM_message_assign(SIM_message_t *smsg, char key[OBJ_MESSAGE_KEY_SIZE], uint64_t group, uint8_t size)
{
	if(!SIM_message_get_open(smsg))
		return false;
	memcpy(smsg->key, key, sizeof(char) * OBJ_MESSAGE_KEY_SIZE);
	smsg->size = size;
	smsg->group = group;
	return true;
}

void SIM_message_append(SIM_message_t *smsg, OBJ_msg_t *omsg)
{

	int val = strncmp(smsg->key, omsg->key, OBJ_MESSAGE_KEY_SIZE);
	assert(val == 0 && "cannot append if keys are not the same");
	assert(smsg->size > smsg->used);
	memcpy(smsg->blks[omsg->index], omsg->msg, OBJ_MESSAGE_SIZE);
	smsg->used++;

}

void SIM_message_clear(SIM_message_t *msg)
{

	memset(msg, 0, sizeof(SIM_message_t));
}