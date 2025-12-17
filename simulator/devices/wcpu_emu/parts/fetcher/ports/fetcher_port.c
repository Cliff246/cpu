#include "fetcher_port_ptr.h"
#include "fetcher_port.h"

#include "fetcher_code_descriptor_port.h"
#include "fetcher_code_table_port.h"
#include "fetcher_immediate_port.h"
#include "fetcher_instruction_port.h"
#include "fetcher_controller.h"

#include <stdlib.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


fetcher_port_class_t fetcher_port_vtable[WCPU_FETCHER_PORTS_COUNT] =
{
	[WCPU_FETCHER_PORT_CODE_DESCRIPTOR] =
	{
		.create = fetcher_port_code_description_create,
		.export = fetcher_port_code_description_export,
		.flush = fetcher_port_code_description_flush,
		.import = fetcher_port_code_description_import,
		.issue = fetcher_port_code_description_issue,
		.poll = fetcher_port_code_description_poll,
		.cycle = fetcher_port_code_description_cycle,
		.order = fetcher_port_code_description_order,
	},

	[WCPU_FETCHER_PORT_CODE_TABLE] =
	{
		.create = fetcher_port_code_table_create,
		.export = fetcher_port_code_table_export,
		.flush = fetcher_port_code_table_flush,
		.import = fetcher_port_code_table_import,
		.issue = fetcher_port_code_table_issue,
		.poll = fetcher_port_code_table_poll,
		.cycle = fetcher_port_code_table_cycle,
		.order = fetcher_port_code_table_order,
	},

	[WCPU_FETCHER_PORT_IMMEDIATE] =
	{
		.create = fetcher_port_immediate_create,
		.export = fetcher_port_immediate_export,
		.flush = fetcher_port_immediate_flush,
		.import = fetcher_port_immediate_import,
		.issue = fetcher_port_immediate_issue,
		.poll = fetcher_port_immediate_poll,
		.cycle = fetcher_port_immediate_cycle,
		.order = fetcher_port_immediate_order,
	},

	[WCPU_FETCHER_PORT_INSTRUCTION] =
	{
		.create = fetcher_port_instruction_create,
		.export = fetcher_port_instruction_export,
		.flush = fetcher_port_instruction_flush,
		.import = fetcher_port_instruction_import,
		.issue = fetcher_port_instruction_issue,
		.poll = fetcher_port_instruction_poll,
		.cycle = fetcher_port_instruction_cycle,
		.order = fetcher_port_instruction_order,
	},
};


fetcher_port_t *wcpu_fetcher_port_create(fetcher_port_type_t type)
{
	fetcher_port_t *port = calloc(1, sizeof(fetcher_port_t));
	assert(port);


	assert(fetcher_port_vtable[type].create != NULL);
	port->type = type;
	fetcher_port_vtable[type].create(port);
	assert(port->port.raw != NULL);

	return port;

}






uint32_t wcpu_fetcher_port_advance(fetcher_port_t *port, uint32_t cap_mask, fetcher_interface_t *interface)
{
	assert(port && interface);
	static int count = 0;
	//todo
	bool deps_satisfied = ((cap_mask & port->requires_mask) == port->requires_mask);

	uint32_t mask = cap_mask;
	if(deps_satisfied)
	{
		//wcpu_fetcher_port_set_state(port, FETCHER_PORT_ISSUE);
		//printf("satisfied %d %d\n", port->state, port->type);

		//port->ready = true;

	}
	switch(port->state)
	{
		case FETCHER_PORT_IDLE:


			if(port->ready)
			{
				 wcpu_fetcher_port_set_state(port, FETCHER_PORT_ISSUE);
			}


			break;

		case FETCHER_PORT_ISSUE:

			if(port->ready)
			{
				assert( fetcher_port_vtable[port->type].issue);
				uint64_t issue = fetcher_port_vtable[port->type].issue(port);
				//printf("%d %llu\n",port->type, issue);
				wcpu_fetcher_interface_add_import(interface, issue);
				wcpu_fetcher_port_set_state(port, FETCHER_PORT_WAITING);
			}


			break;

		case FETCHER_PORT_WAITING:
			if(port->ready)
			{
				//ptr to, this is dangerous.
				fetcher_entry_t entry = {0};
				bool got_entry = false;

				assert(fetcher_port_vtable[port->type].poll);
				assert(fetcher_port_vtable[port->type].import);

				//read ready
				while(wcpu_fetcher_interface_read_ready(interface, &entry))
				{
					uint64_t address_recieved = entry.address;

					if(fetcher_port_vtable[port->type].poll(port, address_recieved))
					{
						//set state of the port
						wcpu_fetcher_port_set_state(port, FETCHER_PORT_DONE);
						//imports the content of the port into it
						fetcher_port_vtable[port->type].import(port, address_recieved, entry.data);
						got_entry = true;

						break;
					}

				}

				if(got_entry)
				{
					assert(	fetcher_port_vtable[port->type].cycle);

					bool issue_again = fetcher_port_vtable[port->type].cycle(port);
					if(issue_again)
					{
						wcpu_fetcher_port_set_state(port, FETCHER_PORT_ISSUE);
					}
					else
					{
						wcpu_fetcher_port_set_state(port, FETCHER_PORT_DONE);
					}
				}

			}
			break;


		case FETCHER_PORT_DONE:

			if(port->ready)
			{
				assert(fetcher_port_vtable[port->type].export);

				part_signal_t *signal = fetcher_port_vtable[port->type].export(port);
				assert(signal);

				assert(wcpu_fetcher_interface_add_export(interface, signal));
				wcpu_fetcher_port_set_state(port, FETCHER_PORT_FLUSH);
				//printf("produced mask\n");
				mask = port->produces_mask;
			}
			break;

		case FETCHER_PORT_FLUSH:
			if(port->ready)
			{
				assert(fetcher_port_vtable[port->type].flush);
				fetcher_port_vtable[port->type].flush(port);
				wcpu_fetcher_port_set_state(port, FETCHER_PORT_IDLE);
			}

			break;

		case FETCHER_PORT_INVAL:



			//wcpu_fetcher_port_set_state(port, FETCHER_PORT_IDLE);


			break;
	}

	count++;

	return mask;
}

void wcpu_fetcher_port_set_state(fetcher_port_t *port, fetcher_port_state_type_t state)
{
	assert(port);
	port->state = state;
}

void wcpu_fetcher_port_consume_order(fetcher_port_t *port, fetcher_port_order_t *order)
{
	assert(port);
	assert(order);
	assert(fetcher_port_vtable[port->type].order);
	fetcher_port_vtable[port->type].order(port, order);

	switch(order->type)
	{

		default:

			free(order);
			break;
	}

}