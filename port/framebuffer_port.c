#include "framebuffer_port.h"
#include "malloc.h"
#include <SDL2/SDL.h>
#include "MTF_io.h"
#include "system_port.h"

#define DEBUG_FRAMEBUFFER(...) //printf(__VA_ARGS__)

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
static uint8_t _dis_pasue = 0;

 int _SDL_flush(void *data)
{
    // Bit per Pixel
// #if LOAD_BGRA
//     const int bpp = 32;
// #elif LOAD_RGB24 | LOAD_BGR24
//     const int bpp = 24;
// #elif LOAD_YUV420P
//     const int bpp = 12;
// #endif

#if LOAD_RGB24 | LOAD_BGR24
    unsigned char buffer_convert[pixel_w * pixel_h * 4];
#endif

    SDL_Rect sdlRect;
    // SDL_Event event;

    while (system_get_state() == 0)
    {
        if (SDL_buf != NULL && _dis_pasue == 0)
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

static char _dis_mem_create = 0;
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

    if (_dis_mem_create == 0) //显示内存只能首次创建
    {
        _dis_mem_create = 1;
        
        SDL_buf = render;
        sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

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
    }

    return render;
}

void MTF_fb_destroy(render_dev_type * render)
{
    if(sdlTexture)
    {
        _dis_pasue = 1; //先通知线程暂停使用资源, 再释放
        SDL_Delay(100);
        SDL_DestroyTexture(sdlTexture);
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = NULL;
        sdlTexture = NULL;
        SDL_buf = NULL;
        _dis_mem_create = 0;
        _dis_pasue = 0;
    }

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
    // fb->scale_flag = 1; //开启缩放
}

static uint16_t ico_pixels[16 * 16] = {// ...or with raw pixel data:
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0aab, 0x0789, 0x0bcc, 0x0eee, 0x09aa, 0x099a, 0x0ddd,
                            0x0fff, 0x0eee, 0x0899, 0x0fff, 0x0fff, 0x1fff, 0x0dde, 0x0dee,
                            0x0fff, 0xabbc, 0xf779, 0x8cdd, 0x3fff, 0x9bbc, 0xaaab, 0x6fff,
                            0x0fff, 0x3fff, 0xbaab, 0x0fff, 0x0fff, 0x6689, 0x6fff, 0x0dee,
                            0xe678, 0xf134, 0x8abb, 0xf235, 0xf678, 0xf013, 0xf568, 0xf001,
                            0xd889, 0x7abc, 0xf001, 0x0fff, 0x0fff, 0x0bcc, 0x9124, 0x5fff,
                            0xf124, 0xf356, 0x3eee, 0x0fff, 0x7bbc, 0xf124, 0x0789, 0x2fff,
                            0xf002, 0xd789, 0xf024, 0x0fff, 0x0fff, 0x0002, 0x0134, 0xd79a,
                            0x1fff, 0xf023, 0xf000, 0xf124, 0xc99a, 0xf024, 0x0567, 0x0fff,
                            0xf002, 0xe678, 0xf013, 0x0fff, 0x0ddd, 0x0fff, 0x0fff, 0xb689,
                            0x8abb, 0x0fff, 0x0fff, 0xf001, 0xf235, 0xf013, 0x0fff, 0xd789,
                            0xf002, 0x9899, 0xf001, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0xe789,
                            0xf023, 0xf000, 0xf001, 0xe456, 0x8bcc, 0xf013, 0xf002, 0xf012,
                            0x1767, 0x5aaa, 0xf013, 0xf001, 0xf000, 0x0fff, 0x7fff, 0xf124,
                            0x0fff, 0x089a, 0x0578, 0x0fff, 0x089a, 0x0013, 0x0245, 0x0eff,
                            0x0223, 0x0dde, 0x0135, 0x0789, 0x0ddd, 0xbbbc, 0xf346, 0x0467,
                            0x0fff, 0x4eee, 0x3ddd, 0x0edd, 0x0dee, 0x0fff, 0x0fff, 0x0dee,
                            0x0def, 0x08ab, 0x0fff, 0x7fff, 0xfabc, 0xf356, 0x0457, 0x0467,
                            0x0fff, 0x0bcd, 0x4bde, 0x9bcc, 0x8dee, 0x8eff, 0x8fff, 0x9fff,
                            0xadee, 0xeccd, 0xf689, 0xc357, 0x2356, 0x0356, 0x0467, 0x0467,
                            0x0fff, 0x0ccd, 0x0bdd, 0x0cdd, 0x0aaa, 0x2234, 0x4135, 0x4346,
                            0x5356, 0x2246, 0x0346, 0x0356, 0x0467, 0x0356, 0x0467, 0x0467,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
                            0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff};

uint8_t MTF_fb_init(framebuffer_dev_type *fb)
{   
    //SDL相关在SDL_Init统一初始化
    
    screen_w = (int *)&(fb->xres);
    screen_h = (int *)&(fb->yres);

    // SDL 2.0 Support for multiple windows
    screen = SDL_CreateWindow("MTF Sagittarius", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              *screen_w, *screen_h, SDL_WINDOW_OPENGL);
    if (!screen)
    {
        DEBUG_FRAMEBUFFER("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return 2;
    }
    
    SDL_Surface *surface;     // Declare an SDL_Surface to be filled in with pixel data from an image file
    surface = SDL_CreateRGBSurfaceFrom(ico_pixels, 16, 16, 16, 16 * 2, 0x0f00, 0x00f0, 0x000f, 0xf000);

    // The icon is attached to the screen pointer
    SDL_SetWindowIcon(screen, surface);

    SDL_CreateThread(_SDL_flush, NULL, NULL); //创建线程, 用于定时更新显示

    return 0;
}

uint8_t MTF_fb_reset(framebuffer_dev_type *fb)
{   
    /*1.销毁窗口*/
    MTF_fb_exit(fb);
    SDL_Delay(5);

    /*2.重建窗口*/
    //SDL相关在SDL_Init统一初始化
    
    screen_w = (int *)&(fb->xres);
    screen_h = (int *)&(fb->yres);

    // SDL 2.0 Support for multiple windows
    screen = SDL_CreateWindow("MTF Sagittarius", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              *screen_w, *screen_h, SDL_WINDOW_OPENGL);
    if (!screen)
    {
        DEBUG_FRAMEBUFFER("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return 2;
    }
    
    SDL_Surface *surface;     // Declare an SDL_Surface to be filled in with pixel data from an image file
    surface = SDL_CreateRGBSurfaceFrom(ico_pixels, 16, 16, 16, 16 * 2, 0x0f00, 0x00f0, 0x000f, 0xf000);

    // The icon is attached to the screen pointer
    SDL_SetWindowIcon(screen, surface);

    return 0;
}

uint8_t MTF_fb_exit(framebuffer_dev_type *fb)
{
    SDL_DestroyWindow(screen);

    return 0;
}
