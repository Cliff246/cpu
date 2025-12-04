#ifndef __WCPU_PART_FETCHER_CONTROLLER_HEADER__
#define __WCPU_PART_FETCHER_CONTROLLER_HEADER__

#include <stdbool.h>

#include "fetcher_request.h"
#include "fetcher_code_descriptor_port.h"


//painnnn
#define WCPU_FETCHER_REQUESTS_MAX 10

typedef struct wcpu_fetcher_controller
{
	bool code_descriptor;

	int current;
	fetcher_request_t requests[WCPU_FETCHER_REQUESTS_MAX];

	fetcher_code_descriptor_t current_descriptor;

}fetcher_controller_t;





#endif