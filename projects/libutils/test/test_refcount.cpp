/*
 * Copyright (c) 2021, LiWangQian. All rights reserved.
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

#include <catch2/catch.hpp>
#include "refcount.h"
#include <iostream>

struct TestObject {
    uint32_t id;
};

static inline
void TestObjectAllocator(AllocatorOpType type, size_t memSize, void **memPtr)
{
    if (memPtr == nullptr) {
        return;
    }

    switch (type) {
        case ALLOCATOR_REQUIRE_MEMORY:
            *memPtr = malloc(memSize);
            break;
        case ALLOCATOR_RELEASE_MEMORY:
            free(*memPtr);
            *memPtr = nullptr;
            break;
        case ALLOCATOR_OBJ_CONSTRUCTOR: {
            auto obj = static_cast<TestObject *>(*memPtr);
            obj->id = 123;
        }
        default:
            break;
    }
}

TEST_CASE("Test RefCount Methods")
{
    RCHandle rcHandle = RefCountCreate(sizeof(TestObject), TestObjectAllocator);
    REQUIRE(RefCountIsValid(rcHandle));
    REQUIRE(RefCountGetCount(rcHandle) == 1);

    REQUIRE(RefCountIsValid(rcHandle));
    auto count = RefCountGetCount(rcHandle);
    auto newRcHandle = RefCountRef(rcHandle);
    REQUIRE(RefCountGetCount(newRcHandle) == (count + 1));
    RefCountUnref(&newRcHandle);
    REQUIRE(!RefCountIsValid(newRcHandle));

    auto obj = static_cast<TestObject *>(RefCountGetData(rcHandle));
    REQUIRE(obj != nullptr);
    REQUIRE(obj->id == 123);

    REQUIRE(RefCountIsValid(rcHandle));
    RefCountUnref(&rcHandle);
    REQUIRE(!RefCountIsValid(rcHandle));
}
