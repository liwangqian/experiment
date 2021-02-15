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

#include <catch2/catch.hpp>
#include "event.h"

static const auto EVENT_SHUT = 1;

static uint32_t calledCnt = 0;
static uint32_t calledSeq[32] = {0};
bool OnEvent1(uint32_t event, void *eventData, void *subscriberData)
{
   calledSeq[1] = calledCnt++;
   return false;
}

bool OnEvent2(uint32_t event, void *eventData, void *subscriberData)
{
   calledSeq[2] = calledCnt++;
   return false;
}

bool OnEvent3(uint32_t event, void *eventData, void *subscriberData)
{
   calledSeq[3] = calledCnt++;
   return false;
}

bool OnEvent4(uint32_t event, void *eventData, void *subscriberData)
{
   calledSeq[4] = calledCnt++;
   return false;
}

bool OnEvent5(uint32_t event, void *eventData, void *subscriberData)
{
   calledSeq[5] = calledCnt++;
   return false;
}

bool OnEvent6(uint32_t event, void *eventData, void *subscriberData)
{
   calledSeq[6] = calledCnt++;
   return false;
}

bool onEventOnceCalled = false;
bool OnEventOnce(uint32_t event, void *eventData, void *subscriberData)
{
    onEventOnceCalled = true;
    return true;
}

TEST_CASE("CASE-01: Test EventObserver Create And Destroy")
{
    EventObserver *observer = EventCreateObserver(nullptr);
    REQUIRE(observer != nullptr);
    REQUIRE(EventListenerCount(observer, EVENT_SHUT) == 0);

    EventDestroyObserver(&observer);
    REQUIRE(observer == nullptr);
}

TEST_CASE("CASE-02: Test Event Subscribe And Emit Functions")
{
    GIVEN("An Valid EventObserver") {
        EventObserver *observer = EventCreateObserver(nullptr);
        REQUIRE(observer != nullptr);

        WHEN("Subscribe Event Success") {
            REQUIRE(EventOn(observer, EVENT_SHUT, OnEvent1, nullptr) == 0);
            REQUIRE(EventOnce(observer, EVENT_SHUT, OnEvent2, nullptr) == 0);
            REQUIRE(EventAppendListener(observer, EVENT_SHUT, OnEvent3, nullptr) == 0);
            REQUIRE(EventAppendOnceListener(observer, EVENT_SHUT, OnEvent4, nullptr) == 0);
            REQUIRE(EventPrependListener(observer, EVENT_SHUT, OnEvent5, nullptr) == 0);
            REQUIRE(EventPrependOnceListener(observer, EVENT_SHUT, OnEvent6, nullptr) == 0);

            REQUIRE(EventListenerCount(observer, EVENT_SHUT) == 6);

            THEN("Emit Event Success") {
                EventEmit(observer, EVENT_SHUT, nullptr);

                /* 检查listener按照期望的顺序执行 */
                REQUIRE(calledSeq[6] == 0);
                REQUIRE(calledSeq[5] == 1);
                REQUIRE(calledSeq[1] == 2);
                REQUIRE(calledSeq[2] == 3);
                REQUIRE(calledSeq[3] == 4);
                REQUIRE(calledSeq[4] == 5);

                /* 检查一次性的listener是不是删除了 */
                REQUIRE(EventListenerCount(observer, EVENT_SHUT) == 3);

                EventDestroyObserver(&observer);
            }

            AND_THEN("Remove EventListener Success") {
                REQUIRE(EventRemoveListener(observer, EVENT_SHUT, OnEvent1, nullptr) == 0);
                REQUIRE(EventListenerCount(observer, EVENT_SHUT) == 5);

                REQUIRE(EventOff(observer, EVENT_SHUT, OnEvent2, nullptr) == 0);
                REQUIRE(EventListenerCount(observer, EVENT_SHUT) == 4);

                REQUIRE(EventRemoveAllListeners(observer, EVENT_SHUT) == 0);
                REQUIRE(EventListenerCount(observer, EVENT_SHUT) == 0);

                EventDestroyObserver(&observer);
            }
        }

        WHEN("Subscribe Event Success With Callback Return true") {
            REQUIRE(EventOn(observer, EVENT_SHUT, OnEventOnce, nullptr) == 0);
            REQUIRE(EventListenerCount(observer, EVENT_SHUT) == 1);

            THEN("After Emit Event, Listener Will Be Removed") {
                REQUIRE(!onEventOnceCalled);
                EventEmit(observer, EVENT_SHUT, nullptr);
                REQUIRE(onEventOnceCalled);
                REQUIRE(EventListenerCount(observer, EVENT_SHUT) == 0);

                EventDestroyObserver(&observer);
            }
        }
    }
}
