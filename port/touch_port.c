/*
Copyright (c) 2019-2023 Aysi 773917760@qq.com. All right reserved
Official site: www.mtf123.club

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

It under the terms of the Apache as published;
either version 2 of the License,
or (at your option) any later version.
*/

#include "touch_port.h"
#include "MTF_io.h"
#include "system_port.h"

#define DEBUG_TOUCH(...) printf(__VA_ARGS__)

//SDL相关在SDL_Init统一初始化
uint8_t touch_init(void) //初始化触摸屏控制器
{
    return 0;
}

uint8_t touch_exit(void) //移除触摸屏控制器
{
    return 0;
}

uint8_t touch_scan(int *x, int *y, uint8_t target_num, uint8_t *result_num) //扫描触摸屏
{
    sdl_touch(x, y, target_num, result_num);
    // return 1; //失败
    return 0; //成功
}
