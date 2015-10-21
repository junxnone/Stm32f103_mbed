#include "mbed.h"

//Serial pc(PC_10, PC_11);//UART4
//Serial pc(PC_12, PD_2);//UART5
Serial pc(PD_5, PD_6);//USART2
Serial pc1(PA_9, PA_10);//USART1
DigitalOut myled1(PD_13);

DigitalOut myled2(PG_14);

AnalogOut myDAC1(PA_4);
AnalogOut myDAC2(PA_5);

//改为用12位表示ADC（ADC是12位的），注释了anlogin_api中的 value = ((value << 4) & (uint16_t)0xFFF0) | ((value >> 8) & (uint16_t)0x000F);
AnalogIn myADC1(PA_0);  
AnalogIn myADC2(PA_2);



PwmOut PWM1(PA_8);

PwmOut PWM2(PA_1);

PwmOut PWM3(PA_6);

PwmOut PWM4(PB_6);
  
void task1(void)
{
	static u16 DacValue=0;
	DacValue++;
	if(DacValue>4096)DacValue=0;
	 myDAC1.write_u16(DacValue);  //12位DAC
	 myDAC2.write_u16(2048);
    
		 //用ADC来读取DAC的值并串口显示
		  pc1.printf("DAC1 = %d,DAC2 = %d.\r\n", myADC1.read_u16(),myADC2.read_u16());
}


int main() {
	PWM2.period_ms(20);
	 PWM2.pulsewidth_ms(10);
	
	
	while(1){
		task1();
	}
}


