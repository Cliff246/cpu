#ifndef __WS_RUN_CHANNEL_HEADER__
#define __WS_RUN_CHANNEL_HEADER__


#include "OBJ_message.h"
#include "OBJ_bundle.h"

#include <stdint.h>
#include <stdbool.h>




//control unit for each end port
//designed to hold and manage merging and more
typedef struct RUN_channel
{

	


}RUN_chnl_t;

void RUN_channel_init(RUN_chnl_t *channel, uint32_t wcid, uint32_t cid, uint32_t wid);


bool RUN_channel_get_has_packet(RUN_chnl_t *channel);
bool RUN_channel_get_output(RUN_chnl_t *channel);

void RUN_channel_print(RUN_chnl_t *channel);

#endif