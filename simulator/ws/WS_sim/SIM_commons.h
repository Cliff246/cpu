#ifndef __WS_SIM_COMMONS_HEADER__
#define __WS_SIM_COMMONS_HEADER__

#include <stdint.h>
#include <stdbool.h>

#define SIM_PACKET_MAX_SIZE 62

#define SIM_PORT_SIZE 8
#define SIM_TAGS_SIZE 1000

//actor id
typedef int64_t SIM_aid_t;
//role id
typedef int64_t SIM_rid_t;

//unique id
typedef int64_t SIM_uid_t;

//device id
typedef uint64_t SIM_did_t;
typedef int64_t SIM_dkey_t;

typedef uint64_t SIM_wid_t;
typedef int64_t SIM_wkey_t;

typedef int64_t SIM_chnlid_t;

typedef int64_t SIM_plugin_id_t;

void SIM_free_tag_table_elem(void *v);


#endif
