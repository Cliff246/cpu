#ifndef __WS_SIM_MESSAGE_HEADER__
#define __WS_SIM_MESSAGE_HEADER__

#include "OBJ_bundle.h"
#include "OBJ_message.h"

#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>

typedef struct WS_SIM_message
{
	char key[OBJ_MESSAGE_KEY_SIZE];
	uint64_t group;
	uint8_t used;
	uint8_t size;
	_Alignas(64) uint8_t blks[OBJ_MESSAGE_SIZE][OBJ_BUNDLE_SIZE];

}SIM_message_t;

bool SIM_message_get_open(SIM_message_t *smsg);
bool SIM_message_assign(SIM_message_t *smsg, char key[OBJ_MESSAGE_KEY_SIZE], uint64_t group, uint8_t size);

void SIM_message_append(SIM_message_t *smsg, OBJ_msg_t *omsg);
void SIM_message_clear(SIM_message_t *msg);

#endif