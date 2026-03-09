#include "ch423.h"
#include <Arduino.h>
#include <Wire.h>

CH423::CH423(int CH423_SCL, int CH423_SDA)
{
    this->CH423_SCL = CH423_SCL;
    this->CH423_SDA = CH423_SDA;
}


void CH423::i2cStart()
{
    digitalWrite(CH423_SDA, HIGH);
    pinMode(CH423_SDA, OUTPUT);
    digitalWrite(CH423_SCL, HIGH);
    pinMode(CH423_SCL, OUTPUT);
    delayMicroseconds(5);;
    digitalWrite(CH423_SDA, LOW);
    delayMicroseconds(5);;
    digitalWrite(CH423_SCL, LOW);
    delayMicroseconds(5);;
}

void CH423::i2cStop()
{
    digitalWrite(CH423_SDA, LOW);
    delayMicroseconds(5);;
    digitalWrite(CH423_SCL, HIGH);
    delayMicroseconds(5);;
    digitalWrite(CH423_SDA, HIGH);
    delayMicroseconds(5);;
}

bool CH423::i2cWriteByte(uint8_t dat)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(CH423_SDA, (dat & 0x80) ? HIGH : LOW);
        delayMicroseconds(1);
        digitalWrite(CH423_SCL, HIGH);
        dat <<= 1;
        delayMicroseconds(1);
        digitalWrite(CH423_SCL, LOW);
    }
    pinMode(CH423_SDA, INPUT);
    delayMicroseconds(1);
    digitalWrite(CH423_SCL, HIGH);
    bool ack = (digitalRead(CH423_SDA) == LOW);
    delayMicroseconds(1);
    digitalWrite(CH423_SCL, LOW);
    pinMode(CH423_SDA, OUTPUT);
    return ack;
}

uint8_t CH423::i2cReadByte(bool ack)
{
    unsigned char dat;
    digitalWrite(CH423_SDA, HIGH);
    pinMode(CH423_SDA, INPUT);
    dat = 0;
    for (int i = 0; i < 8; i++)
    {
        delayMicroseconds(5);;
        digitalWrite(CH423_SCL, HIGH);
        delayMicroseconds(5);;
        dat <<= 1;
        if (digitalRead(CH423_SDA))
            dat++; // 输入1位
        digitalWrite(CH423_SCL, LOW);
    }
    pinMode(CH423_SDA, OUTPUT);
    digitalWrite(CH423_SDA, HIGH);
    delayMicroseconds(5);;
    digitalWrite(CH423_SCL, HIGH);
    delayMicroseconds(5);;
    digitalWrite(CH423_SCL, LOW);
    return (dat);
}
void CH423::CH423_WriteByte(unsigned short cmd)
{
    i2cStart();
    i2cWriteByte((unsigned char)(cmd >> 8));
    i2cWriteByte((unsigned char)cmd);
    i2cStop();
}

unsigned char CH423::CH423_ReadByte()
{
    unsigned char din;
    i2cStart();
    i2cWriteByte(CH423_RD_IO_CMD);
    din = i2cReadByte();
    i2cStop();
    return din;
}
bool CH423::setocPin(uint8_t pin, int value)
{
    if (pin > 15)
        return false;

    uint16_t cmd;
    uint8_t data;
    if (pin < 8)
    {
        if (value==HIGH)
            gpo[pin] = 1;
        else
            gpo[pin] = 0;

        data = 0;
        for (uint8_t i = 0; i < 8; i++)
        {
            if (gpo[i] != 0)
            {
                data |= (1 << i);
            }
        }
        cmd = CH423_OC_L_CMD | data;
    }
    else
    {
        uint8_t idx = pin - 8;
        if (value==HIGH)
            gpo[pin] = 1;
        else
            gpo[pin] = 0;

        data = 0;
        for (uint8_t i = 0; i < 8; i++)
        {
            if (gpo[8 + i] != 0)
            {
                data |= (1 << i);
            }
        }
        cmd = CH423_OC_H_CMD | data;
    }
    CH423_WriteByte(cmd);
    return true;
}
bool CH423::setioPin(uint8_t pin, int value)
{
    if (pin > 7)
        return false;
    if ((sys & BIT_IO_OE) == 0)
        return false;
    gpio[pin] = value ? 1 : 0;
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (gpio[i])
            data |= (1 << i);
    }
    uint16_t cmd = CH423_SET_IO_CMD | data;
    CH423_WriteByte(cmd);
    return true;
}
bool CH423::setPort(uint16_t mask)
{
    uint8_t low_byte = mask & 0x00FF;
    uint8_t high_byte = (mask >> 8) & 0x00FF;

    CH423_WriteByte(CH423_OC_L_CMD | low_byte);
    CH423_WriteByte(CH423_OC_H_CMD | high_byte);

    // 更新缓存
    for (uint8_t i = 0; i < 16; i++)
    {
        gpo[i] = (mask & (1 << i)) ? 1 : 0;
    }
    return true;
}

bool CH423::setSystemConfig(uint8_t configFlags)
{
    sys = configFlags;
    CH423_WriteByte(CH423_SYS_CMD | configFlags);
    return true;
}

uint8_t CH423::readInput(uint8_t pin)
{
    if (pin > 7)
        return 0;
    if ((sys & BIT_IO_OE) != 0)
        return 0;
    uint8_t port_val = readPort();
    return (port_val & (1 << pin)) ? 1 : 0;
}

uint8_t CH423::readPort()
{
    uint8_t in_val = CH423_ReadByte();
    for (uint8_t i = 0; i < 8; i++)
    {
        gpio[i] = (in_val & (1 << i)) ? 1 : 0;
    }
    return in_val;
}
