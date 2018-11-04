#include "ina2xx.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 ina2xx_get_current(uint32 slot_id, uint32 seq_no,
                             void *in_buf, uint32 *in_buf_size,
                             void *out_buf, uint32 *out_buf_size)
    {
        return DEV_OK;
    }

#ifdef __cplusplus
}
#endif // __cplusplus
