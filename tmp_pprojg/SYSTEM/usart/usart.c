#include "usart.h"
#include "delay.h"
#include "string.h"

#if 1
#pragma import(__use_no_semihosting)   

//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;


//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 



//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
char USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
char USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.


//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���	
u16 USART2_RX_STA=0;       //����״̬���	

char aRxBuffer1[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
char aRxBuffer2[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���



UART_HandleTypeDef UART1_Handler; //UART���
UART_HandleTypeDef UART2_Handler; //UART���








_SaveData Save_Data;
















//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}


//��ʼ��IO ����2
//bound:������
void uart2_init(u32 bound)
{	
	//UART ��ʼ������
	UART2_Handler.Instance=USART2;					    //USART1
	UART2_Handler.Init.BaudRate=bound;				    //������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}







//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure1;
	GPIO_InitTypeDef GPIO_Initure2;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure1.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure1.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure1.Pull=GPIO_PULLUP;			//����
		GPIO_Initure1.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure1.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure1);	   	//��ʼ��PA9

		GPIO_Initure1.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure1);	   	//��ʼ��PA10
		
#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);		//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);	//��ռ���ȼ�3�������ȼ�3
#endif	
	}
	
	if(huart->Instance==USART2)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure2.Pin=GPIO_PIN_2;			//PA9
		GPIO_Initure2.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure2.Pull=GPIO_PULLUP;			//����
		GPIO_Initure2.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure2.Alternate=GPIO_AF7_USART2;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure2);	   	//��ʼ��PA9

		GPIO_Initure2.Pin=GPIO_PIN_3;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure2);	   	//��ʼ��PA10
		
#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART2_IRQn);		//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn,2,3);	//��ռ���ȼ�3�������ȼ�3
#endif	
	}
}





#if 1
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)//����Ǵ���1
	{
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer2[0]!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer2[0]==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=aRxBuffer2[0] ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}


	}
	
		if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART1_RX_STA&0x8000)==0)//����δ���
		{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer1[0]!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer1[0]==0x0d)USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=aRxBuffer1[0] ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}

	}
	
	
	
	
}

//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;

	
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>maxDelay) break;	
	}

} 


 
//����1�жϷ������
void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;

	
	HAL_UART_IRQHandler(&UART2_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>maxDelay) break;	
	}

} 
#endif	


void USART2_CLR_Buf(void)                           // ���ڻ�������
{
	memset(USART2_RX_BUF, 0, USART_REC_LEN);      //���
	USART2_RX_STA = 0;                    
}


//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* cmd)  
{  
	u8 len = 0;
	len = strlen(cmd);

	HAL_UART_Transmit(&UART2_Handler,(uint8_t*)cmd,len,1000);	
	while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET);		

	
}




void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}

