#include "ROM_port.h"
#include "MTF_io.h"

#define DEBUG_ROM(...) printf(__VA_ARGS__)

/*************get cpu base info******************/
/**********
 * from: https://www.cnblogs.com/zyl910/archive/2012/08/06/getcpuid_gcc.html
 * ********/
// intrinsics
#if defined(__GNUC__)    // GCC
#include <cpuid.h>
#elif defined(_MSC_VER)    // MSVC
    #if _MSC_VER >=1400    // VC2005
#include <intrin.h>
    #endif    // #if _MSC_VER >=1400
#else
#error Only supports MSVC or GCC.
#endif    // #if defined(__GNUC__)

static void getcpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
{
#if defined(__GNUC__)    // GCC
    __cpuid_count(InfoType, ECXValue, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#elif defined(_MSC_VER)    // MSVC
    #if defined(_WIN64) || _MSC_VER>=1600    // 64位下不支持内联汇编. 1600: VS2010, 据说VC2008 SP1之后才支持__cpuidex.
        __cpuidex((int*)(void*)CPUInfo, (int)InfoType, (int)ECXValue);
    #else
        if (NULL==CPUInfo)    return;
        _asm{
            // load. 读取参数到寄存器.
            mov edi, CPUInfo;    // 准备用edi寻址CPUInfo
            mov eax, InfoType;
            mov ecx, ECXValue;
            // CPUID
            cpuid;
            // save. 将寄存器保存到CPUInfo
            mov    [edi], eax;
            mov    [edi+4], ebx;
            mov    [edi+8], ecx;
            mov    [edi+12], edx;
        }
    #endif
#endif    // #if defined(__GNUC__)
}

static void getcpuid(unsigned int CPUInfo[4], unsigned int InfoType)
{
#if defined(__GNUC__)    // GCC
    __cpuid(InfoType, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#elif defined(_MSC_VER)    // MSVC
    #if _MSC_VER>=1400    // VC2005才支持__cpuid
        __cpuid((int*)(void*)CPUInfo, (int)InfoType);
    #else
        getcpuidex(CPUInfo, InfoType, 0);
    #endif
#endif    // #if defined(__GNUC__)
}

// 取得CPU厂商（Vendor）.
//
// result: 成功时返回字符串的长度（一般为12）。失败时返回0.
// pvendor: 接收厂商信息的字符串缓冲区。至少为13字节.
static int cpu_getvendor(char* pvendor)
{
    unsigned int dwBuf[4];
    if (NULL==pvendor)    return 0;
    // Function 0: Vendor-ID and Largest Standard Function
    getcpuid(dwBuf, 0);
    // save. 保存到pvendor
    *(unsigned int *)&pvendor[0] = dwBuf[1];    // ebx: 前四个字符.
    *(unsigned int *)&pvendor[4] = dwBuf[3];    // edx: 中间四个字符.
    *(unsigned int *)&pvendor[8] = dwBuf[2];    // ecx: 最后四个字符.
    pvendor[12] = '\0';
    return 12;
}

// 取得CPU商标（Brand）.
//
// result: 成功时返回字符串的长度（一般为48）。失败时返回0.
// pbrand: 接收商标信息的字符串缓冲区。至少为49字节.
static int cpu_getbrand(char* pbrand)
{
    unsigned int dwBuf[4];
    if (NULL==pbrand)    return 0;
    // Function 0x80000000: Largest Extended Function Number
    getcpuid(dwBuf, 0x80000000U);
    if (dwBuf[0] < 0x80000004U)    return 0;
    // Function 80000002h,80000003h,80000004h: Processor Brand String
    getcpuid((unsigned int *)&pbrand[0], 0x80000002U);    // 前16个字符.
    getcpuid((unsigned int *)&pbrand[16], 0x80000003U);    // 中间16个字符.
    getcpuid((unsigned int *)&pbrand[32], 0x80000004U);    // 最后16个字符.
    pbrand[48] = '\0';
    return 48;
}

int test_get_cpu_info(int argc, char* argv[])
{
    unsigned int dwBuf[4];
    char szBuf[64];

    // test
    getcpuidex(dwBuf, 0,0);
    // getcpuid(dwBuf, 0);
    DEBUG_ROM("CPU ID: %.8X\t%.8X\t%.8X\t%.8X\n", dwBuf[0],dwBuf[1],dwBuf[2],dwBuf[3]);
    
    cpu_getvendor(szBuf);
    DEBUG_ROM("CPU Vendor:\t%s\n", szBuf);

    cpu_getbrand(szBuf);
    DEBUG_ROM("CPU Name:\t%s\n", szBuf);

    return 0;
}
/*********************************************/

static mFILE *file_ROM = NULL;

uint8_t MTF_ROM_init(void)
{
    file_ROM = MTF_open("./build/mtf_rom", "wb+"); //读写生成打开
    if(file_ROM != NULL)
        return 0;
    else
        return 1;
}

uint8_t MTF_ROM_exit(void)
{
    return (uint8_t)MTF_close(file_ROM); //close and save file
}

uint64_t MTF_unique_id(void) /*64bit唯一ID*/
{
    unsigned int dwBuf[4];

    getcpuidex(dwBuf, 0,0);
    DEBUG_ROM("CPU ID: %.8X\t%.8X\t%.8X\t%.8X\n", dwBuf[0],dwBuf[1], dwBuf[2], dwBuf[3]);

    return (uint64_t)dwBuf[2]<<32|dwBuf[3];
}

uint8_t MTF_ROM_read(uint8_t *pBuffer, size_t ReadAddr, size_t NumByteToRead) //读取flash
{
    MTF_seek(file_ROM, (long int)ReadAddr, SEEK_SET);
    MTF_read(pBuffer, 1, (size_t)NumByteToRead, file_ROM);
    // return 1;; //fail
    return 0; //success
}

uint8_t MTF_ROM_write(uint8_t *pBuffer, size_t WriteAddr, size_t NumByteToWrite) //写入flash
{
    MTF_seek(file_ROM, (long int)WriteAddr, SEEK_SET);
    MTF_write(pBuffer, 1, (size_t)NumByteToWrite, file_ROM);
    return 0; //success
}

uint8_t MTF_ROM_user_data_erase(void) //擦除用户数据
{
    /*FLASH ADDR*/
    //擦除扇区
    uint8_t s[14*1024*1024] = {0};
    MTF_seek(file_ROM, 2*1024*1024, SEEK_SET);
    MTF_write(s, 1, (size_t)sizeof(s), file_ROM);
    // W25QXX_Erase_Sector_Range(512, 4096 - 512); //FATFS区域擦书, 加快读写速度, 超慢
    // W25QXX_Erase_Chip(); //整片芯片刷除, 比删除部分扇区快
    return 0;
}
