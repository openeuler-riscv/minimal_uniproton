#ifndef __MEMORY_POOL_H
#define __MEMORY_POOL_H

#include <stdint.h>

typedef struct pool_mempool_s {
    int block_size; // 32 最小单位
    int free_count; // 128 大的内存块分配的最小单位的数量
    void *mem;      // 内存块开始的位置
    void *ptr;      // 下一次分配的位置
} pool_mempool_t;

extern pool_mempool_t default_pool_mempool;

int pool_memp_init(pool_mempool_t *mp, uint64_t addr, size_t block_size);
void *pool_m_malloc(pool_mempool_t *mp, size_t size);
void pool_m_free(pool_mempool_t *mp, void *ptr);


#endif
