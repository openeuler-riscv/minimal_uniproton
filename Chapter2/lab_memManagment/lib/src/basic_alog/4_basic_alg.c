#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <basic_alog.h>

#define MEM_PAGE_SIZE 0x1000

mempool_t default_mempool;

void print_memory_pool(mempool_t *mp) {
    memblock_t *current = (memblock_t *)mp->ptr;
    printf("Memory Pool State:\n");
    while (current != NULL) {
        printf("Block at %p, size: %zu, free: %d, next: %p\n",
               (void *)current, current->size, current->free, (void *)current->next);
        current = current->next;
    }
    printf("\n");
}

void *m_malloc_first_fit(mempool_t *mp, size_t size) {
    memblock_t *current = (memblock_t *)mp->ptr;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            current->free = 0;

            // 如果块大小远大于请求大小，则拆分块
            if (current->size > size + sizeof(memblock_t)) {
                memblock_t *new_block = (memblock_t *)((char *)current + sizeof(memblock_t) + size);
                new_block->size = current->size - size - sizeof(memblock_t);
                new_block->free = 1;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            mp->free_count--;
            return (char *)current + sizeof(memblock_t);
        }
        current = current->next;
    }

    return NULL;
}

void *m_malloc_best_fit(mempool_t *mp, size_t size) {
    memblock_t *current = (memblock_t *)mp->ptr;
    memblock_t *best_fit = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
        }
        current = current->next;
    }

    if (best_fit != NULL) {
        best_fit->free = 0;

        // 如果块大小远大于请求大小，则拆分块
        if (best_fit->size > size + sizeof(memblock_t)) {
            memblock_t *new_block = (memblock_t *)((char *)best_fit + sizeof(memblock_t) + size);
            new_block->size = best_fit->size - size - sizeof(memblock_t);
            new_block->free = 1;
            new_block->next = best_fit->next;

            best_fit->size = size;
            best_fit->next = new_block;
        }

        mp->free_count--;
        return (char *)best_fit + sizeof(memblock_t);
    }

    return NULL;
}

void *m_malloc_worst_fit(mempool_t *mp, size_t size) {
    memblock_t *current = (memblock_t *)mp->ptr;
    memblock_t *worst_fit = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (worst_fit == NULL || current->size > worst_fit->size) {
                worst_fit = current;
            }
        }
        current = current->next;
    }

    if (worst_fit != NULL) {
        worst_fit->free = 0;

        // 如果块大小远大于请求大小，则拆分块
        if (worst_fit->size > size + sizeof(memblock_t)) {
            memblock_t *new_block = (memblock_t *)((char *)worst_fit + sizeof(memblock_t) + size);
            new_block->size = worst_fit->size - size - sizeof(memblock_t);
            new_block->free = 1;
            new_block->next = worst_fit->next;

            worst_fit->size = size;
            worst_fit->next = new_block;
        }

        mp->free_count--;
        return (char *)worst_fit + sizeof(memblock_t);
    }

    return NULL;
}

void m_free(mempool_t *mp, void *ptr) {
    if (!mp || !ptr) return;

    memblock_t *block = (memblock_t *)((char *)ptr - sizeof(memblock_t));
    block->free = 1;

    // 合并相邻的空闲块
    memblock_t *current = (memblock_t *)mp->ptr;
    while (current != NULL && current->next != NULL) {
        if (current->free && current->next->free) {
            current->size += sizeof(memblock_t) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }

    mp->free_count++;
    //printf("Block freed at %p\n", ptr);
}

int memp_init(mempool_t *mp, uint64_t addr, size_t block_size) {
    if (!mp) return -1;
    memset(mp, 0, sizeof(mempool_t));
    mp->block_size = block_size;
    mp->free_count = 1; // 初始时只有一个大块
    mp->mem = (void*)addr;
    if (!mp->mem) return -1;
    mp->ptr = mp->mem;

    memblock_t *initial_block = (memblock_t *)mp->ptr;
    initial_block->size = MEM_PAGE_SIZE - sizeof(memblock_t);
    initial_block->free = 1;
    initial_block->next = NULL;

    return 0;
}
