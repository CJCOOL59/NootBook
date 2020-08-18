#ifndef __I2C_H__
#define __I2C_H__

#include <reg52.h>

sbit SCL = P2^1;
sbit SDA = P2^0;


void i2cStart();
void i2cStop();
unsigned char i2cSendByte(unsigned char dat);
unsigned char i2cReadByte();
void at24c02Write(unsigned char add,unsigned char dat);
unsigned char at24c02Read(unsigned char add);


#endif