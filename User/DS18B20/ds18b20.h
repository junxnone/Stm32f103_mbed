#ifndef __DS18B20_H
#define __DS18B20_H 

#ifdef __cplusplus
extern "C" {
#endif
#include "mbed.h"

#define	DS18B20_APB		RCC_APB2Periph_GPIOC
#define DS18B20_PORT	GPIOC
#define DS18B20_PIN 	GPIO_Pin_11

//IO方向设置
#define DS18B20_IO_IN() Ds18b20_1.input();//{GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define DS18B20_IO_OUT() Ds18b20_1.output();// {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}
////IO操作函数											   
#define	DS18B20_DQ_OUT PCout(11) //数据端口	PA0 
#define	DS18B20_DQ_IN  PCin(11)  //数据端口	PA0 
   	
u8 DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20    

#ifdef __cplusplus
}
#endif 

#endif















