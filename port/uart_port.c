#include "uart_port.h"
#include "MTF_io.h"

#define DEBUG_UART(...) printf(__VA_ARGS__)

static mFILE *f_com = NULL;

HAL_StatusTypeDef MTF_UART_Init(MTF_HandleDef *huart)
{
    char *COMx = "com6";

    DEBUG_UART("This's Serial Port Testing!\n");

    /** Open Com */
    if ((f_com = fopen(COMx, "wb+")) == NULL)
    {
        DEBUG_UART("Open %s Failed!\n", COMx);
        return HAL_ERROR;
    }
    else
    {
        DEBUG_UART("Open %s successd!\n", COMx);
        return HAL_OK;
    }
}

HAL_StatusTypeDef MTF_UART_exit(MTF_HandleDef *huart)
{
    /** close Com */
    fclose(f_com);
    return HAL_OK;
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

    return 0;
}

size_t MTF_UART_Receive(MTF_HandleDef *huart, uint8_t *pData, size_t Size)
{
    // /** Notice: moves the file position indicator to the beginning in a file */
    // fseek(f_com, 0, SEEK_SET);

    // /** waiting a vaild character */
    // return fread(pData, Size, 1, f_com);

    return 0;
}

uint8_t MTF_UART_Transmit_Empty(MTF_HandleDef *huart)
{
    // uint8_t i = 0;
    
    // /** Notice: moves the file position indicator to the beginning in a file */
    // fseek(f_com, 0, SEEK_SET);

    // /** waiting a vaild character */
    // return !fwrite(&i, 1, 1, f_com);

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

    return 1;
}
