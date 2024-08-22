#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mempool.h>
#define MEM_PAGE_SIZE 0X1000

#define SLOT_SIZE 16

typedef struct pool_mempool_manager{
    pool_mempool_t *slot[SLOT_SIZE];
};

pool_mempool_t default_pool_mempool;

void *pool_m_malloc(pool_mempool_t *mp, size_t Size) {
    if (!mp || mp->free_count == 0 || mp->block_size < Size)
        return NULL;
    void *ptr = mp->ptr;
    mp->ptr = *(char **)ptr;
    mp->free_count--;

    return ptr;
}

void pool_m_free(pool_mempool_t *mp, void *ptr) {
    if (!mp || !ptr) return;

    *(char **)ptr = (char*)mp->ptr;
    mp->ptr = ptr;
    mp->free_count++;

   // printf("free\n");
}

int pool_memp_init(pool_mempool_t *mp, uint64_t addr, size_t block_size) {
    if (!mp) return -1;
    memset(mp, 0, sizeof(pool_mempool_t));
    mp->block_size = block_size; // 控制大小
    mp->free_count = MEM_PAGE_SIZE / block_size;
    mp->mem = (void*)addr;
    if (!mp->mem) return -1;
    mp->ptr = mp->mem;

    char *ptr = (char *)mp->ptr; // 二级指针
    for (int i = 0; i < mp->free_count - 1; i++) {
        *(char **)ptr = ptr + block_size;
        ptr += block_size;
    }
    *(char **)ptr = NULL; // 最后一个块的指针设为NULL

    return 0;
}
