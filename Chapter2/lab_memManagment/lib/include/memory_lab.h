#ifndef __MEMORY_LAB_H
#define __MEMORY_LAB_H

#include <stdint.h>

#define VOID_ALLOCATOR 			0
#define UNIPROTON_ALLOCATOR		1
#define BASIC_ALOG_FIRST_FIT		2
#define BASIC_ALOG_BEST_FIT		3
#define BASIC_ALOG_WORST_FIT		4
#define MEMORY_POOL			5
#define BUDDY_ALLOCATOR			6
#define SLAB_ALLOCATOR			7

#if (ALLOCATOR == SLAB_ALLOCATOR)
#include <slab.h>
static inline uint32_t memory_init(uint64_t addr, uint32_t size)
{
	uint32_t ret = slab_memp_init(&default_slab_pool, addr, size);
        return 0;
}

static inline void  memory_exit(void)
{
        return;
}

static inline void* memory_alloc(size_t size)
{
        return slab_m_malloc(&default_slab_pool, size);
}

static inline void  memory_free(void* ptr)
{
	slab_m_free(&default_slab_pool, ptr);
        return;
}



#endif

#if (ALLOCATOR == VOID_ALLOCATOR)
static inline uint32_t memory_init(uint64_t addr, uint32_t size)
{
        return 0;
}

static inline void  memory_exit(void)
{
        return;
}

static inline void* memory_alloc(size_t size)
{
        return NULL;
}

static inline void  memory_free(void* ptr)
{
        return;
}
#endif


#if (ALLOCATOR == UNIPROTON_ALLOCATOR)
#include <prt_mem.h>
extern uint32_t OsFscMemInit(uintptr_t addr, uint32_t size);
static inline uint32_t memory_init(uint64_t addr, uint32_t size)
{
	return OsFscMemInit(addr, size);
}

static inline void  memory_exit(void)
{
	return;
}

static inline void* memory_alloc(size_t size)
{
	size_t real_size = (size==0?1:size);
	return PRT_MemAlloc(real_size);
}

static inline void  memory_free(void* ptr)
{
	uint32_t ret = PRT_MemFree(ptr);
	(void)ret;
	return;
}
#endif


#if (ALLOCATOR == BASIC_ALOG_FIRST_FIT)
#include "basic_alog.h"

static inline uint32_t memory_init(uint64_t addr, uint32_t size)
{
	uint32_t ret =  memp_init(&default_mempool, addr, size);
        return ret;
}

static inline void  memory_exit(void)
{
        return;
}

static inline void* memory_alloc(size_t size)
{
        return m_malloc_first_fit(&default_mempool, size);
}

static inline void  memory_free(void* ptr)
{
        return m_free(&default_mempool, ptr);
}
#endif

#if (ALLOCATOR == BASIC_ALOG_BEST_FIT)
#include "basic_alog.h"

static inline uint32_t memory_init(uint64_t addr, uint32_t size)
{
        uint32_t ret =  memp_init(&default_mempool, addr, size);
        return ret;
}

static inline void  memory_exit(void)
{
        return;
}

static inline void* memory_alloc(size_t size)
{
        return m_malloc_best_fit(&default_mempool, size);
}

static inline void  memory_free(void* ptr)
{
        return m_free(&default_mempool, ptr);
}
#endif

#if (ALLOCATOR == BASIC_ALOG_WORST_FIT)
#include "basic_alog.h"

static inline uint32_t memory_init(uint64_t addr, uint32_t size)
{
        uint32_t ret =  memp_init(&default_mempool, addr, size);
        return ret;
}

static inline void  memory_exit(void)
{
        return;
}

static inline void* memory_alloc(size_t size)
{
        return m_malloc_worst_fit(&default_mempool, size);
}

static inline void  memory_free(void* ptr)
{
        return m_free(&default_mempool, ptr);
}
#endif


#if (ALLOCATOR == MEMORY_POOL)
#include "mempool.h"

static inline uint32_t memory_init(uint64_t addr, uint32_t size)
{
        uint32_t ret =  pool_memp_init(&default_pool_mempool, addr, size);
        return ret;
}

static inline void  memory_exit(void)
{
        return;
}

static inline void* memory_alloc(size_t size)
{
        return pool_m_malloc(&default_pool_mempool, size);
}

static inline void  memory_free(void* ptr)
{
        return pool_m_free(&default_pool_mempool, ptr);
}
#endif




#endif
