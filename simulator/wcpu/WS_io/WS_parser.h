#ifndef __WS_PARSER_HEADER__
#define __WS_PARSER_HEADER__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "device_command.h"

#define WS_PARSE_NODE_LIST(X)\
	X(HEADER, header)		 \
	X(BODY, body)			 \
	X(ELEMENT, element)		 \

#define WS_PARSE_NODE_LIST_NAME(X) WS_PARSE_NODE_ ## X

#define WS_PARSE_NODE_LIST_ENUM(X, Y) WS_PARSE_NODE_LIST_NAME(X),

typedef enum WS_parse_node_type
{
	WS_PARSE_NODE_LIST(WS_PARSE_NODE_LIST_ENUM)
}WS_parse_node_type_t;

#define WS_PARSE_NODE_LIST_STRUCT_TYPEDEF(X) WS_parse_node_ ## X ## _t
#define WS_PARSE_NODE_LIST_STRUCT_NAME(X) WS_parse_node_struct_ ## X
#define WS_PARSE_NODE_LIST_STRUCT_PROTOTYPES(X, Y) typedef struct WS_PARSE_NODE_LIST_STRUCT_NAME(Y) WS_PARSE_NODE_LIST_STRUCT_TYPEDEF(Y);
WS_PARSE_NODE_LIST(WS_PARSE_NODE_LIST_STRUCT_PROTOTYPES)

//this should be full lol
//header
struct WS_PARSE_NODE_LIST_STRUCT_NAME(header)
{
	char *module_path;

};


//body
struct WS_PARSE_NODE_LIST_STRUCT_NAME(body)
{
	char *id;
};

//element
struct WS_PARSE_NODE_LIST_STRUCT_NAME(element)
{
	WS_dev_cmd_t *cmd;
	toklex_t *lex;
};


#define WS_PARSE_NODE_LIST_UNION(X, Y) WS_PARSE_NODE_LIST_STRUCT_TYPEDEF(Y) Y;


typedef struct WS_parse_node
{
	WS_parse_node_type_t type;

	struct WS_parse_node **children;
	size_t length;
	union
	{
		WS_PARSE_NODE_LIST(WS_PARSE_NODE_LIST_UNION)
	}node;

}WS_parse_node_t;

typedef struct WS_parser
{
	WS_parse_node_t *head;
}WS_parser_t;




#endif