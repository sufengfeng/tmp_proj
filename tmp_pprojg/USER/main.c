#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "string.h"
#include "adc.h"

//����
#define Success 1U
#define Failure 0U

//�������
unsigned long  Time_Cont = 0;       //��ʱ��������
unsigned int count = 0;

char phoneNumber[] = "";		//�滻����Ҫ������绰�ĺ���
char msg[] = "I";		//��������	

char TCPServer[] = "103.46.128.49";		//TCP��������ַ
char Port[] = "28304";						//�˿�	



void phone(char *number);
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
void sendMessage(char *number,char *msg);
void parseGpsBuffer(void);
void printGpsBuffer(void);


void postGpsDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, char* lon_temp, char* lat_temp);


double adcx;
int main(void)
{
    char *height;
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart1_init(115200);              //��ʼ��USART
		uart2_init(9600);              //��ʼ��USART
		MY_ADC_Init();
	delay_ms(1000);

#if 1
		if (sendCommand("AT\r\n", "OK\r\n", 500, 3) == Success);
	else;//rorLog(1);
	delay_ms(10);
	
//Ϊ�˲��忨����ʹ��GPS��������������ָ������SIM��ʱ������ͷ�
 	if (sendCommand("AT+CPIN?\r\n", "+CPIN: READY", 500, 3) == Success);
 	else ;//errorLog(3);
 	delay_ms(10);

 	if (sendCommand("AT+CREG?\r\n", ",1", 500, 3) == Success);	//����SIM��
 //	else if(sendCommand("AT+CREG?\r\n", ",5", 500, 3) == Success	);//����SIM��
 	//else;//errorLog(3);
 	delay_ms(10);
	//if (sendCommand("AT+QISTAT\r\n", "IP", 1000, 3) == Success);	//����SIM��
 	//else;//errorLog(3);
 	delay_ms(10);

	//if (sendCommand("AT+QGNSSC?\r\n", "+QGNSSC: 1", 5000, 3) == Success);
 if (sendCommand("AT+QGNSSC=1\r\n", "OK\r\n", 2000, 5) == Success);
	//else;//eorLog(5);

	clrStruct();
	while(1)
	{
		adcx=Get_Adc_Average(ADC_CHANNEL_5, 3);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
		adcx = adcx *0.3174 +200; // ��λ mm ��෶Χ200 ~ 1500
	sprintf(height,"%f", adcx);
	
		if (sendCommand("AT+QGNSSRD=\"NMEA/RMC\"\r\n", "OK\r\n", 2000, 3) == Success);
		else; //errorLog(6);
		

			Save_Data.isGetData = true;
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
			memcpy(Save_Data.GPS_Buffer, USART2_RX_BUF, (USART2_RX_STA&0x3fff));
			
		
		printf("\r\n==========Save_Data.GPS_Buffer==========\r\n");
		printf(Save_Data.GPS_Buffer); //������յ�����Ϣ
		printf("\r\n");

		
		parseGpsBuffer();
		printGpsBuffer();
		delay_ms(10);
		postGpsDataToOneNet("Http1.0","dev01", height, Save_Data.longitude, Save_Data.latitude);
		delay_ms(500);
			clrStruct();
	}
#endif


}



void parseGpsBuffer(void)
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
			;
				//	errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
				//rrorLog(2);	//��������
				}
			}


		}
	}
}

void printGpsBuffer(void)
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

void sendMessage(char *number,char *msg)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else;//rorLog(6);

	if (sendCommand(msg, msg, 3000, 10) == Success);
	else; //orLog(7);

	memset(send_buf, 0, 20);    //���
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else; //orLog(8);
}



void phone(char *number)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //���
	strcpy(send_buf, "ATD");
	strcat(send_buf, number);
	strcat(send_buf, ";\r\n");

	if (sendCommand(send_buf, "OK\r\n", 10000, 10) == Success);
	else; //orLog(4);
}


#if 0
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	char *ret;
	unsigned char n;
	USART2_CLR_Buf();
	
	for (n = 0; n < 3; n++)
	{
		u2_printf(Command); 		//����GPRSָ��
		
		printf("\r\n***************send****************\r\n");
		printf(Command);
		
		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 500;
			ret = strstr(USART2_RX_BUF, Response);
			printf("strstr = %s\r\n", ret);
			if (ret != NULL)
			{	

				printf("\r\n***************success receive****************\r\n");
				printf(USART2_RX_BUF);
				return Success;		

			}

		}
		Time_Cont = 0;
	}


	
	printf("\r\n***************failed receive****************\r\n");
	printf(USART2_RX_BUF);
	USART2_CLR_Buf();
	return Failure;

	
}
#endif

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	char *ret;
	unsigned char n;
	USART2_CLR_Buf();
	
	for (n = 0; n < Retry; n++)
	{
		u2_printf(Command); 		//����GPRSָ��
		delay_ms(Timeout);
		printf("\r\n***************send****************\r\n");
		printf(Command);
		ret = strstr(USART2_RX_BUF, Response);
		
		if(ret != NULL)
		{
			printf("\r\n***************receive****************\r\n");
			printf("ret = %s\r\n", ret);
			return Success;
		}
		

	}


	
	printf("\r\n***************receive****************\r\n");
	printf(USART2_RX_BUF);
//	USART2_CLR_Buf();
	return Failure;

	
}






void postGpsDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, char* lon_temp, char* lat_temp)
{
	char send_buf[400] = {0};
	char text[200] = {0};
	char tmp[25] = {0};

	char lon_str_end[15] = {0};
	char lat_str_end[15] = {0};

	char sendCom[2] = {0x1A};

	static int onced = 0;
//	dtostrf(longitudeToOnenetFormat(lon_temp), 3, 6, lon_str_end); //ת�����ַ������
//	dtostrf(latitudeToOnenetFormat(lat_temp), 2, 6, lat_str_end); //ת�����ַ������

//	lon_temp = "11224.4992";
//	lat_temp = "3438.1633";

//	sprintf(lon_str_end,"%s", longitudeToOnenetFormat(lon_temp)); 
//	sprintf(lat_str_end,"%s", latitudeToOnenetFormat(lat_temp)); 

	//���ӷ�����
	if(onced < 5){
		memset(send_buf, 0, 400);    //���
		strcpy(send_buf, "AT+QIOPEN=\"TCP\",\"");
		strcat(send_buf, TCPServer);
		strcat(send_buf, "\",\"");
		strcat(send_buf, Port);
		strcat(send_buf, "\"\r\n");
		if (sendCommand(send_buf, "CONNECT", 500, 3) == Success)
		onced++;
	}
	//��������
	if(sendCommand("AT+QISEND\r\n", ">", 4000, 5) == Success);
	//else ;//errorLog(8);
	delay_ms(100);
	
	
	memset(send_buf, 0, 400);    //���

	/*׼��JSON��*/
	//ARDUINOƽ̨��֧��sprintf��double�Ĵ�ӡ��ֻ��ת�����ַ���Ȼ���ӡ
	sprintf(text, "{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%s,\"lat\":%s}}]}]}"
	        , sensor_id_temp, lon_temp, lat_temp);

	/*׼��HTTP��ͷ*/
	send_buf[0] = 0;
	strcat(send_buf, "POST /devices/");
	strcat(send_buf, device_id_temp);
	strcat(send_buf, "/datapoints HTTP/1.1\r\n"); //ע�����������\r\n
	strcat(send_buf, "api-key:");
	strcat(send_buf, API_VALUE_temp);
	strcat(send_buf, "\r\n");
	strcat(send_buf, "Host:");
	strcat(send_buf, TCPServer);
	strcat(send_buf, "\r\n");
	sprintf(tmp, "Content-Length:%d\r\n\r\n", strlen(text)); //����JSON������
	strcat(send_buf, tmp);
	strcat(send_buf, text);

	if (sendCommand(send_buf, send_buf, 1000, 3) == Success);
	else ;//errorLog(9);

	delay_ms(100);
	if (sendCommand(sendCom, "\"succ\"}", 3000, 5) == Success);
	//else ;//errorLog(10);

//	delay_ms(100);
//	if (sendCommand("AT+QICLOSE\r\n", "OK\r\n", 3000, 3) == Success);
//	else ;//errorLog(11);
//	delay_ms(100);

//	if (sendCommand("AT+QIDEACT\r\n", "OK\r\n", 4000, 3) == Success);
//	else ;//errorLog(11);
//	delay_ms(100);

}






