//
// Created by busted on 2021/1/31.
//
#include <catch2/catch.hpp>
#include "refcount.h"

struct TestObject {
    uint32_t id;
};

static inline
void TestObjectAllocator(RCAllocatorOpType type, size_t memSize, void **memPtr)
{
    if (memPtr == NULL) {
        return;
    }

    switch (type) {
        case ALLOCATOR_REQUIRE_MEMORY:
            *memPtr = malloc(memSize);
            break;
        case ALLOCATOR_RELEASE_MEMORY:
            free(*memPtr);
            *memPtr = NULL;
            break;
        case ALLOCATOR_OBJ_CONSTRUCTOR: {
            auto obj = static_cast<TestObject *>(*memPtr);
            obj->id = 123;
        }
        default:
            break;
    }
}

TEST_CASE("SUITE: test RefCount")
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
