#include "mbed.h"
#include "ds18b20.h"
#include "ESP8266_Usart.h"
#include "SysTick.h"
Serial pc1(PA_9, PA_10);//USART1

DigitalOut myled1(PD_13);
DigitalOut myled2(PG_14);

int main() {
	float Temp=0;
	short temperature;
	u32 SysTick;
	SysTick_Init();
	
	pc1.baud(115200);
	ESP8266_Init();
	
	
	while(1){
		Temp++;
		if(Temp>35)Temp=0;
		wait_ms(1000);
		//pc1.printf("SYSTICK: %d\r\n",GetSysTickLow());
		ESP8266_Ds18b20UpdateTlinkIO(Temp);
	}
	
	pc1.printf("Start DS18B20 Init ...");
	while(DS18B20_Init())
		{
		wait_ms(1000); 
		pc1.printf("DS18B20 Init Ing...");
	}
	while(1){
	temperature=DS18B20_Get_Temp();	
			pc1.printf("Get Temperature : %.1f\n",temperature/10.0);
		wait_ms(1000);
		
	}
		

}

