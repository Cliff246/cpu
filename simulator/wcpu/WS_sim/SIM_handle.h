#ifndef __WS_SIM_HANDLE_HEADER__
#define __WS_SIM_HANDLE_HEADER__

#include "MOD_description.h"
#include "dynamic_lib.h"
#include "hashmap.h"

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#define SIM_MODHND_MAX 100

typedef struct WS_SIM_handle
{
	MOD_description_t *desc;
	char *key;
	uint64_t id;
}SIM_handle_t;

typedef struct WS_SIM_modhandle
{
	WS_dynamic_lib_t *dl;
	MOD_description_t *desc;
	char *key;
	uint64_t alloc;
	SIM_handle_t **handles;
	bool used;

}SIM_modhandle_t;


static p_hashtable_t simmodhnds;

static SIM_modhandle_t *SIM_init_modhandle(char *handle);


void SIM_free_modhandle(void *ptr);

void SIM_alloc_simmodhnd(void);
void SIM_modhandle_append(SIM_modhandle_t *modhandle, SIM_handle_t *handle);


SIM_handle_t *SIM_init_handle_modhandle(SIM_modhandle_t *modhandle);
SIM_handle_t *SIM_init_handle(char *handle);
void SIM_free_handle(SIM_handle_t *handle);


#endif