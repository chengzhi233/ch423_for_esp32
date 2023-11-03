#include "ch423.h"
#include <Arduino.h>

CH423::CH423(int CH423_SCL,int CH423_SDA)
{
    this->CH423_SCL = CH423_SCL;
    this->CH423_SDA = CH423_SDA;
}

void CH423::CH423_I2c_Start() // 操作起始
{
    digitalWrite(CH423_SDA, HIGH); // 发送起始条件的数据信号
    pinMode(CH423_SDA, OUTPUT);    // 设置SDA为输出方向
    digitalWrite(CH423_SCL,HIGH);
    pinMode(CH423_SCL, OUTPUT);// 设置SCL为输出方向
    delay(1);
    digitalWrite(CH423_SDA, LOW); // 发送起始信号
    delay(1);
    digitalWrite(CH423_SCL, LOW);// 钳住I2C总线，准备发送或接收数据
    delay(1);
}

void CH423::CH423_I2c_Stop() // 操作结束
{
    digitalWrite(CH423_SDA, LOW);
    delay(1);
    digitalWrite(CH423_SCL,HIGH);
    delay(1);
    digitalWrite(CH423_SDA, HIGH); // 发送I2C总线结束信号
    delay(1);
}

void CH423::CH423_I2c_WrByte(unsigned char dat) // 写一个字节数据
{
    unsigned char i;
    for (i = 0; i != 8; i++) // 输出8位数据
    {
        if (dat & 0x80)
        {
            digitalWrite(CH423_SDA, HIGH);
        }
        else
        {
            digitalWrite(CH423_SDA, LOW);
        }
        delay(1);
        digitalWrite(CH423_SCL,HIGH);
        dat <<= 1;
        delay(1); // 可选延时
        digitalWrite(CH423_SCL, LOW);
    }
    digitalWrite(CH423_SDA, HIGH);
    delay(1);
    digitalWrite(CH423_SCL,HIGH); // 接收应答
    delay(1);
    digitalWrite(CH423_SCL, LOW);
    ;
}

unsigned char CH423::CH423_I2c_RdByte() // 读一个字节数据
{
    unsigned char dat;
    digitalWrite(CH423_SDA, HIGH);
    pinMode(CH423_SDA, INPUT); // 设置SDA为输入方向
    dat = 0;
    for (int i = 0; i < 8; i++) // 输入8位数据
    {
        delay(1); // 可选延时
        digitalWrite(CH423_SCL,HIGH);
        delay(1); // 可选延时
        dat <<= 1;
        if (digitalRead(CH423_SDA))
            dat++; // 输入1位
        digitalWrite(CH423_SCL, LOW);
    }
    pinMode(CH423_SDA, OUTPUT);
    digitalWrite(CH423_SDA, HIGH);
    delay(1);
    digitalWrite(CH423_SCL,HIGH); // 发出无效应答
    delay(1);
    digitalWrite(CH423_SCL, LOW);
    return (dat);
}

void CH423::CH423_Write(unsigned short cmd) // 写命令
{
    CH423_I2c_Start(); // 启动总线
    CH423_I2c_WrByte(((unsigned char)(cmd >> 7) & CH423_I2C_MASK) | CH423_I2C_ADDR1);
    CH423_I2c_WrByte((unsigned char)cmd); // 发送数据
    CH423_I2c_Stop();                     // 结束总线
}

void CH423::CH423_WriteByte(unsigned short cmd) // 写出数据
{
    CH423_I2c_Start(); // 启动总线
    CH423_I2c_WrByte((unsigned char)(cmd >> 8));
    CH423_I2c_WrByte((unsigned char)cmd); // 发送数据
    CH423_I2c_Stop();                     // 结束总线
    // Wire.beginTransmission(0x40);
    // Wire.write((unsigned char)(cmd >> 8));
    // Wire.write((unsigned char)cmd);
    // Wire.endTransmission();

}

unsigned char CH423::CH423_ReadByte() // 读取数据
{
    unsigned char din;
    CH423_I2c_Start();                 // 启动总线
    CH423_I2c_WrByte(CH423_RD_IO_CMD); // 此值为0x4D
    din = CH423_I2c_RdByte();          // 读取数据
    CH423_I2c_Stop();                  // 结束总线
    return din;
}
