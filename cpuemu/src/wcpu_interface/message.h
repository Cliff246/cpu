#ifndef __WCPU_INTERFACE_COMMAND_HEADER__
#define __WCPU_INTERFACE_COMMAND_HEADER__

#define INTERFACE_COMMAND_HEADER_SIZE 20

typedef struct message
{
	char header[INTERFACE_COMMAND_HEADER_SIZE];
	char *contents;
}message_t;



#endif