#include "config.h"
//=========================================================================
//ȫ�ֱ���,��������Ԫ�����õı���
//=========================================================================
//���ƴ�ӡ��Ϣ
uchar logEnable = 0x00;

//=========================================================================
//����Ԫ���õı���
//=========================================================================
//���ڻ�������ͷλ��
uchar Serial_Head[COMNUM] = { 0x00, 0x00 };
//���ڻ�������βλ��
volatile uchar Serial_Tail[COMNUM] = { 0x00, 0x00 };
//���ڻ������ӳ�����
volatile uchar Serial_Count[COMNUM] = { 0x00, 0x00 };
//���ڻ�������С
volatile uchar Serial_Buffer[COMNUM][COMLENGTH] = { { 0x00 }, { 0x00 } };

/************************************************************
 ˵��:����0�����жϺ�������
 ************************************************************/
/*
 SIGNAL(SIG_UART0_RECV)
 //���ڽ����жϷ������
 {
 uchar c;
 c = UDR0;

 Serial_Buffer[SERI0][Serial_Tail[SERI0]] = c;
 Serial_Tail[SERI0] = (Serial_Tail[SERI0] + 1) % COMLENGTH;
 Serial_Count[SERI0]++;
 }
 */
/************************************************************
 ˵��:����0���ͺ�������
 ************************************************************/
void Seri0PushSend(uchar c) {
	USART_SendData(USART1, (u8) c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
	}

}

/************************************************************
 ˵��:����1�����жϺ�������
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
 ˵��:����1���ͺ�������
 ************************************************************/
void Seri1PushSend(uchar c) {
	USART_SendData(USART2, (u8) c);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {
	}
}

/************************************************************
 ˵��:���ڷ��ͺ�������
 ************************************************************/
void SeriPushSend(uchar index, uchar c) {
	if (index == SERI1) {
		Seri0PushSend(c);
	} else {
		Seri1PushSend(c);
	}
}

/************************************************************
 ˵��:���ڷ����ַ�����������
 ************************************************************/
void PushSend_String(uchar index, uchar *str) {
	while (*str != 0x00)
		SeriPushSend(index, *(str++));
}

/**
 * ���ڷ����ַ�����������
 */
void Log_PushSend_String(uchar index, uchar *str) {
	if (logEnable == 1) {
		PushSend_String(index, str);
	}

}

/************************************************************
 ˵��:���ڷ��ͺ�������
 ************************************************************/
void Log_SeriPushSend(uchar index, uchar c) {
	if (logEnable == 1) {
		SeriPushSend(index, c);
	}
}

/************************************************************
 ˵��:���ڻ�������ʼ��
 ************************************************************/
void Serial_BufCls(uchar index) {
	Serial_Head[index] = 0x00;
	Serial_Tail[index] = 0x00;
	Serial_Count[index] = 0x00;
}

/************************************************************
 ˵��:����ȡ����,�ⲿ�������,������1,��ʾ���յ�����,��ָ��C��
 ��û���յ�����,�򷵻�0
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
