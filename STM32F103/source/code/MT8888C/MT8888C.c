/******************** (C) COPYRIGHT 2013 AzaranSystem ********************
 * File Name          : MT88888C.c
 * Author             : Mehdi Abdolmaleki
 * Version            : V1.0
 * Date               : 1392/1/11
 *************************************************************************/


#include "MT8888C.h"
#include "Config.h"
#include "PLC.h"

#include "PhoneService.h"

int RingCounter = 0 ,RingTemp = 0;

static int LastNumberReceive = 20;

static GPIO_InitTypeDef 	GPIO_InitStructure;
static EXTI_InitTypeDef 	EXTI_InitStructure;
static NVIC_InitTypeDef 	NVIC_InitStructure;	


static void Set_Write()
{
 	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3  ; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void Set_Read()
{
 	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


static void Reset(){

 	short int i;

	 for(i = 0; i <5; i++)
	 {
 	 	Set_Write(); // Config io port for write data to mt8888c

	 	delay_ms(10);
	 	//-------------------------- RESET
	 	MT88_RS = 1;
	 	MT88_RW = 0;
	 	MT88_RD = 1;

	 	MT88_D0 = 0;
	 	MT88_D1 = 0;
	 	MT88_D2 = 0;
	 	MT88_D3 = 0; 

	 	delay_ms(1);
	 	MT88_RW = 1;
	 	delay_ms(40);
	 }

	 MT88_RS = 0;
	 MT88_RW = 0;
	 MT88_RD = 0;
	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;
}

void MT8888C_RESET()
{
	RingCounter = 0;
	RingTemp = 0;
}

void  MT8888C_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;



	/* PORTA  Output */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_8;      
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* PINB 1 Input */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;		
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* PORTC  Output */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15 ;      
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* PINC 0, 1, 2, 3, 4, 5 Output */
 	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* PORTD  Output */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 ;      
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  	GPIO_Init(GPIOD, &GPIO_InitStructure);




   /* Connect EXTI Line to GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);


	/* Enable the EXTI0_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = 6; 	//EXTI0_IRQn     
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the EXTI1_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = 7;      //EXTI1_IRQn    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);


	EXTI_InitStructure.EXTI_Line = EXTI_IMR_MR0;			 
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	   	  
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	  
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				 
  	EXTI_Init(&EXTI_InitStructure); 

	EXTI_InitStructure.EXTI_Line = EXTI_IMR_MR1;			 
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	   	  
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	  
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				 
  	EXTI_Init(&EXTI_InitStructure);  

	//Reset();
	
}

void  MT8888C_DTMF_Send(unsigned char String[])
{
	short int num, i;
	unsigned char ch;

    Set_Write();

	PrintfSelect = 3;
	printf("\rPhone Line Send: ", num);
    RELAY_BUSY = 1;
	RELAY_CALL = 1;
	delay_ms(200);

	for (i=0; String[i] ; i++)
	{
		ch = String[i];
		ch -= 48;
		num = ch;

		PrintfSelect = 3;
		printf("%d", num); 
		MT8888C_Ring(num);
	}

	PrintfSelect = 3;
	printf("\r", num); 


	delay_ms(200);
	RELAY_CALL = 0;

}

void  MT8888C_CP_Receive_Init()
{
    
	 Set_Write(); // Config io port for write data to mt8888c

	 delay_ms(1);
	 //-------------------------- RESET
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;

	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0; 

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(40);

	 //-------------------------- CRA
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;

	 MT88_D0 = 0;
	 MT88_D1 = 1; //CP
	 MT88_D2 = 1;
	 MT88_D3 = 0;

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(1);

	 //-------------------------- CRB
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;

	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(40);

	 //--------------------------
	 MT88_RS = 0;
	 MT88_RW = 0;
	 MT88_RD = 0;

	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;

	 //Set_Read(); //Config io port for read data of mt8888c		
}



void  MT8888C_DTMF_Receive_Init()
{
    
	 Set_Write(); // Config io port for write data to mt8888c

	 delay_ms(1);
	 //-------------------------- RESET
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;

	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0; 

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(40);

	 //-------------------------- CRA
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;

	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 1;
	 MT88_D3 = 1;

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(1);

	 //-------------------------- CRB
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;

	 MT88_D0 = 1;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(40);

	 //--------------------------
	 MT88_RS = 0;
	 MT88_RW = 0;
	 MT88_RD = 0;

	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;

	 Set_Read(); //Config io port for read data of mt8888c		
}


void  MT8888C_Ring(short int num)
{   
   	 short int DecimalToBinary[4];
  
	 switch(num)
	 {
	 	case 0:
	 		DecimalToBinary[0] = 0;
	 		DecimalToBinary[1] = 1;
	 		DecimalToBinary[2] = 0;
	 		DecimalToBinary[3] = 1;	
		break;			
	 	case 1:
	 		DecimalToBinary[0] = 1;
	 		DecimalToBinary[1] = 0;
	 		DecimalToBinary[2] = 0;
	 		DecimalToBinary[3] = 0;	
		break;	
	 	case 2:
	 		DecimalToBinary[0] = 0;
	 		DecimalToBinary[1] = 1;
	 		DecimalToBinary[2] = 0;
	 		DecimalToBinary[3] = 0;	
		break;	
	 	case 3:
	 		DecimalToBinary[0] = 1;
	 		DecimalToBinary[1] = 1;
	 		DecimalToBinary[2] = 0;
	 		DecimalToBinary[3] = 0;	
		break;	
	 	case 4:
	 		DecimalToBinary[0] = 0;
	 		DecimalToBinary[1] = 0;
	 		DecimalToBinary[2] = 1;
	 		DecimalToBinary[3] = 0;	
		break;	
	 	case 5:
	 		DecimalToBinary[0] = 1;
	 		DecimalToBinary[1] = 0;
	 		DecimalToBinary[2] = 1;
	 		DecimalToBinary[3] = 0;	
		break;			
	 	case 6:
	 		DecimalToBinary[0] = 0;
	 		DecimalToBinary[1] = 1;
	 		DecimalToBinary[2] = 1;
	 		DecimalToBinary[3] = 0;	
		break;	
	 	case 7:
	 		DecimalToBinary[0] = 1;
	 		DecimalToBinary[1] = 1;
	 		DecimalToBinary[2] = 1;
	 		DecimalToBinary[3] = 0;	
		break;	
	 	case 8:
	 		DecimalToBinary[0] = 0;
	 		DecimalToBinary[1] = 0;
	 		DecimalToBinary[2] = 0;
	 		DecimalToBinary[3] = 1;	
		break;	
	 	case 9:
	 		DecimalToBinary[0] = 1;
	 		DecimalToBinary[1] = 0;
	 		DecimalToBinary[2] = 0;
	 		DecimalToBinary[3] = 1;	
		break;
	 }

	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;
	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;
	  
	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(1);

	 //-------------------------- CRA
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;
	 MT88_D0 = 1;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 1;

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(1);

	 //-------------------------- CRB
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;
	 MT88_D0 = 1;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(1);

	 //-------------------------- Number 
	 MT88_RS = 0;
	 MT88_RW = 0;
	 MT88_RD = 1;
	 MT88_D0 = DecimalToBinary[0];
	 MT88_D1 = DecimalToBinary[1];
	 MT88_D2 = DecimalToBinary[2];
	 MT88_D3 = DecimalToBinary[3];

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(130);

	 //--------------------------  CRA 
	 MT88_RS = 1;
	 MT88_RW = 0;
	 MT88_RD = 1;
	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;

	 delay_ms(1);
	 MT88_RW = 1;
	 delay_ms(90);

	 //--------------------------
	 MT88_RS = 0;
	 MT88_RW = 0;
	 MT88_RD = 0;
	 MT88_D0 = 0;
	 MT88_D1 = 0;
	 MT88_D2 = 0;
	 MT88_D3 = 0;
}


short int MT8888C_Read_Data()
{

	short int MT88888C_Data;

	MT88888C_Data =  (MT8888C_D0 == 1) ? 1 : 0;
	MT88888C_Data += (MT8888C_D1 == 1) ? 2 : 0;
	MT88888C_Data += (MT8888C_D2 == 1) ? 4 : 0;
	MT88888C_Data += (MT8888C_D3 == 1) ? 8 : 0;

	return MT88888C_Data;
}

int MT8888C_GetLastNumber()
{
	return LastNumberReceive;  

}
void MT8888C_SetLastNumber(int Default)
{
	 LastNumberReceive = Default;  

}
/*******************************************************************************
 * Function Name  : EXTI0_IRQHandler
 * Description    : This function handles External interrupt Line 0 request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void EXTI0_IRQHandler(void) //MT8888c IRQ
{ 
	int tmp = 0;  

  	if(EXTI_GetITStatus(EXTI_IMR_MR0) != RESET)
  	{	 

		
		tmp =  MT8888C_Read_Data();
		PrintfSelect = 3;

		if(tmp >= 1 && tmp <=12){
			LastNumberReceive =	 tmp;

			printf("\rPhone: DTMF Receive: %d", LastNumberReceive);
		}
		else	
			printf("\rPhone: Error DTMF Receive: %d", LastNumberReceive);

		
		
		if(Start_Telephone == 1 && LastNumberReceive == 11)// user enter star(*) key for intercom select
		{
			Start_Telephone = 0;	
			SwitchToIntercom = 1;
			
			RELAY_INTERCOM = 1;
			delay_ms(300);
			RELAY_BUSY = 0;
			delay_ms(200);
			SwitchToIntercom = 0;
			
			IntercomSelect = 1;

		}
		
		
				
		MT8888C_DTMF_Receive_Init();
		
		//printf("*" );
			
    	EXTI_ClearITPendingBit(EXTI_IMR_MR0);		 
  	}
}


/*******************************************************************************
 * Function Name  : EXTI1_IRQHandler
 * Description    : This function handles External interrupt Line 1 request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void EXTI1_IRQHandler(void)	 //Ring Detect
{ 
   
  	if(EXTI_GetITStatus(EXTI_IMR_MR1) != RESET)
  	{	 
		RingTemp++;
	 	PrintfSelect = 3;
		
		if(RingTemp > 44)
		{
			RingTemp = 0;
			printf("\rPhone Ring Counter: %d", ++RingCounter);
		}	
    	EXTI_ClearITPendingBit(EXTI_IMR_MR1);	 
  	}
}











