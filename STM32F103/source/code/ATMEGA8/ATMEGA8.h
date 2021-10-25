

#ifndef MT8888C_H
#define MT8888C_H

//-------------------------------------------------	

#define 	M8_Q0   	PCin(12)	  
#define 	M8_Q1   	PCin(11)
#define 	M8_Q2   	PCin(10)	  
#define 	M8_Q3   	PAin(12)


#define 	STM32_TO_M8   	PCout(13)
#define 	STM32_Q0   		PCout(12)
#define 	STM32_Q1   		PCout(11)
#define 	STM32_Q2   		PCout(10)
#define 	STM32_Q3   		PAout(12)

//-------------------------------------------------


void  ATMEGA8_Init();
void  ATMEGA8_RESET();
void  ATMEGA8_Write();
void  ATMEGA8_Read();





#endif