#ifndef __CFG_SETTING_HEADER__
#define __CFG_SETTING_HEADER__

#include "IO_parser.h"
#include <stdint.h>

//temporary
typedef struct CFG_setting
{
	uint64_t id;
	uint64_t latency;
	uint64_t throughput;
}CFG_setting_t;


CFG_setting_t *CFG_init_setting(IO_pnode_t *setting);
void CFG_free_setting(CFG_setting_t *setting);
#endif