#ifndef __OBJECT_HEADER__
#define __OBJECT_HEADER__



//#include "emulator_configure.h"


#include <stdint.h>
#include <stdlib.h>

//set up device


typedef struct WS_OBJ_object
{
	void *ptr;
	//simulator id
	const int sid;
	//index id
	const int iid;
}OBJ_object_t;






#endif
