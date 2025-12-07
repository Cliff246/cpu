#include "fetcher_entry.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void wcpu_part_fetcher_print_entry(fetcher_entry_t entry)
{
	printf("entry:([%llu] = %lld | time:%d | ready:%d)\n",entry.address, entry.data, entry.time, entry.ready);
}