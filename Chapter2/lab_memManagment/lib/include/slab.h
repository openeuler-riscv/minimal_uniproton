#ifndef __SLAB_H
#define __SLAB_H

typedef struct slab_mempool_s {
    int block_size; // 每个内存块的大小
    int free_count; // 当前空闲块的数量
    void *mem;      // 内存池起始位置
    void *ptr;      // 下一个可分配的空闲块
} slab_mempool_t;

extern slab_mempool_t default_slab_pool;

int slab_memp_init(slab_mempool_t *mp, uint64_t addr, size_t block_size);
void *slab_m_malloc(slab_mempool_t *mp, size_t size);
void slab_m_free(slab_mempool_t *mp, void *ptr);

#endif
