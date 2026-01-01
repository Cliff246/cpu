#ifndef __DIRECTIVE_HEADER__
#define __DIRECTIVE_HEADER__

#include "parser.h"
#include "inter.h"


//the define directives types count must be n - 1 size


#define DIRECTIVE_TYPE_ENUM(DIR_X)	\
	DIR_X(INVAL)					\
	DIR_X(IMP)						\
	DIR_X(PUB)						\
	DIR_X(DEF)						\
	DIR_X(ORDER)					\


#define DIR_X(val) DIR_ ## val,

typedef enum directive_type
{
	DIRECTIVE_TYPE_ENUM(DIR_X)
}directive_type_t;
#undef DIR_X

#define DIR_X_ADD(val) + 1

#define DIRECTIVES_TYPES_COUNT (0 DIRECTIVE_TYPE_ENUM(DIR_X_ADD))



struct context;



typedef enum dirarg_type
{
	DIRARG_UNDEFINED,
	DIRARG_INVAL,
	DIRARG_FILE,
	DIRARG_REFERENCE,
	DIRARG_IMPLICIT,
	DIRARG_TAG,
}dirarg_type_t;

typedef struct dirarg
{
	char *content;
	dirarg_type_t type;

}dirarg_t;
//shit
#define MAX_DIRECTIVE_CONTENTS 10
typedef struct directive
{

	parse_node_t *head;
	int index;
	directive_type_t type;
	dirarg_t contents[MAX_DIRECTIVE_CONTENTS];
}directive_t;




void apply_directive(struct context *ctx, directive_t *dir);
directive_t *create_directive(struct context *ctx, parse_node_t *head);

/*
 * Directive semantic effects:
 *   DIR_PUB - Exposes a symbol reference externally, marking it as public for the linker.
 *   DIR_DEF - Declares a placeholder definition for a symbol that will exist later.
 *   DIR_INC - Signals the assembler to include another file into the current context.
 */



#endif