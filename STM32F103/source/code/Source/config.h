

#ifndef __Config_h
#define __Config_h

//#include "stm32f10x.h"
#include "stm32f10x_lib.h"
#include "stm32f10x_type.h"
#include "integer.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ff.h"
#include "RTC_Time.h"
#include "stm32f10x_rcc.h"
#include "fuel.h"
#include "Serial.h"
#include "SerialProcess.h"
#include "shell.h"
#include "FATFS_cmds.h"
#include "stm32f10x_exti.h"


#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n) 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n) 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n) 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n) 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n) 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  

#define 	KeyTest   		 PCin(15)

#define 	ON         			1
#define 	OFF        			0

#define 	VALID         		 1
#define 	INVALID        		-1

#define		ENTER				-8
#define		EXIT				-9



#define 	RELAY_RECORD   		PAout(8)
#define 	RELAY_INTERCOM   		PCout(14)
#define 	RELAY_BUSY   			PCout(15)
#define 	RELAY_CALL   			PDout(2)



extern int RemotDelay;
extern int PrintfSelect;
extern int PLC_Telephone_Request ;

extern u8 TimeDisplay;
extern FATFS fs[2]; // Work area (file system object) for logical drive
extern FIL fsrc, fdst, fap, file, fii; // file objects
extern FRESULT res, re; // FatFs function common result code
extern UINT br, bw, bt;
 // File R/W count
extern FILINFO finfo, fno;
extern DIR dirs;
extern time_t current_time;
extern struct tm time_now;
extern u8 passwordflag;
extern u16 AD_value_mul4;
extern char machine[20];
extern u8 timeFirstSet;
extern char shellEnable;
#define UPDATESECONDS 3

extern u16 filter(void);

#endif
