#ifndef _CMU_PRIV_DATATYPE_H_
#define _CMU_PRIV_DATATYPE_H_

#include "datatype.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef int32 (*psu_intf_t)(uint32 slot_id, uint32 seq_no,
                                void *in_buf, uint32 *in_buf_size,
                                void *out_buf, uint32 *out_buf_size);

    typedef struct tag_CMUI_PSU_INTF_S
    {
        psu_intf_t callbacks[PSU_SUB_CMD_MAX];
        psu_intf_t callbacks_async[PSU_SUB_CMD_MAX];
    } CMUI_PSU_INTF_S;

    extern CMUI_PSU_INTF_S g_psu_intf;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_CMU_PRIV_DATATYPE_H_
