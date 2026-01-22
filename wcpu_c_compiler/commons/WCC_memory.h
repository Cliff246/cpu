#ifndef __WCC_MEMORY_HEADER__
#define __WCC_MEMORY_HEADER__

#include "WCC_alloc.h"
#include <stdint.h>
#include <stdbool.h>
//shared pointer key required for editing

typedef int64_t WCC_skey_t;

typedef struct WCC_shared_pointer
{
	//dont touch this
	WCC_skey_t lock;
	int64_t refcount;
	const WCC_malias_t ptr;
}*WCC_sptr_t;


//shared pointer release
void WCC_sptr_release(WCC_sptr_t sptr);
//shared pointer share
WCC_sptr_t WCC_sptr_share(WCC_sptr_t sptr);

//reserves
WCC_sptr_t WCC_sptr_reserve(uint64_t size);
WCC_sptr_t WCC_sptr_extend(WCC_sptr_t sptr, uint64_t size);

//returns negative on failure
WCC_skey_t WCC_sptr_lock(WCC_sptr_t sptr);

bool WCC_sptr_unlock(WCC_sptr_t sptr, WCC_skey_t key);

//8 bit reader of sptr
uint8_t WCC_sptr_read_8(WCC_sptr_t sptr, uint64_t index);
//16 bit reader of sptr
uint16_t WCC_sptr_read_16(WCC_sptr_t sptr, uint64_t index);
//32 bit reader of sptr
uint32_t WCC_sptr_read_32(WCC_sptr_t sptr, uint64_t index);
uint64_t WCC_sptr_read_64(WCC_sptr_t sptr, uint64_t index);

//the unlocked functions, wont work if locked
//returns false on lock
//return true on write
bool WCC_sptr_write_8(WCC_sptr_t sptr, uint64_t index, uint8_t value);
bool WCC_sptr_write_16(WCC_sptr_t sptr, uint64_t index, uint16_t value);
bool WCC_sptr_write_32(WCC_sptr_t sptr, uint64_t index, uint32_t value);
bool WCC_sptr_write_64(WCC_sptr_t sptr, uint64_t index, uint64_t value);

//returns false on lock
//return true on write
//the locked functions, requires a key to work
bool WCC_sptr_write_locked_8(WCC_sptr_t sptr, uint64_t index, uint8_t value, WCC_skey_t skey);
bool WCC_sptr_write_locked_16(WCC_sptr_t sptr, uint64_t index, uint16_t value, WCC_skey_t skey);
bool WCC_sptr_write_locked_32(WCC_sptr_t sptr, uint64_t index, uint32_t value, WCC_skey_t skey);
bool WCC_sptr_write_locked_64(WCC_sptr_t sptr, uint64_t index, uint64_t value, WCC_skey_t skey);





#endif