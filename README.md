# ch423_for_esp32  
基于沁恒CH423模组开发的ESP32(arduino)控制代码  
## 使用方法  
将头文件及函数文件加入PlatformIO工程,Arduino IDE请自行添加  
### 初始化
```Cpp
CH423 ch423(22, 21);  
``` 
### GPIO读取  
```Cpp
ch423.CH423_WriteByte(CH423_SYS_CMD | BIT_IO_OE);  
uint8_t data = ch423.CH423_ReadByte();  
```  
### GPIO输出  
```Cpp
ch423.CH423_WriteByte(CH423_SYS_CMD);  
ch423.CH423_WriteByte(CH423_SET_IO_CMD|BIT_IO0_DAT); //Set GPIO_0 HIGH  
```  
### GPO输出  
```Cpp
ch423.CH423_WriteByte(CH423_SYS_CMD);  
ch423.CH423_WriteByte(CH423_OC_L_CMD|BIT_OC0_L_DAT);//Set GPO_0 HIGH  
ch423.CH423_WriteByte(CH423_OC_H_CMD|BIT_OC8_L_DAT);//Set GPO_8 HIGH  
```  

