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
 * Description: 内存基本功能的C文件。
 */
#include <prt_mem.h>
#include "prt_mem_internal.h"
#include <stdint.h>
#include <stddef.h>
#include <errno.h>

void *PRT_MemAlloc(uint32_t size)
{
    void *addr;
    //如果你在并发编程，请在这里上锁
    addr = g_memArithAPI.alloc(size);
    //解锁
    return addr;
}

void *PRT_MemAllocAlign(uint32_t size, enum MemAlign alignPow)
{
    void *addr;
    //如果你在并发编程，请在这里上锁
    addr = g_memArithAPI.allocAlign(size, alignPow);
    //解锁
    return addr;
}

uint32_t PRT_MemFree(void *addr)
{
    uint32_t ret;
    //如果你在并发编程，请在这里上锁
    ret = g_memArithAPI.free(addr);
    //解锁
    return ret;
}
