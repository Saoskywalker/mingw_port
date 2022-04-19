#ifndef _SYSTEM_PORT_H
#define _SYSTEM_PORT_H

#include "types_base.h"
#include <SDL2/SDL.h>

#define MTF_sys_reset()

void sdl_touch(int *x, int *y, uint8_t target_num, uint8_t *result_num); //返回触摸参数
uint8_t hardware_init_weak(void);
uint8_t system_exit_weak(void);
uint8_t system_process_weak(void);
uint32_t system_get_state(void);

typedef SDL_mutex MTF_mutex;

static inline MTF_mutex *MTF_CreateMutex(void)
{
    return SDL_CreateMutex();
}

static inline void MTF_DestroyMutex(MTF_mutex *mutex)
{
    SDL_DestroyMutex(mutex);
}

static inline int MTF_TryLockMutex(MTF_mutex *mutex)
{
    return SDL_TryLockMutex(mutex);
}

static inline int MTF_LockMutex(MTF_mutex *mutex)
{
    return SDL_LockMutex(mutex);
}

static inline int MTF_UnlockMutex(MTF_mutex *mutex)
{
    return SDL_UnlockMutex(mutex);
}

#endif
