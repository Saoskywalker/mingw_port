#ifndef __FATTESTER_H
#define __FATTESTER_H

#include "types_plus.h"

uint8_t MTF_read_dir(void);
u8 mf_scan_files(u8 *path);
u8 mf_scan_files2(u8 *path, char *ptr[], u8 *cnt);
// u32 mf_showfree(u8 *drv);
uint8_t MTF_disk_fromat(uint8_t *path, uint8_t mode, uint16_t au);
void MTF_disk_get_label(uint8_t *path);
void MTF_disk_set_label(uint8_t *path);
u8 mf_copy(u8 *psrc, u8 *pdst, u8 fwmode);
u8 mf_dcopy(u8 *psrc, u8 *pdst, u8 fwmode);
uint8_t MTF_exist_file(char * path, const char *name);
uint8_t MTF_load_file(char **out, long *outsize, const char *filename);
uint8_t MTF_disk_get_free(uint8_t *drv,uint32_t *total,uint32_t *free); //得到磁盘总容量和剩余容量

#endif
