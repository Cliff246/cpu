#ifndef __WS_OBJ_HANDLE_HEADER__
#define __WS_OBJ_HANDLE_HEADER__


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


//ownns subhandles in the object
typedef struct WS_OBJ_handle
{
	//simulator id
	int sid;
	//object id
	int oid;
	//handle id
	int hid;
	//context id;
	int cid;
}OBJ_handle_t;

#endif
