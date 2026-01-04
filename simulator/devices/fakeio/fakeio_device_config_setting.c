#include "fakeio.h"
#include "fakeio_device_config_setting.h"
#include "device.h"

#include "device_list.h"
#include <stdlib.h>
#include <assert.h>

WS_dev_flag_apply_fn device_fakeio_producer_functions[DEVICE_FAKEIO_CMD_OPTIONS_COUNT] =
{
	DEVICE_FAKEIO_CMD_OPTIONS_LIST(DEVICE_FAKEIO_CMD_PRODUCER_FUNC_ARRAY)
};

char *device_fakeio_producer_names[DEVICE_FAKEIO_CMD_OPTIONS_COUNT] =
{
	DEVICE_FAKEIO_CMD_OPTIONS_LIST(DEVICE_FAKEIO_CMD_PRODUCER_NAME_ARRAY)
};

WS_dev_flag_type_t device_fakeio_producer_types[DEVICE_FAKEIO_CMD_OPTIONS_COUNT] =
{
	DEVICE_FAKEIO_CMD_OPTIONS_LIST(DEVICE_FAKEIO_CMD_PRODUCER_TYPE_ARRAY)

};



bool device_cmd_option_producer_SIZE(WS_dev_t *dev, WS_dev_flag_t *flag)
{
	//printf("size %d\n", flag->value->INT);

	dev_fakeio_t *fakeio = dev->ptr;
	fakeio->length = flag->value->INT;

	dev->address_range_length = flag->value->INT;

	return true;

}

bool device_cmd_option_producer_START(WS_dev_t *dev, WS_dev_flag_t *flag)
{
	//printf("start %d\n", flag->value->INT);
	dev_fakeio_t *fakeio = dev->ptr;
	fakeio->start = flag->value->INT;
	dev->address_range_start = flag->value->INT;

	return true;

}

bool device_cmd_option_producer_PRINT_CONTENT(WS_dev_t *dev, WS_dev_flag_t *flag)
{

	dev_fakeio_t *fakeio = dev->ptr;

	return true;

}

bool device_cmd_option_producer_RESET(WS_dev_t *dev, WS_dev_flag_t *flag)
{
	dev_fakeio_t *fakeio = dev->ptr;

	return true;

}

bool device_cmd_option_producer_OUTPUT_BUTTON(WS_dev_t *dev, WS_dev_flag_t *flag)
{
	dev_fakeio_t *fakeio = dev->ptr;
	fakeio->button_address = flag->value->INT;
	return true;

}
