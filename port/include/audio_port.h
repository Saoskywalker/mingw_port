#ifndef _AUDIO_PORT_H
#define _AUDIO_PORT_H

#include "types_base.h"
#include <SDL2/SDL.h>

typedef SDL_AudioSpec audio_pcm_dev;

void MTF_audio_time(void);
uint32_t MTF_audio_output_time_get(void);
char MTF_audio_pcm_output_init(audio_pcm_dev *dev, audio_pcm_dev *dest);
char MTF_audio_pcm_output_exit(audio_pcm_dev *dev);
uint8_t MTF_audio_pcm_output_busy(audio_pcm_dev *dev);
uint8_t MTF_audio_pcm_output(audio_pcm_dev *dev, uint8_t *stream, int len);
void MTF_audio_vol(unsigned char i);

#endif
