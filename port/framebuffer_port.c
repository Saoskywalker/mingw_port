#include "framebuffer_port.h"
#include "LCD_parameter.h"
#include "malloc.h"
#include <SDL2/SDL.h>

//目前F1C100S配置威1级缓冲
#define DIS_BUF_LEVEL 1 //显示缓存级数: 1~3

//set '1' to choose a type of file to play
#define LOAD_BGRA    1
#define LOAD_RGB24   0
#define LOAD_BGR24   0
#define LOAD_YUV420P 0

static render_dev_type *SDL_buf = NULL;
static SDL_Window *screen = NULL;
static SDL_Renderer *sdlRenderer = NULL;
static SDL_Texture *sdlTexture = NULL;
static int *screen_w = NULL, *screen_h = NULL;

//Refresh Event
#define REFRESH_EVENT  (SDL_USEREVENT + 1)
 
static int thread_exit=0;
 int sdl_event_get(void *opaque)
{
    SDL_Event event;
    // while (thread_exit == 0)
    {

        // event.type = REFRESH_EVENT;
        // SDL_PushEvent(&event);
        // SDL_Delay(40);

        // Wait
        SDL_WaitEventTimeout(&event, 1);

        if (event.type == SDL_WINDOWEVENT)
        {
            // If Resize
            SDL_GetWindowSize(screen, screen_w, screen_h);
        }
        else if (event.type == SDL_QUIT)
        {
            thread_exit = 1;
            return 1;
            // break;
        }
    }
    return 0;
}

static int _SDL_flush(void *data)
{
    // Bit per Pixel
#if LOAD_BGRA
    const int bpp = 32;
#elif LOAD_RGB24 | LOAD_BGR24
    const int bpp = 24;
#elif LOAD_YUV420P
    const int bpp = 12;
#endif

#if LOAD_RGB24 | LOAD_BGR24
    unsigned char buffer_convert[pixel_w * pixel_h * 4];
#endif

    SDL_Rect sdlRect;
    SDL_Event event;

    while (1)
    {
        if (SDL_buf != NULL)
        {

            //  if(event.type==REFRESH_EVENT)
            {
#if LOAD_BGRA
                // We don't need to change Endian
                // Because input BGRA pixel data(B|G|R|A) is same as ARGB8888 in Little Endian (B|G|R|A)
                SDL_UpdateTexture(sdlTexture, NULL, SDL_buf->pixels, SDL_buf->width * 4);
#elif LOAD_RGB24 | LOAD_BGR24
                // change 24bit to 32 bit
                // and in Windows we need to change Endian
                CONVERT_24to32(SDL_buf->pixels, buffer_convert, SDL_buf->width, SDL_buf->height);
                SDL_UpdateTexture(sdlTexture, NULL, buffer_convert, SDL_buf->width * 4);
#elif LOAD_YUV420P
                SDL_UpdateTexture(sdlTexture, NULL, SDL_buf->pixels, SDL_buf->width);
#endif

                // FIX: If window is resize
                sdlRect.x = 0;
                sdlRect.y = 0;
                sdlRect.w = *screen_w;
                sdlRect.h = *screen_h;

                SDL_RenderClear(sdlRenderer);
                SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
                SDL_RenderPresent(sdlRenderer);
                // Delay 16ms
                // SDL_Delay(16);
            }
        }

        SDL_Delay(10); // delay 10ms
    }

    return 0;
}

//截屏功能
void MTF_fb_write_back_start(void) //截屏一帧 
{
}

uint8_t MTF_fb_write_back_state(void) //查询完成状态
{
    return 0;
}

void MTF_fb_set_backlight(framebuffer_dev_type *fb, int32_t brightness) //设置背光
{
}

render_dev_type *MTF_fb_render_create(framebuffer_dev_type *fb, uint32_t width, uint32_t height)
{
	render_dev_type * render;
	void * pixels;
    size_t pixlen = width * height * fb->bits_per_pixel / 8;

    /*****MTF port render create******/
    render = malloc(sizeof(render_dev_type));
    if (render == NULL)
        return NULL;

    //不直接写显存, 通过特定函数将其资料复制至显存
    pixels = malloc(pixlen); // ARGB
    if (pixels == NULL)
    {
        free(render);
        return NULL;
    }

    render->width = fb->xres;
	render->height = fb->yres;
	render->format = PIXEL_FORMAT_ARGB32;
	render->pixels = pixels;
	render->pixlen = pixlen;
    render->bytes_per_pixel = fb->bits_per_pixel / 8;
	render->priv = NULL;

    SDL_buf = render;
    /******************************/

    /*****sdlTexture create******/
    uint32_t pixformat = 0;
    
#if LOAD_BGRA
    // Note: ARGB8888 in "Little Endian" system stores as B|G|R|A
    pixformat = SDL_PIXELFORMAT_ARGB8888;
#elif LOAD_RGB24
    pixformat = SDL_PIXELFORMAT_RGB888;
#elif LOAD_BGR24
    pixformat = SDL_PIXELFORMAT_BGR888;
#elif LOAD_YUV420P
    // IYUV: Y + U + V  (3 planes)
    // YV12: Y + V + U  (3 planes)
    pixformat = SDL_PIXELFORMAT_IYUV;
#endif

    sdlTexture = SDL_CreateTexture(sdlRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, SDL_buf->width, SDL_buf->height);
    /**************************/
    
    return render;
}

void MTF_fb_destroy(render_dev_type * render)
{
    SDL_DestroyTexture(sdlTexture);
	if(render)
	{
        free(render->pixels);
        free(render);
    }
}

void MTF_fb_present(framebuffer_dev_type *fb, render_dev_type * render)
{
#if DIS_BUF_LEVEL == 3
    // fb_f1c100s_present(&fb_f1c100s_pData, render->pixels, render->pixlen); //三级缓冲, 缩放显示时不能用使用此方法
#endif
}

void *MTF_fb_get_dis_mem(render_dev_type *render) //获取可直接使用的显存地址
{
    return render->pixels;
}

void MTF_fb_TV_input(framebuffer_dev_type *fb, uint8_t state, uint8_t contrast, uint8_t bright, uint8_t saturation, uint8_t hue) //是否开启AV输入
{
}

void MTF_fb_scale(framebuffer_dev_type *fb, uint8_t state, void *data) //是否开启硬件缩放
{
    fb->scale_flag = 1; //开启缩放
}

void MTF_fb_init(framebuffer_dev_type *fb)
{   
    //获取液晶参数
    screen_w = (int *)&(fb->xres);
    screen_h = (int *)&(fb->yres);
    fb->xres = LCD_X_PIXEL;
    fb->yres = LCD_Y_PIXEL;
    fb->xres_virtual = fb->xres;
    fb->yres_virtual = fb->yres;
    fb->sync = LCD_DE_HV_MODE;               // 0: DE_HV 1:DE 2:HV
    fb->pixclock = LCD_CLK;                  // pixel_clock_hz
    fb->left_margin = LCD_HFP;               // h_front_porch
    fb->right_margin = LCD_HBP;              // h_back_porch
    fb->hsync_len = LCD_HSL;                 // h_sync_len
    fb->upper_margin = LCD_VFP;              // v_front_porch
    fb->lower_margin = LCD_VBP;              // v_back_porch
    fb->vsync_len = LCD_VSL;                 // v_sync_len
    fb->width = 216;                         //液晶尺寸(单位:mm)
    fb->height = 135;

    //像素信息
    fb->bits_per_pixel = 32;

    fb->scale_flag = 0; //不开启缩放
    fb->tv_input_flag = 0; //不使用cvbs输入
    fb->write_back_addr = NULL; //回写显存地址

    // SDL 2.0 Support for multiple windows
    screen = SDL_CreateWindow("MTF Sagittarius", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              *screen_w, *screen_h, SDL_WINDOW_OPENGL);
    if (!screen)
    {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return;
    }
    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

    SDL_CreateThread(_SDL_flush, NULL, NULL); //创建线程, 用于定时更新显示
}

void MTF_fb_exit(framebuffer_dev_type *fb)
{
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(screen);
}
