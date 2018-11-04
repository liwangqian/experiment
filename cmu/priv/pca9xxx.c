
#include "pca9xxx.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 pca9xxx_get_pcb_version(uint32 slot_id, uint32 seq_no,
                                  void *in_buf, uint32 *in_buf_size,
                                  void *out_buf, uint32 *out_buf_size)
    {
        return DEV_OK;
    }
    int32 pca9xxx_get_alarm_state(uint32 slot_id, uint32 seq_no,
                                  void *in_buf, uint32 *in_buf_size,
                                  void *out_buf, uint32 *out_buf_size)
    {
        return DEV_OK;
    }
    int32 pca9xxx_get_input_type(uint32 slot_id, uint32 seq_no,
                                 void *in_buf, uint32 *in_buf_size,
                                 void *out_buf, uint32 *out_buf_size)
    {
        return DEV_OK;
    }
#ifdef __cplusplus
}
#endif // __cplusplus