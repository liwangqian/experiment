#ifndef _CMU_PRIV_BUCKBOOST_H_
#define _CMU_PRIV_BUCKBOOST_H_

#include "datatype.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 buckboost_get_in_current(uint32 slot_id, uint32 seq_no,
                                   void *in_buf, uint32 *in_buf_size,
                                   void *out_buf, uint32 *out_buf_size);
    int32 buckboost_get_out_current(uint32 slot_id, uint32 seq_no,
                                    void *in_buf, uint32 *in_buf_size,
                                    void *out_buf, uint32 *out_buf_size);
    int32 buckboost_get_in_voltage(uint32 slot_id, uint32 seq_no,
                                   void *in_buf, uint32 *in_buf_size,
                                   void *out_buf, uint32 *out_buf_size);
    int32 buckboost_get_out_voltage(uint32 slot_id, uint32 seq_no,
                                    void *in_buf, uint32 *in_buf_size,
                                    void *out_buf, uint32 *out_buf_size);
    int32 buckboost_get_in_power(uint32 slot_id, uint32 seq_no,
                                 void *in_buf, uint32 *in_buf_size,
                                 void *out_buf, uint32 *out_buf_size);
    int32 buckboost_get_out_power(uint32 slot_id, uint32 seq_no,
                                  void *in_buf, uint32 *in_buf_size,
                                  void *out_buf, uint32 *out_buf_size);
    int32 buckboost_get_soft_version(uint32 slot_id, uint32 seq_no,
                                     void *in_buf, uint32 *in_buf_size,
                                     void *out_buf, uint32 *out_buf_size);
    int32 buckboost_get_blackbox(uint32 slot_id, uint32 seq_no,
                                 void *in_buf, uint32 *in_buf_size,
                                 void *out_buf, uint32 *out_buf_size);
    int32 buckboost_reset_self(uint32 slot_id, uint32 seq_no,
                               void *in_buf, uint32 *in_buf_size,
                               void *out_buf, uint32 *out_buf_size);
    int32 buckboost_upgrade_bootrom(uint32 slot_id, uint32 seq_no,
                                    void *in_buf, uint32 *in_buf_size,
                                    void *out_buf, uint32 *out_buf_size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif