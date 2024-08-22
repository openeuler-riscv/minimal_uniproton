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
 * Description: 内存基本功能对外头文件。
 */
#ifndef PRT_MEM_H
#define PRT_MEM_H

#include <stdint.h>
#include <stddef.h>
#include <errno.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*
 * 申请的内存地址对齐标准:4/8字节对齐。
 */
#define OS_MEM_ADDR_ALIGN sizeof(uintptr_t)

/*
 * 内存算法类型。
 */
enum MemArith {
    MEM_ARITH_FSC,          // 私有FSC算法
    MEM_ARITH_BUTT          // 内存算法非法
};

/*
 * 内存对齐方式。
 */
enum MemAlign {
    MEM_ADDR_ALIGN_004 = 2, /* 4字节对齐 */
    MEM_ADDR_ALIGN_008 = 3, /* 8字节对齐 */
    MEM_ADDR_ALIGN_016 = 4, /* 16字节对齐 */
    MEM_ADDR_ALIGN_032 = 5, /* 32字节对齐 */
    MEM_ADDR_ALIGN_064 = 6, /* 64字节对齐 */
    MEM_ADDR_ALIGN_128 = 7, /* 128字节对齐 */
    MEM_ADDR_ALIGN_256 = 8, /* 256字节对齐 */
    MEM_ADDR_ALIGN_512 = 9, /* 512字节对齐 */
    MEM_ADDR_ALIGN_1K = 10, /* 1K字节对齐 */
    MEM_ADDR_ALIGN_2K = 11, /* 2K字节对齐 */
    MEM_ADDR_ALIGN_4K = 12, /* 4K字节对齐 */
    MEM_ADDR_ALIGN_8K = 13, /* 8K字节对齐 */
    MEM_ADDR_ALIGN_16K = 14, /* 16K字节对齐 */
    MEM_ADDR_BUTT /* 字节对齐非法 */
};

/*
 * @brief 申请内存
 *
 * @par 描述
 * <li>申请大小为size的内存。</li>
 * @attention
 * <ul>
 * <li>调用函数后，注意判断返回的地址是否为空以避免后续访问空指针。</li>
 * <li>对于FSC内存算法，申请到的内存首地址是按4字节对齐的 </li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将以错误码的形式返回。</li>
 * </ul>
 *
 * @param size [IN]  类型#uint32_t，申请的大小。
 *
 * @retval #NULL  0，申请失败。
 * @retval #!NULL 内存首地址值。
 * @par 依赖
 * <ul><li>prt_mem.h：该接口声明所在的头文件。</li></ul>
 * @see PRT_MemFree
 */
extern void *PRT_MemAlloc(uint32_t size);

/*
 * @brief 申请指定大小且指定对齐方式的内存块。
 *
 * @par 描述
 * <li>申请大小为size的内存，对齐方式为alignPow。</li>
 * @attention
 * <ul>
 * <li>调用函数后，注意判断返回的地址是否为空以避免后续访问空指针。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误码。</li>
 * </ul>
 *
 * @param size     [IN]  类型#uint32_t，申请的大小。
 * @param alignPow [IN]  类型#enum MemAlign，动态对齐。
 *
 * @retval #NULL  0，申请失败。
 * @retval #!NULL 内存首地址值。
 * @par 依赖
 * <ul><li>prt_mem.h：该接口声明所在的头文件。</li></ul>
 * @see PRT_MemFree
 */
extern void *PRT_MemAllocAlign(uint32_t size, enum MemAlign alignPow);

/*
 * @brief 释放申请的内存。
 *
 * @par 描述
 * 该接口根据内存块的地址addr，找到该内存块所属的内存分区，用户使用的地址addr释放该内存。
 * @attention
 * <ul>
 * <li>如果返回值不是(0)，则内存不会被释放。</li>
 * <li>被破坏的内存不能被释放。</li>
 * <li>对于入参addr，OS已做基本校验，无法校验所有非法地址，其合法性由业务保证。</li>
 * </ul>
 *
 * @param addr [IN]  类型#void *，释放的地址。
 *
 * @retval  0x00000000，内存释放成功。
 * @retval #其它值，释放失败。
 * @par 依赖
 * <ul><li>prt_mem.h：该接口声明所在的头文件。</li></ul>
 * @see PRT_MemAlloc
 */
extern uint32_t PRT_MemFree(void *addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* PRT_MEM_H */
