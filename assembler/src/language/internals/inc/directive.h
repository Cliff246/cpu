#ifndef __DECLERATION_HEADER__
#define __DECLERATION_HEADER__



typedef enum directive_type
{
	DIRECTIVE_TYPE_INCLUDE,
	DIRECTIVE_TYPE_PUBLIC,
	DIRECTIVE_TYPE_DEFINE,

}directive_type_t;


typedef struct directive_include
{
	char *file_include;
	int id;
}directive_include_t;

typedef struct directive_public
{
	char *key_string;
	int count;
}directive_public_t;

typedef struct directive_define
{
	char *key_imply;
}directive_define_t;


typedef struct directive
{
	directive_type_t type;

	union
	{
		directive_include_t inc;
		directive_define_t def;
		directive_public_t pub;
	} directive;
}directive_t;

#endif