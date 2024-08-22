/*
 * Copyright (c) 2009-2022 Huawei Technologies Co., Ltd. All rights reserved.
 *
 * UniProton is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Create: 2009-12-22
 * Description: SC内存算法模块的模块内头文件。
 */
#ifndef PRT_FSCMEM_INTERNAL_H
#define PRT_FSCMEM_INTERNAL_H

#include "../include/prt_fscmem_external.h"
#include "../prt_mem_internal.h"

/*
 * 模块内宏定义
 */
#define OS_FSC_MEM_PREV_USED 0
#define OS_FSC_MEM_USED_MAGIC OS_MEM_HEAD_MAGICWORD

#define OS_FSC_MEM_SZ2IDX(size) (31 - OsGetLmb1((uint32_t)(size)))
#define OS_FSC_MEM_IDX2BIT(idx) (0x80000000UL >> (idx))

#define OS_FSC_MEM_SIZE_ALIGN OS_MEM_ADDR_ALIGN
#define OS_FSC_MEM_MIN_SIZE (OS_FSC_MEM_SLICE_HEAD_SIZE + OS_FSC_MEM_SIZE_ALIGN)

static inline uint32_t OsGetLmb1(uint32_t value)
{
    int i;
    uint32_t max = 0;

    for (i = 31; i >= 0; i--) {
        if (((1U << (uint32_t)i) & value) != 0) {
            max = (uint32_t)i;
            break;
        }
    }
    return (31U - max);
}


/*
 * 模块内全局变量声明
 */

/*
 * 模块内函数声明
 */
extern void *OsFscMemSplit(uintptr_t size, uintptr_t align,
                           struct TagFscMemCtrl *fscFreeListHead, uint32_t *bitMapPtr);
extern uint32_t OsMemPtParaCheck(uintptr_t addr, uintptr_t size, uintptr_t *ptAddr);

/*
 * 模块内内联函数定义
 */
static inline void OsFscMemDelete(struct TagFscMemCtrl *currBlk)
{
    currBlk->next->prev = currBlk->prev;
    currBlk->prev->next = currBlk->next;
}

static inline void OsFscMemInsert(struct TagFscMemCtrl *currBlk,
                                             struct TagFscMemCtrl *fscFreeList,
                                             uint32_t *bitMapPtr)
{
    uint32_t idx = OS_FSC_MEM_SZ2IDX(currBlk->size);
    struct TagFscMemCtrl *headBlk = &(fscFreeList[idx]);

    *bitMapPtr |= OS_FSC_MEM_IDX2BIT(idx);

    currBlk->prev = headBlk;
    currBlk->next = headBlk->next;
    headBlk->next->prev = currBlk;
    headBlk->next = currBlk;
}

static inline void OsFscMemBlockInit(struct TagFscMemCtrl *memBlk)
{
    /* 后续考虑整块内存初始化为随机值，当前仅清零控制头 */
    memBlk->next = NULL;
    memBlk->size = 0;
    memBlk->prevSize = 0;
    memBlk->prev = NULL;
}

#endif /* PRT_FSCMEM_INTERNAL_H */
