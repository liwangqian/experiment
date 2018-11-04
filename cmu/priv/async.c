#include "async.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define CMU_ASYNC_REQUEST_MAX (256)
    CMU_ASYNC_RESUEST_S *g_async_queue[CMU_ASYNC_REQUEST_MAX] = {0};
    int32 g_head_index = -1;
    int32 g_tail_index = -1;
    int32 g_timer;

    void _cmui_async_handle_request();
    CMU_ASYNC_RESUEST_S *_cmui_async_get_next_request();
    int32 _cmui_async_is_pending(CMU_ASYNC_REQUEST_KEY_S *key);
    int32 _cmui_async_enter_event_loop();

    // STUB
    int32 cmui_timer_interval(uint32 interval, void *func, uint32 params)
    {
        return 0;
    }

    void cmui_timer_cancel(int32 atimer)
    {
        return;
    }

    int32 cmui_async_init(void)
    {
        g_timer = cmui_timer_interval(10, _cmui_async_enter_event_loop, 0);
        return DEV_OK;
    }
    int32 cmui_async_exit(void)
    {
        cmui_timer_cancel(g_timer);
        return DEV_OK;
    }
    int32 cmui_async_post_request(CMU_ASYNC_RESUEST_S *rq)
    {
        //TODO: add mutex
        const int32 insert_index = (g_head_index + 1) % CMU_ASYNC_REQUEST_MAX;
        if ((insert_index < g_tail_index) && !_cmui_async_is_pending(&rq->key))
        {
            g_async_queue[insert_index] = rq;
            g_head_index = insert_index;
            return DEV_OK;
        }
        else
        {
            return DEV_ERROR;
        }
    }
    int32 _cmui_async_is_pending(CMU_ASYNC_REQUEST_KEY_S *key)
    {
        int32 i;
        for (i = g_tail_index; i != g_head_index; i %= CMU_ASYNC_REQUEST_MAX)
        {
            CMU_ASYNC_RESUEST_S *rq = g_async_queue[++i];
            if (rq && (&rq->key == key))
            {
                return DEV_OK;
            }
        }

        return DEV_ERROR;
    }

    void _cmui_async_handle_request(CMU_ASYNC_RESUEST_S *rq)
    {
        if (rq && rq->key.io_func)
        {
            rq->data.ret = rq->key.io_func(rq->key.slot_id, rq->key.seq_no,
                                           rq->data.in_buf, rq->data.in_buf_size,
                                           rq->data.out_buf, rq->data.out_buf_size);
        }
    }

    CMU_ASYNC_RESUEST_S *_cmui_async_get_next_request()
    {
        const int32 handle_index = (g_tail_index + 1) % CMU_ASYNC_REQUEST_MAX;
        if (handle_index > g_head_index)
        {
            return 0;
        }

        CMU_ASYNC_RESUEST_S *rq = g_async_queue[handle_index];
        g_async_queue[handle_index] = 0;
        g_tail_index = handle_index;

        return rq;
    }

    int32 _cmui_async_enter_event_loop()
    {
        for (;;)
        {
            CMU_ASYNC_RESUEST_S *rq = _cmui_async_get_next_request();
            if (!rq)
            {
                break;
            }
            _cmui_async_handle_request(rq);
        }
    }

#ifdef __cplusplus
}
#endif // __cplusplus
