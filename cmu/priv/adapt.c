
#include "adapt.h"
#include "priv_datatype.h"
#include "buckboost.h"
#include "eeprom.h"
#include "ina2xx.h"
#include "pca9xxx.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    CMUI_PSU_INTF_S g_psu_intf = {{0}};

    int32 _cmui_psu_adapt_do_call(CMUI_PSU_IO_CTRL_KEY_S *key,
                                  CMUI_PSU_IO_CTRL_DATA *data,
                                  psu_intf_t *callbacks);

    int32 cmui_psu_adapt_init(uint32 hard_type)
    {
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_PCB_VERSION] = pca9xxx_get_pcb_version;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_ALARM_STATE] = pca9xxx_get_alarm_state;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_INPUT_TYPE] = pca9xxx_get_input_type;

        g_psu_intf.callbacks[PSU_SUB_CMD_GET_SOFT_VERSION] = buckboost_get_soft_version;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_IN_CURRENT] = buckboost_get_in_current;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_OUT_CURRENT] = buckboost_get_out_current;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_IN_VOLTAGE] = buckboost_get_in_voltage;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_OUT_VOLTAGE] = buckboost_get_out_voltage;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_IN_POWER] = buckboost_get_in_power;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_OUT_POWER] = buckboost_get_out_power;
        g_psu_intf.callbacks[PSU_SUB_CMD_GET_BLACKBOX] = buckboost_get_blackbox;
        g_psu_intf.callbacks[PSU_SUB_CMD_RESET_SELF] = buckboost_reset_self;
        g_psu_intf.callbacks[PSU_SUB_CMD_UPGRADE] = buckboost_upgrade_bootrom;

        g_psu_intf.callbacks[PSU_SUB_CMD_GET_BALANCE_CURRENT] = ina2xx_get_current;

        g_psu_intf.callbacks[PSU_SUB_CMD_GET_ELABEL] = eeprom_get_elabel;
        g_psu_intf.callbacks[PSU_SUB_CMD_SET_ELABEL] = eeprom_set_elabel;

        return DEV_OK;
    }

    int32 cmui_psu_adapt_call(CMUI_PSU_IO_CTRL_KEY_S *key,
                              CMUI_PSU_IO_CTRL_DATA *data)
    {
        return _cmui_psu_adapt_do_call(key, data, key->async ? g_psu_intf.callbacks_async : g_psu_intf.callbacks);
    }

    int32 _cmui_psu_adapt_do_call(CMUI_PSU_IO_CTRL_KEY_S *key,
                                  CMUI_PSU_IO_CTRL_DATA *data,
                                  psu_intf_t *callbacks)
    {
        psu_intf_t cb = callbacks[key->sub_cmd];
        if (cb)
        {
            return cb(key->slot_id, key->seq_no,
                      data->in_buf, data->in_buf_size,
                      data->out_buf, data->out_buf_size);
        }

        return DEV_ERROR;
    }

#ifdef __cplusplus
}
#endif // __cplusplus
