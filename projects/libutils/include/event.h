/*
 * Copyright (c) 2021, LiWangQian<liwangqian@huawei.com>. All rights reserved.
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

/**
 * @brief 事件回调接口。
 * @param event             事件类型
 * @param eventData         事件数据，可以为空
 * @param subscriberDate    订阅者订阅事件时提供的数据指针
 * @return bool             返回true则删除事件订阅，否则保留事件订阅
*/
typedef bool(*EventCallback)(uint32_t event, void *eventData, void *listenerData);

/**
 * @brief 向事件订阅者队列尾部添加新的订阅。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param callback          事件发生时的回调接口
 * @param listenerData      订阅者数据，在事件发生时传递给callback，该指针由用户保证订阅期间的有效性
 * @return uint32_t         0表示订阅成功，其他值表示订阅失败
 */
uint32_t EventAppendListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * @brief 向事件订阅者队列尾部添加新的一次性订阅，该订阅在事件发生后就会从订阅队列中删除。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param callback          事件发生时的回调接口
 * @param listenerData      订阅者数据，在事件发生时传递给callback，该指针由用户保证订阅期间的有效性
 * @return uint32_t         0表示订阅成功，其他值表示订阅失败
 */
uint32_t EventAppendOnceListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * @brief 向事件订阅者队列头部添加新的订阅。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param callback          事件发生时的回调接口
 * @param listenerData      订阅者数据，在事件发生时传递给callback，该指针由用户保证订阅期间的有效性
 * @return uint32_t         0表示订阅成功，其他值表示订阅失败
 */
uint32_t EventPrependListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * @brief 向事件订阅者队列头部添加新的一次性订阅，该订阅在事件发生后就会从订阅队列中删除。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param callback          事件发生时的回调接口
 * @param listenerData      订阅者数据，在事件发生时传递给callback，该指针由用户保证订阅期间的有效性
 * @return uint32_t         0表示订阅成功，其他值表示订阅失败
 */
uint32_t EventPrependOnceListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * @brief 从事件订阅者队列中删除匹配的订阅。
 *        匹配的条件为callback和listenerData都相等，
 *        如果存在多个匹配的订阅者，只删除队列第一个匹配的订阅者。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param callback          事件发生时的回调接口
 * @param listenerData      订阅者数据，在事件发生时传递给callback，该指针由用户保证订阅期间的有效性
 * @return uint32_t         0表示删除订阅成功，其他值表示删除订阅失败
 */
uint32_t EventRemoveListener(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * @brief 删除事件的所有订阅者。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @return uint32_t         0表示删除成功，其他值表示删除失败
 */
uint32_t EventRemoveAllListeners(EventObserver *observer, uint32_t event);

/**
 * @brief 获取事件的订阅者数量。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @return uint32_t         订阅者数量，如果入参非法或者事件不存在订阅则，则返回0
 */
uint32_t EventListenerCount(EventObserver *observer, uint32_t event);

/**
 * @brief 发送一个事件到事件观察者，事件由观察者通知到所有订阅该事件的订阅者。
 *        这是一个同步操作，阻塞等待所有订阅者都处理玩该事件后才会返回。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param eventData         事件的关联数据
 */
void EventEmit(EventObserver *observer, uint32_t event, void *eventData);

/**
 * @brief 订阅事件，接口行为同EventAppendListener。
 * 
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param callback          事件发生时的回调接口
 * @param listenerData      订阅者数据，在事件发生时传递给callback，该指针由用户保证订阅期间的有效性
 * @return uint32_t         0表示订阅成功，其他值表示订阅失败
 */
uint32_t EventOn(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * @brief 从事件订阅者队列中删除匹配的订阅，接口行为同EventRemoveListener。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param callback          事件发生时的回调接口
 * @param listenerData      订阅者数据，在事件发生时传递给callback，该指针由用户保证订阅期间的有效性
 * @return uint32_t         0表示删除订阅成功，其他值表示删除订阅失败
 */
uint32_t EventOff(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * @brief 向事件订阅者队列尾部添加新的一次性订阅，行为同EventAppendOnceListener。
 *
 * @param observer          事件观察者对象指针
 * @param event             订阅的事件类型
 * @param callback          事件发生时的回调接口
 * @param listenerData      订阅者数据，在事件发生时传递给callback，该指针由用户保证订阅期间的有效性
 * @return uint32_t         0表示订阅成功，其他值表示订阅失败
 */
uint32_t EventOnce(EventObserver *observer, uint32_t event,
    EventCallback callback, void *listenerData);

/**
 * @brief 创建并初始化事件观察者。
 *
 * @param allocator         资源分配器接口
 * @return EventObserver*   成功则返回事件观察者指针，否则返回空指针
 */
EventObserver *EventCreateObserver(IAllocator allocator);

/**
 * @brief 销毁事件观察者，观察者销毁后，指针置空。
 * 
 * @param observer          事件观察者对象指针
 */
void EventDestroyObserver(EventObserver **observer);

#ifdef __cplusplus
}
#endif
#endif /* LIBUTILS_EVENT_H */
