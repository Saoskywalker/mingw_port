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

#include "timer_port.h"
#include "Sagittarius_timer.h"
#include <SDL2/SDL.h>
#include "touch_port.h"
#include "MTF_io.h"
#include "system_port.h"

// #define _USE_MULIT_THREAD //使用多线程代替定时器, emsdk对多线程支持不完善

/*
定时器中断
*/
#ifdef _USE_MULIT_THREAD
static int TIMER0_ISR(void *arg)
{
	while (system_get_state() == 0)
	{
		// TEST_TOGGLE_PIN();
		Sagittarius_timer();
		SDL_Delay(1); // delay 1ms
	}

	return 0;
}
#else
static uint32_t TIMER0_ISR(uint32_t interval, void *param)
{
	// TEST_TOGGLE_PIN();
	Sagittarius_timer();

	return interval;
}
#endif

void TIMER1_ISR(void *arg)
{
	// TEST_TOGGLE_PIN();
	// TIMER1_CLEAR(); //清中断
}

void TIMER2_ISR(void *arg)
{
	// TEST_TOGGLE_PIN();
	// TIMER2_CLEAR(); //清中断
}

void MTF_timer_init_handle(void)
{
#ifdef _USE_MULIT_THREAD
    SDL_CreateThread(TIMER0_ISR, NULL, NULL); //创建线程, 用于定时器
#else
	SDL_AddTimer(1, TIMER0_ISR, NULL); //创建定时器, 用于定时器
#endif
}
