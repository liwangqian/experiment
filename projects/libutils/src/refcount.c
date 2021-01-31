#include "refcount.h"
#include <stdatomic.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RC_MAGIC_NUMBER 0x5A5A5A5AU

typedef struct RCHandleT {
    atomic_uint refCount;
    uint32_t magicNumber;
    RCAllocatorAdapter allocator;
    uint8_t data[];
} RCHandleT;

static inline
void DefaultAllocator(RCAllocatorOpType type, size_t memSize, void **memPtr)
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
    default:
        break;
    }
}

static inline
bool IsHandleValid(RCHandleT *handle)
{
    return (handle != RCHANDLE_INVALID) &&
           (handle->magicNumber == RC_MAGIC_NUMBER);
}

RCHandle RefCountCreate(size_t objSize, RCAllocatorAdapter allocator)
{
    const size_t totalMemSize = objSize + sizeof(RCHandleT);
    allocator = allocator == NULL ? DefaultAllocator : allocator;

    RCHandleT *handle = NULL;
    allocator(ALLOCATOR_REQUIRE_MEMORY, totalMemSize, &handle);
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
    RCAllocatorAdapter allocator = tmpHandle->allocator;

    if (atomic_fetch_sub_explicit(&tmpHandle->refCount, 1, memory_order_seq_cst) == 1) {
        void *data = &tmpHandle->data;
        allocator(ALLOCATOR_OBJ_DESTRUCTOR, 0, &data);
        allocator(ALLOCATOR_RELEASE_MEMORY, 0, &tmpHandle);
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
