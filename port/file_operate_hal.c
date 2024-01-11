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

#include "file_operate_hal.h" 
#include "MTF_io.h"
#include "malloc.h"		  
#include "string.h"

// #define file_operate_debug(...) printf(__VA_ARGS__)
#define file_operate_debug(...)

//为磁盘注册工作区	 
//path:磁盘路径，比如"0:"、"1:"
//mt:0，不立即注册（稍后注册）；1，立即注册
//返回值:执行结果
// u8 mf_mount(u8* path,u8 mt)
// {		   
// 	return f_mount(&fs[1],(const TCHAR*)path,mt); 
// }

//打读取文件夹
//返回值:执行结果
uint8_t MTF_read_dir(void)
{
	return 0;
}			 

 //遍历文件
 //path:路径
 //返回值:执行结果
u8 mf_scan_files(u8 * path)
{
    return 0;	  
}

 //遍历文件到内存
 //path: 路径, ptr: 存放数组
 //返回值:执行结果
u8 mf_scan_files2(u8 * path, char *ptr[], u8 *cnt)
{
    return 0;	  
}

//显示剩余容量
//drv:盘符
//返回值:剩余容量(字节)
/* u32 mf_showfree(u8 *drv)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//得到总扇区数
	    fre_sect = fre_clust * fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		if(tot_sect<20480)//总容量小于10M
		{
		    //Print free space in unit of KB (assuming 512 bytes/sector)
		    file_operate_debug("\r\n磁盘总容量:%d KB\r\n"
		           "可用空间:%d KB\r\n",
		           tot_sect>>1,fre_sect>>1);
		}else
		{
		    //Print free space in unit of KB (assuming 512 bytes/sector)
		    file_operate_debug("\r\n磁盘总容量:%d MB\r\n"
		           "可用空间:%d MB\r\n",
		           tot_sect>>11,fre_sect>>11);
		}
	}
	return fre_sect;
}	 */	    

//格式化
//path:磁盘路径，比如"0:"、"1:"
//mode:模式
//au:簇大小
//返回值:执行结果
uint8_t MTF_disk_fromat(uint8_t* path,uint8_t mode,uint16_t au)
{
	return 0; //f_mkfs((const TCHAR*)path,mode,au);//格式化,drv:盘符;mode:模式;au:簇大小
} 

//获取盘符（磁盘名字）
//path:磁盘路径，比如"0:"、"1:"  
void MTF_disk_get_label(uint8_t *path)
{
	u8 buf[20];
	u32 sn=0;
	u8 res = 0;
	// res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==0)
	{
		file_operate_debug("\r\n磁盘%s 的盘符为:%s\r\n",path,buf);
		file_operate_debug("磁盘%s 的序列号:%X\r\n\r\n",path,sn); 
	}else file_operate_debug("\r\n获取失败，错误码:%X\r\n",res);
}

//设置盘符（磁盘名字），最长11个字符！！，支持数字和大写字母组合以及汉字等
//path:磁盘号+名字，比如"0:ALIENTEK"、"1:OPENEDV"  
void MTF_disk_set_label(uint8_t *path)
{
	u8 res = 0;
	// res=f_setlabel ((const TCHAR *)path);
	if(res==0)
	{
		file_operate_debug("\r\n磁盘盘符设置成功:%s\r\n",path);
	}else file_operate_debug("\r\n磁盘盘符设置失败，错误码:%X\r\n",res);
} 

//文件复制(需用的malloc)
//将psrc文件,copy到pdst.
//psrc,pdst:源文件和目标文件
//fwmode:文件写入模式
//0:不覆盖原有的文件
//1:覆盖原有的文件
u8 mf_copy(u8 *psrc, u8 *pdst, u8 fwmode)
{
/* 	#define FATFS_COPY_ONCE_SIZE 4096 //等于存储芯片单位刷写大小或成倍数关系, 写入才快速

	UINT bw = 0, br = 0;
	FIL *fsrc = 0;
	FIL *fdst = 0;
	u8 *fbuf = 0;
	u8 res = 0;
	u32 total, freeStorage = 0;
	u8 dp[] = {0, ':', 0};
	
	dp[0] = pdst[0];
    MTF_disk_get_free(&dp[0],&total,&freeStorage); //获取磁盘容量情况
	total *= 1024; //KB->B
	freeStorage *= 1024;

	fsrc = (FIL *)malloc(sizeof(FIL)); //申请内存
	fdst = (FIL *)malloc(sizeof(FIL));
	fbuf = (u8 *)malloc(FATFS_COPY_ONCE_SIZE);
	if (fsrc == NULL || fdst == NULL || fbuf == NULL)
	{
		res = 100; //前面的值留给fatfs
	}
	else
	{
		if (fwmode == 0)
			fwmode = FA_CREATE_NEW; //不覆盖
		else
			fwmode = FA_CREATE_ALWAYS; //覆盖存在的文件

		res = f_open(fsrc, (const TCHAR *)psrc, FA_READ | FA_OPEN_EXISTING); //打开只读文件
		if (res == 0)
		{
			// file_operate_debug("storage: total %dB, free %dB, size %dB\r\n", total, freeStorage, f_size(fsrc));
			res = f_open(fdst, (const TCHAR *)pdst, FA_READ); //只读打开存在文件
			if(res==0) //存在
			{
				if(f_size(fsrc)<=freeStorage+f_size(fdst)-32*1024) //保留32k
				{
					f_close(fdst); //先关闭
					res = f_open(fdst, (const TCHAR *)pdst, 
								FA_WRITE | fwmode); //第一个打开成功,才开始打开第二个
				}
				else
				{
					res = FR_DENIED; //磁盘满
				}
			}
			else if(f_size(fsrc)<=freeStorage-32*1024) //确认磁盘有空间, 保留32k
			{
				res = f_open(fdst, (const TCHAR *)pdst, 
							FA_WRITE | fwmode); //第一个打开成功,才开始打开第二个
			}
			else
			{
				res = FR_DENIED; //磁盘满
			}
		
			if (res == 0)					//两个都打开成功了
			{
				while (res == 0) //开始复制
				{
					res = f_read(fsrc, fbuf, FATFS_COPY_ONCE_SIZE, &br); //源头读出
					if (res || br == 0)
						break;
					res = f_write(fdst, fbuf, br, &bw); //写入目的文件
					if (res || bw < br)
						break;
				}
				f_close(fsrc);
				f_close(fdst);
			}
			else
			{
				f_close(fsrc); //第二个打开失败,关闭第一个后退出
			}
		}
	}
	free(fsrc); //释放内存
	free(fdst);
	free(fbuf);
	return res; */
	return 0;
}

//得到路径下的文件夹
//返回值:0,路径就是个卷标号.
//    其他,文件夹名字首地址
static u8 *get_src_dname(u8 *dpfn)
{
	u16 temp = 0;
	while (*dpfn != 0)
	{
		dpfn++;
		temp++;
	}
	if (temp < 4)
		return 0;
	while ((*dpfn != 0x5c) && (*dpfn != 0x2f))
		dpfn--; //追述到倒数第一个"\"或者"/"处
	return ++dpfn;
}

//文件夹复制(需用到malloc)
//将psrc文件夹,copy到pdst文件夹.
//pdst:必须形如"X:"/"X:XX"/"X:XX/XX"之类的.而且要实现确认上一级文件夹存在
//psrc,pdst:源文件夹和目标文件夹
//fwmode:文件写入模式
//0:不覆盖原有的文件
//1:覆盖原有的文件
u8 mf_dcopy(u8 *psrc, u8 *pdst, u8 fwmode)
{
/* #define MAX_PATHNAME_DEPTH 512 + 1 //最大目标文件路径+文件名深度
	u8 res = 0;
	DIR *srcdir = 0;	//源目录
	DIR *dstdir = 0;	//源目录
	FILINFO *finfo = 0; //文件信息
	u8 *fn = 0;			//长文件名

	u8 *dstpathname = 0; //目标文件夹路径+文件名
	u8 *srcpathname = 0; //源文件夹路径+文件名

	u16 dstpathlen = 0; //目标路径长度
	u16 srcpathlen = 0; //源路径长度

	srcdir = (DIR *)malloc(sizeof(DIR)); //申请内存
	dstdir = (DIR *)malloc(sizeof(DIR));
	finfo = (FILINFO *)malloc(sizeof(FILINFO));

	if (srcdir == NULL || dstdir == NULL || finfo == NULL)
		res = 100;
	if (res == 0)
	{
		finfo->lfsize = _MAX_LFN * 2 + 1;
		finfo->lfname = malloc(finfo->lfsize); //申请内存
		dstpathname = malloc(MAX_PATHNAME_DEPTH);
		srcpathname = malloc(MAX_PATHNAME_DEPTH);
		if (finfo->lfname == NULL || dstpathname == NULL || srcpathname == NULL)
			res = 101;
		if (res == 0)
		{
			dstpathname[0] = 0;
			srcpathname[0] = 0;
			strcat((char *)srcpathname, (const char *)psrc); //复制原始源文件路径
			strcat((char *)dstpathname, (const char *)pdst); //复制原始目标文件路径
			res = f_opendir(srcdir, (const TCHAR *)psrc);	//打开源目录
			if (res == FR_OK)									 //打开目录成功
			{
				strcat((char *)dstpathname, (const char *)"/"); //加入斜杠
				fn = get_src_dname(psrc);
				if (fn == 0) //卷标拷贝
				{
					dstpathlen = strlen((const char *)dstpathname);
					dstpathname[dstpathlen] = psrc[0]; //记录卷标
					dstpathname[dstpathlen + 1] = 0;   //结束符
				}
				else
					strcat((char *)dstpathname, (const char *)fn); //加文件名
				res = f_mkdir((const TCHAR *)dstpathname);		   //如果文件夹已经存在,就不创建.如果不存在就创建新的文件夹.
				if (res == FR_EXIST)
					res = 0;
				while (res == 0) //开始复制文件夹里面的东东
				{
					res = f_readdir(srcdir, finfo); //读取目录下的一个文件
					if (res != FR_OK || finfo->fname[0] == 0)
						break; //错误了/到末尾了,退出
					if (finfo->fname[0] == '.')
						continue;												//忽略上级目录
					fn = (u8 *)(*finfo->lfname ? finfo->lfname : finfo->fname); //得到文件名
					dstpathlen = strlen((const char *)dstpathname);				//得到当前目标路径的长度
					srcpathlen = strlen((const char *)srcpathname);				//得到源路径长度

					strcat((char *)srcpathname, (const char *)"/"); //源路径加斜杠
					if (finfo->fattrib & 0X10)						//是子目录   文件属性,0X20,归档文件;0X10,子目录;
					{
						strcat((char *)srcpathname, (const char *)fn);					  //源路径加上子目录名字
						file_operate_debug("\r\ncopy folder %s to %s\r\n", srcpathname, dstpathname); //拷贝文件
						res = mf_dcopy(srcpathname, dstpathname, fwmode);				  //拷贝文件夹
					}
					else //非目录
					{
						strcat((char *)dstpathname, (const char *)"/");					//目标路径加斜杠
						strcat((char *)dstpathname, (const char *)fn);					//目标路径加文件名
						strcat((char *)srcpathname, (const char *)fn);					//源路径加文件名
						file_operate_debug("\r\ncopy file %s to %s\r\n", srcpathname, dstpathname); //拷贝文件
						mf_copy(srcpathname, dstpathname, fwmode);						//复制文件
					}
					srcpathname[srcpathlen] = 0; //加入结束符
					dstpathname[dstpathlen] = 0; //加入结束符
				}
				f_closedir(srcdir);
			}
			free(dstpathname);
			free(srcpathname);
			free(finfo->lfname);
		}
	}
	free(srcdir);
	free(dstdir);
	free(finfo);
	return res; */
	return 0;
}

 //查找一个文件是否存在
 //path:路径, nanme: 文件名
 //返回值:执行结果 0:存在, 其他:不存在
uint8_t MTF_exist_file(char * path, const char *name)
{
	char k[64] = {0};
	uint8_t res = 1;
    mFILE *ff = NULL;

    strcat(k, path);
    strcat(k, name);

    ff = MTF_open(k, "rb");
    if (ff != NULL)
    {
        MTF_close(ff);
        res = 0;
    }

    return res;	  
}

//将一个文件加载至指定内存
/* load file into buffer that already has the correct allocated size. Returns error code.*/
static uint8_t _buffer_file(unsigned char *out, size_t size, const char *filename)
{
	FILE *file;
	size_t readsize;
	file = MTF_open(filename, "rb");
	if (!file)
		return 78;

	readsize = MTF_read(out, 1, size, file);
	MTF_close(file);

	if (readsize != size)
		return 78;
	return 0;
}

uint8_t MTF_load_file(unsigned char **out, size_t *outsize, const char *filename)
{
	size_t size = 0;
	FILE *file;

	file = MTF_open(filename, "rb");
	if (!file)
		return 78;

	size = MTF_size(file);

	MTF_close(file);

	if (size < 0)
		return 78;

	*outsize = (size_t)size;

	*out = (unsigned char *)malloc((size_t)size);
	if (!(*out) && size > 0)
		return 83; /*the above malloc failed*/

	return _buffer_file(*out, (size_t)size, filename);
}

//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
uint8_t MTF_disk_get_free(uint8_t *drv,uint32_t *total,uint32_t *free)
{
	uint8_t res = 0;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
	// FATFS *fs1;
    // res =(uint32_t)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
//     if(res==0)
// 	{											   
// 	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
// 	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
// #if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
// 		tot_sect*=fs1->ssize/512;
// 		fre_sect*=fs1->ssize/512;
// #endif	  
// 		*total=tot_sect>>1;	//单位为KB
// 		*free=fre_sect>>1;	//单位为KB 
//  	}
	return res;
}	
