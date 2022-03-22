#ifndef _SYSTEM_PORT_H
#define _SYSTEM_PORT_H

#include "types_base.h"

#define MTF_sys_reset()

void sdl_touch(int *x, int *y, uint8_t target_num, uint8_t *result_num); //返回触摸参数
uint8_t hardware_init_weak(void);
uint8_t system_exit_weak(void);
uint8_t system_process_weak(void);
uint32_t system_get_state(void);

#endif
