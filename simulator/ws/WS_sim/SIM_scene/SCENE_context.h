#ifndef __SCENE_CONTEXT_HEADER__
#define __SCENE_CONTEXT_HEADER__

//scene

#include "SCENE_scope.h"
#include "SCENE_topology.h"
#include <stdint.h>
typedef struct SCENE_scene SCENE_scene_t;

#define SCENE_CONTEXT_STEP_LIST(X)\
	X(START)					  \
	X(ALLOC)					  \
	X(VALIDATE)					  \
	X(ASSIGN)					  \
	X(SYMBOLIZE)				  \
	X(BUILD)					  \
	

#define SCENE_CONTEXT_STEP_LIST_NAME(X) SCENE_CONTEXT_STEP_ ## X
#define SCENE_CONTEXT_STEP_LIST_ENUM(X) SCENE_CONTEXT_STEP_LIST_NAME(X),

typedef enum SCENE_context_step
{
	SCENE_CONTEXT_STEP_LIST(SCENE_CONTEXT_STEP_LIST_ENUM)
}SCENE_ctx_step_t;

typedef struct SCENE_context
{
	SCENE_ctx_step_t step;
	SCENE_scene_t *scene;
	SCENE_scope_t *scope;
	SCENE_topology_t *topology;

	//CFG_context_t *context;
}SCENE_context_t;


SCENE_context_t *SCENE_alloc_context(SCENE_scene_t *scene);

bool SCENE_step_context(SCENE_context_t *context, SCENE_ctx_step_t step);

void SCENE_init_context(SCENE_context_t *context);
void SCENE_print_context(SCENE_context_t *context);
#endif