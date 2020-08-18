#include <reg52.h>
#include <i2c.h>

typedef unsigned int u16;
typedef unsigned char u8;

sbit k1 = P2^1;
sbit k2 = P2^0;
sbit k3 = P2^2;
sbit k4 = P2^3;

sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

u8 smgduan[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
u8 num = 0;
u8 disp[4];

void delay(u16 i)
{
    while(i--);
}

void keyProse()
{
    if (k1 == 0)//把num写入AT24C02的地址1
    {
        delay(1000);
        if (k1 == 0)
        {
            ac24c02Write(1,num);
        }
        while(!k1);
    }
    if (k2 == 0)//读取AT24C02地址1中的字节到num中
    {
        delay(1000);
        if (k2 == 0)
        {
            num = ac24c02read(1);
        }
        while(!k2);
    }
    if (k3 == 0)//对num累加
    {
        delay(1000);
        if (k3 == 0)
        {
            num++;
            if ( num > 255)//if不带花括号只执行后面紧跟的一条语句
            num = 0;
        }
        while(!k3);
    }
    if (k4 == 0)//num清0
    {
        delay(1000);
        if (k4 == 0)
        num = 0;
        while(!k4);
    }   
}

dataPors()
{
    disp[0] = smgduan[num/1000];//千位
    disp[1] = smgduan[num%1000/100];//百味
    disp[2] = smgduan[num%1000&100/10];//十位
    disp[3] = smgduan[num%1000%100%10];//各位
}

void digDisplay()//38译码器使能
{
    u8 i;
    for (i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            LSA = 0; LSB = 0; LSC = 0;//各位
            break;
        case 1:
            LSA = 1; LSB = 0; LSC = 0;
            break;
        case 2:
            LSA = 0; LSB = 1; LSC = 0;
            break;
        case 3:
            LSA = 1; LSB = 1; LSC = 0;
            break;
        default:
            break;
        }
    P0 = disp[3-i];//发送段码
    delay(100);//扫描间隔
    P0 = 0x00;//数码管消隐
    }
    
}

void mian()
{
    while (1)
    {
        keyProse();
        dataPors();
        digDisplay();
    }
    
}