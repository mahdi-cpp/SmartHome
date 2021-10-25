
/******************** (C) COPYRIGHT 2013 AzaranSystem ********************
 * File Name          : ATMEGA8.c
 * Author             : Mehdi Abdolmaleki
 * Version            : V1.0
 * Date               : 1392/1/11
 *************************************************************************/


#include "ATMEGA8.h"
#include "PhoneService.h"
#include "Config.h"

//#include "VS1003.h"



static int RingCounter = 0 ,RingTemp = 0, Atmega8Data;

static GPIO_InitTypeDef GPIO_InitStructure;
static EXTI_InitTypeDef EXTI_InitStructure;
static NVIC_InitTypeDef NVIC_InitStructure;	



static void Reset(){

 	short int i;

}

void ATMEGA8_RESET()
{
	RingCounter = 0;
	RingTemp = 0;
}


void ATMEGA8_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 ;  //PORTC 13
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;  //PINC 10 , 11 , 12 Input
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;								   
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 ;  //PINA 12 Input
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//------------------------------------------------------------

	/* Connect EXTI Line to GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
   
	/* Enable the EXTI11_IRQn Interrupt */
  	EXTI_InitStructure.EXTI_Line =  EXTI_IMR_MR11 ;			 
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	   	  
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	  
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				 
  	EXTI_Init(&EXTI_InitStructure);  
   
	/*
	// Enable the EXTI12_IRQn Interrupt 
  	EXTI_InitStructure.EXTI_Line =  EXTI_IMR_MR12 ;			 
  	EXTI_InitStructure.EXTI_Mode = 	EXTI_Mode_Interrupt;	   	  
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	  
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				 
  	EXTI_Init(&EXTI_InitStructure);
	
	// Enable the EXTI13_IRQn Interrupt
  	EXTI_InitStructure.EXTI_Line =  EXTI_IMR_MR13 ;			 
  	EXTI_InitStructure.EXTI_Mode = 	EXTI_Mode_Interrupt;	   	  
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	  
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				 
  	EXTI_Init(&EXTI_InitStructure);
	
	*/
		
	// Enable the EXTI11_IRQn Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = 40;   //EXTI15_10_IRQn   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
}


void Stm32_To_M8(){

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;  //PINC 10 , 11 , 12 Output
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 ;  //PINA 12 Output
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*
	STM32_Q0 = 1;
	STM32_Q1 = 1;
	STM32_Q2 = 1;
	STM32_Q3 = 1;

	Delay(200);
	STM32_TO_M8 = 1;
	Delay(200);
	STM32_TO_M8 = 0;
	Delay(200);

	STM32_Q0 = 0;
	STM32_Q1 = 0;
	STM32_Q2 = 0;
	STM32_Q3 = 0;
	
 	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;  //PINC 10 , 11 , 12 Input
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 ;  //PINA 12 Input
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Delay(200);
   
	*/
}





/*******************************************************************************
 * Function Name  : EXTI11_IRQHandler
 * Description    : This function handles External interrupt Line 11 request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void EXTI15_10_IRQHandler(void) //Mega8 To STM32
{ 
   
  	if(EXTI_GetITStatus(EXTI_IMR_MR11) != RESET)
  	{	 
		
		Atmega8Data =  (M8_Q0 == 1) ? 1 : 0;
		Atmega8Data += (M8_Q1 == 1) ? 2 : 0;
		Atmega8Data += (M8_Q2 == 1) ? 4 : 0;
		Atmega8Data += (M8_Q3 == 1) ? 8 : 0;

		if(RemotDelay > 60)
		{
    			switch(Atmega8Data)
				{
					case 1:
						
						PrintfSelect = 1;
    				printf("BB101");
					  PrintfSelect = 3;
						printf("\rRemote Code B0101: %02d", Atmega8Data);

					  //PrintfSelect = 1;
    				//printf("PPP");

						break;
					case 2:
						//PrintfSelect = 2;
						//printf("AT+CMGL=\"ALL\"");
						//delay_ms(20);
						//printf("\r");
					
					
					
						PrintfSelect = 1;
    				printf("BB102");
						PrintfSelect = 3;
						printf("\rRemote Code B0102: %02d", Atmega8Data);
						break;
					case 3:
						//PrintfSelect = 2;
						//printf("AT");

						PrintfSelect = 1;
    					printf("BB103");
						PrintfSelect = 3;
						printf("\rRemote Code B0103: %02d", Atmega8Data);
						break;
					case 4:
						PrintfSelect = 1;
    					printf("BB104");
						PrintfSelect = 3;
						printf("\rRemote Code B0104: %02d", Atmega8Data);
						break;
					case 5:
						PrintfSelect = 1;
    					printf("BB105");
						PrintfSelect = 3;
						printf("\rRemote Code B0105: %02d", Atmega8Data);
						break;
					case 6:
						PrintfSelect = 1;
    					printf("BB106");
						PrintfSelect = 3;
						printf("\rRemote Code B0106: %02d", Atmega8Data);
						break;
					case 7:
						PrintfSelect = 1;
    					printf("BB107");
						PrintfSelect = 3;
						printf("\rRemote Code B0107: %02d", Atmega8Data);
						break;
					case 8:
						PrintfSelect = 1;
    					printf("BB108");
						PrintfSelect = 3;
						printf("\rRemote Code B0107: %02d", Atmega8Data);
						break;
					case 9:
						PrintfSelect = 1;
    					printf("BB109");
						PrintfSelect = 3;
						printf("\rRemote Code B0109: %02d", Atmega8Data);
						break;
					case 10:
						PrintfSelect = 1;
    					printf("BB110");
						PrintfSelect = 3;
						printf("\rRemote Code B0110: %02d", Atmega8Data);
						break;
					case 11:
						PrintfSelect = 1;
    					printf("BB111");
						PrintfSelect = 3;
						printf("\rRemote Code B0111: %02d", Atmega8Data);
						break;
					case 12:
						PrintfSelect = 1;
    					printf("BB112");
						PrintfSelect = 3;
						printf("\rRemote Code B0112: %02d", Atmega8Data);
						break;

				}

		 	RemotDelay = 0;
		}
		//BUZZER = 1;
		//delay_ms(600);
		//BUZZER = 0;

    	EXTI_ClearITPendingBit(EXTI_IMR_MR11); 
  	}
	
	/*
	if(EXTI_GetITStatus(EXTI_IMR_MR12) != RESET) //End of call
  	{		
			PrintfSelect = 3;
			printf("\rEXTI 12");
		
			if(SwitchToIntercom == 0)
			{
				RELAY_BUSY = 0;
				RELAY_INTERCOM = 0;
				
				printf("\rSystem Reset... \r");
				NVIC_GenerateSystemReset();
			}
		
			EXTI_ClearITPendingBit(EXTI_IMR_MR12); 
	}
	
	if(EXTI_GetITStatus(EXTI_IMR_MR13) != RESET)// start telephone select
  	{		
			PrintfSelect = 3;
			printf("\rEXTI 13");
			if(PhoneServiceBusy == 0 && SwitchToIntercom == 0)
			{
				StartTelephoneSelect++;

				Start_Telephone = 1;
			}
		
			EXTI_ClearITPendingBit(EXTI_IMR_MR13); 
	}

	*/
}











