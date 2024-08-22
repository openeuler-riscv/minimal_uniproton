#ifndef __BASIC_ALOG_H
#define __BASIC_ALOG_H


typedef struct memblock_s {
    size_t size;            // 当前块的大小
    int free;               // 是否空闲
    struct memblock_s *next; // 下一个块
} memblock_t;

typedef struct mempool_s {
    int block_size; // 每个块的大小
    int free_count; // 剩余块的数量
    void *mem;      // 内存池起始地址
    void *ptr;      // 指向第一个空闲块
} mempool_t;

extern mempool_t default_mempool;

int memp_init(mempool_t *mp, uint64_t addr, size_t block_size);
void *m_malloc_first_fit(mempool_t *mp, size_t size);
void *m_malloc_best_fit(mempool_t *mp, size_t size);
void *m_malloc_worst_fit(mempool_t *mp, size_t size);
void m_free(mempool_t *mp, void *ptr);

#endif
