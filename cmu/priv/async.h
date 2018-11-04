#ifndef _CMU_PRIV_ASYNC_H_
#define _CMU_PRIV_ASYNC_H_

#include "priv_datatype.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct tag_CMU_ASYNC_REQUEST_KEY
    {
        psu_intf_t io_func;
        uint32 slot_id;
        uint32 seq_no;
    } CMU_ASYNC_REQUEST_KEY_S;

    typedef struct tag_CMU_ASYNC_REQUEST_DATA
    {
        void *in_buf;
        uint32 *in_buf_size;
        void *out_buf;
        uint32 *out_buf_size;

        int32 ret; /* 接口io_func的返回值 */
    } CMU_ASYNC_REQUEST_DATA_S;

    typedef struct tag_CMU_ASYNC_REQUEST
    {
        CMU_ASYNC_REQUEST_KEY_S key;
        CMU_ASYNC_REQUEST_DATA_S data;
    } CMU_ASYNC_RESUEST_S;

    int32 cmui_async_init(void);
    int32 cmui_async_exit(void);
    int32 cmui_async_post_request(CMU_ASYNC_RESUEST_S *rq);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
