/*
 * Copyright (c) 2021, LiWangQian<liwangqian@huawei.com> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LIBUTILS_ALLOCATOR_H
#define LIBUTILS_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum AllocatorOpType {
    ALLOCATOR_REQUIRE_MEMORY,  /* 申请对象内存 */
    ALLOCATOR_RELEASE_MEMORY,  /* 释放对象内存 */
    ALLOCATOR_OBJ_CONSTRUCTOR, /* 对象构造，发生在申请内存成功后 */
    ALLOCATOR_OBJ_DESTRUCTOR   /* 对象析构，发生在释放内存前 */
} AllocatorOpType;

/**
 * @brief 资源分配器接口.
 *
 * @param type      资源分配类型
 * @param memSize   内存大小
 * @param memPtr    内存指针，对于申请内存，保存申请的内存地址返回，对于其他场景，保存对象指针
 */
typedef void(*IAllocator)(AllocatorOpType type, size_t memSize, void **memPtr);

/**
 * @brief 资源分配器的默认实现，仅实现内存的申请和释放，不处理对象的构造和析构.
 *
 * @param type      资源分配类型
 * @param memSize   内存大小
 * @param memPtr    内存指针，对于申请内存操作，保存申请的内存地址返回；
 *                  对于释放内存操作，携带内存地址
 */
void DefaultAllocator(AllocatorOpType type, size_t memSize, void **memPtr);

#ifdef __cplusplus
}
#endif
#endif
