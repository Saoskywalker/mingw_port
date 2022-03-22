#include "types_base.h"
#include <SDL2/SDL.h>
#include "MTF_io.h"
#include "ROM_port.h"
#include "uart_port.h"
#include "system_port.h"

#define DEBUG_SYSTEM(...) printf(__VA_ARGS__)

#define _TOUCH_NUM 10
static int sdl_x[_TOUCH_NUM], sdl_y[_TOUCH_NUM];
static uint8_t sdl_key_target_num = _TOUCH_NUM, sdl_key_result_num = 0;

void sdl_touch(int *x, int *y, uint8_t target_num, uint8_t *result_num) //返回触摸参数
{
    for (size_t i = 0; i < target_num && target_num <= _TOUCH_NUM; i++)
    {
        x[i] = sdl_x[i];
        y[i] = sdl_y[i];
        (*result_num)++;
    }
}

uint8_t hardware_init_weak(void)
{
    //初始化sdl
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO))
    {
        DEBUG_SYSTEM("Could not initialize SDL - %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}

static uint32_t system_exit = 0; //标记系统状态
uint8_t system_exit_weak(void)
{
    system_exit = 1; //标记退出
    SDL_Delay(100); // 用于等待线程和其他东西退出
    MTF_ROM_exit();
    MTF_UART_exit(NULL);
    SDL_Quit();

    return 0;
}

uint32_t system_get_state(void)
{
    //获取系统状态
    return system_exit;
}

uint8_t system_process_weak(void)
{
    SDL_Event ev;

    sdl_key_result_num = 0;

    while (SDL_PollEvent(&ev)) //SDL事件获取
    {
        if (SDL_KEYDOWN == ev.type) // SDL_KEYUP
        {
            if (SDLK_DOWN == ev.key.keysym.sym)
            {
                DEBUG_SYSTEM("SDLK_DOWN ...............\n");
            }
            else if (SDLK_UP == ev.key.keysym.sym)
            {
                DEBUG_SYSTEM("SDLK_UP ...............\n");
            }
            else if (SDLK_LEFT == ev.key.keysym.sym)
            {
                DEBUG_SYSTEM("SDLK_LEFT ...............\n");
            }
            else if (SDLK_RIGHT == ev.key.keysym.sym)
            {
                DEBUG_SYSTEM("SDLK_RIGHT ...............\n");
            }
        }
        else if (SDL_MOUSEBUTTONDOWN == ev.type)
        {
            if (SDL_BUTTON_LEFT == ev.button.button)
            {
                int px = ev.button.x;
                int py = ev.button.y;
                DEBUG_SYSTEM("SDL_BUTTON_LEFT x, y %d %d ...............\n", px, py);

                if (sdl_key_target_num > sdl_key_result_num)
                {
                    sdl_x[sdl_key_result_num] = ev.motion.x;
                    sdl_y[sdl_key_result_num] = ev.motion.y;
                    sdl_key_result_num++; //标记按下数量
                }
            }
            else if (SDL_BUTTON_RIGHT == ev.button.button)
            {
                int px = ev.button.x;
                int py = ev.button.y;
                DEBUG_SYSTEM("SDL_BUTTON_RIGHT x, y %d %d ...............\n", px, py);

                if (sdl_key_target_num > sdl_key_result_num)
                {
                    sdl_x[sdl_key_result_num] = ev.motion.x;
                    sdl_y[sdl_key_result_num] = ev.motion.y;
                    sdl_key_result_num++; //标记按下数量
                }
            }
        }
        else if (SDL_MOUSEMOTION == ev.type)
        {
            int px = ev.motion.x;
            int py = ev.motion.y;

            DEBUG_SYSTEM("x, y %d %d ...............\n", px, py);

            sdl_x[0] = ev.motion.x;
            sdl_y[0] = ev.motion.y;
        }
        else if (SDL_QUIT == ev.type)
        {
            DEBUG_SYSTEM("SDL_QUIT ...............\n");
            return 1;
        }
    }

    return 0; //成功
}
