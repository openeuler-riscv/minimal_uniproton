/*
 * Copyright (c) 2009-2023 Huawei Technologies Co., Ltd. All rights reserved.
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
 * Description: Fsc 内存实现
 */
#include "prt_fscmem_internal.h"
#include <string.h>
#define ALIGN(addr, boundary) (((uintptr_t)(addr) + (boundary) - 1) & ~((uintptr_t)(boundary) - 1))
/* 判断初始化内存地址和大小是否为4字节对齐 */
#define OS_MEM_GETBIT(addr) (addr & (uintptr_t)(sizeof(uintptr_t) - 1))

struct TagMemFuncLib g_memArithAPI; /* 算法对应API */
struct TagFscMemCtrl g_fscMemNodeList[OS_FSC_MEM_LAST_IDX];
uint32_t g_fscMemBitMap = 1;
uintptr_t g_memTotalSize = 0;
uintptr_t g_memUsage = 0;
uintptr_t g_memPeakUsage = 0;
uintptr_t g_memStartAddr = 0;

struct TagFscMemCtrl *OsFscMemSearch(uint32_t size, uint32_t *idx)
{
    uint32_t staIdx;
    struct TagFscMemCtrl *currBlk = NULL;
    struct TagFscMemCtrl *headBlk = NULL;
    
    staIdx = OS_FSC_MEM_SZ2IDX(size);
    *idx = staIdx + 1;

    while (1) {
        *idx = OsGetLmb1((g_fscMemBitMap << *idx) >> *idx);
        if (OS_FSC_MEM_LAST_IDX <= *idx) {
            *idx = staIdx;

            headBlk = &g_fscMemNodeList[*idx];
            currBlk = headBlk->next;

            /* 空闲链表非空 */
            while (currBlk != headBlk) {
                /* 找到可用的内存块 */
                if (OS_FSC_MEM_SZGET(currBlk) >= size) {
                    return currBlk;
                }

                currBlk = currBlk->next;
            }

            return NULL;
        }

        headBlk = &g_fscMemNodeList[*idx];
        /* 空闲链表为空，清除BitMap标志位 */
        if (headBlk->next == headBlk) {
            g_fscMemBitMap &= ~(OS_FSC_MEM_IDX2BIT(*idx));
        } else {
            break;
        }
    }
    currBlk = headBlk->next;

    return currBlk;
}

void *OsFscMemAllocInner(uint32_t size, uintptr_t align)
{
    uint32_t idx;
    uint32_t allocSize;
    uint32_t *blkTailMagic = NULL;
    uintptr_t usrAddr;
    struct TagFscMemCtrl *plotBlk = NULL;
    struct TagFscMemCtrl *currBlk = NULL;
    struct TagFscMemCtrl *nextBlk = NULL;

    if (size == 0) {
        return NULL;
    }

    if (align < sizeof(uintptr_t)) {
        align = sizeof(uintptr_t);
    }

    /* 由于已经按OS_FSC_MEM_SIZE_ALIGN字节对齐，最大可能补齐的大小是align - OS_FSC_MEM_SIZE_ALIGN */
    allocSize = ALIGN(size, OS_FSC_MEM_SIZE_ALIGN) + (align - OS_FSC_MEM_SIZE_ALIGN) +
        OS_FSC_MEM_USED_HEAD_SIZE + OS_FSC_MEM_TAIL_SIZE;
    if ((allocSize < size) || allocSize >= ((OS_FSC_MEM_MAXVAL - OS_FSC_MEM_USED_HEAD_SIZE) - OS_FSC_MEM_TAIL_SIZE)) {
        return NULL;
    }

    currBlk = OsFscMemSearch(allocSize, &idx);
    if (currBlk == NULL) {
        return NULL;
    }

    /* 找到足够空间的空闲链表，并对其进行分割 */
    if (OS_FSC_MEM_SZGET(currBlk) >= (allocSize + OS_FSC_MEM_MIN_SIZE)) {
        currBlk->size -= allocSize;

        /* 调整链表 */
        if (idx != OS_FSC_MEM_SZ2IDX(currBlk->size)) {
            OsFscMemDelete(currBlk);
            OsFscMemInsert(currBlk, g_fscMemNodeList, &g_fscMemBitMap);
        }

        plotBlk = (struct TagFscMemCtrl *)((uintptr_t)currBlk + (uintptr_t)currBlk->size);
        plotBlk->prevSize = currBlk->size;
        plotBlk->size = allocSize;

        currBlk = plotBlk;
    } else {
        OsFscMemDelete(currBlk);
    }

    nextBlk = (struct TagFscMemCtrl *)((uintptr_t)currBlk + (uintptr_t)currBlk->size);
    nextBlk->prevSize = 0;
    currBlk->next = OS_FSC_MEM_MAGIC_USED;

    /* 设置内存越界检查魔术字 */
    blkTailMagic = (uint32_t *)((uintptr_t)currBlk + (uintptr_t)currBlk->size - (uintptr_t)OS_FSC_MEM_TAIL_SIZE);
    *blkTailMagic = OS_FSC_MEM_TAIL_MAGIC;

    // currBlk->prev 复用为内存对齐的偏移地址
    currBlk->prev = 0;
    usrAddr = (((uintptr_t)currBlk + OS_FSC_MEM_SLICE_HEAD_SIZE + align - 1) & ~(align - 1));
    OsMemSetHeadAddr(usrAddr, ((uintptr_t)currBlk + OS_FSC_MEM_SLICE_HEAD_SIZE));
    g_memUsage += currBlk->size;
    if (g_memPeakUsage < g_memUsage) {
        g_memPeakUsage = g_memUsage;
    }
    return (void *)usrAddr;
}

uint32_t OsFscMemFree(void *addr)
{
    struct TagFscMemCtrl *prevBlk = NULL; /* 前一内存块指针 */
    struct TagFscMemCtrl *currBlk = NULL; /* 当前内存块指针 */
    struct TagFscMemCtrl *nextBlk = NULL; /* 后一内存块指针 */
    uint32_t *blkTailMagic = NULL;
    uintptr_t blkSize;

    if (addr == NULL) {
        return -EINVAL;
    }

    currBlk = (struct TagFscMemCtrl *)OsMemGetHeadAddr((uintptr_t)addr);
    blkSize = currBlk->size;

    if ((currBlk->next != OS_FSC_MEM_MAGIC_USED) || (currBlk->size == 0)) {
        return -EFAULT;
    }

    blkTailMagic = (uint32_t *)((uintptr_t)currBlk + blkSize - (uintptr_t)OS_FSC_MEM_TAIL_SIZE);
    if (*blkTailMagic != OS_FSC_MEM_TAIL_MAGIC) {
        return -EFAULT;
    }

    nextBlk = (struct TagFscMemCtrl *)((uintptr_t)currBlk + blkSize);

    /* 后一内存块未使用，当前模块释放后与其合并 */
    if (nextBlk->next != OS_FSC_MEM_MAGIC_USED) {
        OsFscMemDelete(nextBlk);

        currBlk->size += nextBlk->size;
	
	memset(nextBlk, 0, sizeof(struct TagFscMemCtrl));
    }

    /* 前一内存块未使用，当前内存模块与其合并 */
    if (currBlk->prevSize != 0) {
        prevBlk = (struct TagFscMemCtrl *)((uintptr_t)currBlk - (uintptr_t)currBlk->prevSize);
        prevBlk->size += currBlk->size;

        OsFscMemDelete(prevBlk);
	
	memset(currBlk, 0, sizeof(struct TagFscMemCtrl));
        currBlk = prevBlk;
    }

    /* 合并后的总内存块插入链表 */
    OsFscMemInsert(currBlk, g_fscMemNodeList, &g_fscMemBitMap);

    nextBlk = (struct TagFscMemCtrl *)((uintptr_t)currBlk + (uintptr_t)currBlk->size);
    nextBlk->prevSize = currBlk->size;
    g_memUsage -= blkSize;

    return 0;
}

void *OsMemAlloc(uint32_t size)
{
    return OsFscMemAllocInner(size, OS_FSC_MEM_SIZE_ALIGN);
}

void *OsMemAllocAlign(uint32_t size, enum MemAlign alignPow)
{
    if (alignPow >= MEM_ADDR_BUTT || alignPow < MEM_ADDR_ALIGN_004) {
        return NULL;
    }
    return OsFscMemAllocInner(size, (1U << (uint32_t)alignPow));
}

/*
 * 描述：初始化内存
 */
uint32_t OsFscMemInit(uintptr_t addr, uint32_t size)
{
    uint32_t idx;
    struct TagFscMemCtrl *headBlk = NULL;
    struct TagFscMemCtrl *currBlk = NULL;
    struct TagFscMemCtrl *nextBlk = NULL;

    /* 异常判断 */
    if ((void *)(uintptr_t)addr == NULL) {
        return -EINVAL;
    }

    if (OS_MEM_GETBIT(addr) != 0U) {
        return -EINVAL;
    }

    if (OS_MEM_GETBIT(size) != 0U) {
        return -EINVAL;
    }

    if (size < OS_FSC_MEM_USED_HEAD_SIZE) {
        return -EINVAL;
    }

    if (size > OS_FSC_MEM_MAXVAL) {
        return -EINVAL;
    }

    memset((void *)(uintptr_t)addr, 0, size);

    /* 链表初始化，指向自己 */
    headBlk = &g_fscMemNodeList[0];
    for (idx = 0; idx < OS_FSC_MEM_LAST_IDX; idx++, headBlk++) {
        headBlk->prev = headBlk;
        headBlk->next = headBlk;
    }

    size -= OS_FSC_MEM_USED_HEAD_SIZE;

    g_fscMemBitMap |= 1U << (31 - OS_FSC_MEM_LAST_IDX);

    /* 获取索引号 */
    idx = OS_FSC_MEM_SZ2IDX(size);
    g_fscMemBitMap |= OS_FSC_MEM_IDX2BIT(idx);

    /* 挂载链表初始化 */
    headBlk = &g_fscMemNodeList[idx];
    currBlk = (struct TagFscMemCtrl *)(uintptr_t)addr;
    currBlk->next = headBlk;
    currBlk->prevSize = 0;
    currBlk->size = size;
    currBlk->prev = headBlk;
    headBlk->next = currBlk;
    headBlk->prev = currBlk;

    g_memTotalSize = (uintptr_t)size;
    g_memStartAddr = addr;

    nextBlk = (struct TagFscMemCtrl *)((uintptr_t)currBlk + (uintptr_t)currBlk->size);
    nextBlk->next = OS_FSC_MEM_MAGIC_USED;
    nextBlk->size = 0;

    g_memArithAPI.alloc = OsMemAlloc;
    g_memArithAPI.allocAlign = OsMemAllocAlign;
    g_memArithAPI.free = OsFscMemFree;

    return 0;
}
