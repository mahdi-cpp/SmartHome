
#ifndef _INIT_H
#define _INIT_H



extern TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
extern TIM_ICInitTypeDef TIM_ICInitStructure;
extern TIM_OCInitTypeDef TIM_OCInitStructure;

extern ErrorStatus HSEStartUpStatus;

extern u8 timeFirstSet ;
			  
void RCC_Configuration(void);
void TIM_Configuration(void);
void NVIC_Configuration(void);


void USART3_Configuration();


void GPIO_Configuration(void);
void RTC_Configuration(void);
void RTC_Config(void);



#endif