#ifndef __ESP8266_USART_H
#define __ESP8266_USART_H

#ifdef __cplusplus
extern "C" {
#endif

	
	
void ESP8266_Init();	
void ESP8266_UsartRxIrq(void);
void ESP8266_JoinWifi(const char * SSID, const char * passwd);
void ESP8266_SetMode(int Mode);
void ESP8266_Reset();
void ESP8266_SetMuxClient();
void ESP8266_StartServer(uint16_t Port);	
void ESP8266_SendMessage(const char *Msg);
void ESP8266_StartTCPClient(const char *IP,uint16_t Port);
void ESP8266_GetIp();
void ESP8266_StartSerialNet();
void ESP8266_CloseSerialNet();
void ESP8266_Ds18b20UpdateLeWei50();	
void ESP8266_Ds18b20UpdateTlinkIO(float i);	
u8 ESP8266_AckVerify();



#ifdef __cplusplus
}
#endif 

#endif


