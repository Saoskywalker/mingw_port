#include "timer_port.h"
// #include "Sagittarius_timer.h"
#include <SDL2/SDL.h>
#include "touch_port.h"
#include "MTF_io.h"
#include "system_port.h"

/*
定时器中断
*/
static int TIMER0_ISR(void *arg)
{
	while (system_get_state() == 0)
	{
		// TEST_TOGGLE_PIN();
		// Sagittarius_timer();
		SDL_Delay(5); // delay 5ms
	}

	return 0;
}

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
    SDL_CreateThread(TIMER0_ISR, NULL, NULL); //创建线程, 用于定时器
}
