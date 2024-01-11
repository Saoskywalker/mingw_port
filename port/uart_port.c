/*
Copyright (c) 2019-2023 Aysi 773917760@qq.com. All right reserved
Official site: www.mtf123.club

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

It under the terms of the Apache as published;
either version 2 of the License,
or (at your option) any later version.
*/

#include "uart_port.h"
#include "MTF_io.h"
#include "Serial.h"
#include <SDL2/SDL.h>
#include "system_port.h"
#include "fifo.h"

#define DEBUG_UART(...) printf(__VA_ARGS__)

/****************
 * 此uart库因接收时等待需无数据超时后才跳出, 为避免阻塞在此, 需要开辟线程
 * ************/
#define UART_REC_FIFO_SIZE 1024
struct fifo_t *uart_rec_fifo = NULL;
static uint8_t _uart_thread_start = 0;

#ifndef __EMSCRIPTEN__

static int uart_thread(void *arg)
{
    size_t r = 0;
    uint8_t temp_rec[UART_REC_FIFO_SIZE];

    while (system_get_state() == 0)
    {
        if (_uart_thread_start)
        {
            serialRead(temp_rec, sizeof(temp_rec), &r);
            fifo_put(uart_rec_fifo, temp_rec, r);
            for (size_t i = 0; i < r; i++)
            {
                DEBUG_UART("%#X ", temp_rec[i]);
            }
        }
        SDL_Delay(2); // delay 2ms
    }

    return 0;
}

// static mFILE *f_com = NULL;

HAL_StatusTypeDef MTF_UART_Init(MTF_HandleDef *huart)
{
    // /** Open Com */
    // char *COMx = "com6";
    // if ((f_com = fopen(COMx, "wb+")) == NULL)
    // {
    //     DEBUG_UART("Open %s Failed!\n", COMx);
    //     return HAL_ERROR;
    // }
    // else
    // {
    //     DEBUG_UART("Open %s successd!\n", COMx);
    //     return HAL_OK;
    // }

    uint8_t res = 0, c = huart->addr + '0', str[] = {'c', 'o', 'm', '0', 0};
    if (c < '0' && c > '9')
        c = '6';
    str[3] = c;    
    res = serialOpen((const char *)str, (int)huart->Init.BaudRate, (char)huart->Init.Parity, 
                    (char)huart->Init.WordLength, (char)huart->Init.StopBits, 0);
    if(res)
    {
        uart_rec_fifo = NULL;
        _uart_thread_start = 0;
    }
    else
    {
        uart_rec_fifo = fifo_alloc(UART_REC_FIFO_SIZE);
        DEBUG_UART("open serial: %c, res: %d\n", c, res);
        SDL_CreateThread(uart_thread, NULL, NULL); //创建线程
        _uart_thread_start = 1; //开始线程
    }

    return res;
}

#else

HAL_StatusTypeDef MTF_UART_Init(MTF_HandleDef *huart)
{
    return 0;
}

#endif

HAL_StatusTypeDef MTF_UART_exit(MTF_HandleDef *huart)
{
    /** close Com */
    // fclose(f_com);
    // return HAL_OK;

    if (_uart_thread_start == 0)
        return 0;

#ifndef __EMSCRIPTEN__
    _uart_thread_start = 0; //暂停线程
    SDL_Delay(5);
    fifo_free(uart_rec_fifo);
    return serialClose();
#else
    return 0;
#endif
}

HAL_StatusTypeDef MTF_UART_Reset(MTF_HandleDef *huart)
{
    return HAL_OK;
}

size_t MTF_UART_Transmit(MTF_HandleDef *huart, uint8_t *pData, size_t Size)
{
    // /** Notice: moves the file position indicator to the beginning in a file */
    // fseek(f_com, 0, SEEK_SET);

    // /** Send "Hello,world!" */
    // return fwrite(pData, Size, 1, f_com);

    if (_uart_thread_start == 0)
        return 0;

    size_t r = 0;
#ifndef __EMSCRIPTEN__    
    serialWrite(pData, Size, &r);
#endif

    for (size_t i = 0; i < Size; i++)
    {
        DEBUG_UART("%#X ", pData[i]);
    }

    return r;
}

size_t MTF_UART_Receive(MTF_HandleDef *huart, uint8_t *pData, size_t Size)
{
    // /** Notice: moves the file position indicator to the beginning in a file */
    // fseek(f_com, 0, SEEK_SET);

    // /** waiting a vaild character */
    // return fread(pData, Size, 1, f_com);

    if (_uart_thread_start == 0)
        return 0;
#ifndef __EMSCRIPTEN__
    return fifo_get(uart_rec_fifo, pData, (unsigned int)Size);
#else
    return 1;
#endif
}

uint8_t MTF_UART_Receive_FIFO_Count(MTF_HandleDef *huart)
{
    if (_uart_thread_start == 0)
        return 0;
#ifndef __EMSCRIPTEN__        
    return (uint8_t)fifo_len(uart_rec_fifo);
#else
    return 1;
#endif
}

uint8_t MTF_UART_Transmit_Empty(MTF_HandleDef *huart)
{
    // uint8_t i = 0;
    
    // /** Notice: moves the file position indicator to the beginning in a file */
    // fseek(f_com, 0, SEEK_SET);

    // /** waiting a vaild character */
    // return !fwrite(&i, 1, 1, f_com);
    DEBUG_UART("\r\n");
    return 1;
}

uint8_t MTF_UART_Receive_Empty(MTF_HandleDef *huart)
{
    // uint8_t i = 0, res = 0;

    // /** Notice: moves the file position indicator to the beginning in a file */
    // fseek(f_com, 0, SEEK_SET);

    // /** waiting a vaild character */
    // res = (uint8_t)fread(&i, 1, 1, f_com);

    // /** Notice: moves the file position indicator to the beginning in a file */
    // fseek(f_com, 0, SEEK_SET);
    DEBUG_UART("send uart: ");
    return 0;
}
