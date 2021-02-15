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

#ifndef LIBUTILS_REFCOUNT_H
#define LIBUTILS_REFCOUNT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *RCHandle;

#define RCHANDLE_INVALID NULL

/**
 * @brief 创建一个引用计数对象。
 *
 * @param objSize       对象的字节大小
 * @param allocator     对象的分配器
 * @return RCHandle     成功则返回对象句柄，否则返回无效句柄RCHANDLE_INVALID
 */
RCHandle RefCountCreate(size_t objSize, IAllocator allocator);

/**
 * @brief 引用对象，调用该接口会增加对象的引用计数。
 *
 * @param handle        引用对象句柄
 * @return RCHandle     新创建的引用对象，和handle指向同一个对象
 */
RCHandle RefCountRef(RCHandle handle);

/**
 * @brief 解引用对象，调用该接口会减少对象的引用计数。如果引用计数为1，则删除对象
 *
 * @param handle        引用对象句柄，如果对象被删除，句柄被置无效
 */
void RefCountUnref(RCHandle *handle);

/**
 * @brief 获取引用计数对象的引用计数
 *
 * @param handle        引用对象句柄
 * @return uint32_t     对象当前的引用计数
 */
uint32_t RefCountGetCount(RCHandle handle);

/**
 * @brief 获取引用对象的数据指针
 *
 * @param handle        引用对象句柄
 * @return void*        引用对象的数据指针，指向用户数据
 */
void *RefCountGetData(RCHandle handle);

/**
 * @brief 判断引用对象句柄是否有效
 *
 * @param handle        引用对象句柄
 * @return true         对象有效
 * @return false        对象无效
 */
bool RefCountIsValid(RCHandle handle);

#ifdef __cplusplus
}
#endif

#endif
