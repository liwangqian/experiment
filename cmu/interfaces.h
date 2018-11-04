#ifndef _CMU_INTERFACES_H_
#define _CMU_INTERFACES_H_

#include "datatype.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 cmui_psu_io_ctrl(CMUI_PSU_IO_CTRL_KEY_S *key,
                           CMUI_PSU_IO_CTRL_DATA *data);
    int32 cmui_psu_io_init(uint32 hard_type);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_CMU_INTERFACES_H_