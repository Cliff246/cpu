#ifndef __DIRECTIVE_HEADER__
#define __DIRECTIVE_HEADER__

#include "parser.h"
#include "inter.h"


//the define directives types count must be n - 1 size
typedef enum directive_type
{
	DIR_INVAL,
	DIR_PUB,
	DIR_IMP,
	DIR_DEF,
	DIR_START,
}directive_type_t;

#define DIRECTIVES_TYPES_COUNT 4


struct context;



typedef enum dirarg_type
{
	DIRARG_UNDEFINED,
	DIRARG_INVAL,
	DIRARG_FILE,
	DIRARG_REFERENCE,
	DIRARG_IMPLICIT
}dirarg_type_t;

typedef struct dirarg
{
	char *content;
	dirarg_type_t type;

}dirarg_t;

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