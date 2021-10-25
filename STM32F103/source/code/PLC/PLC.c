/******************** (C) COPYRIGHT 2013 AzaranSystem ********************
 * File Name          : PLC.c
 * Author             : Mehdi Abdolmaleki
 * Version            : V1.0
 * Date               : 1392/1/14
 *************************************************************************/


#include "PLC.h"
#include "Config.h"
#include "PhoneService.h"

extern unsigned char 	A_PhoneNumber[11]="";
extern unsigned char    B_PhoneNumber[11]="";
extern unsigned char    C_PhoneNumber[11]="";
extern unsigned char 	D_PhoneNumber[11]="";
extern unsigned char    E_PhoneNumber[11]="";
extern unsigned char    F_PhoneNumber[11]="";
extern unsigned char    G_PhoneNumber[11]="";
extern unsigned char 	H_PhoneNumber[11]="";
extern unsigned char    I_PhoneNumber[11]="";
extern unsigned char    J_PhoneNumber[11]="";


#define  MAX_PHONE_BUFFER     120

static GPIO_InitTypeDef GPIO_InitStructure;
static EXTI_InitTypeDef EXTI_InitStructure;
static NVIC_InitTypeDef NVIC_InitStructure;	

static int tmp[2], i=0,k=0, step=1;

int phone_i = 0 , phone_t = 0 , phone_N = 0;
uchar  phone_num[15];
uchar  phone_buffer[MAX_PHONE_BUFFER];

int PLC_SMS_Request = 0;


//uchar SMSPhoneNumber[15];
uchar SMSMessage[40];

uchar Alert_Buffer[40];
int Alert_i = 0;
uchar Alert_CMD = '0';




void Delete_First_Code()
{
  int i;

  	if(A_PhoneNumber[0] == '2' &&  A_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	A_PhoneNumber[i] =  A_PhoneNumber[i+2];
		A_PhoneNumber[8] = '\0';
	}


	if(B_PhoneNumber[0] == '2' &&  B_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	A_PhoneNumber[i] =  B_PhoneNumber[i+2];
		B_PhoneNumber[8] = '\0';
	}

  	if(C_PhoneNumber[0] == '2' &&  C_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	C_PhoneNumber[i] =  C_PhoneNumber[i+2];
		C_PhoneNumber[8] = '\0';
	}

	if(D_PhoneNumber[0] == '2' &&  D_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	D_PhoneNumber[i] =  D_PhoneNumber[i+2];
		D_PhoneNumber[8] = '\0';
	}


  	if(E_PhoneNumber[0] == '2' &&  E_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	E_PhoneNumber[i] =  E_PhoneNumber[i+2];
		E_PhoneNumber[8] = '\0';
	}

	if(F_PhoneNumber[0] == '2' &&  F_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	F_PhoneNumber[i] =  F_PhoneNumber[i+2];
		F_PhoneNumber[8] = '\0';
	}

  	if(G_PhoneNumber[0] == '2' &&  G_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	G_PhoneNumber[i] =  G_PhoneNumber[i+2];
		G_PhoneNumber[8] = '\0';
	}

	if(H_PhoneNumber[0] == '2' && H_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	H_PhoneNumber[i] =  H_PhoneNumber[i+2];
		H_PhoneNumber[8] = '\0';
	}


  	if(J_PhoneNumber[0] == '2' || J_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	J_PhoneNumber[i] =  J_PhoneNumber[i+2];
		J_PhoneNumber[8] = '\0';
	}

	if(I_PhoneNumber[0] == '2' || I_PhoneNumber[1]=='6'){
  		for(i=0; i<8;i++)
		 	I_PhoneNumber[i] =  I_PhoneNumber[i+2];
		I_PhoneNumber[8] = '\0';
	}



}


void PLC_Send(unsigned char String[])
{
	PrintfSelect = 1;
	printf("%s", String);
	PrintfSelect = 0;
}
void PLC_SendInt(unsigned char String[], int ch)
{
	PrintfSelect = 1;
	printf("%c",ch);
	PrintfSelect = 0; 
}

void PLC_Init()
{

	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Connect EXTI Line to GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);


	/* Enable the EXTI1_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = 23;      //EXTI9_5_IRQn    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_IMR_MR9;			 
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	   	  
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	  
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				 
  	EXTI_Init(&EXTI_InitStructure);  



	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl
			= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




/*******************************************************************************
 * Function Name  : EXTI9_5_IRQHandler
 * Description    : This function handles External lines 9 to 5 interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void EXTI9_5_IRQHandler(void) 
{
    if(EXTI_GetITStatus(EXTI_IMR_MR9) != RESET)
  	{
	
	if(PhoneServiceBusy == 0)
	{	 
	 	PrintfSelect = 1;
		printf("C00000");

	 	PrintfSelect = 3;
		printf("\rPLC SMS Request");
		PLC_SMS_Request = 1;   

		delay_ms(5);
	}
    	EXTI_ClearITPendingBit(EXTI_IMR_MR9);
			 
  	}
}





/*******************************************************************************
 * Function Name  : USART1_IRQHandler
 * Description    : This function handles USART1 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART1_IRQHandler(void) 
{
	
 uchar ch;
 int j;

 char *pch = NULL;

 if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
 {

	ch = USART_ReceiveData(USART1);
	
   PrintfSelect = 3;
    printf("%c", ch);



	


	if(PLC_SMS_Request == 1)
	{

	  	Alert_Buffer[Alert_i++] = ch;

		if(Alert_i > 40)
		{
			Alert_i = 0;
			PLC_SMS_Request = 0;

			for(i=0; i<40; i++)
				SMSMessage[i++] = 0;


		   if(Alert_Buffer[0] == 'C' && Alert_Buffer[1] == '0')
		    {

				Alert_CMD = Alert_Buffer[2];
				for(i=0; i<40; i++)
					SMSMessage[i] = Alert_Buffer[i+14];


			    PrintfSelect = 3;
				printf("\rAlert Command: \"%c\"      Message: \"%s\" \n" ,Alert_CMD, SMSMessage);
				
				
				if(Alert_CMD == 'r')//Report to user 
				{
					printf("\rPLC:Report To User!\r");
		  			//printf("\rSystem Reset... \r");

					SIM900_SendSMS(Latest_PhoneNumber , SMSMessage);

					//delay_ms(500);
		  			//NVIC_GenerateSystemReset();
					return;
				}

			    //GetPassword();
				//delay_ms(200);


				SIM900_SendSMS(B_PhoneNumber, SMSMessage);
				Call_Alert(A_PhoneNumber, Alert_CMD);


				SIM900_SendSMS(A_PhoneNumber, SMSMessage);
				Call_Alert(B_PhoneNumber, Alert_CMD);

				SIM900_SendSMS(D_PhoneNumber, SMSMessage);
				Call_Alert(C_PhoneNumber, Alert_CMD);


				SIM900_SendSMS(C_PhoneNumber, SMSMessage);
				Call_Alert(D_PhoneNumber, Alert_CMD);

				SIM900_SendSMS(F_PhoneNumber, SMSMessage);
				Call_Alert(E_PhoneNumber, Alert_CMD);


				SIM900_SendSMS(E_PhoneNumber, SMSMessage);
				Call_Alert(F_PhoneNumber, Alert_CMD);

				printf("\rPLC:End Of Send Alert To Manager!\r");
		  		printf("\rSystem Reset... \r");
		  		NVIC_GenerateSystemReset();
				

			}
			else {
				PrintfSelect = 3;
				printf("\rPLC:Error Alert Packet!  %s", Alert_Buffer);
			}
		
				
		}
	}
	
	/*
	if(PLC_SMS_Request == 1)
	{
		if(step == 1)
		{
			tmp[i++] = ch;
			if(i == 2)
			{
				if(tmp[0] == 'C' && tmp[1]== '0')
				{
					step = 2;
					i = 0;
					tmp[0] = 0;
				    tmp[1] = 0;
				    PrintfSelect = 3;
    				printf("\rSTEP 0\r");

					for(j=0; j<15; j++)
						SMSPhoneNumber[j] = '\0';
					for(j=0; j<62; j++)
						SMSMessage[j] = '\0';
				}
				else
				{
					i = 0;
					tmp[0] = 0;
				    tmp[1] = 0;
					PrintfSelect = 3;
    				printf("\rSTEP 1\r");
				}
			}

		}
		else if(step == 2)
		{
		    
			
			if(ch != '*')
				SMSPhoneNumber[i++] = ch;
			else{
				printf("\rSTAR");	
				step = 3;
				i = 0;
			}
		}
		else if(step == 3)
		{
		    PrintfSelect = 3;
    		printf("\rSTEP 3");	

		   	if(ch != '\r')
				SMSMessage[i++] = ch;		

			else
			{

			    PrintfSelect = 3;
    			printf("\rPhone Number: \"%s\"      Message: \"%s\"", SMSPhoneNumber, SMSMessage);
				
				
				SIM900_SendSMS(A_PhoneNumber, SMSMessage);
				delay_ms(500);
				SIM900_SendSMS(B_PhoneNumber, SMSMessage);
				delay_ms(500);
				SIM900_SendSMS(C_PhoneNumber, SMSMessage);
				delay_ms(500);
				//SIM900_SendSMS(D_PhoneNumber, SMSMessage);
				//delay_ms(500);
				//SIM900_SendSMS(E_PhoneNumber, SMSMessage);
				//delay_ms(500);
				//SIM900_SendSMS(F_PhoneNumber, SMSMessage);

				Call_Alert(A_PhoneNumber, 1);
				Call_Alert(B_PhoneNumber, 1);
				Call_Alert(C_PhoneNumber, 1);
				//Call_Alert(D_PhoneNumber, 1);
				//Call_Alert(E_PhoneNumber, 1);
				//Call_Alert(F_PhoneNumber, 1);

				i = 0;
				step = 1;
				PLC_SMS_Request = 0;

				PrintfSelect = 3;
    			printf("\rPhone Number: \"%s\"      Message: \"%s\"", SMSPhoneNumber, SMSMessage);

				

			}
		}
	}

	*/
	
	
	
	
	
	
	
	
	
	
	
		
	else if(PLC_Telephone_Request == 1)
	{	
	 	PLC_Telephone_Response = ch;
		PLC_Telephone_Request = 0;
		PrintfSelect = 3;
    	printf("\rPLC Telephone Request: %c", PLC_Telephone_Response);	
	}


	else if(PLC_Telephone_Password == 1)
	{	
	 	//Telephone_Password[k++] = ch;
		/*
		phone_t++;

		if(phone_t<4)
			printf("\rpassword%c", ch);

		if(phone_t >= 4)
		{
			//phone_i++;

			phone_num[phone_i++] = ch;
		}
		*/

		phone_buffer[phone_i] = ch;
		phone_i++;

		//PrintfSelect = 3;
		//printf("%c", ch);

		if(phone_i > 69)
		{

			//PrintfSelect = 3;
			//printf("\rPhone Buffer: %s",phone_buffer);

			for(i=0; i<4; i++)
				Telephone_Password[i] = phone_buffer[i];
				//if( phone_buffer[i] =='0')
					//Telephone_Password[i] = '0';
				//else
				

			PrintfSelect = 3;
	    	printf("\rTelephone Password: %s", Telephone_Password);
			
			for(i=0; i<10; i++)
				A_PhoneNumber[i] = 	phone_buffer[i+10];
			A_PhoneNumber[10] = '\0'; 
			PrintfSelect = 3;
	    	printf("\rA Phone Number: %s", A_PhoneNumber);


			for(i=0; i<10; i++)
				B_PhoneNumber[i] = 	phone_buffer[i+20];
			B_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rB Phone Number: %s", B_PhoneNumber);


			for(i=0; i<10; i++)
				C_PhoneNumber[i] = 	phone_buffer[i+30];
			C_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rC Phone Number: %s", C_PhoneNumber);


			for(i=0; i<10; i++)
				D_PhoneNumber[i] = 	phone_buffer[i+40];
			D_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rD Phone Number: %s", D_PhoneNumber);


			for(i=0; i<10; i++)
				E_PhoneNumber[i] = 	phone_buffer[i+50];
			E_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rE Phone Number: %s", E_PhoneNumber);

			
			for(i=0; i<10; i++)
				F_PhoneNumber[i] = 	F_PhoneNumber[i+60];
			F_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rF Phone Number: %s", F_PhoneNumber);

			
			/*
			for(i=0; i<10; i++)
				G_PhoneNumber[i] = 	G_PhoneNumber[i+70];
			G_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rG Phone Number: %s", G_PhoneNumber);

			for(i=0; i<10; i++)
				H_PhoneNumber[i] = 	H_PhoneNumber[i+80];
			H_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rH Phone Number: %s", H_PhoneNumber);
		

			for(i=0; i<10; i++)
				I_PhoneNumber[i] = 	I_PhoneNumber[i+90];
			I_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rI Phone Number: %s", I_PhoneNumber);

			for(i=0; i<10; i++)
				J_PhoneNumber[i] = 	J_PhoneNumber[i+100];
			J_PhoneNumber[10] = '\0';
			PrintfSelect = 3;
	    	printf("\rJ Phone Number: %s", J_PhoneNumber);
			*/

			Delete_First_Code();

			for(i=0; i<MAX_PHONE_BUFFER; i++)
				phone_buffer[i] = 0;


			printf("\n", phone_num);

			phone_i = 0;

			PLC_Telephone_Password = 0;

			PrintfSelect = 3;
    		printf("\rPLC Telephone Password: %s", Telephone_Password);	

			//phone_N++;
			//if(phone_N == 7)
			//{
				//phone_t = 0;
			//	PLC_Telephone_Password = 0;
			//}
		}

		
		/*
		if(k >= 4){
			PLC_Telephone_Password = 0;
			PrintfSelect = 3;
    		printf("\rPLC Telephone Password: %s", Telephone_Password);	
		}
		*/
	}			
 }				  
}



