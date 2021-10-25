
/******************** (C) COPYRIGHT 2013 AzaranSystem ********************
 * File Name          : PhoneService.c
 * Author             : Mehdi Abdolmaleki
 * Version            : V1.0
 * Date               : 1392/1/11
 *************************************************************************/


#include "Config.h"
#include "PhoneService.h"
#include "VS1003.h"
#include "MT8888C.h"

#include <stdio.h>

unsigned char NewPhoneNumber[11];

uchar Alert_Sound[20];

int SwitchToIntercom = 0;
int IntercomSelect = 0;

int PhoneServiceBusy = 0;

int Start_Telephone = 0;
int StartTelephoneSelect; //used for detect phon ring of intercom select

#define     MAIN_FILES		16

/* IMA ADPCM Header */
static unsigned char header[60] = {  
'R',  'I',  'F',  'F',  // Chunk ID (RIFF) 
0x34, 0xCE, 0x00, 0x00, // Chunk payload size (calculate after rec!) (FileSize - 8)(n * 256 + 60 - 8)
'W',  'A',  'V',  'E',	// RIFF resource format type

'f',  'm',  't',  ' ',	// Chunk ID (fmt ) 
0x14, 0x00, 0x00, 0x00, // Chunk payload size (0x14 = 20 bytes) 
0x11, 0x00,				// Format Tag (IMA ADPCM)
0x01, 0x00,			    // Mono Sound 
0x40, 0x1f, 0x00, 0x00, // Sample Rate, 0x1f40 = 8kHz
0x08, 0xFD, 0x00, 0x00, // Average Bytes Per Second
0x00, 0x01, 		    // Data Block Size (256 bytes)
0x04, 0x00,			    // ADPCM encoded bits per sample (4 bits)
0x02, 0x00, 			// Extra data (2 bytes)
0xf9, 0x01,				// Samples per Block (505 samples)
				
'f',  'a', 'c',  't',	// Chunk ID (fact)		
0x04, 0x00, 0x00, 0x00, // Chunk payload size (456 bytes (zeropad!))
0x00, 0xCE, 0x00, 0x00,	// Number of Samples (calculate after rec!)	 (n * 256)
0x64, 0x61, 0x74, 0x61,	// Chunk ID (data)
0x00, 0x00, 0x32, 0x00	// Chunk payload size (calculate after rec!) (n * 256)
};

static unsigned char db[256] ; /* data buffer for saving to disk */

char Telephone_Password[4];

int  PLC_Telephone_Response;
int  PLC_Telephone_Password;

int  PhoneServiceTimeout = 5;

int  PhoneServicePassword[4] ;

int  OnOffStatus;
int  DeviceValue;

static int InvalidPasswordCount = 0;

//-------------------------------------------------------------------------------------
int GetPassword()
{
	int i;	
	//PrintfSelect = 3;
	//printf("\rPhone: Get Password Timeout!"); 
		
    PrintfSelect = 1;
    printf("PPP");
   
	PLC_Telephone_Password = 1;

 	PhoneServiceTimeout = 0;  //Reset Timeout
    while(PhoneServiceTimeout < 200) // Wait for 2 second 
    {	
		if(PLC_Telephone_Password == 0)
		{
			for(i = 0; i < 4; i++)
 				PhoneServicePassword[i] = (Telephone_Password[i] - 48);

			PrintfSelect = 3;
			printf("\rPssword: %d %d %d %d", PhoneServicePassword[0],
					PhoneServicePassword[1], PhoneServicePassword[2], PhoneServicePassword[3]);
			return 	1;
		}
		delay_ms(50);
    }

	PrintfSelect = 3;
	printf("\rPhone: Get Password Timeout!"); 
	return -1;

}

//-------------------------------------------------------------------------------------
static int ReportDevice()
{
	PhoneServiceTimeout = 0;  //Reset Timeout

    while(PhoneServiceTimeout < 300) // Wait for 3 second 
    {	
		if(PLC_Telephone_Response != '0')
		{
			return 	1;
		}
		delay_ms(50);
    }

	PrintfSelect = 3;
	printf("\rPhone: Report Device Timeout!"); 
	return -1;
}

//-------------------------------------------------------------------------------------
static int GetNumber()
{	
	int LastNumberReceive;

	PhoneServiceTimeout = 0;  //Reset Timeout
	MT8888C_DTMF_Receive_Init();

    while(PhoneServiceTimeout < 800) // Wait for 5 second 
    {	
		LastNumberReceive = MT8888C_GetLastNumber();

		if(LastNumberReceive != 20)
		{
			MT8888C_SetLastNumber(20);

			return 	LastNumberReceive;
		}
		delay_ms(50);
   }
	PrintfSelect = 3;
	printf("\rPhone: Get Number Timout!"); 
	return -1;
}

//-------------------------------------------------------------------------------------
static int GetNumberMicrophone()
{	
	int LastNumberReceive;

	PhoneServiceTimeout = 0;  //Reset Timeout
	MT8888C_DTMF_Receive_Init();

    while(PhoneServiceTimeout < 3000) // Wait for 30 second 
    {	
		LastNumberReceive = MT8888C_GetLastNumber();

		if(LastNumberReceive != 20)
		{
			MT8888C_SetLastNumber(20);

			return 	LastNumberReceive;
		}
		delay_ms(50);
   }
	PrintfSelect = 3;
	printf("\rPhone: Get Number Timout!"); 
	return -1;
}

//-------------------------------------------------------------------------------------
static int OnOffDevice()
{	
	int LastNumberReceive;
	int i = 0, tmp;
	PhoneServiceTimeout = 0;  //Reset Timeout
	MT8888C_DTMF_Receive_Init();
	DeviceValue = 0; //init

    while(PhoneServiceTimeout < 800) // Wait for 5 second 
    {	
		LastNumberReceive = MT8888C_GetLastNumber();

		if(LastNumberReceive != 20)
		{
			MT8888C_SetLastNumber(20);

			if(LastNumberReceive == 11 || LastNumberReceive == 12 && DeviceValue !=0){
				return 	LastNumberReceive;
			}
			else{
			
				if(i == 0)
				{
					DeviceValue = LastNumberReceive ;
					tmp = LastNumberReceive ;
				}
				else if(i == 1)
				{
				 	DeviceValue =  (tmp * 10);
					DeviceValue += LastNumberReceive;
				}
				i++;
			}
		}
		delay_ms(50);
    }
	PrintfSelect = 3;
	printf("\rPhone: On Off Device Timeout!"); 
	return -1;
}

//-------------------------------------------------------------------------------------
int PhoneServiceCheckPassword()
{
	int i= 0, j , LastNumberReceive;
    char tmp[MaxPasswordLen];
  
	MT8888C_DTMF_Receive_Init();

	PhoneServiceTimeout = 0; //Reset Timeout
    while(PhoneServiceTimeout < 800) // Wait for 5 second 
    {
		
		LastNumberReceive = MT8888C_GetLastNumber();
		if(LastNumberReceive != 20)
		{
		    MT8888C_SetLastNumber(20);

			if(LastNumberReceive == 11)	 // Press * Key for compare passwords
			{ 
				 for(i=0 ; i < PasswordLen; i++)
				 {
				  	if(tmp[i] != PhoneServicePassword[i])
						return -1;
				 }
				 return 1;
			}
			else
			{
				if(i > MaxPasswordLen ){
					printf("\rMaxPasswordLen"); 
					return -1;
				}

				if(LastNumberReceive == 10)
					tmp[i] = 0;  //Save New Number of password
				else
					tmp[i] = LastNumberReceive;  //Save New Number of password



				i++;
				PhoneServiceTimeout = 0;	//Set for receive new key
				MT8888C_DTMF_Receive_Init();
			}

		}

		delay_ms(100);
    }
	PrintfSelect = 3;
	printf("\rPhone: Check Password Timeout!"); 
    return -1;
}

//-----------------------------------------------------------------------------------------
int PhoneService()
{
	int Result;

	
	RingCounter = 0;
	

	PhoneServiceBusy = 1;
	//GetPassword();

	RELAY_BUSY = ON;
	RELAY_INTERCOM = OFF;
	RELAY_CALL = OFF;
	RELAY_RECORD = OFF;

	delay_ms(200);

	PhoneServiceBusy = 0;
	
	
	
	PHONE_WELCOME:
	printf("\rPhone: Welcome!");
	PhoneServicePlay("1:/system/welcome.mp3"); 	//Welcome to System
	Result = GetNumber();


	
	if(Result == -1){
	  	PrintfSelect = 3;
		printf("\rPhone: Disconnect! ");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	}
	else if(Result == 1){
 		PhoneServiceRecord();
		return 0;
	}
	else if(Result != 2)
		goto  PHONE_WELCOME;


   	
    PHONE_REPEAT_PASSWORD:				
	printf("\rEnter Password:"); 
	PhoneServicePlay("1:/system/4.mp3");  //Enter Password

	Result = PhoneServiceCheckPassword();

	if(Result == INVALID){
		printf("\rInvalid Password!"); 
		PhoneServicePlay("1:/system/3.mp3");  //Invalid Password

		if(InvalidPasswordCount < 2){
			InvalidPasswordCount++;
			goto PHONE_REPEAT_PASSWORD;
		}
		else{
			printf("\rPhone: Invalid Password!");
			printf("\rSystem Reset... \r");
			NVIC_GenerateSystemReset();
		}
	}
	
   	
	

	PHONE_SET_REPORT:
	PhoneServicePlay("1:/system/6.mp3");  //SET devices or Report devices status
	Result = GetNumber();
	if(Result == -1){
	  	PrintfSelect = 3;
		printf("\rPhone: Disconnect!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	}




	if(Result == 1) //Set Device
	{ 
		PhoneServicePlay("1:/system/5.mp3");  

		while(Result != -1)
		{
			MT8888C_DTMF_Receive_Init();
			Result = OnOffDevice();

		    if(Result == 12 && DeviceValue == 12) //Back to main menu
				goto  PHONE_SET_REPORT;
			else if(Result == 11){
				PrintfSelect = 3;
				printf("\rPhone: Device %02d ON ", DeviceValue);
				PrintfSelect = 1;
    			printf("A11%02d", DeviceValue);
				PhoneServicePlay("1:/system/7.mp3");  //Turn on device 
			}
			if(Result == 12){
				PrintfSelect = 3;
				printf("\rPhone: Device %02d Off ", DeviceValue);
				PrintfSelect = 1;
    			printf("A12%02d", DeviceValue);
				PhoneServicePlay("1:/system/8.mp3");  //Turn off device
			}
			else{
				PrintfSelect = 3;
				printf("\rPhone: Error Set Device");
			}
		} 

		PrintfSelect = 3;

		printf("\rPhone: Timeout Set Device");
		printf("\rPhone: Disconnect!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	}





	else if(Result == 2)
	{  //Report Device
		PhoneServicePlay("1:/system/9.mp3");  //Report device select

		while(Result != -1)
		{
			MT8888C_DTMF_Receive_Init();
			Result = OnOffDevice();

			if(Result == 12 && DeviceValue == 12) //Back to main menu
				goto  PHONE_SET_REPORT;
			else if(Result == 11)
			{
				
				PrintfSelect = 1;
				//printf("A2103");
    			printf("A21%02d", DeviceValue);

				PLC_Telephone_Request = 1;

				PrintfSelect = 3;
				printf("\rPhone: Device Report %02d", DeviceValue);

				delay_ms(500);
		    	ReportDevice();

				if(PLC_Telephone_Response == '1'){
					PLC_Telephone_Response = '0';
					PrintfSelect = 3;
					printf("\rPhone: Device Response %02d ON", DeviceValue);
	  				PhoneServicePlay("1:/system/11.mp3");  //Device is ON
				}
				else if(PLC_Telephone_Response == '2'){
					PLC_Telephone_Response = '0';
					PrintfSelect = 3;
					printf("\rPhone: Device Response %02d OFF", DeviceValue);
	  				PhoneServicePlay("1:/system/12.mp3");  //Device is Off 
				}
				else{
					PrintfSelect = 3;
					printf("\rPhone: Error Report Device: %c", PLC_Telephone_Response);
				}
			}
		}
		PrintfSelect = 3;

		printf("\rPhone: Timeout Report Device");
		printf("\rPhone: Disconnect!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
		 
	}
	else if(Result == 3)
	{
		PlayRecordMessages();
		goto  PHONE_SET_REPORT;

	}
	else if(Result == 4)
	{
		MicrophoneEnable();
		goto  PHONE_SET_REPORT;

	}
	else
		goto  PHONE_SET_REPORT;	
		


		
		
}










//-------------------------------------------------------------------------------------
static int PlayRecordMessages(void) 
{
 	FIL file;
	FRESULT res;
    FILINFO fno;
    DIR dir;
	int w = 0, idx = 0, i, j, Result, WritenCount = 0, FileCount = 0, RecordMessagesCount;
	char Str[2], Path[15];

	PrintfSelect = 3;

	SPI_Configuration();
    res = f_opendir(&dir, "1:messages"); 							/* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   	/* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) 
				break; 
			FileCount++;									/* Break on error or end of dir */           
        }
    }
	else{
	    printf("\rPhone: Can not open directory messages");
	    printf("\rPhone: Play Record Messages!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	}
	FileCount -= 2;



	printf("\rRecord Messages is: %d", FileCount);

	if(FileCount == 0){ 	
		printf("\rPhone: No message is record!");
		PhoneServicePlay("1:/system/15.mp3");
		return -1;
	} 

	j = FileCount;

    PhoneServicePlay("1:/system/10.mp3");
	Result = GetNumber();
	
	if(Result != 12){					 // Play last record message 
		strcpy(Path, "1:/messages/");
		sprintf(Str, "%d", j);
		strcat(Path, Str);
   		strcat(Path, ".wav");

		printf("\rPhone: PLAY File %s", Path);
		PhoneServicePlay(Path);
	}

	while(Result != -1)
	{
		MT8888C_DTMF_Receive_Init();
		Result = GetNumber();

		if(Result == 1)
		{
			j -= 1;
			REPEAT_PLAY_1:
			if(j > 0)
			{
				strcpy(Path, "1:/messages/");
				sprintf(Str, "%d", j);
				strcat(Path, Str);
   				strcat(Path, ".wav");

				printf("\rPhone: PLAY File %s", Path);

				PhoneServicePlay(Path);
 			}
			else{
				j = 1;
				goto REPEAT_PLAY_1;
			}
		}
		else if(Result == 2)
		{
			j += 1;
			REPEAT_PLAY_2:
			if(j <= FileCount)
			{
				strcpy(Path, "1:/messages/");
				sprintf(Str, "%d", j);
				strcat(Path, Str);
   				strcat(Path, ".wav");

				printf("\rPhone: PLAY File %s", Path);
				PhoneServicePlay(Path);
 			}
			else{
				j = FileCount;
				goto REPEAT_PLAY_2;
			}
		}

		else if(Result == 11)
		{
   			SPI_Configuration();
    		res = f_opendir(&dir, "1:/messages"); 						
    		if (res == FR_OK) 
			{
        		for (;;)
				{ 
            		res = f_readdir(&dir, &fno);                   	
            		if (res != FR_OK || fno.fname[0] == 0) 
						break; 

					strcpy(Path, "1:/messages/");
					strcat(Path, fno.fname);

					res = f_unlink(Path); 
					printf("\r%s  Result:%d" , Path, res);	         
        		}

				PhoneServicePlay("1:/system/13.mp3");
				return 1;
			
			}
			else{
	    		printf("\rPhone: Can not open directory messages");
	    		printf("\rPhone: Play Record Messages!");

				printf("\rSystem Reset... \r");
				NVIC_GenerateSystemReset();
			}			
		}
		else 
			return 1;

	}
	
	printf("\rPhone: Timeout Play Record Messages");
	printf("\rPhone: Disconnect!");

	printf("\rSystem Reset... \r");
	NVIC_GenerateSystemReset();


}

//-------------------------------------------------------------------------------------
int PhoneServiceRecord(void) 
{ 
 	FIL file;
	FRESULT res;
    FILINFO fno;
    DIR dir;
	int w = 0, idx = 0, i, WritenCount = 0, FileCount = 0;
	char Str[2], Path[15];

	PrintfSelect = 3;

	//printf("\rPhone: Please next listen melodi place message!");
	//PhoneServicePlay("1:/system/14.mp3"); 
	delay_ms(100);
	printf("\rPhone: PLAY MELODI!");
	PhoneServicePlay("1:/system/16.mp3");

	VS1003B_Config();
		
   VS1003B_SoftReset(); /* Normal reset, restore default settings */

	delay_ms(50);
	VS1003B_WriteCMD(0x0b, 0x00, 0x00); /* Recording monitor volume */

	delay_ms(50);
	VS1003B_WriteCMD(SPI_BASS, 0, 0); /* Bass/treble disabled */

	delay_ms(50);
	VS1003B_WriteCMD(SPI_CLOCKF, 0x44, 0x30); /* 2.0x 12.288MHz */

	delay_ms(50);
	VS1003B_WriteCMD(SPI_AICTRL0, 0, 12); /* Div -> 12=8kHz 8=12kHz 6=16kHz */

	delay_ms(50);
	VS1003B_WriteCMD(SPI_AICTRL1, 0x00, 0x00); /* Auto gain */
	delay_ms(50);	

	VS1003B_WriteCMD(SPI_MODE, 0x58, 0x04); /* Normal SW reset + other bits  line_in*/


   RELAY_CALL = OFF;
   RELAY_RECORD = ON;


	 SPI_Configuration();
    res = f_opendir(&dir, "1:messages"); 							/* Open the directory */
    if (res == FR_OK) 
    {
        for (;;) {
            res = f_readdir(&dir, &fno);                   	/* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) 
				break; 
			FileCount++;													/* Break on error or end of dir */           
        }
    }	 
	 else
	 {
		  f_mkdir("1:messages");
		  delay_ms(50);
	     printf("\rPhone: Can not open directory messages");
	     printf("\rPhone: Play Record Messages!");

		  printf("\rSystem Reset... \r");
		  NVIC_GenerateSystemReset();
	 }

	
	FileCount -= 2;

	strcpy(Path, "1:/messages/");
	sprintf(Str, "%d", ++FileCount);
	strcat(Path, Str);
   strcat(Path, ".wav");

	printf("\rPhone: File Count is: %d", FileCount); 
	if ((res = f_open(&file, Path, FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK)
	{ 
		printf("\rPhone: Can not open file: %s  ErrorCode:%d", Path, res);
		f_close(&file);

	    printf("\rPhone: Service Record!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	}			

	f_write(&file, &header, sizeof(header), &WritenCount); /* Write output block to disk */
	printf(" %d ", WritenCount); 
	
	VS1003B_Config();	

	/* Record loop */	
	for(i=0; i<800; i++) 
	{
		do{
			while (GPIO_ReadInputData(GPIOB) & MP3_DREQ == 0); 
			w = VS1003B_ReadCMD(SPI_HDAT1);

		} while (w <= 128 || w >= 896); /* wait until 512 bytes available */
		

		while (idx < 256) 
		{
			while (GPIO_ReadInputData(GPIOB) & MP3_DREQ == 0); 
			w = VS1003B_ReadCMD(SPI_HDAT0);
			db[idx++] = w>>8;
			//printf("%d ", idx);
			db[idx++] = w&0xFF;
			//printf("%d ", idx);
		}
		idx = 0;

		SPI_Configuration();
		f_write(&file, &db, sizeof(db), &WritenCount); /* Write output block to disk */
		printf(" %d ", WritenCount); 
  		VS1003B_Config();	/* Port Initialize */
	}
   
   	f_close(&file);    
	printf("\rPhone: End of Recording!\n");

	printf("\rSystem Reset... \n");
	NVIC_GenerateSystemReset();
}

//-------------------------------------------------------------------------------------
int PhoneServicePlay(char *path) {
     
	UINT   br;
   unsigned char  buffer[512];
   int count=0;
   FIL file;
    
	VS1003B_Config();
	VS1003B_SoftReset();  
	delay_ms(5);

	SPI_Configuration();    
    if ((res = f_open(&file, path, FA_READ)) != FR_OK){ 
		PrintfSelect = 3;
		printf("\rPhone Service Play: Can not open file %s : %d", path, res);
		f_close(&fdst);
		return 2;
	} 
	      
	Delay(500);
	VS1003B_Config();      
	//VS1003B_Fill2048Zero();

    while(1)                             
    { 
		//SPI_Configuration();
		f_read(&file, buffer, 512, &br); 
		if (br < 512) {       
			f_close(&file);
			return 10; 
		}

 		//VS1003B_Config(); 
		for(count = 0; count < 16;)
		{             
			while ((GPIO_ReadInputData(GPIOB) & MP3_DREQ) == 0); 
				VS1003B_Write32B(&buffer[count*32]);

			if(MT8888C_GetLastNumber() != 20){
				f_close(&file);
				return 18;
			}
            count++;    
        }
    }		
    f_close(&file); 				     
}

//-------------------------------------------------------------------------------------
void Call_Alert(unsigned char PhoneNumber[], int AlertType)
{
	int i;
	

	delay_ms(500);
	//phne number is mobile
	if(PhoneNumber[0] == '9')
	{
		NewPhoneNumber[0] = '0';

		for(i=1; i<11; i++)
			NewPhoneNumber[i] = PhoneNumber[i-1];

		MT8888C_DTMF_Send(NewPhoneNumber);
	}
	else  //phone number is dakheli
		MT8888C_DTMF_Send(PhoneNumber);


 	

	for(i=0; i<10; i++)
		delay_ms(500);


	sprintf(Alert_Sound, "1:/Alert/%c.mp3" , AlertType);

	PrintfSelect = 3;
	printf("\rPlay File: %s", Alert_Sound);

	for(i=0; i<8; i++)
		PhoneServicePlay(Alert_Sound);
	
	//delay_ms(900);
	//delay_ms(900);



	RELAY_BUSY = OFF;
	RELAY_CALL = OFF;

	for(i=0; i<10; i++)
		delay_ms(500);
}


int IntercomPhoneService()
{
	int Result;

	
	IntercomSelect = 0;
	


	//GetPassword();

	
	
	PHONE_WELCOME:
	printf("\rPhone: Welcome!");
	PhoneServicePlay("1:/system/welcome.mp3"); 	//Welcome to System
	Result = GetNumber();

	
	if(Result == -1){
	  	PrintfSelect = 3;
		printf("\rPhone: Disconnect! ");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	}
	else if(Result == 1){
 		PhoneServiceRecord();
		return 0;
	}
	else if(Result != 2)
		goto  PHONE_WELCOME;

   	
   PHONE_REPEAT_PASSWORD:				
	printf("\rEnter Password:"); 
	PhoneServicePlay("1:/system/4.mp3");  //Enter Password

	Result = PhoneServiceCheckPassword();

	if(Result == INVALID){
		printf("\rInvalid Password!"); 
		PhoneServicePlay("1:/system/3.mp3");  //Invalid Password

		if(InvalidPasswordCount < 2){
			InvalidPasswordCount++;
			goto PHONE_REPEAT_PASSWORD;
		}
		else{
			printf("\rPhone: Invalid Password!");
			printf("\rSystem Reset... \r");
			NVIC_GenerateSystemReset();
		}
	}
   	
	

	PHONE_SET_REPORT:
	PhoneServicePlay("1:/system/6.mp3");  //SET devices or Report devices status
	Result = GetNumber();
	if(Result == -1){
	  	PrintfSelect = 3;
		printf("\rPhone: Disconnect!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	}



	if(Result == 1) //Set Device
	{ 
		PhoneServicePlay("1:/system/5.mp3");  

		while(Result != -1)
		{
			MT8888C_DTMF_Receive_Init();
			Result = OnOffDevice();

		    if(Result == 12 && DeviceValue == 12) //Back to main menu
				goto  PHONE_SET_REPORT;
			else if(Result == 11){
				PrintfSelect = 3;
				printf("\rPhone: Device %02d ON ", DeviceValue);
				PrintfSelect = 1;
    			printf("A11%02d", DeviceValue);
				PhoneServicePlay("1:/system/7.mp3");  //Turn on device 
			}
			if(Result == 12){
				PrintfSelect = 3;
				printf("\rPhone: Device %02d Off ", DeviceValue);
				PrintfSelect = 1;
    			printf("A12%02d", DeviceValue);
				PhoneServicePlay("1:/system/8.mp3");  //Turn off device
			}
			else{
				PrintfSelect = 3;
				printf("\rPhone: Error Set Device");
			}
		} 

		PrintfSelect = 3;

		printf("\rPhone: Timeout Set Device");
		printf("\rPhone: Disconnect!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	}





	else if(Result == 2)
	{  //Report Device
		PhoneServicePlay("1:/system/9.mp3");  //Report device select

		while(Result != -1)
		{
			MT8888C_DTMF_Receive_Init();
			Result = OnOffDevice();

			if(Result == 12 && DeviceValue == 12) //Back to main menu
				goto  PHONE_SET_REPORT;
			else if(Result == 11)
			{
				
				PrintfSelect = 1;
				//printf("A2103");
    			printf("A21%02d", DeviceValue);

				PLC_Telephone_Request = 1;

				PrintfSelect = 3;
				printf("\rPhone: Device Report %02d", DeviceValue);

				delay_ms(500);
		    	ReportDevice();

				if(PLC_Telephone_Response == '1'){
					PLC_Telephone_Response = '0';
					PrintfSelect = 3;
					printf("\rPhone: Device Response %02d ON", DeviceValue);
	  				PhoneServicePlay("1:/system/11.mp3");  //Device is ON
				}
				else if(PLC_Telephone_Response == '2'){
					PLC_Telephone_Response = '0';
					PrintfSelect = 3;
					printf("\rPhone: Device Response %02d OFF", DeviceValue);
	  				PhoneServicePlay("1:/system/12.mp3");  //Device is Off 
				}
				else{
					PrintfSelect = 3;
					printf("\rPhone: Error Report Device: %c", PLC_Telephone_Response);
				}
			}
		}
		PrintfSelect = 3;

		printf("\rPhone: Timeout Report Device");
		printf("\rPhone: Disconnect!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
		 
	}
	else if(Result == 3)
	{
		PlayRecordMessages();
		goto  PHONE_SET_REPORT;

	}
	else
		goto  PHONE_SET_REPORT;	
}



//-------------------------------------------------------------------------------------
int MicrophoneEnable(void) 
{ 
 	FIL file;
	FRESULT res;
   FILINFO fno;
   DIR dir;
	int w = 0, idx = 0, i, WritenCount = 0, FileCount = 0;
	char Str[2], Path[15];

	PrintfSelect = 3;

	//printf("\rPhone: Please next listen melodi place message!");
	//PhoneServicePlay("1:/system/14.mp3"); 
	delay_ms(100);
	//printf("\rPhone: PLAY MELODI!");
	//PhoneServicePlay("1:/system/16.mp3");

	printf("\rPhone: Start Microphone Service");

	VS1003B_Config();
		
   //VS1003B_SoftReset(); /* Normal reset, restore default settings */

	//delay_ms(50);
	VS1003B_WriteCMD(0x0b, 0x02, 0x02); /* Recording monitor volume */

	delay_ms(50);
	VS1003B_WriteCMD(SPI_BASS, 0, 0); /* Bass/treble disabled */

	//delay_ms(50);
	//VS1003B_WriteCMD(SPI_CLOCKF, 0x44, 0x30); /* 2.0x 12.288MHz */

	//delay_ms(50);
	//VS1003B_WriteCMD(SPI_AICTRL0, 0, 12); /* Div -> 12=8kHz 8=12kHz 6=16kHz */

	delay_ms(50);
	VS1003B_WriteCMD(SPI_AICTRL1, 0x00, 0x00); /* Auto gain */
	delay_ms(50);	

	//VS1003B_WriteCMD(SPI_MODE, 0x58, 0x04); /* Normal SW reset + other bits  line_in*/
	 VS1003B_WriteCMD(SPI_MODE, 0x18, 0x04); /* Normal SW reset + other bits  microphone*/


	 //Result = 

	GetNumberMicrophone();// End off program  with all keys

	//if(Result == 12)  
	//{				
		printf("\rPhone: End of Microphone service");
		printf("\rPhone: Disconnect!");

		printf("\rSystem Reset... \r");
		NVIC_GenerateSystemReset();
	//}

}
//-------------------------------------------------------------------------------------





