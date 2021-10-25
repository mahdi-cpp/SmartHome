



#include "stm32f10x_lib.h"
#include "fuel.h"
#include "Serial.h"

#include "init.h"



#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */
   

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;


ErrorStatus HSEStartUpStatus;

u8 timeFirstSet ;



void TIM_Configuration(void) 
{ 
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_DeInit( TIM2);                            
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  

	TIM_BaseInitStructure.TIM_Period = 10000; 		
	TIM_BaseInitStructure.TIM_Prescaler = 71;  	
	TIM_BaseInitStructure.TIM_ClockDivision = 0; 
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure); 
	

	TIM_ClearFlag(TIM2, TIM_FLAG_Update); 
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
	 
	TIM_Cmd(TIM2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = 28;    //TIM2 Global Interrupt
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 20;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
	NVIC_Init(&NVIC_InitStructure);    
}


			  
void RCC_Configuration(void) {
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig( RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS) {
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency( FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig( RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config( RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config( RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		/* Enable PLL */
		RCC_PLLCmd( ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);


		/* Select PLL as system clock source */
		RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08);

	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
			| RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); /* TIM1 clock enable */

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

}




void NVIC_Configuration(void) {

	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

	//NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1);
	/* Enable the RTC Interrupt */
	//NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQChannel;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);


	/* Enable the USART1 Interrupt */
	//NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

	/* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}










void RTC_Configuration(void) {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd( ENABLE);

	BKP_DeInit();

	RCC_LSEConfig( RCC_LSE_ON);

	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	RCC_RTCCLKConfig( RCC_RTCCLKSource_LSE);

	RCC_RTCCLKCmd(ENABLE);

	RTC_WaitForSynchro();

	RTC_WaitForLastTask();

	//RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_SetPrescaler(32767);

	RTC_WaitForLastTask();

	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	RTC_WaitForLastTask();

	return;
}

void RTC_Config(void) {

	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) {
		printf("\r\n\n RTC not yet configured....");

		printf("\r\n请输入时间，如: !t2010 10 10 10 10 10!");
		RTC_Configuration();

		while (timeFirstSet != 1) {
			ProcessProtocol( SERI1);
		}

		//		Time_Adjust();
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	} 
	else {

		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) {

		} else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) {

		}
		printf("\r\n No need to configure RTC....");

		RCC_ClearFlag();

		RCC_RTCCLKCmd( ENABLE);

		RTC_WaitForSynchro();

		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		RTC_WaitForLastTask();
	}
	RCC_ClearFlag();

#ifdef RTCClockOutput_Enable
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd( ENABLE);

	/* Disable the Tamper Pin */
	BKP_TamperPinCmd( DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
	 functionality must be disabled */

	/* Enable RTC Clock Output on Tamper Pin */
	BKP_RTCOutputConfig( BKP_RTCOutputSource_CalibClock);
#endif

	return;
}
