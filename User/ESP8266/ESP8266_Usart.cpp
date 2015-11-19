#include "mbed.h"
#include "stm32f10x.h"
#include "ESP8266_Usart.h"
#include "SysTick.h"
#include "stdio.h"
Serial ESP8266_Usart(PD_5,PD_6);
extern Serial pc1;
#define RX_BUF_LEN 2048
char RxBuf[RX_BUF_LEN];
int RxHead=0;
int RxEnd=0;
char AckBuf[20]={0};
int Step=0;
int AckComLen=0;
u32 ESP8266_TgtSysTick;
u32 ESP8266_SysTickCache;
void ESP8266_Init()
{
	ESP8266_Usart.baud(115200);
	ESP8266_Usart.attach(&ESP8266_UsartRxIrq,SerialBase::RxIrq);
	//ESP8266_Reset();
}
void ESP8266_UsartRxIrq(void)
{
			RxEnd++;
		if(RxEnd>RX_BUF_LEN){
			RxEnd=0;
		}
		if(RxEnd==RxHead){
			pc1.printf("RxBuf is Full\r\n");
			RxHead=0;
			RxEnd=0;
		}
    RxBuf[RxEnd]= ESP8266_Usart.getc();
		pc1.putc(RxBuf[RxEnd]);
}
void ESP8266_JoinWifi(const char * SSID, const char * passwd)
{

}
void ESP8266_SetMode(int Mode)
{

}
void ESP8266_Reset()
{
	ESP8266_Usart.printf("AT+RST\r\n");
}
void ESP8266_SetMuxClient()
{

}
void ESP8266_StartServer(uint16_t Port)
{

}
void ESP8266_SendMessage(const char *Msg)
{

}
void ESP8266_StartTCPClient(const char *IP,uint16_t Port)
{

}
void ESP8266_GetIp()
{

}
void ESP8266_StartSerialNet()
{

}
void ESP8266_CloseSerialNet()
{

}

void ESP8266_WaitmS(u32 Ms)
{
	ESP8266_SysTickCache=GetSysTickLow();
	ESP8266_TgtSysTick=ESP8266_SysTickCache+Ms*100;
}
u8 DelayIsOver()
{
	u32 NowSysTick=GetSysTickLow();
	if(NowSysTick>ESP8266_SysTickCache){
			if(NowSysTick>ESP8266_TgtSysTick){
					return 1;
			}
			return 0;
	}
	else{
		return 1;
	}


}
void ESP8266_ATCmdSend(const char *Cmd,const char *Ack,int AckLen,u32 WaitMs)
{
		ESP8266_Usart.printf(Cmd);
		pc1.printf("%s",Cmd);
		sprintf(AckBuf,"%s",Ack);
		AckComLen=AckLen;
		ESP8266_WaitmS(WaitMs);
}

void DebugStep(u8 Step)
{
		pc1.printf("NowStepis :%d\r\n",Step);
}
static u8 ErrCnt=0;
static u8 ErrTotalCnt=0;
u8 IsErr()
{
		ErrCnt++;
		ErrTotalCnt++;
		if(ErrTotalCnt>10){
			
			return 2;
		}
		if(ErrCnt>5){
			ErrCnt=0;
			return 1;
		}
		else return 0;
}
void ESP8266_Ds18b20UpdateLeWei50(float UpdateData)
{
		static float UpdateDataCache;
	//static u8 Step=0;
  static char CmdBuf[100]={0};
	static char Cmd2Buf[100]={0};
	int i;
	if(DelayIsOver()){
		DebugStep(Step);
		if(0x00==Step){
			ESP8266_ATCmdSend("AT+RST\r\n","ready\r\n",0,3000);
			Step |=0x80;
			return;
		}
		if(0x80==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
					
			}
			else{
				Step &=0x7F;
			}
		}
		if(0x01==Step){
			ESP8266_ATCmdSend("AT+CWJAP=\"Go\",\"xiaomi12\"\r\n","OK\r\n",4,5000);
			Step |=0x80;
			return;
		}
		if(0x81==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=0;
				}
			}
		}
		if(0x02==Step){
			ESP8266_ATCmdSend("AT+CIFSR\r\n","OK\r\n",4,5000);
			Step |=0x80;
			return;
		}
		if(0x82==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=0;
				}
			}
		}
		if(0x03==Step){
			ESP8266_ATCmdSend("AT+CIPSTART=\"TCP\",\"42.121.128.216\",9960\r\n","OK\r\n",4,3000);
			//ESP8266_ATCmdSend("AT+CIPSTART=\"TCP\",\"192.168.31.243\",8647\r\n","OK\r\n",4,3000);
			Step |=0x80;
			return;
		}
		if(0x83==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				//ESP8266_Usart.printf("AT+CIPCLOSE\r\n");
				Step &=0x7F;
				if(IsErr()){
					Step=0;
				}
			}
		}
		if(0x04==Step){
			sprintf(Cmd2Buf,"{\"method\": \"update\",\"gatewayNo\": \"03\",\"userkey\": \"f7c5b45dfca844bfab610cc999529882\"}&^!");
			i=strlen(Cmd2Buf);
			sprintf(CmdBuf,"AT+CIPSEND=%d\r\n",i);
			ESP8266_ATCmdSend(CmdBuf,"OK\r\n> ",6,3000);
			Step |=0x80;
			return;
		}
		if(0x84==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=3;
				}
			}
		}
		if(0x05==Step){
			ESP8266_ATCmdSend(Cmd2Buf,"OK\r\n> ",4,3000);
			Step |=0x80;
			return;
		}
		if(0x85==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=3;
				}
			}
		}
		if(0x06==Step){
			UpdateDataCache=UpdateData;
			sprintf(Cmd2Buf,"{\"method\":\"upload\",\"data\":[{\"Name\":\"PM25\",\"Value\":\"%.1f\"}]}&^!",UpdateDataCache);
			i=strlen(Cmd2Buf);
			sprintf(CmdBuf,"AT+CIPSEND=%d\r\n",i);
			ESP8266_ATCmdSend(CmdBuf,"OK\r\n> ",6,3000);
		
			Step |=0x80;
			return;
		}
		if(0x86==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=3;
				}
			}
		}
		if(0x07==Step){
			//sprintf(CmdBuf,"{\"method\":\"upload\",\"data\":[{\"Name\":\"PM25\",\"Value\":\"%.1f\"}]}&^!",UpdateDataCache);
			ESP8266_ATCmdSend(Cmd2Buf,"OK\r\n",4,3000);
			Step |=0x80;
			return;
		}
		if(0x87==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step=4;
			}
			else{
				Step=3;
				Step &=0x7F;
				
			}
		}
	}
	else{
		return;
	}
}
void ESP8266_Ds18b20UpdateTlinkIO(float Temp)
{
	static float TempCache;
	//static u8 Step=0;
  char CmdBuf[50]={0};
	if(DelayIsOver()){
		DebugStep(Step);
		if(0x00==Step){
			ESP8266_ATCmdSend("AT+RST\r\n","ready\r\n",0,3000);
			Step |=0x80;
			return;
		}
		if(0x80==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
					
			}
			else{
				Step &=0x7F;
			}
		}
		if(0x01==Step){
			ESP8266_ATCmdSend("AT+CWJAP=\"Go\",\"xiaomi12\"\r\n","OK\r\n",4,5000);
			Step |=0x80;
			return;
		}
		if(0x81==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=0;
				}
			}
		}
		if(0x02==Step){
			ESP8266_ATCmdSend("AT+CIFSR\r\n","OK\r\n",4,5000);
			Step |=0x80;
			return;
		}
		if(0x82==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=0;
				}
			}
		}
		if(0x03==Step){
			ESP8266_ATCmdSend("AT+CIPSTART=\"TCP\",\"115.29.170.230\",8647\r\n","OK\r\n",4,3000);
			//ESP8266_ATCmdSend("AT+CIPSTART=\"TCP\",\"192.168.31.243\",8647\r\n","OK\r\n",4,3000);
			Step |=0x80;
			return;
		}
		if(0x83==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				//ESP8266_Usart.printf("AT+CIPCLOSE\r\n");
				Step &=0x7F;
				if(IsErr()){
					Step=0;
				}
			}
		}
		if(0x04==Step){
			ESP8266_ATCmdSend("AT+CIPSEND=16\r\n","OK\r\n> ",6,3000);
			Step |=0x80;
			return;
		}
		if(0x84==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=3;
				}
			}
		}
		if(0x05==Step){
			ESP8266_ATCmdSend("E733B9V08BTS6K63","OK\r\n> ",4,3000);
			Step |=0x80;
			return;
		}
		if(0x85==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=3;
				}
			}
		}
		if(0x06==Step){
			TempCache=Temp;
			if(TempCache<10)
				ESP8266_ATCmdSend("AT+CIPSEND=11\r\n","OK\r\n> ",6,3000);
			else
				ESP8266_ATCmdSend("AT+CIPSEND=12\r\n","OK\r\n> ",6,3000);
			Step |=0x80;
			return;
		}
		if(0x86==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step++;
			}
			else{
				Step &=0x7F;
				if(IsErr()){
					Step=3;
				}
			}
		}
		if(0x07==Step){
			sprintf(CmdBuf,"#RTU,+%3.1f\r\n",TempCache);
			ESP8266_ATCmdSend(CmdBuf,"OK\r\n",4,3000);
			Step |=0x80;
			return;
		}
		if(0x87==Step){
			if(ESP8266_AckVerify()){
					Step &=0x7F;
					Step=4;
			}
			else{
				Step=3;
				Step &=0x7F;
				
			}
		}
	}
	else{
		return;
	}
	
}

void ESP8266_RxBufClear()
{
	RxHead=0;
	RxEnd=0;
}
u8 ESP8266_AckVerify()
{
	int RxEndC=RxEnd;
	while(AckComLen--){
		if(AckBuf[AckComLen]!=RxBuf[RxEndC]){
			//pc1.printf("%s  RxBuf is Error\r\n",RxBuf);	
			ESP8266_RxBufClear();
			return 0;
		}
		if(RxEndC==0)RxEndC=RX_BUF_LEN;
		RxEndC--;
		
	}
	ESP8266_RxBufClear();
	
	return 1;
}




