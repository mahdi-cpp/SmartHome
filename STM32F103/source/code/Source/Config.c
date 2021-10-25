#include "config.h"


int fputc(int ch, FILE *f) {


	switch(PrintfSelect)
	{
		case 1:
			/* Write a character to the USART */
			USART_SendData(USART1, (u8) ch);

			/* Loop until the end of transmission */
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			break;
		case 2:
			/* Write a character to the USART */
			USART_SendData(USART2, (u8) ch);

			/* Loop until the end of transmission */
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			break;
		case 3:
			/* Write a character to the USART */
			USART_SendData(USART3, (u8) ch);

			/* Loop until the end of transmission */
			while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
			break;
	}


	return ch;
}








void USART3_Configuration() {
	GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl
			= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);

}


