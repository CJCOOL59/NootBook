#ifndef _DS1302_H_
#define _DS1302_H_

#include <reg52.h>
#include <intrins.h>

//重定义关键字
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

//定义ds1302使用的IO口
sbit DSIO = P3^4;
sbit RST = P3^5;
sbit SCLK = P3^6;

//---定义全局函数---//
void ds1302Write(uchar addr,uchar dat);
uchar ds1302Read(uchar addr);
void ds1302Init();
void ds1302ReadTime();

//---加入全局变量--//
extern uchar TIME[7];	//加入全局变量

#endif