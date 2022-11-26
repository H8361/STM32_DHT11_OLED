#include "DHT11.h"

/**
 * @brief	DHT11 输入模式
 * @param	None
 * @retval 	None
 *
 */
void DHT11_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin =DHT11_PIN ; // 输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}


/**
 * @brief	DHT11 输出模式
 * @param	None
 * @retval 	None
 *
 */
void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出 // 上拉输入
	GPIO_InitStructure.GPIO_Pin =DHT11_PIN ; // 输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);
}


/**
 * @brief	重置DHT11
 * @param	None
 * @retval 	None
 *
 */
void DHT11_RST(void)
{
	
	
	DHT11_IO_OUT(); // 数值输出模式
	GPIO_ResetBits(GPIO_DHT11,DHT11_PIN); // 拉低电平
	DelayMs(20); // 拉低至少 18ms
	GPIO_SetBits(GPIO_DHT11,DHT11_PIN);
	DelayUs(30); // 主机拉高20 - 40 us
	GPIO_ResetBits(GPIO_DHT11,DHT11_PIN); // 拉低电平
}


/**
 * @brief	检测DHT11 的响应信号
 * @param	None
* @retval 	1:DHT11工作正常；0:DHT11出错
 *
 */
u8 DHT11_Check(void)
{
	u8 retry = 0;
	DHT11_IO_IN(); // 设置输入模式
	// DHT11 会拉低 40 - 50us
	while(GPIO_ReadInputDataBit(GPIO_DHT11,DHT11_PIN) && retry <100){
		retry++;
		DelayUs(1);
	}
	
	if(retry >= 100)
	{
		return 1;
	}else{
		retry = 0;
	}
	
	// DHT11 会再次拉低 40 - 50 us
	while(GPIO_ReadInputDataBit(GPIO_DHT11,DHT11_PIN) && retry < 100){
		retry++;
		DelayUs(1);
	}
	
	// 超时 DHT11 工作出错，退出检测
	if(retry >= 100) 
	{
		return 0; 
	}
	
	return 1; // 设备工作正常
	
}


/**
 * @brief	DHT11 初始化
* @param	None
* @retval 	1:表示DHT11工作正常；0:表示DHT11有错误
 *
 */
u8 DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	DHT11_IO_OUT(); // 设置输出模式
	DHT11_RST();  // 发送开始信号
	return DHT11_Check();
}

/**
 * @brief	读取1bit的数据
 * @param	None 
 * @retval 	0:低电平；1:高电平
 *
 */
u8 DHT11_ReadBit(void)
{
	u8 retry = 0;
	// 等待变为低电平(等待bit开始信号)，拉低电平的时间为80us
	while(GPIO_ReadInputDataBit(GPIO_DHT11,DHT11_PIN) && retry<100){
		retry++;
		DelayUs(1);
	}
	
	retry = 0;
	
	// 等待变为高电平（拉高电平的时间为80us,之后开始传输数据,这里等待了100us）
	while(!GPIO_ReadInputDataBit(GPIO_DHT11,DHT11_PIN) && retry < 100){
		retry++;
		DelayUs(1);
	}
	
	// 1bit 开始
	DelayUs(30); 
	
	// 读取数据
	// 0信号的时长为 26-28us
	// 1信号的时长为 70us
	// 所以超过30us去读就可以直到是 0信号还是 1信号
	if(GPIO_ReadInputDataBit(GPIO_DHT11,DHT11_PIN) == SET){
		return 1;
	}
	return 0;
}

/**
 * @brief	读取1byte的数据
 * @param	None
 * @retval 	返回读取的字节
 *
 */
u8 DHT11_ReadByte(void)
{
	u8 i = 0,Byte = 0;
	for(;i<8;i++){
		Byte <<= 1;
		Byte |= DHT11_ReadBit();
	}
	return Byte;
}

/**
 * @brief	读取温湿度数据
 * @param	TempH: 传入参数，记录温度的整数部分
 * @param	TempL: 传入参数，记录温度的小数部分
 * @param	HumiH: 传入参数，记录温度的整数部分
 * @param	HumiL: 传入参数，记录温度的小数部分
 * @retval 	0:表示读取数据失败；1:表示读取数据成功
 *
 */
u8 DHT11_ReadData(u8 *TempH,u8 *TempL,u8 *HumiH,u8 *HumiL)
{
	u8 Buf[5];
	u8 i;
	DHT11_RST();
	
	if(DHT11_Check() == 1)
	{
		for(i = 0;i <5;i++){
			Buf[i] =DHT11_ReadByte();
		}
		if(Buf[0] + Buf[1] + Buf[2] + Buf[3] == Buf[4]){
			*HumiH = Buf[0];
			*HumiL = Buf[1];
			*TempH = Buf[2];
			*TempL = Buf[3];
		}
	}else{
		return 0; // 设置未响应
	}
	return 1; // 设备读取数据成功
}
