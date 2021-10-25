/******************** (C) COPYRIGHT 2013 AzaranSystem ********************
 * File Name          : SIM900.c
 * Author             : Mehdi Abdolmaleki
 * Version            : V1.0
 * Date               : 1392/1/11
 *************************************************************************/


#include "SIM900.h"
#include "Config.h"

char ON_Status[2];

const int MessageLengh = 50;
int SIM900_Power_Tiomout = 0;
int SIM900_Power_Status;
int SIM900_TurnOn_Config = 0;
int SIM900_Connfig = 0;

int SIM900_Timer_1 = 0;
int SIM900_Timer_2 = 0;

int  SMS_Packet_Index = 0;
char SMS_Message[MessageLengh];
char SMS_RecieveMessage[200];
char SMS_tmp[200];
char SMS_PhoneNumber[14];
char Latest_PhoneNumber[14]; // 
char SMS_Recieve_CMTI[10];


static GPIO_InitTypeDef   GPIO_InitStructure;
static NVIC_InitTypeDef   NVIC_InitStructure;	
static USART_InitTypeDef  USART_InitStructure;

static short int PowerOnOff = 0;



void USART3_Send_Byte(u16 Data)
{ 
   	while (!(USART3->SR & USART_FLAG_TXE));
   	USART3->DR = (Data & (u16)0x01FF);	   
}


void SIM900_Send(unsigned char String[])
{
	PrintfSelect = 2;
	printf("%s", String);
	PrintfSelect = 0;
}
void SIM900_Sendi(unsigned char String[], char ch)
{
	PrintfSelect = 2;
	printf("%c",ch);
	PrintfSelect = 0; 
}
void SIM900_SendChar(char ch)
{
	PrintfSelect = 2;
	printf("%c",ch);
	PrintfSelect = 0; 
}

int SIM900_TurnOn()
{	
		SIM900_Power_Status = 0;
		SIM900_Power_Tiomout = 0; //Reset Value
	
		PrintfSelect = 2;
		printf("AT");
		printf("\r");
	
		while(SIM900_Power_Tiomout <100) // Wait for 2 second 
		{
			if(SIM900_Power_Status == 1)
			{
				PrintfSelect = 3;
				printf("\rSIM900 is already On");
				return 	1;
			}
			delay_ms(50);
		}
		
		PrintfSelect = 3;
		printf("\rTurn On SIM900");
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;      
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		return 2;
}

void SIM900_SendSMS(uchar PhoneNumber[], uchar Message[])
{
	PrintfSelect = 2;
    printf("AT+CMGS=\"%s\"\r", PhoneNumber);
	delay_ms(1000);
 	printf("%s", Message);
	delay_ms(500);
	printf("%c", 0x1A); // Ctr+Z

	PrintfSelect = 3; 
	printf("\rSend SMS \"%s\"  \"%s\"\r", PhoneNumber, Message);
	PrintfSelect = 0; 
}

void SIM900_ClearSMS()
{
	delay_ms(20);
	PrintfSelect = 2;
  printf("AT+CMGD=0,4");
	printf("\r");	
}
void SIM900_Read_SMS()
{
		PrintfSelect = 2;
		printf("AT+CMGL=\"ALL\"");
		delay_ms(20);
		printf("\r");
}

void SMS_Clear_Buffer()
{
	int j;
		for(j = 0; j<200; j++)
				SMS_RecieveMessage[j] = '\0';
					
		for(j = 0; j<MessageLengh; j++)
				SMS_Message[j] = '\0';	
		SMS_Packet_Index = 0;

		for(j = 0; j<14; j++)
			SMS_PhoneNumber[j] = '\0';
}

void SIM900_Init()
{
	
	int i;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  
	//SIM900 POWER PORTB 0  
	/*
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;      
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/
	
	
	// SIM900 RESET PORTB 5 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Configure USARTx_Tx as alternate function push-pull 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	delay_ms(50);
	for(i =0; i<70; i++)
    SMS_Recieve_CMTI[i++] = 0;	
}

void SIM900_RESET()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	SIM900_Send("\rSIM900 Reset");
}


int SIM900_POWER_ON()
{
	int i = 0;

	PrintfSelect = 2;
	printf("\r");
	printf("AT\r");

	while(i < 10)
	{
		if(ON_Status[0] == 'o' && ON_Status[1] == 'k'){
			SIM900_Send("\rSIM900 is On");
			ON_Status[0] == '0';
			ON_Status[1] == '0';
			return 1;
		}
		delay_ms(50);
	 	i++;
	}


	PowerOnOff = ON;  //Turn on SIM900 device
	SIM900_Send("\rSIM900 Turn On");
	
}

void SIM900_POWER_OFF()
{
	 if(PowerOnOff == ON){
	 	SIM_POWER;

		/* SIM900 RESET PORTB 0 */
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;      
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		SIM900_Send("\rSIM900 Turn Off");
	 }
}

void SIM900_Check()
{

}

void SIM900_Send12(unsigned char String[])
{
	short int num, i;
	unsigned char ch;


	PrintfSelect = 2;
	printf("\rAT: ", num);
}


/*******************************************************************************
 * Function Name  : USART2_IRQHandler
 * Description    : This function handles USART2 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART2_IRQHandler(void) 
{
		int i, j, k = 0, cotishen = 0;
		static int EndPacket = 0, SMS_END_Packet = 0;
		char *pch = NULL;

		char tmp;

		uchar ch;
	
		if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
		{
			
			if(SIM900_Timer_1 != 1)
			{
			
			ch = USART_ReceiveData(USART2);
			SMS_RecieveMessage[SMS_Packet_Index] = ch;
				
			PrintfSelect = 3;
			//printf("%c", ch);
	

			if(SMS_Packet_Index > 5  && SMS_Packet_Index < 12)
			{			
				pch = strstr (SMS_RecieveMessage,"+CMTI");
				if(pch != NULL)
				{
						//printf("\rRecive SMS!\r");	

						SMS_Clear_Buffer();
					
						SIM900_Timer_1 = 1;
						pch = NULL;
						return;
				}					
			}
			
			//for test status turn on or turn off SIM900
			if(SMS_RecieveMessage[SMS_Packet_Index] == '\r' && SMS_Packet_Index > 3  && (SMS_END_Packet == 0  && EndPacket == 0) && SIM900_Connfig == 0)
			{
				 pch = strstr(SMS_RecieveMessage,"OK");
				 if(pch != NULL )
				 {
						printf("\rOK!");	
						pch = NULL;
						SMS_Clear_Buffer();
						SIM900_Power_Status = 1;
						return;			
				 }
			}

			if(SMS_END_Packet == 1 && SMS_RecieveMessage[SMS_Packet_Index] == '\r') 
			{
				EndPacket = 1;
			}
			
			if(SMS_RecieveMessage[SMS_Packet_Index] == '\r' && SMS_Packet_Index > 5 && (SMS_END_Packet == 0))
			{

				 pch = strstr (SMS_RecieveMessage,"RING");
				 if(pch != NULL)
				 {
						printf("\rRING!");	
					
						pch = NULL;
						SMS_Clear_Buffer();
						return;			
				 }
				 
				 pch = strstr (SMS_RecieveMessage,"NO CARRIER");
				 if(pch != NULL)
				 {
						printf("\rNO CARRIER!");	
					
						pch = NULL;
						SMS_Clear_Buffer();
						return;			
				 }
				 
				 pch = strstr (SMS_RecieveMessage,"AT+CMGL");
				 if(pch != NULL)
				 {
						printf("\rInvalid Command 2!\r");	
					    
						SMS_Packet_Index = 0;
						pch = NULL;
						SMS_Clear_Buffer();
						return;			
				 }
				 
			
				pch = strstr (SMS_RecieveMessage,"+CMGL");
				if(pch != NULL)
				{
					 SMS_END_Packet = 1;
					 pch = NULL;
				}
				
			}
			
			SMS_Packet_Index++;
			
			
			
			if(EndPacket == 1)
			{

						for(j=0; j<50; j++)
						{
							if(SMS_RecieveMessage[j] == '"')
								cotishen++;
						
							if(cotishen > 3){
								cotishen = 0;
								k = 0;
								break;
							}	
						
							if(cotishen == 3)
								if(SMS_RecieveMessage[j+1] != '"')
									SMS_PhoneNumber[k++] = SMS_RecieveMessage[j+1];
						}

						//PrintfSelect = 3;
						//printf("\rSMS Phone Number: \"%s\"", SMS_PhoneNumber);
						
						//----------------------------------------------------------
						k = 0;
						for(j=0; j<150; j++)
						{
							if(SMS_RecieveMessage[j] == '"')
								cotishen++;
						
							if(cotishen == 8){
								break;
							}	
						}
						j++;
						
						
						if(cotishen == 8)
						{
							
							for(i=0; i<MessageLengh; i++)
							{
								if(i > 2 && SMS_RecieveMessage[j] == '\r')
										break;
								
								//if((SMS_RecieveMessage[j] >= '0' && SMS_RecieveMessage[j] <= 'z') || ( SMS_RecieveMessage[j] == ' '))	
								if(SMS_RecieveMessage[j] < 127  && SMS_RecieveMessage[j] > 31)
									 if((SMS_RecieveMessage[j] != '\r'))
										 SMS_Message[k++] = SMS_RecieveMessage[j];
								
								j++;
						
							}
							//printf("\rSMS Message: \"%s\"", SMS_Message);	
							
							
							//pch = strstr (SMS_Message, "Turn");
							//if(pch != NULL)
							//{
									//PrintfSelect = 3;
									//printf("\rTurn ON LAMP 1!");	
									//pch = NULL;		
							//}


 					   	//-------------------------------------------------
							
							//PrintfSelect = 3;
							//printf("\rSMS Receive: \"%s\"  \"%s\"", SMS_PhoneNumber, SMS_Message);

							pch = strstr(SMS_RecieveMessage,"+");
				 			if(pch != NULL )
				 			{
								//printf("\r+ Delete!");	
								pch = NULL;	

								for(i=0; i<MessageLengh; i++)
								{
								 	tmp = SMS_PhoneNumber[i+1];
									SMS_PhoneNumber[i] = tmp;
								}
				 			}

							PrintfSelect = 3;
							printf("\rSMS Receive: \"%s\"  \"%s\"", SMS_PhoneNumber, SMS_Message);

							delay_ms(50);

							PrintfSelect = 1;
							printf("SSS\"%s\"  \"%s\"", SMS_PhoneNumber, SMS_Message);

							strcpy(Latest_PhoneNumber,SMS_PhoneNumber);

							//PrintfSelect = 3;
							//printf("\rLatest Phone Number \"%s\"", Latest_PhoneNumber);
							
						}	
						//----------------------------------------------------------						
						
						

					 SMS_Clear_Buffer();		
					 j= 0;
					 EndPacket = 0;
					 SMS_END_Packet = 0;
						
					 SIM900_Timer_2 = 1;
			}
			
		}
			

		}
}





