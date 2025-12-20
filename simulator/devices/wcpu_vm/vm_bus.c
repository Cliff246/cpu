#include "vm_bus.h"
#include "vm.h"


vm_bus_port_id_t vm_bus_create_port(vima_t *vm, vm_addr_mode_t mode)
{

}	


vm_bus_hnd_t vm_bus_put_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_evnt_t evnt)
{

}
//polls event with the handle recieved
bool vm_bus_poll_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_hnd_t hnd)
{

}
//response of event
vm_bus_response_t vm_bus_pull_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_hnd_t hnd)
{

}


bool vm_bus_free_port(vima_t *vm, vm_bus_port_id_t port)
{

}	



