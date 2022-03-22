#ifdef __WINDOWS__
#include <windows.h>
#endif

#include "types_plus.h"
#include "MTF_io.h"
#include "music_play.h"
#include "framebuffer_port.h"
#include "touch_port.h"
#include "ROM_port.h"
#include "timer_port.h"
#include "uart_port.h"
#include "system_port.h"

int sdl_event_get(void *opaque);
int main(int argc, char *argv[])
{

    render_dev_type *render_front = NULL;
    framebuffer_dev_type framebuffer_var_info; //屏幕信息
    uint32_t color = 0XFFFF0000;

    printf("GOGOGO \n");

    hardware_init_weak();

    /*********SDL display*************/
    MTF_fb_init(&framebuffer_var_info);
	render_front = MTF_fb_render_create(&framebuffer_var_info, framebuffer_var_info.xres, framebuffer_var_info.yres);

    /*************timer init**************/
    MTF_timer_init_handle();

    /*********SDL PCM*************/
    // MP3WAVplay("./aiwer_smile.wav");

    /********rom driver test**************/
    uint8_t rom[] = "fwefsf";
    MTF_ROM_init();
    MTF_unique_id();
    MTF_ROM_write(rom, 1024, sizeof(rom));
    memset(rom, 0, sizeof(rom));
    MTF_ROM_read(rom, 1024, sizeof(rom));
    printf("rom: %s\r\n", rom);
    // MTF_ROM_exit();

    /**********uart driver test****************/
    // uint8_t uart_rec_data[128] = {0};
    // uint8_t uart_send_data[] = "****jo";
    MTF_UART_Init(NULL);
    // MTF_UART_Transmit(NULL, uart_send_data, sizeof(uart_send_data));
    // SDL_Delay(100);
    // MTF_UART_Receive(NULL, uart_rec_data, sizeof(uart_send_data));
    // printf("uart_rec_data: \r\n %s \r\n", uart_rec_data);
    // MTF_UART_exit(NULL);
    
    while (1)
    {
        if (color <= 0XFFFF0000)
            color += 0X00100000;
        else
            color = 0XFFFF0000;

        for (size_t i = 0; i < render_front->width * render_front->height; i++)
        {
            ((uint32_t *)render_front->pixels)[i] = color;
        }
        SDL_Delay(100);

        if(system_process_weak())
            break;
    }

    MTF_fb_destroy(render_front);
    MTF_fb_exit(&framebuffer_var_info);

    // SDL_Quit();
    system_exit_weak();

#ifdef __WINDOWS__
    // system("pause");
#endif

    exit(0);

    return 0;
}
