#ifndef __SHELL_HEADER__
#define __SHELL_HEADER__

#include "cli.h"

typedef struct hydra_entry
{
	char *keyword;
	
	void (*head)(cmd_t *cmd);

}hydra_entry_t;

extern hydra_entry_t hydra_list[];


void hydra(cmd_t *cmd);


#endif
