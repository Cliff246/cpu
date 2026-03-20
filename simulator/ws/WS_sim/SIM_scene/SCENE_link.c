#include "SCENE_link.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SCENE_link_t *SCENE_init_link(SIM_uid_t puid, char *positive, char *negative)
{

	SCENE_link_t *link = calloc(1, sizeof(SCENE_link_t));
	assert(link);

	char *pos_copy = strdup(positive);
	assert(pos_copy);
	char *neg_copy = strdup(negative);
	assert(neg_copy);

	link->puid = puid;
	link->pos = pos_copy;
	link->neg = neg_copy;

	return link;
}

char *SCENE_get_positive_link(SCENE_link_t *link)
{
	return link->pos;
}

char *SCENE_get_negative_link(SCENE_link_t *link)
{
	return link->neg;
}

void SCENE_free_link(SCENE_link_t *link)
{
	free(link->neg);
	free(link->pos);
	free(link);
}

void SCENE_print_link(SCENE_link_t *link)
{
	printf("link %ld:[+<%s>] |%ld:[-<%s>]\n",link->puid, link->pos, link->nuid, link->neg);
}