#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	


#define false 0
#define true 1

//定义数组长度
#define GPS_Buffer_Length 120
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} _SaveData;


extern _SaveData Save_Data;

void CLR_Buf(void);
void clrStruct(void);
void USART2_CLR_Buf(void); 
void u2_printf(char* cmd);














#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern char  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern char  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符


extern u16 USART1_RX_STA;         		//接收状态标记	
extern u16 USART2_RX_STA;         		//接收状态标记	

extern UART_HandleTypeDef UART1_Handler; //UART句柄
extern UART_HandleTypeDef UART2_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小

extern char aRxBuffer1[RXBUFFERSIZE];//HAL库USART接收Buffer
extern char aRxBuffer2[RXBUFFERSIZE];//HAL库USART接收Buffer




//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart2_init(u32 bound);

#endif
