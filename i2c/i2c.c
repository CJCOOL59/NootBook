#include <i2c.h>

void delay10us(void)   //12MHz晶振误差 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=2;a>0;a--);
}

/**********************************
    启动信号：时钟线高电平时间，数据线下降沿
**********************************/
void i2cStart()
{
    SDA = 1;
    delay10us();
    SCL = 1;
    delay10us();
    SDA = 0;
    delay10us();
    SCL = 0;
    delay10us();
}

/**********************************
    停止信号：时钟线高电平时间，数据线上升沿
**********************************/
void i2cStop()
{
    SDA = 0;
    delay10us();
    SCL = 1;
    delay10us();
    SDA = 1;
    delay10us();
}

/**********************************
    发送字节：成功返回1，失败返回0
**********************************/
unsigned char i2cSendByte(unsigned char dat)
{
    unsigned int count = 0;
    unsigned char dat = 0;
    for (unsigned int i = 0; i < 8; i++)
    {
        SDA = dat >> 7;
        dat = dat << 1;
        delay10us();
        SCL = 1;
        delay10us();
        SCL = 0;//低电平下次数据可以更改
        delay10us();
    }
    //释放总线
    SDA = 1;
    delay10us();
    SCL = 1;
    //等待应答
    while (SDA)//sda为低电平时表示应答，跳出循环
    {
        count++;
        if(count > 200)//大大超出4us,无应答
        {
            SCL = 0;
            delay10us();
            return 0;
        }
    }
    SCL = 0;
    delay10us();
    return 1;  
}

/**********************************
    读取字节
**********************************/
unsigned char i2cReadByte()
{
    unsigned char dat = 0;
    for (unsigned int i = 0; i < 8; i++)
    {
        SCL = 1;
        delay10us();
        dat <<= 1;
        dat |= SDA;//按位或  dat = dat | SAD
        delay10us();
        SCL = 0;//读到数据后拉低时钟线，等待数据改变
        delay10us();
    } 
    return dat; 
}


/**********************************
i2c读写过程
写：
    S起始信号/
    写控制字/A/
    写入首地址/A/
    DATA1/A/DATA2/A.../DATAn/A/
    P终止信号
读：
    S起始信号/
    伪写控制字/A/
    发送要读的首地址/A/
    S起始信号/
    写控制字/A/
    DATA1/A/DATA2/A.../DATAn/A/
    P终止信号
控制字：
    高7位为slave的地址，最低位（1：读，0：写）
**********************************/


/**********************************
    AT24C02写、读
**********************************/
void at24c02Write(unsigned char add,unsigned char dat)
{
    i2cStart();
    i2cSendByte(0xa0); //0000 1010
    i2cSendByte(add);
    i2cSendByte(dat);
    i2cStop();
}

unsigned char at24c02Read(unsigned char add)
{
    unsigned char dat;
    i2cStart();
    i2cSendByte(0xa0);
    i2cSendByte(add);
    i2cStart();
    i2cSendByte(0xa1);
    dat = i2cReadByte();
    i2cStop();
    return dat;
}

