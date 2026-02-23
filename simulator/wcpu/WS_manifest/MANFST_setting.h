#ifndef __MANFST_SETTING_HEADER__
#define __MANFST_SETTING_HEADER__

#include "SYNTAX_parser.h"
#include <stdint.h>

//temporary
typedef struct MANFST_setting
{
	uint64_t id;
	uint64_t latency;
	uint64_t throughput;
}MANFST_setting_t;


MANFST_setting_t *MANFST_init_setting(SYNTAX_pnode_t *setting);
void MANFST_free_setting(MANFST_setting_t *setting);
#endif