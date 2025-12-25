#include "vm_op.h"

#define VM_OP_STATUS_STRING_ARRAY(X) [VM_OP_STATUS_NAME(X)] = #X,

char *vm_op_status_str[] =
{
	VM_OP_STATUS_LIST(VM_OP_STATUS_STRING_ARRAY)
};