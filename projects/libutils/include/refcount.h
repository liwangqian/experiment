#ifndef LIBUTILS_REFCOUNT_H
#define LIBUTILS_REFCOUNT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALLOCATOR_REQUIRE_MEMORY,
    ALLOCATOR_RELEASE_MEMORY,
    ALLOCATOR_OBJ_CONSTRUCTOR,
    ALLOCATOR_OBJ_DESTRUCTOR
} RCAllocatorOpType;

typedef void(*RCAllocatorAdapter)(RCAllocatorOpType type, size_t memSize, void **memPtr);
typedef void *RCHandle;

#define RCHANDLE_INVALID NULL

RCHandle RefCountCreate(size_t objSize, RCAllocatorAdapter allocator);

RCHandle RefCountRef(RCHandle handle);
void RefCountUnref(RCHandle *handle);

uint32_t RefCountGetCount(RCHandle handle);
void *RefCountGetData(RCHandle handle);
bool RefCountIsValid(RCHandle handle);

#ifdef __cplusplus
}
#endif

#endif
