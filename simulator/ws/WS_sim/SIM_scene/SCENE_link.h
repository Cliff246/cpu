#ifndef __SCENE_LINK_HEADER__
#define __SCENE_LINK_HEADER__

#include "SIM_commons.h"
#include <stdint.h>

typedef struct SCENE_link
{
	SIM_uid_t uid;
	char *pos;
	char *neg;

}SCENE_link_t;


SCENE_link_t *SCENE_init_link(SIM_uid_t uid, char *positive, char *negative);
void SCENE_free_link(SCENE_link_t *link);
char *SCENE_get_positive_link(SCENE_link_t *link);
char *SCENE_get_negative_link(SCENE_link_t *link);
void SCENE_print_link(SCENE_link_t *link);
#endif