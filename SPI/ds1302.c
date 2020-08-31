#include <ds1302.h>

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
uchar code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};
uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302时钟初始化2020年9月1日星期二12点00分00秒。---//
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
uchar TIME[7] = {0,0,0x12,0x01,0x09,0x01,0x20}; 

void ds1302Write(uchar addr,uchar dat)
{
    uchar n;
    RST = 0;//拉低片选
    _nop_();

    SCLK = 0; //拉低时钟
    _nop_();
    RST = 1;    
    _nop_();

    for (n = 0; n < 8; n++)//8位指令数据
    {
        DSIO = addr & 0x01; //数据从低位开始传送，按位与
        addr >>= 1;     //addr右移1位 
        SCKL = 1;   //上升沿,DS1302读数据
        _nop_();
        SCLK = 0;
        _nop_();
    }
    for ( n = 0; i < 8; n++)//写入8位数据
    {
        DSIO = dat & 0x01;
        dat >> 1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    
    RST = 0;//传送数据完成，拉低片选释放芯片
    _nop_();
}


uchar ds1302Read(uchar addr)
{
    uchar n,dat,dat1;
    RST = 0;//拉低片选
    _nop_();

    SCLK = 0; //拉低时钟
    _nop_();
    RST = 1;    
    _nop_();

    for (n = 0; n < 8; n++)//传送8位地址命令
    {
        DSIO = addr & 0x01;//数据先从低位传送
        addr >>= 1;
        SCLK = 1;//时钟上升沿，DS1302读数据
        _nop_();
        SCLK = 0;
        _nop_();
    }
    _nop_();
    for (n = 0; n < 8; n++)
    {
        dat1 = DSIO;//低位开始接收
        dat = (dat >> 1) | (dat1 << 7);//放到最左边然后，往右移
        SCLK = 1;
        _nop_();
        SCLK = 0;//时钟下降沿，ds1302放数据
        _nop_();
    }

    RST = 0;
    _nop_();    //以下为DS1302复位的稳定时间,必须。
    SCLK = 1;
    _nop_();
    DSIO = 0;
    _nop_();
    DSIO = 1;
    _nop_();
    return dat;
}

void ds1302Init()
{
    uchar n;
    ds1302Write(0x8e,0x00);//关闭写保护
    for(n = 0;n < 7;n++)//写入7个字节的时钟信号：分秒时日月周年
    {
        ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);
    }
    ds1302Write(0x8e,0x00);//打开写保护
}

void ds1302ReadTime()
{
    uchar n;
    for (n = 0; i < 7; n++)//读取7个字节的时钟信号：分秒时日月周年
    {
        TIME[n] = ds1302Read(READ_RTC_ADDR[n]);
    }
    

}