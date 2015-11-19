#include "mbed.h"
#include "ds18b20.h"
#include "ESP8266_Usart.h"
#include "SysTick.h"
Serial pc1(PA_9, PA_10);//USART1

DigitalOut IRLED(PD_14);
DigitalOut myled2(PG_14);
AnalogIn	DustIn(PA_0);


int main() {
	float Temp=0;
	u16 DustVal=0;
	float DustVol;
	float PM25Val;
	short temperature;
	char Cmd1Buf[100]={0};
	u32 SysTick;
	SysTick_Init();
	
	pc1.baud(115200);
	ESP8266_Init();
	
	
	while(1){
		Temp++;
		if(Temp>35)Temp=0;
		IRLED.write(0);
		wait_us(280);
		DustVal=DustIn.read_u16();
		wait_us(40);
		IRLED.write(1);
		wait_us(9680);
		wait_ms(1000);
		DustVol=DustVal*2*3.3/4096.0;
		pc1.printf("DustIn Vol is %0.3f V\r\n",DustVol);
		if(DustVol>0.45){
			PM25Val=(DustVol-0.45)*1000/6.1;
			pc1.printf("Dust Value is: %0.2fug/m3\r\n",PM25Val);
		}
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

