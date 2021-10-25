#include "config.h"
//=========================================================================
//全局变量,被其它单元所引用的变量
//=========================================================================
//控制打印信息
uchar logEnable = 0x00;

//=========================================================================
//本单元自用的变量
//=========================================================================
//串口缓冲区队头位置
uchar Serial_Head[COMNUM] = { 0x00, 0x00 };
//串口缓冲区队尾位置
volatile uchar Serial_Tail[COMNUM] = { 0x00, 0x00 };
//串口缓冲区队长变量
volatile uchar Serial_Count[COMNUM] = { 0x00, 0x00 };
//串口缓冲区大小
volatile uchar Serial_Buffer[COMNUM][COMLENGTH] = { { 0x00 }, { 0x00 } };

/************************************************************
 说明:串口0接收中断函数调用
 ************************************************************/
/*
 SIGNAL(SIG_UART0_RECV)
 //串口接收中断服务程序
 {
 uchar c;
 c = UDR0;

 Serial_Buffer[SERI0][Serial_Tail[SERI0]] = c;
 Serial_Tail[SERI0] = (Serial_Tail[SERI0] + 1) % COMLENGTH;
 Serial_Count[SERI0]++;
 }
 */
/************************************************************
 说明:串口0发送函数调用
 ************************************************************/
void Seri0PushSend(uchar c) {
	USART_SendData(USART1, (u8) c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
	}

}

/************************************************************
 说明:串口1接收中断函数调用
 ************************************************************/
/*
 SIGNAL(SIG_UART1_RECV)
 {
 uchar c;
 c = UDR1;
 Serial_Buffer[SERI1][Serial_Tail[SERI1]] = c;
 Serial_Tail[SERI1] = (Serial_Tail[SERI1] + 1) % COMLENGTH;
 Serial_Count[SERI1]++;
 }
 */
///////////////////////////////////////////////////////////////////////////////
/************************************************************
 说明:串口1发送函数调用
 ************************************************************/
void Seri1PushSend(uchar c) {
	USART_SendData(USART2, (u8) c);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {
	}
}

/************************************************************
 说明:串口发送函数调用
 ************************************************************/
void SeriPushSend(uchar index, uchar c) {
	if (index == SERI1) {
		Seri0PushSend(c);
	} else {
		Seri1PushSend(c);
	}
}

/************************************************************
 说明:串口发送字符串函数调用
 ************************************************************/
void PushSend_String(uchar index, uchar *str) {
	while (*str != 0x00)
		SeriPushSend(index, *(str++));
}

/**
 * 串口发送字符串函数调用
 */
void Log_PushSend_String(uchar index, uchar *str) {
	if (logEnable == 1) {
		PushSend_String(index, str);
	}

}

/************************************************************
 说明:串口发送函数调用
 ************************************************************/
void Log_SeriPushSend(uchar index, uchar c) {
	if (logEnable == 1) {
		SeriPushSend(index, c);
	}
}

/************************************************************
 说明:串口缓冲区初始化
 ************************************************************/
void Serial_BufCls(uchar index) {
	Serial_Head[index] = 0x00;
	Serial_Tail[index] = 0x00;
	Serial_Count[index] = 0x00;
}

/************************************************************
 说明:串口取数据,外部程序调用,若返回1,表示有收到数据,在指针C中
 若没有收到数据,则返回0
 ************************************************************/
uchar SeriPopRece(uchar index, uchar *c) {
	if (Serial_Count[index] == 0x00)
		return 0;
	else {
		*c = Serial_Buffer[index][Serial_Head[index]];
		Serial_Head[index] = (Serial_Head[index] + 1) % COMLENGTH;
		Serial_Count[index]--;
		if (Serial_Count[index] > COMLENGTH)
			Serial_BufCls(index);
		return 1;
	}
}
