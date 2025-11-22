#ifndef __DEVICE_TYPE_PTR_HEADER__
#define __DEVICE_TYPE_PTR_HEADER__


typedef struct device_ram dev_ram_t;
typedef struct device_wcpu dev_wcpu_t;
typedef struct device_fakeio dev_fakeio_t;

#define DEVICE_TYPE_ENUM(X) 	\
	X(INVAL)					\
	X(WCPU)					\
	X(FAKEIO)					\
	X(RAM)					\

#define DEVICE_TYPE_NAME_RESOLVE(name) DEVICE_ ## name

#define DEVICE_TYPE_NAME_ENUM(X) DEVICE_TYPE_NAME_RESOLVE(X),

typedef enum device_type
{
	DEVICE_TYPE_ENUM(DEVICE_TYPE_NAME_ENUM)
}device_type_t;

#define DEVICE_TYPE_NAME_COUNTER(X) + 1

#define DEVICES_TYPE_COUNT (0 DEVICE_TYPE_ENUM(DEVICE_TYPE_NAME_COUNTER))


typedef union device_type_ptr
{
	dev_ram_t *ram;
	dev_wcpu_t *wcpu;
	dev_fakeio_t *fakeio;

	void *ptr;
}device_type_ptr_t;




#endif