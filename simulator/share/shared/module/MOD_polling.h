#ifndef __WS_MOD_POLLING_HEADER__
#define __WS_MOD_POLLING_HEADER__

#include <stdint.h>
#include <stdbool.h>

#define MOD_POLLING_KEY 20

typedef struct WS_MOD_polling
{
	const char key[MOD_POLLING_KEY];
	const int status_check_wakeup;
}MOD_polling_t;

#endif