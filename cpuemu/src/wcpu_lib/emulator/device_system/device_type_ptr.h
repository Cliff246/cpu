#ifndef __DEVICE_TYPE_PTR_HEADER__
#define __DEVICE_TYPE_PTR_HEADER__


typedef struct device_ram dev_ram_t;
typedef struct device_wcpu dev_wcpu_t;
typedef struct device_fakeio dev_fakeio_t;

typedef enum device_type
{
	DEVICE_INVAL,
	DEVICE_WCPU,
	DEVICE_FAKEIO,
	DEVICE_RAM,
}device_type_t;

typedef union device_type_ptr
{
	dev_ram_t *ram;
	dev_wcpu_t *wcpu;
	dev_fakeio_t *fakeio;

	void *ptr;
}device_type_ptr_t;




#endif