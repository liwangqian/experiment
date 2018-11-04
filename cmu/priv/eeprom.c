
#include "eeprom.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 eeprom_get_elabel(uint32 slot_id, uint32 seq_no,
                            void *in_buf, uint32 *in_buf_size,
                            void *out_buf, uint32 *out_buf_size)
    {
        return DEV_OK;
    }

    int32 eeprom_set_elabel(uint32 slot_id, uint32 seq_no,
                            void *in_buf, uint32 *in_buf_size,
                            void *out_buf, uint32 *out_buf_size)
    {
        return DEV_OK;
    }

#ifdef __cplusplus
}
#endif // __cplusplus