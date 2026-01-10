#ifndef __WS_OBJ_HANDLE_HEADER__
#define __WS_OBJ_HANDLE_HEADER__


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


//ownns subhandles in the object
typedef struct WS_OBJ_handle
{
	const int sid;
	//object id
	const int oid;
	//handle id
	const int hid;
	//context id;
	const int cid;
}OBJ_hnd_t;

#endif