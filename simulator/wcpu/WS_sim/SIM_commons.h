#ifndef __WS_SIM_COMMONS_HEADER__
#define __WS_SIM_COMMONS_HEADER__

#include <stdint.h>
#include <stdbool.h>

#define SIM_PACKET_MAX_SIZE 62

#define SIM_PORT_SIZE 8
#define SIM_TAGS_SIZE 1000

typedef int64_t SIM_dtag_t;
typedef int64_t SIM_wireid_t;
typedef int64_t SIM_chnlid_t;


void SIM_free_tag_table_elem(void *v);


#endif
