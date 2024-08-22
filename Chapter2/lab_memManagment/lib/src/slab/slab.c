#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <slab.h>
#define MEM_PAGE_SIZE 0x1000

#define SLAB_SIZE 8

slab_mempool_t default_slab_pool;

// 初始化单个内存池
int slab_memp_init(slab_mempool_t *mp, uint64_t addr, size_t block_size) {
    if (!mp) return -1;
    memset(mp, 0, sizeof(slab_mempool_t));
    mp->block_size = block_size;
    mp->free_count = MEM_PAGE_SIZE / block_size;
    mp->mem = (void*)addr;
    if (!mp->mem) return -1;
    mp->ptr = mp->mem;

    char *ptr = (char *)mp->ptr;
    for (int i = 0; i < mp->free_count - 1; i++) {
        *(char **)ptr = ptr + block_size;
        ptr += block_size;
    }
    *(char **)ptr = NULL; // 最后一个块的指针设为NULL

    return 0;
}

// 从指定内存池分配内存块
void *slab_m_malloc(slab_mempool_t *mp, size_t size) {
    if (!mp || mp->free_count == 0 || mp->block_size < size)
        return NULL;
    void *ptr = mp->ptr;
    mp->ptr = *(char **)ptr;
    mp->free_count--;

    return ptr;
}

// 将内存块释放回指定内存池
void slab_m_free(slab_mempool_t *mp, void *ptr) {
    if (!mp || !ptr) return;

    *(char **)ptr = (char*)mp->ptr;
    mp->ptr = ptr;
    mp->free_count++;
}
