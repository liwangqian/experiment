#ifndef _CMU_PRIV_PCA9XXX_H_
#define _CMU_PRIV_PCA9XXX_H_

#include "datatype.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 pca9xxx_get_pcb_version(uint32 slot_id, uint32 seq_no,
                                  void *in_buf, uint32 *in_buf_size,
                                  void *out_buf, uint32 *out_buf_size);
    int32 pca9xxx_get_alarm_state(uint32 slot_id, uint32 seq_no,
                                  void *in_buf, uint32 *in_buf_size,
                                  void *out_buf, uint32 *out_buf_size);
    int32 pca9xxx_get_input_type(uint32 slot_id, uint32 seq_no,
                                 void *in_buf, uint32 *in_buf_size,
                                 void *out_buf, uint32 *out_buf_size);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif