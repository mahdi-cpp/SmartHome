
#include "config.h"
#include "string.h"
#include "stm32f10x_lib.h"
#include "init.h"
#include "VS1003.h"
#include "MT8888C.h"
#include "SIM900.h"
#include "PLC.h"
#include "ATMEGA8.h"

#include "PhoneService.h"

//-------------------------------------------------------------
char IR_Remote_Data[12];


//-------------------------------------------------------------

FATFS fs[2]; // Work area (file system object) for logical drive
FIL fsrc, fdst, fap; // file objects
FRESULT res, re; // FatFs function common result code
UINT br, bw, bt;


#define FILEAMOUNT 11


char machine[20] = "machine1";
char shellEnable = FALSE;


long int SystemStart = -1;

int PrintfSelect = 3;

int RemotDelay;

static int tmp[3], index=0;

int PLC_Telephone_Request = 0;

void TIM_Configuration(void);

   uchar Alert_Sound12[20];

int main(void) {

	char str[] = "Hello mhdi12 123456789";
    char key[] = "mehdi";
	char *pch;

	RCC_Configuration();
	NVIC_Configuration();
	USART3_Configuration();

	Delay_init(72); 

	SPI_Configuration();
 	VS1003B_Init();

	printf("\rVS1003 Init...");

   VS1003B_WriteCMD(0x0b, 0x0a, 0x0a);

   f_mount(1, &fs[1]);	

	delay_ms(50);
	SIM900_Init();



	delay_ms(50);
	PLC_Init();
	ATMEGA8_Init();

  delay_ms(50);
	MT8888C_Init();
	MT8888C_DTMF_Receive_Init();



	
	
	TIM_Configuration();
	if(SIM900_TurnOn() == 2){
		SIM900_TurnOn_Config = 1;
	}

	PrintfSelect = 1;
	printf("123456");
	delay_ms(900);

	GetPassword();


//	delay_ms(900);
//	delay_ms(900);


  
	//Call_Alert(A_PhoneNumber, 1);
	//Call_Alert(B_PhoneNumber, 1);
	//Call_Alert(C_PhoneNumber, 1);
	
	//ServiceRecord();
	
	//play_file("1:/system/welcome.mp3"); 	//Welcome to System

   /*
   	sprintf(Alert_Sound12, "1:/Alert/%c.mp3" , 52);

	PrintfSelect = 3;
	printf("\rPlay File: %s", Alert_Sound12);

	PhoneServicePlay(Alert_Sound12);
	*/

	
	printf("\rSystem Start...");
	//PhoneServicePlay("1:/system/welcome.mp3"); 	//Welcome to System
	//RELAY_BUSY = ON;
	
   //PrintfSelect = 2;
	//printf("AT+CSQ");
	//printf("\r");

	
	//PrintfSelect = 2;
	//printf("AT+CMGF=1\r");
		

	
	//PhoneServicePlay("1:/system/welcome.mp3"); 	//Welcome to System
	
	while(1)
	{
		//PrintfSelect = 1;
    	//printf("PPP");
		//PLC_Telephone_Password = 3;

		//GetPassword();
		//PhoneService();
		//PrintfSelect = 1;


	 	if(RingCounter >= 8)
	 	{
			PhoneService();
	 	}
		
 
	 	//PLC_OUT_1 = !PLC_OUT_1;	

		delay_ms(50);

	}
}








/*******************************************************************************
 * Function Name  : TIM2_IRQHandler
 * Description    : This function handles TIM2 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/

void TIM2_IRQHandler(void) //10ms
{
	static int count = 0, Interupt_13_Counter , RingTone_Counter = 0 ,  RingTemp = 0;;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
   {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);       
 	 
		RemotDelay++;
		RingTone_Counter++;

	  //--------------------------------System Service-----------------------------------
	  count++;
	  if(count == 50)
	  {	
			PLC_OUT_2 =! PLC_OUT_2;	
			count = 0;		
	  }
	  //--------------------------------Phone Service-----------------------------------
	  PhoneServiceTimeout++;
	  
	  if(PhoneServiceTimeout > 50000)
	  {
			PhoneServiceTimeout = 0;// Reset value		
	  }
			
	  //--------------------------------SIM900 Service-----------------------------------
	  
	  	SIM900_Power_Tiomout++;
		SystemStart++;
		 
		if(SIM900_TurnOn_Config > 0) // if SIM900 is Turn off  during sytem start.
			 SIM900_TurnOn_Config++;
		
	  if(SIM900_Timer_1 == 1)
		{

			PrintfSelect = 2;
			printf("AT+CMGL=\"ALL\"");
			delay_ms(20);
			printf("\r");
			
			//delay_ms(50);
			
			//PrintfSelect = 2;
			//printf("AT+CMGL=\"ALL\"");
			//delay_ms(20);
			//printf("\r");
			
			SIM900_Timer_1 = 0;
						
			//PrintfSelect = 3;
			//printf("\rSIM900_Timer_1...");
			
		}
		
		if((SIM900_Timer_2 == 1) || (SystemStart == -1) || (SystemStart >4320000)) //Clear Value next 12 hour
		{	
			SIM900_Connfig = 1;
			
			SIM900_ClearSMS();
			delay_ms(50);
			SIM900_ClearSMS();
			delay_ms(50);
			
			SIM900_Read_SMS();
			delay_ms(50);
			SIM900_Read_SMS();

			
			SIM900_Timer_2 = 0;
			SystemStart = 1;
			
			delay_ms(50);
			SIM900_Connfig = 0;
			
			//PrintfSelect = 3;
			//printf("\rSIM900_Timer_2...");
		}
		
		if((SIM900_TurnOn_Config >= 1200)) //Clear Value next 8 minute
		{
			PrintfSelect = 2;
			printf("AT");
			printf("\r");
			delay_ms(20);
			
			SIM900_ClearSMS();
			delay_ms(50);
			SIM900_ClearSMS();
			
			SIM900_Read_SMS();
			delay_ms(50);
			SIM900_Read_SMS();
			
			
			SIM900_Timer_2 = 0;
			SystemStart = 1;			
			SIM900_TurnOn_Config = 0;
			
			PrintfSelect = 3;
			printf("\rSIM900_TurnOn_Config...");
		}
		
		
		//---------------------------------------------------------------------------------		
		if(Start_Telephone == 1)
		{
			Interupt_13_Counter++;			
		}
		
		if(Interupt_13_Counter > 50)
		{
			Interupt_13_Counter = 0;
			if(StartTelephoneSelect == 1)
			{
				RELAY_BUSY = 1;
				Start_Telephone = 1;
			}
			else
			{
				RELAY_BUSY = 0;
				Start_Telephone = 0;
				StartTelephoneSelect = 0;
			}			
		}
		
		//---------------------------------------------------------------------------------		
		if(RingTone_Counter > 600)// This section used for  zero ringCounter if not Detect Ring
		{
			 if(RingCounter == 1){
				  RingTemp = 1;
			 }
			 else if(RingCounter > RingTemp){
				 RingTemp = RingCounter;
			 }
			 else{
				  RingCounter = 0;
					RingTemp = 0;
			 }

			 //PrintfSelect = 3;
			 //printf("\rRingTone_Counter ..........");
			
			 RingTone_Counter = 0;
		}
		//---------------------------------------------------------------------------------		
		
	}
}





/*******************************************************************************
 * Function Name  : USART3_IRQHandler
 * Description    : This function handles USART3 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART3_IRQHandler(void) 
{
	uchar ch;
	int j;
	static int i = 0;
	char *pch = NULL;

	

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
		ch = USART_ReceiveData(USART3);

		//PrintfSelect = 3;
	    //printf("%c", ch);
		
		
		i = (i > 12) ? 0 : i;
			
		//if(ch == '\r') //Start of frame
			//printf("+ ");
		
		
		
		if(ch == 4) //Start of frame
		{
			i = 0;
			for(j=0; j<12; j++)
				IR_Remote_Data[j] = 0;
		}
		else if(ch == '\r')
		{
			    printf("\r\"%s\"", IR_Remote_Data);
		

				pch = strstr (IR_Remote_Data,"BLUE");
				if(pch != NULL)
				{	
						PrintfSelect = 1;
    					printf("BB1BL");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB1BL");
						return;
				}
 				pch = strstr (IR_Remote_Data,"YELLOW");
				if(pch != NULL)
				{	
						PrintfSelect = 1;
    					printf("BB1YL");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB1YL");
						return;
				}
				pch = strstr (IR_Remote_Data,"GREEN");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB1GN");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB1GN");
						return;
				}
				pch = strstr (IR_Remote_Data,"RED");
				if(pch != NULL)
				{	
						PrintfSelect = 1;
    					printf("BB1RD");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB1RD");
						return;
				}




				pch = strstr (IR_Remote_Data,"1");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB101");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB101: %02d", 1);
						return;
				}
				pch = strstr (IR_Remote_Data,"2");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB102");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB102: %02d", 2);
						return;
				}
				pch = strstr (IR_Remote_Data,"3");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB103");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: B0103: %02d", 3);
						return;
				}
				pch = strstr (IR_Remote_Data,"4");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB104");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB104: %02d", 4);
						return;
				}
				pch = strstr (IR_Remote_Data,"5");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB105");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB105: %02d", 5);
						return;
				}
				pch = strstr (IR_Remote_Data,"6");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB106");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB106: %02d", 6);
						return;
				}

				pch = strstr (IR_Remote_Data,"7");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB107");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB107: %02d", 7);
						return;
				}
				pch = strstr (IR_Remote_Data,"8");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB108");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB108: %02d", 8);
						return;
				}
				pch = strstr (IR_Remote_Data,"9");
				if(pch != NULL)
				{
						PrintfSelect = 1;
    					printf("BB109");

						PrintfSelect = 3;
						printf("\rREMOTE CODE: BB109: %02d", 9);
						return;
				}





	   }



				

						
		else
		{
			IR_Remote_Data[i++] = ch;
		}


	    
	
		//i++;
		//ch = 0;
		
		/*
		if(ch == '\r') 
		{
			index = 0;

		    if(tmp[1] == 'A') // Telephone  Control
			{
    			switch(tmp[2])
				{
					case '1':
						printf("Phone: Device 1 ON \r");
						PrintfSelect = 1;
    					printf("A1101");
						break;
					case '2':
						printf("Phone: Device 2 ON \r");
						PrintfSelect = 1;
    					printf("A1102");
						break;
					case '3':
						printf("Phone: Device 3 ON \r");
						PrintfSelect = 1;
    					printf("A1103");
						break;
					case '4':
						printf("Phone: Device 12 ON \r");
						PrintfSelect = 1;
    					printf("A1112");
						break;


					case '5':
						printf("Phone: Device 1 OFF \r");
						PrintfSelect = 1;
    					printf("A1201");
						break;
					case '6':
						printf("Phone: Device 2 OFF \r");
						PrintfSelect = 1;
    					printf("A1202");
						break;
					case '7':
						printf("Phone: Device 3 OFF \r");
						PrintfSelect = 1;
    					printf("A1203");
						break;
					case '8':
						printf("Phone: Device 12 OFF \r");
						PrintfSelect = 1;
    					printf("A1212");
						break;
			    }
			}
		    else if(tmp[1] == 'F') // Telephone  Report
			{
    			switch(tmp[2])
				{
					case '1':
						printf("Phone: Device 1 Report \r");
						PrintfSelect = 1;
    					printf("A2101");
						PLC_Telephone_Request = 1;
						break;
					case '2':
						printf("Phone: Device 2 Report \r");
						PrintfSelect = 1;
    					printf("A2102");
						PLC_Telephone_Request = 1;
						break;
					case '3':
						printf("Phone: Device 3 Report \r");
						PrintfSelect = 1;
    					printf("A2103");
						PLC_Telephone_Request = 1;
						break;
					case '4':
						printf("Phone: Device 4 Report \r");
						PrintfSelect = 1;
    					printf("A2112");
						PLC_Telephone_Request = 1;
						break;
				 }

			}


			else if(tmp[1] == 'B') // Remot Control
			{
    			switch(tmp[2])
				{
					case '1':
						printf("REMOTE Control 1 \r");
						PrintfSelect = 1;
    					printf("BB101");
						break;
					case '2':
						printf("REMOTE Control 2 \r");
						PrintfSelect = 1;
    					printf("BB102");
						break;
					case '3':
						printf("REMOTE Control 3 \r");
						PrintfSelect = 1;
    					printf("BB103");
						break;
					case '4':
						printf("REMOTE Control 4 \r");
						PrintfSelect = 1;
    					printf("BB104");
						break;
					case '5':
						printf("REMOTE Control 5 \r");
						PrintfSelect = 1;
    					printf("BB105");
						break;
					case '6':
						printf("REMOTE Control 6 \r");
						PrintfSelect = 1;
    					printf("BB106");
						break;
					case '7':
						printf("REMOTE Control 7 \r");
						PrintfSelect = 1;
    					printf("BB107");
						break;
					case '8':
						printf("REMOTE Control 12 \r");
						PrintfSelect = 1;
    					printf("BB112");
						break;
			    }
		   }

			tmp[0] = 0;
			tmp[1] = 0;
			tmp[2] = 0;
		}
		else
			tmp[index++] = ch;
			*/

	}

}



























