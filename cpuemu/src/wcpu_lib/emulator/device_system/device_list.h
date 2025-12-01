#ifndef __DEVICE_LIST_HEADER__
#define __DEVICE_LIST_HEADER__

#define DEVICE_LIST(X)	\
	X(WCPU, wcpu)		\
	X(RAM, ram)			\
	X(FAKEIO, fakeio)	\



#define DEVICE_NAME_TYPE(name, Y) DEVICE_ ## name

#define DEVICE_NAME_STRUCT(X, name) name

#define DEVICE_TYPE_ENUM(X, Y) DEVICE_NAME_TYPE(X, Y),


typedef enum device_type
{
	DEVICE_INVAL,
	DEVICE_LIST(DEVICE_TYPE_ENUM)
}device_type_t;


#define DEVICE_LIST_COUNTER(X, Y) + 1

#define DEVICES_LIST_COUNT (0 DEVICE_LIST(DEVICE_LIST_COUNTER))
#define DEVICES_LIST_TYPE_COUNT DEVICES_LIST_COUNT + 1
#define DEVICE_ID_INVAL -1
typedef int dev_id_t;

#define DEVICE_STRUCT_PROTOTYPE_NAME(X) dev_ ## X ## _t

#define DEVICE_LIST_STRUCT_PROTOTYPES(X, Y) typedef struct device_## Y DEVICE_STRUCT_PROTOTYPE_NAME(Y);

DEVICE_LIST(DEVICE_LIST_STRUCT_PROTOTYPES)


#define DEVICE_LIST_UNION(X, Y) DEVICE_STRUCT_PROTOTYPE_NAME(Y) *Y;
typedef union device_type_ptr
{
	DEVICE_LIST(DEVICE_LIST_UNION)
	void *ptr;
}device_type_ptr_t;



#define DEVICE_LIST_CONFIG_SETTING_PROTOTYPE(X, Y) typedef struct device_ ## Y ## _config_setting dev_ ## Y ## _config_setting_t;

DEVICE_LIST(DEVICE_LIST_CONFIG_SETTING_PROTOTYPE)

#define DEVICE_LIST_CONFIG_SETTING_UNION(X, Y) dev_ ## Y ## _config_setting_t *Y;

typedef union device_config_setting_ptr
{
	DEVICE_LIST(DEVICE_LIST_CONFIG_SETTING_UNION)
	void *ptr;
}device_config_setting_ptr_t;

#endif