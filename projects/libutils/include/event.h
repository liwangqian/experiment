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

#ifndef LIBUTILS_EVENT_H
#define LIBUTILS_EVENT_H

#include <stdint.h>
#include <stdbool.h>
#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 事件观察者类，实现事件的订阅和发布解耦
*/
typedef struct EventObserver EventObserver;
typedef struct EventListener EventListener;

/**
 * 事件回调接口
 * @param event             事件类型
 * @param eventData         事件数据，可以为空
 * @param subscriberDate    订阅者订阅事件时提供的数据指针
 * @return 返回true则删除事件订阅，否则保留事件订阅
*/
typedef bool(*EventCallback)(uint32_t event, void *eventData, void *listenerData);

uint32_t EventAppendListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

uint32_t EventAppendOnceListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

uint32_t EventPrependListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

uint32_t EventPrependOnceListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

uint32_t EventRemoveListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

uint32_t EventRemoveAllListeners(EventObserver *observer, uint32_t event);

uint32_t EventListenerCount(EventObserver *observer, uint32_t event);

void EventEmit(EventObserver *observer, uint32_t event, void *eventData);

uint32_t EventOn(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

uint32_t EventOff(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

uint32_t EventOnce(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * 创建并初始化事件观察者
 * @return 成功则返回事件观察者指针，否则返回空指针
*/
EventObserver *EventCreateObserver(IAllocator allocator);

/**
 * 销毁事件观察者，观察者销毁后，指针置空。
*/
void EventDestroyObserver(EventObserver **observer);

#ifdef __cplusplus
}
#endif
#endif //UTILS_EVENT_H
