#include "audio_port.h"

//Buffer:
//|-----------|-------------|
//chunk-------pos---len-----|
static  uint8_t  *audio_chunk; 
static  uint32_t  audio_len = 0; 
static  uint8_t  *audio_pos; 

/* Audio Callback
 * The audio function callback takes the following parameters: 
 * stream: A pointer to the audio buffer to be filled 
 * len: The length (in bytes) of the audio buffer 
 * 
*/
static void fill_audio(void *udata, uint8_t *stream, int len)
{
	// SDL 2.0
	SDL_memset(stream, 0, len);
	if (audio_len == 0)
		return;
	len = (len > audio_len ? audio_len : len);

	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

static unsigned char _pcmFristOutput = 0;
static uint32_t pcm_run_time = 0;

void MTF_audio_time(void)
{
	pcm_run_time++; //每10ms一次
}

uint32_t MTF_audio_output_time_get(void)
{
	// return AVS_Time_Read(AVS_TIME_0) / 100; //硬件方式计时
	return pcm_run_time; //软件方式计时
}

char MTF_audio_pcm_output_init(audio_pcm_dev *dev, audio_pcm_dev *dest)
{
	pcm_run_time = 0;
	_pcmFristOutput = 0;

	/*audio HW dev init*/
	dev->callback = fill_audio;
	if (SDL_OpenAudio((SDL_AudioSpec *)&dev, NULL) < 0)
	{
		// printf("can't open audio.\n");
		return -1;
	}

	// Play
	SDL_PauseAudio(0);

	return 0;
}

char MTF_audio_pcm_output_exit(audio_pcm_dev *dev)
{
	pcm_run_time = 0;
	_pcmFristOutput = 0;

	return 0;
}

uint8_t MTF_audio_pcm_output_busy(audio_pcm_dev *dev)
{
	if (audio_len > 0) // Wait until finish
		return 1;
	else
		return 0;
}

uint8_t MTF_audio_pcm_output(audio_pcm_dev *dev, uint8_t *stream, int len)
{
	/*output pcm data*/
	// Set audio buffer (PCM data)
	audio_chunk = stream;
	// Audio buffer length
	audio_len = len;
	audio_pos = audio_chunk;

	return 0;
}

void MTF_audio_vol(unsigned char i)
{
}
