#ifndef _AUDIO_PORT_H
#define _AUDIO_PORT_H

#include "types_base.h"
#include <SDL2/SDL.h>

#define AUDIO_S24SYS 0 //SDL2不支持

typedef SDL_AudioSpec audio_pcm_dev_type;

void MTF_audio_time(void);
uint32_t MTF_audio_output_time_get(void);
char MTF_audio_pcm_output_init(audio_pcm_dev_type *dev, audio_pcm_dev_type *dest);
char MTF_audio_pcm_output_exit(audio_pcm_dev_type *dev);
uint8_t MTF_audio_pcm_output_busy(audio_pcm_dev_type *dev);
uint8_t MTF_audio_pcm_output(audio_pcm_dev_type *dev, uint8_t *stream, int len);
void MTF_audio_vol(unsigned char i);

#endif
