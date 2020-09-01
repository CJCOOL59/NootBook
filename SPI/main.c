#include <reg52.h>
#include <ds1302.h>

typedef unsigned char u8;
typedef unsigned int u16;

/*
0	000
1	001
2	010
3	011
4	100
5	101
6	110
7	111
*/

sbit LSA = P2^2;		  //低位
sbit LSB = P2^3;
sbit LSC = P2^4;		  //高位

char num = 0;
u8 displayData[8];
/*
code是keil C51里面的关键字，一bai般用于定义常量数组，意思du是告诉编译zhi说把这个数组放在daoROM存储。
code的作用是告诉单片机，定义的数据要放在ROM（程序存储区）里面，写入后就不能再更改。因为C语言中没办法详细描述存入的是ROM还是RAM（寄存器），所以在软件中添加了这一个语句起到代替汇编指令的作用，对应的还有data是存入RAM的意思。
程序可以简单的分为code（程序）区，和data （数据）区，code区在运行的时候是不可以更改的，data区放全局变量和临时变量，是要不断的改变的，cpu从code区读取指令，对data区的数据进行运算处理。
*/
//定义0~9的8段数码管显示
u8 code smgduan[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};

void delay(u16 i)
{
	while(i--);		
}

void dataPros()
{
	ds1302ReadTime();
	displayData[0] = smgduan[TIME[2]/16];				//时
	displayData[1] = smgduan[TIME[2]&0x0f];				 
	displayData[2] = 0x40;
	displayData[3] = smgduan[TIME[1]/16];				//分
	displayData[4] = smgduan[TIME[1]&0x0f];	
	displayData[5] = 0x40;
	displayData[6] = smgduan[TIME[0]/16];				//秒
	displayData[7] = smgduan[TIME[0]&0x0f];
}

void digDisplay()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; break;//显示第3位
			case(4):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(5):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(6):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(7):
				LSA=1;LSB=1;LSC=1; break;//显示第7位	
		}
		P0=displayData[7-i];//发送数据
		delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
	}		
}


void main()
{
	ds1302Init();
	while(1)
	{
		dataPros();
		digDisplay();
		}
}