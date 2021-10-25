

/* Includes ------------------------------------------------------------------*/
#ifndef VS10XX_H
#define VS10XX_H

#include "config.h"


#define CLOCK_REG               0xc000

#define VS1003_AUDATA			0x05
#define VS1003_HEDATA0			0x08
#define VS1003_HEDATA1			0x09
#define VS1003_VOLUME	        0x0B

#define TREMBLE_VALUE	8		/* 0~15, 8 means off  */
#define TREMBLE_LOW_FS	6		/* 0~15, 0Hz-15KHz, lower frequency of tremble enhancement */
#define BASS_VALUE	    4		/* 0~15, 0 means off  */
#define BASS_HIGH_FS	2		/* 2~15, up limit frequency of bass enhancement */

#define DEFAULT_BASS_TREMBLE   ((TREMBLE_VALUE<<12)|(TREMBLE_LOW_FS<<8)|(BASS_VALUE<<4)|(BASS_HIGH_FS))


/* Defines ------------------------------------------------------------------*/
#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03


#define SPI_MODE        	0x00   
#define SPI_STATUS      	0x01   
#define SPI_BASS        	0x02   
#define SPI_CLOCKF      	0x03   
#define SPI_DECODE_TIME 	0x04   
#define SPI_AUDATA      	0x05   
#define SPI_WRAM        	0x06   
#define SPI_WRAMADDR    	0x07   
#define SPI_HDAT0       	0x08   
#define SPI_HDAT1       	0x09   
#define SPI_AIADDR      	0x0a   
#define SPI_VOL         	0x0b   
#define SPI_AICTRL0     	0x0c   
#define SPI_AICTRL1     	0x0d   
#define SPI_AICTRL2     	0x0e   
#define SPI_AICTRL3     	0x0f  

 
#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 
#define SM_LINE_IN       	0x4000 





#define MP3CMD_InitVS1003	0x11
#define MP3CMD_Play			0x12
#define MP3CMD_Pause		0x13
#define MP3CMD_Stop			0x14
#define MP3CMD_Next			0x15
#define MP3CMD_TestVS1003	0x16


#define SCLK (1 << 5)						  	
#define MOSI (1 << 7)
#define MISO (1 << 6)

#define MP3_XRESET   			( 1 << 6 )
#define Mp3PutInReset()  		{ GPIOB->ODR &= ~MP3_XRESET; }
#define Mp3ReleaseFromReset() 	{ GPIOB->ODR |= MP3_XRESET; }
 

#define MP3_XCS  				( 1 << 9)
#define Mp3SelectControl()  	{ GPIOB->ODR &= ~MP3_XCS; }
#define Mp3DeselectControl()  	{ GPIOB->ODR |= MP3_XCS; }

 
#define MP3_XDCS 				( 1 << 8 )
#define Mp3SelectData()			{ GPIOB->ODR &= ~MP3_XDCS; }
#define Mp3DeselectData()		{ GPIOB->ODR |= MP3_XDCS; }


#define MP3_DREQ 		( 1 << 7)

#define VS_Start		0x01
#define VS_End			0x02

#define Mp3SetVolume(leftchannel, rightchannel){Mp3WriteRegister(11,(leftchannel),(rightchannel));}		
	
int Play_ADPCM(const char *path);
		
int 			VS1003B_Init(void);                     /* Initialization, include prot and chip config */
void 			VS1003B_Config(void);
		 
				
void  			VS1003B_SoftReset(void);								
void  			VS1003B_Reset(void);
																
void  			VS1003B_WriteCMD(unsigned char addressbyte, unsigned char highbyte, unsigned char lowbyte);
unsigned short 	VS1003B_ReadCMD(unsigned char addressbyte);	
	
void 			VS1003B_Fill2048Zero();
void 			VS1003B_Write32B(unsigned char *buf);


void WriteMp3SpiReg(unsigned char addressbyte, int data);




#endif