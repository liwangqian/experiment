#ifndef __CMU_PRIV_ADAPT_H_
#define __CMU_PRIV_ADAPT_H_

#include "datatype.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 cmui_psu_adapt_init(uint32 hard_type);
    int32 cmui_psu_adapt_call(CMUI_PSU_IO_CTRL_KEY_S *key,
                              CMUI_PSU_IO_CTRL_DATA *data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
