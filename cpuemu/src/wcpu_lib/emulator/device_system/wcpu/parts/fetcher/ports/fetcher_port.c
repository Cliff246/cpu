#include "fetcher_port_ptr.h"
#include "fetcher_port.h"

#include "fetcher_code_descriptor_port.h"
#include "fetcher_code_table_port.h"
#include "fetcher_immediate_port.h"
#include "fetcher_instruction_port.h"

#include <stdlib.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


fetcher_port_class_t fetcher_port_vtable[WCPU_FETCHER_PORTS_COUNT] =
{
	[WCPU_FETCHER_PORT_CODE_DESCRIPTOR] =
	{
		.create = fetcher_port_code_description_create
	},

	[WCPU_FETCHER_PORT_CODE_TABLE] =
	{
		.create = fetcher_port_code_table_create
	},

	[WCPU_FETCHER_PORT_IMMEDIATE] =
	{
		.create = fetcher_port_immediate_create
	},

	[WCPU_FETCHER_PORT_INSTRUCTION] =
	{
		.create = fetcher_port_instruction_create
	},
};


fetcher_port_t *wcpu_fetcher_port_create(fetcher_port_type_t type)
{
	fetcher_port_t *port = calloc(1, sizeof(fetcher_port_t));
	assert(port);

	port->type = type;

	assert(fetcher_port_vtable[type].create != NULL);

	port->port = fetcher_port_vtable[type].create(type);
	assert(port->port.raw != NULL);

	return port;

}
void wcpu_fetcher_port_advance(fetcher_port_t *port, fetcher_interface_t *interface)
{
	//todo

	switch(port->state)
	{
		case FETCHER_PORT_IDLE:


			if(port->ready)
			{
				port->state = FETCHER_PORT_ISSUE;
				port->ready = false;
			}


			break;

		case FETCHER_PORT_ISSUE:

			if(port->ready)
			{
				uint64_t issue = fetcher_port_vtable[port->type].issue(port);

				wcpu_fetcher_interface_add_import(interface, issue);
				port->state = FETCHER_PORT_WAITING;
				port->ready = false;
			}


			break;

		case FETCHER_PORT_WAITING:
			if(port->ready)
			{
				fetcher_entry_t *entry;
				while(wcpu_fetcher_interface_read_ready(interface, &entry))
				{
					uint64_t address_recieved = entry->address;
					if(fetcher_port_vtable[port->type].poll(port, address_recieved))
					{
						port->state = FETCHER_PORT_DONE;
						port->ready = false;
						fetcher_port_vtable[port->type].import(port, address_recieved, entry->data);
						//sets the entry for clean up
						entry->ready = false;
					}

				}




			}
			break;


		case FETCHER_PORT_DONE:
			if(port->ready)
			{
				part_signal_t *signal = fetcher_port_vtable[port->type].export(port);

				wcpu_fetcher_interface_add_export(interface, signal);

				port->state = FETCHER_PORT_FLUSH;
				port->ready = false;
			}
			break;

		case FETCHER_PORT_FLUSH:
			if(port->ready)
			{
				fetcher_port_vtable[port->type].flush(port);
				port->state = FETCHER_PORT_IDLE;
				port->ready = false;
			}

			break;

		case FETCHER_PORT_INVAL:





			break;
	}



}

void wcpu_fetcher_port_set_state(fetcher_port_t *port, fetcher_port_state_type_t state)
{

	port->state = state;
}