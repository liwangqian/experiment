#include "interfaces.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("Hello World!\n");
    printf("init: %d\n", cmui_psu_io_init(0x90000870));

    uint32 in_current = 0;
    uint32 in_current_size = sizeof(in_current);
    CMUI_PSU_IO_CTRL_KEY_S key = {IO_CTRL_CMD_GET, PSU_SUB_CMD_GET_IN_CURRENT, 1, 0, 0};
    CMUI_PSU_IO_CTRL_DATA data = {NULL, NULL, &in_current, &in_current_size};
    printf("call get_in_current: %d\n", cmui_psu_io_ctrl(&key, &data));
    printf("in_current: %#x\n", in_current);

    return 0;
}
