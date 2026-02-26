#ifndef __STAGE_COSTUME_HEADER__
#define __STAGE_COSTUME_HEADER__
//tag
#include "TAG_tag.h"

//stage
#include "STAGE_commons.h"
//std
#include <stdint.h>

typedef struct STAGE_costume
{
	STAGE_cid_t costume_id;
	TAG_tag_t *tag;		
	
}STAGE_costume_t;

#endif
