#ifndef _CMU_PRIV_INA2XX_H_
#define _CMU_PRIV_INA2XX_H_

#include "datatype.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 ina2xx_get_current(uint32 slot_id, uint32 seq_no,
                             void *in_buf, uint32 *in_buf_size,
                             void *out_buf, uint32 *out_buf_size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif