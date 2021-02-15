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

#include "refcount.h"
#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RC_MAGIC_NUMBER 0x5A5A5A5AU

typedef struct RCHandleT {
    atomic_uint refCount;
    uint32_t magicNumber;
    IAllocator allocator;
    uint8_t data[];
} RCHandleT;

static inline
bool IsHandleValid(RCHandleT *handle)
{
    return (handle != RCHANDLE_INVALID) &&
           (handle->magicNumber == RC_MAGIC_NUMBER);
}

RCHandle RefCountCreate(size_t objSize, IAllocator allocator)
{
    const size_t totalMemSize = objSize + sizeof(RCHandleT);
    allocator = allocator == NULL ? DefaultAllocator : allocator;

    RCHandleT *handle = NULL;
    allocator(ALLOCATOR_REQUIRE_MEMORY, totalMemSize, (void**)&handle);
    if (handle == NULL) {
        return RCHANDLE_INVALID;
    }

    atomic_init(&handle->refCount, 1);
    handle->magicNumber = RC_MAGIC_NUMBER;
    handle->allocator = allocator;

    void *tmp = &handle->data;
    allocator(ALLOCATOR_OBJ_CONSTRUCTOR, 0, &tmp);

    return handle;
}

RCHandle RefCountRef(RCHandle handle)
{
    if (!IsHandleValid(handle)) {
        return RCHANDLE_INVALID;
    }

    RCHandleT *tmpHandle = handle;
    atomic_fetch_add_explicit(&tmpHandle->refCount, 1, memory_order_acq_rel);
    return tmpHandle;
}

void RefCountUnref(RCHandle *handle)
{
    if (handle == NULL || !IsHandleValid(*handle)) {
        return;
    }

    RCHandleT *tmpHandle = *handle;
    IAllocator allocator = tmpHandle->allocator;

    if (atomic_fetch_sub_explicit(&tmpHandle->refCount, 1, memory_order_seq_cst) == 1) {
        void *data = &tmpHandle->data;
        allocator(ALLOCATOR_OBJ_DESTRUCTOR, 0, &data);
        allocator(ALLOCATOR_RELEASE_MEMORY, 0, (void**)&tmpHandle);
    }

    *handle = NULL;
}

uint32_t RefCountGetCount(RCHandle handle)
{
    if (!IsHandleValid(handle)) {
        return 0;
    }

    RCHandleT *tmpHandle = handle;
    return tmpHandle->refCount;
}

void *RefCountGetData(RCHandle handle)
{
    if (!IsHandleValid(handle)) {
        return NULL;
    }

    RCHandleT *tmpHandle = handle;
    return &tmpHandle->data;
}

bool RefCountIsValid(RCHandle handle)
{
    return IsHandleValid(handle);
}

#ifdef __cplusplus
}
#endif
