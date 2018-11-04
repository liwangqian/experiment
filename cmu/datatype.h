#ifndef _CMU_DATATYPE_H_
#define _CMU_DATATYPE_H_

#include "typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef enum tag_CMUI_IO_CTRL_CMD_E
    {
        IO_CTRL_CMD_GET = 0,
        IO_CTRL_CMD_SET,
        IO_CTRL_CMD_RESET,
        IO_CTRL_CMD_UPGRADE,
        IO_CTRL_CMD_MAX
    } CMUI_IO_CTRL_CMD_E;

    typedef enum tag_CMUI_PSU_SUB_CMD_E
    {
        PSU_SUB_CMD_GET_PCB_VERSION = 0,
        PSU_SUB_CMD_GET_SOFT_VERSION,
        PSU_SUB_CMD_GET_ALARM_STATE,
        PSU_SUB_CMD_GET_INPUT_TYPE,
        PSU_SUB_CMD_GET_IN_CURRENT,
        PSU_SUB_CMD_GET_OUT_CURRENT,
        PSU_SUB_CMD_GET_IN_VOLTAGE,
        PSU_SUB_CMD_GET_OUT_VOLTAGE,
        PSU_SUB_CMD_GET_IN_POWER,
        PSU_SUB_CMD_GET_OUT_POWER,
        PSU_SUB_CMD_GET_BALANCE_CURRENT,
        PSU_SUB_CMD_GET_BLACKBOX,
        PSU_SUB_CMD_GET_ELABEL,
        PSU_SUB_CMD_SET_ELABEL,
        PSU_SUB_CMD_RESET_SELF,
        PSU_SUB_CMD_UPGRADE,
        PSU_SUB_CMD_MAX
    } CMUI_PSU_SUB_CMD_E;

    typedef struct tag_CMUI_PSU_IO_CTRL_KEY
    {
        CMUI_IO_CTRL_CMD_E cmd;
        CMUI_PSU_SUB_CMD_E sub_cmd;
        uint32 slot_id;
        uint32 seq_no;
        uint8 async;
    } CMUI_PSU_IO_CTRL_KEY_S;

    typedef struct tag_CMUI_PSU_IO_CTRL_DATA
    {
        void *in_buf;
        uint32 *in_buf_size;
        void *out_buf;
        uint32 *out_buf_size;
    } CMUI_PSU_IO_CTRL_DATA;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_CMU_DATATYPE_H_