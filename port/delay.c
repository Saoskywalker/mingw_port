#include "delay.h"
#include <SDL2/SDL.h>

#define CPU_FREQUENCY 3300 //CPU: 720M

#if CPU_FREQUENCY == 3300
#define _MS_CNT 450000
#define _US_CNT 400
#else
#define _MS_CNT 51000
#define _US_CNT 36
#endif

void delay(volatile uint32_t ms)
{
	SDL_Delay(ms);
}

void delay_us(volatile uint32_t us)
{
	while(us--)
	{
		volatile uint32_t n = _US_CNT;
		while(n--);
	}
}

void delay_ms(volatile uint32_t ms)
{
	SDL_Delay(ms);
}

