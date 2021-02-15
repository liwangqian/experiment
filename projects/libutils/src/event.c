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

#include "event.h"
#include <string.h>
#include <threads.h>

#ifdef __cplusplus
extern "C" {
#endif

struct EventListener {
    EventListener *next;
    EventCallback callback;
    void *listenerData;
    bool once;
};

typedef struct EventList {
    struct EventList *next;

    /* 作为头节点，不定义成指针，便于链表遍历操作 */
    EventListener listener;

    /* 单链表尾部指针，便于快速从尾部添加节点 */
    EventListener *tail;

    /* 互斥锁，支持多线程 */
    mtx_t lock;

    /* 队列订阅的事件类型 */
    uint32_t event;

    /* 队列订阅事件的listener数量 */
    uint32_t count;
} EventList;

struct EventObserver {
    IAllocator allocator;

    /* 作为头节点，不定义成指针，便于链表遍历操作 */
    EventList events;
    mtx_t lock;
};

static inline
EventListener *CreateListener(EventCallback callback, void *listenerData,
    bool once, IAllocator allocator)
{
    EventListener *listener = NULL;
    allocator(ALLOCATOR_REQUIRE_MEMORY, sizeof(EventListener), (void**)&listener);
    if (listener == NULL) {
        return NULL;
    }

    listener->next = NULL;
    listener->callback = callback;
    listener->listenerData = listenerData;
    listener->once = once;

    return listener;
}

static inline
void DestroyListener(EventListener **listener, IAllocator allocator)
{
    allocator(ALLOCATOR_RELEASE_MEMORY, 0, (void**)listener);
    *listener = NULL;
}

static inline
EventList *CreateEventList(uint32_t event, IAllocator allocator)
{
    EventList *list = NULL;
    allocator(ALLOCATOR_REQUIRE_MEMORY, sizeof(EventList), (void**)&list);
    if (list == NULL) {
        return NULL;
    }

    if (mtx_init(&list->lock, mtx_plain) != thrd_success) {
        allocator(ALLOCATOR_RELEASE_MEMORY, 0, (void**)&list);
        return NULL;
    }

    list->next = NULL;
    (void)memset(&list->listener, 0, sizeof(list->listener));
    list->tail = &list->listener;
    list->event = event;
    list->count = 0;

    return list;
}

static inline
void RemoveAllListeners(EventList *list, IAllocator allocator)
{
    EventListener *listener = list->listener.next;
    list->listener.next = NULL;
    list->tail = &list->listener;
    list->count = 0;

    while (listener) {
        EventListener *curr = listener;
        listener = listener->next;
        DestroyListener(&curr, allocator);
    }
}

static inline
void DestroyEventList(EventList *list, IAllocator allocator)
{
    RemoveAllListeners(list, allocator);
    mtx_destroy(&list->lock);
    allocator(ALLOCATOR_RELEASE_MEMORY, 0, (void**)&list);
}

static inline
void DestroyAllEventList(EventList *list, IAllocator allocator)
{
    while (list) {
        EventList *node = list;
        list = list->next;
        DestroyEventList(node, allocator);
    }
}

static inline
EventList *PrependEventList(EventObserver *observer, uint32_t event)
{
    EventList *list = CreateEventList(event, observer->allocator);
    if (list == NULL) {
        return NULL;
    }

    list->next = observer->events.next;
    observer->events.next = list;
    return list;
}

static inline
EventList *FindEventList(EventObserver *observer, uint32_t event)
{
    EventList *list = observer->events.next;
    while (list) {
        if (list->event == event) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

static inline
EventList *RemoveEventList(EventObserver *observer, uint32_t event)
{
    EventList *prev = &observer->events;
    EventList *curr = prev->next;
    while (curr) {
        if (curr->event == event) {
            prev->next = curr->next;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

static inline
void AppendListener(EventList *list, EventListener *listener)
{
    list->tail->next = listener;
    list->tail = listener;
}

static inline
void PrependListener(EventList *list, EventListener *listener)
{
    listener->next = list->listener.next;
    list->listener.next = listener;
}

static inline
void AddListener(EventObserver *observer, uint32_t event,
    EventListener *listener, bool prepend)
{
    if (mtx_lock(&observer->lock) == thrd_success) {
        EventList *list = FindEventList(observer, event);
        if (list == NULL) {
            list = PrependEventList(observer, event);
        }

        prepend ? PrependListener(list, listener)
                : AppendListener(list, listener);

        ++list->count;

        mtx_unlock(&observer->lock);
    }
}

static inline
EventListener *RemoveListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    EventList *list = FindEventList(observer, event);
    if (list == NULL) {
        return NULL;
    }

    EventListener *listener = &list->listener;
    while (listener->next) {
        EventListener *prev = listener;
        listener = listener->next;

        if (listener->callback == callback &&
            listener->listenerData == listenerData) {
            prev->next = listener->next;
            --list->count;

            /* 如果移除的是尾节点，则更新尾指针 */
            if (listener == list->tail) {
                list->tail = prev;
            }

            return listener;
        }
    }
    return NULL;
}

static inline
void NotifyEventSync(EventList *list, void *eventData, IAllocator allocator)
{
    EventListener *prev = &list->listener;
    EventListener *listener = list->listener.next;
    while (listener) {
        bool remove = listener->callback(list->event, eventData, listener->listenerData);
        if (listener->once || remove) {
            prev->next = listener->next;
            --list->count;

            if (list->tail == listener) {
                list->tail = prev;
            }

            DestroyListener(&listener, allocator);
            listener = prev;
        }
        listener = listener->next;
    }
}

uint32_t EventAppendListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    if (observer == NULL || callback == NULL) {
        return -1;
    }

    EventListener *listener = CreateListener(callback, listenerData,
        false, observer->allocator);
    if (listener == NULL) {
        return -2;
    }

    AddListener(observer, event, listener, false);
    return 0;
}

uint32_t EventAppendOnceListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    if (observer == NULL || callback == NULL) {
        return -1;
    }

    EventListener *listener = CreateListener(callback, listenerData,
        true, observer->allocator);
    if (listener == NULL) {
        return -2;
    }

    AddListener(observer, event, listener, false);
    return 0;
}

uint32_t EventPrependListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    if (observer == NULL || callback == NULL) {
        return -1;
    }

    EventListener *listener = CreateListener(callback, listenerData,
        false, observer->allocator);
    if (listener == NULL) {
        return -2;
    }

    AddListener(observer, event, listener, true);
    return 0;
}

uint32_t EventPrependOnceListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    if (observer == NULL || callback == NULL) {
        return -1;
    }

    EventListener *listener = CreateListener(callback, listenerData,
        true, observer->allocator);
    if (listener == NULL) {
        return -2;
    }

    AddListener(observer, event, listener, true);
    return 0;
}

uint32_t EventRemoveListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    if (observer == NULL || callback == NULL) {
        return -1;
    }
    
    if (mtx_lock(&observer->lock) == thrd_success) {
        EventListener *listener = RemoveListener(observer, event, callback, listenerData);
        mtx_unlock(&observer->lock);

        if (listener != NULL) {
            DestroyListener(&listener, observer->allocator);
            return 0;
        }
    }

    return -2;
}

uint32_t EventRemoveAllListeners(EventObserver *observer, uint32_t event)
{
    if (observer == NULL) {
        return -1;
    }

    if (mtx_lock(&observer->lock) == thrd_success) {
        EventList *list = RemoveEventList(observer, event);
        mtx_unlock(&observer->lock);

        if (list) {
            DestroyEventList(list, observer->allocator);
        }

        return 0;
    }

    return -2;
}

uint32_t EventListenerCount(EventObserver *observer, uint32_t event)
{
    if (observer == NULL) {
        return 0;
    }

    uint32_t count = 0;

    mtx_lock(&observer->lock);
    EventList *list = FindEventList(observer, event);
    if (list != NULL) {
        count = list->count;
    }
    mtx_unlock(&observer->lock);

    return count;
}


void EventEmit(EventObserver *observer, uint32_t event, void *eventData)
{
    if (observer == NULL) {
        return;
    }

    if (mtx_lock(&observer->lock) == thrd_success) {
        EventList *list = FindEventList(observer, event);
        mtx_unlock(&observer->lock);

        if (list) {
            if (mtx_lock(&list->lock) == thrd_success) {
                NotifyEventSync(list, eventData, observer->allocator);
                mtx_unlock(&list->lock);
            }
        }
    }
}

uint32_t EventOn(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    return EventAppendListener(observer, event, callback, listenerData);
}

uint32_t EventOnce(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    return EventAppendOnceListener(observer, event, callback, listenerData);
}

uint32_t EventOff(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData)
{
    return EventRemoveListener(observer, event, callback, listenerData);
}

EventObserver *EventCreateObserver(IAllocator allocator)
{
    allocator = allocator != NULL ? allocator : DefaultAllocator;
    EventObserver *observer = NULL;
    allocator(ALLOCATOR_REQUIRE_MEMORY, sizeof(EventObserver), (void**)&observer);
    if (observer == NULL) {
        return NULL;
    }

    observer->allocator = allocator;
    (void)memset(&observer->events, 0, sizeof(observer->events));

    if (mtx_init(&observer->lock, mtx_plain) != thrd_success) {
        allocator(ALLOCATOR_RELEASE_MEMORY, 0, (void**)&observer);
        return NULL;
    }

    return observer;
}

void EventDestroyObserver(EventObserver **observer)
{
    if (observer == NULL) {
        return;
    }

    EventObserver *tmp = *observer;

    DestroyAllEventList(tmp->events.next, tmp->allocator);
    mtx_destroy(&tmp->lock);

    tmp->allocator(ALLOCATOR_RELEASE_MEMORY, 0, (void**)observer);
    *observer = NULL;
}


#ifdef __cplusplus
}
#endif
