/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
#include "VS1003.h"
#include "stm32f10x_spi.h"

#include "config.h"

/* Const define  -------------------------------------------------------------*/
#define RXNE    0x01
#define TXE     0x02
#define BSY     0x80


unsigned char  	SPIPutChar(unsigned char c);

/*******************************************************************************
* Function Name  : Delay
* Description    : Delay
* Input          : nTime--delay time 
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nTime)
{
	unsigned int i;
	unsigned long j;
	for(i = nTime;i > 0;i--)
		for(j = 1000;j > 0;j--);	
}

/*******************************************************************************
* Function Name  : SPIPutChar
* Description    : Send one byte by SPI1
* Input          : outb--the byte to be sended 
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char  SPIPutChar(unsigned char outb)
{   
   /* Write and Read a byte on SPI interface. */
   unsigned char inb;

   /* Wait if TXE cleared, Tx FIFO is full. */
   while ((SPI1->SR & TXE) == 0);
   SPI1->DR = outb;
   /* Wait if RNE cleared, Rx FIFO is empty. */
   while ((SPI1->SR & RXNE) == 0);
   inb = SPI1->DR;
   return (inb);

}

/*******************************************************************************
* Function Name  : SPIGetChar
* Description    : Read a byte from the SPI.
* Input          : None.
* Output         : None
* Return         : The received byte.
*******************************************************************************/
u8 SPIGetChar(void)
{
  u8 Data = 0;

  /* Wait until the transmit buffer is empty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_I2S_SendData(SPI1, 0xFF);

  /* Wait until a data is received */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* Get the received data */
  Data = SPI_I2S_ReceiveData(SPI1);

  /* Return the shifted data */
  return Data;
}

/*******************************************************************************
* Function Name  : VS1003B_Fill2048Zero
* Description    : Write 32 byte 
* Input          : Input buffer
* Output         : None
* Return         : None
*******************************************************************************/
void VS1003B_Fill2048Zero()
{
	unsigned char i,j;

	for(i=0; i<64; i++)
	{    
		Mp3SelectData();
		while (GPIO_ReadInputData(GPIOB) & MP3_DREQ == 0);
		//Delay(12);
		for(j=0;j<32;j++)
			SPIPutChar(0x00);

		Mp3DeselectData();
	}
}

/*******************************************************************************
* Function Name  : VS1003B_Write32B
* Description    : Send one byte by SPI1
* Input          : outb--the byte to be sended 
* Output         : None
* Return         : None
*******************************************************************************/
void VS1003B_Write32B(unsigned char * buf)
{
	unsigned char n = 32;

    Mp3SelectData();

	while(n--){
		SPIPutChar(*buf++);
	}

	Mp3DeselectData();
}

/*******************************************************************************
* Function Name  : VS1003B_Init
* Description    : Init the VS1003 chip
* Input          : None.
* Output         : None
* Return         : None
*******************************************************************************/
int VS1003B_Init(void)
{
	unsigned char retry;

	VS1003B_Config();	/* Prot Initialize */
	VS1003B_SoftReset();
  	Mp3PutInReset();	/* A hardware reset */
	Delay(150);
    Mp3ReleaseFromReset();


	VS1003B_WriteCMD(SPI_MODE, 0x08, 0x04); 			

	Delay(170); 	
	while (GPIO_ReadInputData(GPIOB) & MP3_DREQ == 0); 

	while(VS1003B_ReadCMD(0x03) != CLOCK_REG)	/* set PLL register */
	{
		VS1003B_WriteCMD(SPI_CLOCKF, 0xc0, 0x00);
		if(retry++ >10 )
			return 1;
	}

	Delay(150);	
    retry=0;
	VS1003B_WriteCMD(0x05, 0x00, 0x0a);		      /* Soft start */
	
	Delay(150);	
    retry=0;
    while(VS1003B_ReadCMD(0x0b) != 0xfefe)        /* set Volume to minimum */
    {
        VS1003B_WriteCMD(0x0b, 0xfe, 0xfe);
        if(retry++ >10 )
			return 1;
    }	

	Delay(150);	
    retry=0;
    while(VS1003B_ReadCMD(0x00) != 0x0800)        /* set mode register */
    {
         VS1003B_WriteCMD(0x00, 0x08, 0x00);
         if(retry++ >10 )
		 	return 1;
    }	

	Delay(250);	
    retry = 0;		
	while(VS1003B_ReadCMD(0x02) != DEFAULT_BASS_TREMBLE)        /* set bass/tremble register */
    {
          VS1003B_WriteCMD(0x02, 0X86 , 0x42); // DEFAULT_BASS_TREMBLE
          if(retry++ >10 )
		  	 return 1;
    }
	
	Delay(150);	
	VS1003B_SoftReset();        /* A soft reset */
}

  
/*******************************************************************************
* Function Name  : SoftReset
* Description    : Reset the VS1003 chip
* Input          : None.
* Output         : None
* Return         : None
*******************************************************************************/
void VS1003B_SoftReset()
{
        VS1003B_WriteCMD(0x00, 0x08, 0x04);        /* reset */
        Delay(200);
}

/*******************************************************************************
* Function Name  : VS1003B_Reset
* Description    : Reset the VS1003 chip
* Input          : None.
* Output         : None
* Return         : None
*******************************************************************************/
void VS1003B_Reset(void)
{
	Mp3PutInReset();									     
	Delay(100);		
	SPIPutChar(0xff);									
	Mp3DeselectControl();   							
	Mp3DeselectData();     								
	Mp3ReleaseFromReset(); 								
	Delay(200);	           								
	while (GPIO_ReadInputData(GPIOB) & MP3_DREQ == 0);	
    Delay(100);	
    VS1003B_SoftReset();									
}


/*******************************************************************************
* Function Name  : VS1003B_WriteCMD
* Description    : Write VS1003 register
* Input          : addressbyte--the vs1003 register address
				   highbyte--the hight 8 bits
				   lowbyte--the low 8 bits
* Output         : None
* Return         : None
*******************************************************************************/
void VS1003B_WriteCMD(unsigned char addressbyte, unsigned char highbyte, unsigned char lowbyte)
{
	Mp3DeselectData();
	Mp3SelectControl();
					
	SPIPutChar(VS_WRITE_COMMAND); 	
	SPIPutChar(addressbyte);      	
	SPIPutChar(highbyte);         	
	SPIPutChar(lowbyte);
	          	
	Mp3DeselectControl();
}

void WriteMp3SpiReg(unsigned char addressbyte, int data){

	Mp3DeselectData();
	Mp3SelectControl();				
	SPIPutChar(VS_WRITE_COMMAND); 	
	SPIPutChar(addressbyte);      	
	SPIPutChar(data >> 8);         
	SPIPutChar(data & 0xFF);          
	Mp3DeselectControl();

}


/*******************************************************************************
* Function Name  : VS1003B_ReadCMD
* Description    : Read VS1003 register
* Input          : addressbyte--the vs1003 register address
* Output         : None
* Return         : The register value
*******************************************************************************/
u16 VS1003B_ReadCMD(unsigned char addressbyte)
{
	u16 resultvalue = 0;
	Mp3DeselectData();
	Mp3SelectControl();				//XCS = 0
	SPIPutChar(VS_READ_COMMAND); 	
	SPIPutChar(addressbyte);	 
	resultvalue = SPIGetChar() << 8;
	resultvalue |= SPIGetChar();  	
	Mp3DeselectControl();              
	return resultvalue;           	
}



/*******************************************************************************
* Function Name  : VS1003_Config
* Description    : Configures the GPIO ports and SPI1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void VS1003B_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


  /* PB6->XRESET */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  /* PB97->DREQ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  /* PB8->XDCS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  /* PB9->XCS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);





  /* Disable all pins */
   //GPIOA->ODR = 0xFF;  

  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* SPI1 Config --------------------------------------------------*/ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  SPI_SSOutputCmd(SPI1, ENABLE);

  /* Enable SPI1 */			 
  SPI_Cmd(SPI1, ENABLE);

}


int play_file (const char *path)    
{
    unsigned char  buffer[512];

    int count=0;
    FIL file;
    
	if ((res = f_open(&file, path, FA_READ)) != FR_OK)
	{ 
		printf("\rCan not open file: %s  ErrorCode:%d", path, res);
		f_close(&fdst);
		return 2;
	}       
	Delay(500);  

	VS1003B_Fill2048Zero();

    while(1)                             
    { 			
		f_read(&file, buffer, 512, &br); 
        if (br < 512) 
        {       
             f_close(&file);
             return 10; 
        }

        for(count = 0; count < 16;)
        {           
			     		
			while ((GPIO_ReadInputData(GPIOB) & MP3_DREQ) == 0);
			VS1003B_Write32B(&buffer[count*32]);
			count++;    
        }
    }

    f_close(&file); 	                            
}

/*-----------------------------------------------------------------------*/
int Play_ADPCM (const char *path)    
{
    unsigned char  buffer[512];

    int count=0;
    FIL file;
    
	if ((res = f_open(&file, path, FA_READ)) != FR_OK)
	{ 
		printf("\rCan not open file: %s  ErrorCode:%d", path, res);
		f_close(&fdst);
		return 2;
		
	}       
	Delay(500);  

	//VS1003B_Fill2048Zero();

    while(1)                             
    { 			
		f_read(&file, buffer, 512, &br); 
        if (br < 512) 
        {       
             f_close(&file);
			 printf("\r----------------- 2");
             return 10; 
        }

        for(count = 0; count < 16;)
        {           
			     		
			while ((GPIO_ReadInputData(GPIOB) & MP3_DREQ) == 0);
			VS1003B_Write32B(&buffer[count*32]);
			count++;    
        }
    }

    f_close(&file); 	                            
}