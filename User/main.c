/************************************************************************************
* Author		:	H8361
* Date			:	2022-11-26
* Description	:	DHT11 模块，温湿度显示
* Other			:	None
*************************************************************************************/
#include "stm32f10x.h"
#include "OLED_I2C.h"
#include "Delay.h"
#include "DHT11.h"
	
extern unsigned char BMP_TEMP[]; // 温度图标
extern unsigned char BMP_HUMI[]; // 湿度图标

u8 TempH,TempL; // 温度整数+小数
u8 HumiH,HumiL; // 湿度整数+小数
int main(void)
{

	DelayInit();
	I2C_Configuration();
	OLED_Init();
	DHT11_Init();

	OLED_Fill(0xFF);// 全屏点亮
	DelayMs(200);
	OLED_Fill(0x00);// 全屏熄灭
	DelayMs(200);

	OLED_CLS(); // 清屏
	// 显示屏像素为 128*64
	// 所以需要自行计算显示的行列
	OLED_DrawBMP(0,0,32,8,(unsigned char *)BMP_TEMP);//显示温度图标
	OLED_DrawBMP(0,4,32,8,(unsigned char *)BMP_HUMI);//显示湿度图标
	OLED_ShowChar(33,1,':',2);
	OLED_ShowChar(33,5,':',2);
	OLED_ShowStr(41,1,(unsigned char *)"  .   C",2);// ":"占用8个像素宽度，所以下一个开始坐标为 41，° 字符无法显示，故省略
	OLED_ShowStr(41,5,(unsigned char *)"  .   %",2);
	
	while(1)
	{
		// 读取温湿度数据
		if(DHT11_ReadData(&TempH,&TempL,&HumiH,&HumiL)){
			OLED_ShowNum(41,1,TempH,2,2); // 显示温度部分
			OLED_ShowNum(65,1,TempL,2,2);
		
			OLED_ShowNum(41,5,HumiH,2,2); // 显示湿度部分
			OLED_ShowNum(65,5,HumiL,2,2);
			DelayMs(200);
		}else{
			OLED_CLS(); // 清屏
			OLED_ShowStr(32,4,(unsigned char *)"DHT11 Occured Errors !!!",2);
		}
	}
}
