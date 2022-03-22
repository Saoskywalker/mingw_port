#ifndef _ROM_PORT_H
#define _ROM_PORT_H

#include "types_base.h"

uint8_t MTF_ROM_init(void);
uint8_t MTF_ROM_exit(void);
uint64_t MTF_unique_id(void); /*64bit唯一ID*/
uint8_t MTF_ROM_read(uint8_t *pBuffer, size_t ReadAddr, size_t NumByteToRead); //读取flash
uint8_t MTF_ROM_write(uint8_t *pBuffer, size_t WriteAddr, size_t NumByteToWrite); //写入flash
uint8_t MTF_ROM_user_data_erase(void); //擦除用户数据

#endif
