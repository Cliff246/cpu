#ifndef __VIRTUAL_MACHINE_HEADER__
#define __VIRTUAL_MACHINE_HEADER__

#include <stdint.h>

typedef struct virtual_machine vima_t;

#include "vm_bus.h"
#include "vm_regs.h"
#include "vm_op.h"
#include "vm_txn.h"

#include "token.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


//TODO move towards more correct design, with multiple inflight instructions for speed


typedef struct virtual_machine
{
	vm_bus_t bus;

	vm_bookmark_t bookmarks[BOOKMARKS_MAX];

	vm_regfile_t regs;


	struct
	{
		bool wait_until_inst_end;
		bool load_cd;
		int cd_count;
		int cd_base;
		vm_bus_port_id_t port;
		vm_bus_hnd_t hnd;
		bool new_inst;
	}control;


	vm_txn_t *txn;
}vima_t;






#endif
