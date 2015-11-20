#include "mbed.h"
#include "GP2Y1010au.h"

AnalogIn	DustIn(PA_0);
DigitalOut IRLED(PD_14);
#define BASE_VOL 0.15

extern Serial pc1;
void GP2Y1010auInit(void)
{
	
}
u16 GP2Y1010auGetPM25()
{
		u16 DustVal=0;
		float DustVol;
		u16 PM25Val;
		IRLED.write(0);
		wait_us(280);
		DustVal=DustIn.read_u16();
		wait_us(40);
		IRLED.write(1);
		wait_us(9680);
		wait_ms(1000);
		DustVol=DustVal*2*3.3/4096.0;
	
		pc1.printf("DustIn Vol is %0.3f V\r\n",DustVol);
	
		if(DustVol>BASE_VOL){
			PM25Val=(u16)((DustVol-BASE_VOL)*1000/6.1);
		}
		else{
			PM25Val=0;
		}
		return PM25Val;
}



