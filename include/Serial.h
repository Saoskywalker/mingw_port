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

#ifndef _WZSERIALPORT_H
#define _WZSERIALPORT_H

#ifdef __cplusplus

#include <iostream>
using namespace std;
/*
作者：欧阳伟
日期：2017-12-14
类名：WZSerialPort
用途：串口读写
示例：
WZSerialPort w;
if(w.open("COM1",9600，0，8，1))
{
w.send("helloworld",10);
char buf[1024];
w.receive(buf,1024);
}
*/
 
class WZSerialPort
{
public:
	WZSerialPort();
	~WZSerialPort();
 
	// 打开串口,成功返回true，失败返回false
	// portname(串口名): 在Windows下是"COM1""COM2"等，在Linux下是"/dev/ttyS1"等
	// baudrate(波特率): 9600、19200、38400、43000、56000、57600、115200 
	// parity(校验位): 0为无校验，1为奇校验，2为偶校验，3为标记校验
	// databit(数据位): 4-8，通常为8位
	// stopbit(停止位): 1为1位停止位，2为2位停止位,3为1.5位停止位
	// synchronizable(同步、异步): 0为异步，1为同步
	bool open(const char* portname, int baudrate = 115200, char parity = 0, char databit = 8, char stopbit = 1, char synchronizeflag = 0);
 
	//关闭串口，参数待定
	void close();
 
	//发送数据或写数据，成功返回发送数据长度，失败返回0
	int send(string dat);
 
	//接受数据或读数据，成功返回读取实际数据的长度，失败返回0
	string receive();
 
private:
	int pHandle[16];
	char synchronizeflag;
};

#endif

//C++和C互调用时需要
//extern "C" 为 C++特性, extern “C”是由Ｃ＋＋提供的一个连接交换指定符号，
//用于告诉Ｃ＋＋这段代码是Ｃ函数。extern “C”后面的函数不使用的C++的名字修饰,而是用C。
//这是因为C++编译后库中函数名会变得很长，与C生成的不一致，也就是命名规则不一样, 造成不能调用
//extern "C" 是一种链接约定，通过它可以实现兼容C与C++之间的相互调用，即对调用函数能够达成一致的意见，
//使用统一的命名规则，使得实现方提供的接口和调用方需要的接口经按照指定规则编译后，得到的都是一致的函数符号命名。
#ifdef __cplusplus
extern "C" {
#endif

unsigned char serialOpen(const char *portname, int baudrate, char parity, char databit, char stopbit, char synchronizeflag);
unsigned char serialClose(void);
unsigned char serialRead(unsigned char *d, size_t order_size, size_t *result_size);
unsigned char serialWrite(unsigned char *d, size_t order_size, size_t *result_size);
int test_serial(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif
 
#endif
