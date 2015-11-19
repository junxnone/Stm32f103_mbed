#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#define U32_MAX	(0xffffffff)
void SysTick_Init(void);
void Delay_10us(__IO u32 nTime);	  //单位10us
u32 GetSysTickLow(void);
#define Delay_ms(x) Delay_10us(100*x)	 //单位ms

#ifdef __cplusplus
}
#endif 
	
	
#endif /* __SYSTICK_H */
