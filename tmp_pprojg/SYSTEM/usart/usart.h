#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	


#define false 0
#define true 1

//�������鳤��
#define GPS_Buffer_Length 120
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} _SaveData;


extern _SaveData Save_Data;

void CLR_Buf(void);
void clrStruct(void);
void USART2_CLR_Buf(void); 
void u2_printf(char* cmd);














#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern char  USART1_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern char  USART2_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�


extern u16 USART1_RX_STA;         		//����״̬���	
extern u16 USART2_RX_STA;         		//����״̬���	

extern UART_HandleTypeDef UART1_Handler; //UART���
extern UART_HandleTypeDef UART2_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С

extern char aRxBuffer1[RXBUFFERSIZE];//HAL��USART����Buffer
extern char aRxBuffer2[RXBUFFERSIZE];//HAL��USART����Buffer




//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);
void uart2_init(u32 bound);

#endif
