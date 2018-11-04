
#include "interfaces.h"
#include "adapt.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    int32 cmui_psu_io_ctrl(CMUI_PSU_IO_CTRL_KEY_S *key, CMUI_PSU_IO_CTRL_DATA *data)
    {
        if (!key || !data)
        {
            return DEV_NULLPTR;
        }

        if (key->cmd >= IO_CTRL_CMD_MAX)
        {
            return DEV_PARAMERR;
        }

        if (key->sub_cmd >= PSU_SUB_CMD_MAX)
        {
            return DEV_PARAMERR;
        }

        return cmui_psu_adapt_call(key, data);
    }

    int32 cmui_psu_io_init(uint32 hard_type)
    {
        return cmui_psu_adapt_init(hard_type);
    }

#ifdef __cplusplus
}
#endif // __cplusplus
