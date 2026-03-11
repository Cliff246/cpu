#ifndef __SCENE_PORT_HEADER__
#define __SCENE_PORT_HEADER__

#include "SCENE_anchor.h"
#include "SCENE_link.h"
#include "TAG_tag.h"
#include <stdint.h>

//resource goes to port, port ->

/*
	class = "mem"
		lanes = 1
		channels = 2
		channels_positive = ["cpI4_0", "cpI4_1"]
		channels_negative = ["cnI4_0", "cnI4_1"]
		connects =
		{
			link = ["cpI4_0", "cnc0"]
			link = ["cpI4_1", "cne0"]


*/

typedef struct SCENE_port
{
	char *class;
	uint64_t lanes;
	uint64_t channels_count;

	char **postive;
	char **negative;

	uint64_t links_size;
	SCENE_link_t *links;



}SCENE_port_t;

SCENE_port_t *SCENE_init_port(TAG_tag_t *port_map);
void SCENE_print_port(SCENE_port_t *port);
void SCENE_free_port(SCENE_port_t *port);
#endif
