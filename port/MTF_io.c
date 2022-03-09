/********************************************
 * 用于平替stdio.h功能, 增加可移植性
 * http://www.mtf123.club/
 * Copyright © MTF. All Rights Reserved
********************************************/

#include "MTF_io.h"

mFILE *cache_open(const char *filename, const char *mode)
{
   return fopen(filename, mode);
}

int cache_close(mFILE *stream)
{
    return fclose(stream);
}

size_t cache_read(void *ptr, size_t nmemb, mFILE *stream)
{
    return fread(ptr, 1, nmemb, stream);
}

size_t cache_write(const void *ptr, size_t nmemb, mFILE *stream)
{
    return fwrite(ptr, 1, nmemb, stream);
}

long int cache_tell(mFILE *stream)
{
    return ftell(stream);
}

int cache_lseek(mFILE *stream, long int offset, int whence)
{
    return fseek(stream, offset, whence);
}

/*****************************************/

/************************
 * ferror，函数名，在调用各种输入输出函数（如 putc.getc.fread.fwrite等）时，
 * 如果出现错误，除了函数返回值有所反映外，还可以用ferror函数检查。 
 * 它的一般调用形式为 ferror(fp)；如果ferror返回值为0（假），表示未出错。
 * 如果返回一个非零值，表示出错。应该注意，对同一个文件 每一次调用输入输出函数，
 * 均产生一个新的ferror函 数值，因此，应当在调用一个输入输出函数后立即检 
 * 查ferror函数的值，否则信息会丢失。在执行fopen函数时，ferror函数的初始值自动置为0。
 * **********************/
int MTF_error(mFILE *stream)
{
    return ferror(stream);
}

mFILE *MTF_open(const char *filename, const char *mode)
{
    return fopen(filename, mode);
}

int MTF_close(mFILE *stream)
{
    return fclose(stream);
}

size_t MTF_read(void *ptr, size_t size, size_t nmemb, mFILE *stream)
{
    return fread(ptr, size, nmemb, stream);
}

size_t MTF_write(const void *ptr, size_t size, size_t nmemb, mFILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

int MTF_seek(mFILE *stream, long int offset, int whence)
{
    return fseek(stream, offset, whence);
}

long int MTF_tell(mFILE *stream)
{
    return ftell(stream);
}

int MTF_remove(const char *filename)
{
    return remove(filename);
}

int MTF_rename(const char *old_filename, const char *new_filename)
{
    return rename(old_filename, new_filename);
}

size_t MTF_size(mFILE *stream)
{
    if (fseek(stream, 0, SEEK_END) != 0)
        return 0;
    else
        return (size_t)ftell(stream);
}

/**********************
 * 返回值 当文件指针到达文件结尾时，函数返回1，否则返回0.
 * ********************/
int MTF_eof(mFILE *stream)
{
    return feof(stream);
}

/*******************************
 * 挂载文件系统, 包括: 文件系统初始化和存储设备初始化
 * ******************************/	
mMount file_system_set[2] = {{2, NULL}, {3, NULL}};
int MTF_mount(mMount *dest)
{
    return 0;
}
