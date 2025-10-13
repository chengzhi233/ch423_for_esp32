
#ifndef CH423IF_H
#define CH423IF_H

#include <Arduino.h>

/*  设置系统参数命令 */

#define CH423_SYS_CMD 0x4800    // 设置系统参数命令
#define BIT_SLEEP 0x80          // 低功耗睡眠,输入电平变化/操作命令唤醒并复位
#define BIT_INTENS100B 0x00     // 显示驱动亮度占空比4/4，但外部串接限流电阻
#define BIT_INTENS50 0x40       // 显示驱动亮度占空比2/4
#define BIT_INTENS25 0x20       // 显示驱动亮度占空比1/4
#define BIT_INTENS100A 0x00     // 显示驱动亮度占空比4/4
#define BIT_OD_EN 0x10          // 使能开漏输出，为0推挽输出；为1开漏输出
#define BIT_X_INT 0x08          // 使能输入电平变化中断，为0禁止输入电平变化中断；为1并且DEC_H为0允许OC15输出电平变化中断
#define BIT_DEC_H 0x04          // 控制开漏输出引脚高8位的片选译码
#define BIT_DEC_L 0x02          // 控制开漏输出引脚低8位的片选译码
#define BIT_IO_OE 0x01          // 控制双向输入输出引脚的三态输出，为1允许输出

#define CH423_OC_L_CMD 0x4400   // 设置低8位(0~7)输出命令,为0输出低电平，为1输出高电平
#define CH423_OC_H_CMD 0x4600   // 设置高8位(8~15)输出命令,为0输出低电平，为1输出高电平
#define CH423_SET_IO_CMD 0x6000 // 设置双向输入输出命令
#define BIT_IO0_DAT 0x01        // 写入双向输入输出引脚的输出命令，为0输出低电平，为1输出高电平
#define CH423_RD_IO_CMD 0x4D    // 读取I/O引脚当前状态

class CH423
{
private:
    uint8_t CH423_SCL, CH423_SDA; // 2线接口的连接,与实际电路有关
    uint8_t gpo[16] = {0};        // 记录输出状态
    uint8_t gpio[8] = {0};        // 记录输入状态
    uint8_t sys = 0;              // 记录系统状态
    void i2cStart();
    void i2cStop();
    bool i2cWriteByte(uint8_t dat);
    uint8_t i2cReadByte(bool ack = false);
    void CH423_WriteByte(unsigned short cmd);
    unsigned char CH423_ReadByte(void);

public:
    CH423(int CH423_SCL, int CH423_SDA);
    bool setocPin(uint8_t pin, int value);
    bool setioPin(uint8_t pin, int value);
    bool setPort(uint16_t mask);
    bool setSystemConfig(uint8_t configFlags);
    uint8_t readInput(uint8_t pin);
    uint8_t readPort();
};
#endif
