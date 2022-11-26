#ifndef __DHT11_H__
#define __DHT11_H__

/* 参考：https://blog.csdn.net/weixin_52531699/article/details/119737442*/
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/* 定义使用 GPIOA 及其 Pin1(A1) 引脚 ，可以根据自己的情况来设置*/
#define GPIO_DHT11 GPIOA
#define DHT11_PIN GPIO_Pin_1 
void DHT11_IO_IN(void);
void DHT11_IO_OUT(void);
void DHT11_RST(void);
u8 DHT11_Check(void);
u8 DHT11_Init(void);
u8 DHT11_ReadBit(void);
u8 DHT11_ReadByte(void);
u8 DHT11_ReadData(u8 *TempH,u8 *TempL,u8 *HumiH,u8 *HumiL);

#endif
