#include "uart_port.h"
#include "MTF_io.h"
#include "Serial.h"

#define DEBUG_UART(...) printf(__VA_ARGS__)

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
    DEBUG_UART("open serial: %c, res: %d\n", c, res);
    return res;
}

HAL_StatusTypeDef MTF_UART_exit(MTF_HandleDef *huart)
{
    /** close Com */
    // fclose(f_com);
    // return HAL_OK;

    return serialClose();
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

    size_t r = 0;
    serialWrite(pData, Size, &r);

    for (size_t i = 0; i < Size; i++)
    {
        DEBUG_UART("%#X ", pData[i]);
    }

    return r;
}

static uint8_t temp_rec[1024];
static size_t temp_rec_cnt = 0, temp_rec_cnt_p = 0;

size_t MTF_UART_Receive(MTF_HandleDef *huart, uint8_t *pData, size_t Size)
{
    // /** Notice: moves the file position indicator to the beginning in a file */
    // fseek(f_com, 0, SEEK_SET);

    // /** waiting a vaild character */
    // return fread(pData, Size, 1, f_com);

    size_t i = 0;
    while (temp_rec_cnt > 0 && temp_rec_cnt_p < temp_rec_cnt && i < Size)
    {
        pData[i] = temp_rec[temp_rec_cnt_p];
        temp_rec_cnt_p++;
        i++;
    }
    return i;
}

uint8_t MTF_UART_Receive_FIFO_Count(MTF_HandleDef *huart)
{
    size_t r = 0;
    serialRead(temp_rec, sizeof(temp_rec), &r);
    temp_rec_cnt = r;
    temp_rec_cnt_p = 0;
    for (size_t i = 0; i < r; i++)
    {
        DEBUG_UART("%#X ", temp_rec[i]);
    }

    return (uint8_t)r;
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
