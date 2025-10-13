# CH423_for_ESP32

基于沁恒 CH423 模块的 ESP32 (Arduino) 控制库
支持 **I2C 两线接口** 方式直接控制 CH423 的输出（GPO）和双向IO引脚。

> CH423 具备 16 个开漏/推挽输出（OC0~OC15）和 8 个双向 IO 引脚（IO0~IO7），可通过系统参数控制亮度、开漏/推挽输出模式、中断等功能。

---

## 📦 使用方法

### 1. 添加到工程
将 `ch423.h` 和 `ch423.cpp` 添加到你的 **PlatformIO** 或 **Arduino IDE** 项目中。

---

### 2. 初始化对象
```cpp
#include "ch423.h"

// SCL 接 22, SDA 接 21（根据实际电路修改）
CH423 ch423(22, 21);
```

---

### 3. 系统参数设置
比如开启 IO 输出功能（BIT_IO_OE）：
```cpp
ch423.setSystemConfig(BIT_IO_OE);        // 允许双向 IO 输出
ch423.setSystemConfig(BIT_OD_EN);        // 开启开漏输出模式
```

---

### 4. GPO输出控制
```cpp
ch423.setocPin(0, HIGH);   // 设置 OC0 高电平
ch423.setocPin(8, LOW);    // 设置 OC8 低电平

// 一次性设置整个 GPO 端口（16 位）
ch423.setPort(0x00FF);     // OC0~OC7 高电平，OC8~OC15 低电平
```

---

### 5. GPIO输出
> 注意：必须先开启双向 IO 输出 (BIT_IO_OE) 才能设置电平
```cpp
ch423.setSystemConfig(BIT_IO_OE);
ch423.setioPin(0, HIGH);   // IO0 输出高电平
ch423.setioPin(1, LOW);    // IO1 输出低电平
```

---

### 6. GPIO 读取
> 注意：必须关闭输出功能才能读取输入电平
```cpp
ch423.setSystemConfig(0);               // 禁止 IO 输出 (BIT_IO_OE = 0)
uint8_t val = ch423.readInput(0);       // 读取 IO0 电平
uint8_t portVal = ch423.readPort();     // 读取整个 IO 端口(8 位数据)
```

---

## 系统参数位说明

| 宏定义               | 说明 |
|---------------------|------|
| BIT_SLEEP           | 低功耗睡眠模式 |
| BIT_INTENS100B      | 显示亮度占空比 4/4（外部限流） |
| BIT_INTENS50        | 显示亮度占空比 2/4 |
| BIT_INTENS25        | 显示亮度占空比 1/4 |
| BIT_OD_EN           | 开漏输出模式 |
| BIT_X_INT           | 输入电平变化中断允许 |
| BIT_DEC_H           | 高 8 位开漏译码 |
| BIT_DEC_L           | 低 8 位开漏译码 |
| BIT_IO_OE           | 双向 IO 输出允许 |

---

## 🔗 参考
- [沁恒 CH423 数据手册](http://www.wch.cn/products/CH423.html)

---