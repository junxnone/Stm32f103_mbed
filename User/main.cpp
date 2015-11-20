#include "mbed.h"
#include "ds18b20.h"
#include "ESP8266_Usart.h"
#include "SysTick.h"
#include "GP2Y1010au.h"

Serial pc1(PA_9, PA_10);//USART1
DigitalOut myled2(PG_14);

int main() {
	u16 PM25Val;
	short temperature;
	
	SysTick_Init();	
	pc1.baud(115200);
	ESP8266_Init();
	while(1){
		PM25Val=GP2Y1010auGetPM25();		
		pc1.printf("Dust Value is: %d ug/m3\r\n",PM25Val);
		ESP8266_Ds18b20UpdateLeWei50(PM25Val);
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

