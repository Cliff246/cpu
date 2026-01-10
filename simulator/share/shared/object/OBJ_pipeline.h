#ifndef __WS_OBJ_PIPELINE_HEADER__
#define __WS_OBJ_PIPELINE_HEADER__


#include "OBJ_packet.h"
#include "OBJ_handle.h"
#include "OBJ_context.h"
#include "OBJ_locals.h"

typedef struct WS_OBJ_pipeline
{
	//pulls in pkt's in
	void (*pull)(OBJ_hnd_t *hnd, OBJ_ctx_t *ctx, OBJ_pkt_t *pkt);
	//converts to context
	void (*update)(OBJ_hnd_t *hnd, OBJ_ctx_t *ctx, OBJ_locals_t *lcl);
	//pushes out new packets
	void (*push)(OBJ_hnd_t *hnd, OBJ_ctx_t *ctx, OBJ_pkt_t *pkt);
}OBJ_pipeline_t;

#endif