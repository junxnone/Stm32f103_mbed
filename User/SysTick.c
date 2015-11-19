/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：SysTick.c
 * 描述    ：SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
 *           常用的有 1us 10us 1ms 中断。         
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------
 *          |                 |
 *          |      无         |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelayHigh;
static __IO u32 TimingDelayLow;
/*
 * 函数名：SysTick_Init
 * 描述  ：启动系统滴答定时器 SysTick
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用 
 */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */	
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	//SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	TimingDelayHigh=0;
	TimingDelayLow=0;
	
	
}


/*
 * 函数名：Delay_10us
 * 描述  ：us延时程序,10us为一个单位
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：Delay_10us( 1 ) 则实现的延时为 1 * 10us = 10us
 *       ：外部调用 
 */

void Delay_10us(__IO u32 nTime)
{ 
	u32 TimingDelay = TimingDelayLow+nTime;	

	// 使能滴答定时器  
	//SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay >=TimingDelayLow);
	
}

void SysTick_Handler(void)
{
	TimingDelayLow++;
	if(U32_MAX==TimingDelayLow){
		TimingDelayLow=0;
		TimingDelayHigh++;		
	}
}

u32 GetSysTickLow(void)
{
	return TimingDelayLow;
}



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
