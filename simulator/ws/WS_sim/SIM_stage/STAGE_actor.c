#include "STAGE_actor.h"
#include "SIM_commons.h"
#include "TAG_map.h"
#include "TAG_tag.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void STAGE_init_actor_costumes(STAGE_actor_t *actor)
{
	TAG_argptr_t arg_start = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_START);
	TAG_argptr_t arg_get_size = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_SIZE);
	TAG_argptr_t arg_reset_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_RESET);
	TAG_argptr_t arg_up_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_UP);
	TAG_argptr_t arg_is_end_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_IS_END);
	TAG_argptr_t arg_end = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_END);

	TAG_argptr_t arg_is_int_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_IS_KEY_INT);
	TAG_argptr_t arg_is_str_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_IS_KEY_STR);
	TAG_argptr_t arg_get_str_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_GET_KEY_STR);
	TAG_argptr_t arg_get_int_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_GET_KEY_INT);

	arg_start.MAP->iter_start(actor->tags);
	uint64_t size = arg_get_size.MAP->get_size(actor->tags);
	//printf("size: %ld\n", size);
	arg_reset_iter.MAP->iter_reset(actor->tags);


	for(uint64_t i = 0; i < size; ++i)
	{
		if(arg_is_end_iter.MAP->iter_is_end(actor->tags) == true)
		{

			break;
		}

		if(arg_is_int_iter.MAP->iter_is_key_int(actor->tags))
		{
			//printf("is int\n");
			int64_t key = arg_get_int_iter.MAP->iter_get_key_int(actor->tags);
			//printf("key: %ld\n", key);


		}
		else if(arg_is_str_iter.MAP->iter_is_key_str(actor->tags))
		{
			//printf("is int\n");
			char *key = arg_get_str_iter.MAP->iter_get_key_str(actor->tags);
			//printf("key: %s\n", key);
			//STAGE_append_costume(commons, )

		}
		else
		{
			//printf("not a\n");
		}

		arg_up_iter.MAP->iter_up(actor->tags);

		//printf("%ld\n", i);
	}
	arg_reset_iter.MAP->iter_reset(actor->tags);
	arg_end.MAP->iter_end(actor->tags);


}

STAGE_actor_t *STAGE_alloc_actor(SIM_aid_t aid)
{
	STAGE_actor_t *actor = calloc(1, sizeof(STAGE_actor_t));
	assert(actor);
	actor->actor_id = aid;
	return actor;

}
void STAGE_init_actor(STAGE_actor_t *actor, TAG_tag_t *tag, SIM_rid_t rid)
{

	actor->role_id = rid;
	TAG_tag_t *actor_tags = TAG_copy(tag);
	actor->tags = actor_tags;
	STAGE_init_actor_costumes(actor);

}

void STAGE_print_actor(STAGE_actor_t *actor)
{
	char *role_id_str = STAGE_get_str_rid(actor->role_id);
	printf("[%ld] %s\n",actor->actor_id, role_id_str);
	//
	TAG_print(actor->tags);
}