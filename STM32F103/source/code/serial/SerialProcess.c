#include "config.h"
/////////////////////////////////////////////////////////////
//����Э�������ʽ:
//��ͷ +������ + �������� + ��β
//!n=machine1!
//!t=20100312202020
//����Э���ܳ���Ϊ256,��ͷ,��β,�豸����,������,�����Ⱦ�Ϊһ
//�ֽ��ֶ�,���Դ�Э������ܴ������ݳ���Ϊ249���ֽڵ����ݰ�.
//��ͷ���β��Ϊ0x21,��"!"��
//����Ϊ�����ֽ�,���������豸���͵������ֽ�ǰ������,���鷽
//ʽΪ�����
//�豸����Ϊ������

//ת�崦��:
//���豸���Ϳ�ʼ,ֱ���������ݽ���,���п��ܳ�����Э��ͷ/β��ͬ���ַ�
//�����ִ��ַ�ʱ,��Ҫ����ת�崦��,��������:
//1.�����ݰ����Ȼ����������г���0x21ʱ,������Щ�ַ�֮ǰ����һ������
//  ��0x3D�������(0x1C)��ֵ,��������0x21ʱ,ʵ�ʴ����ַ�Ϊ�����ֽ�:
//   0x3D 0x1C
//2.�����ݰ����Ȼ����������г���0x3Dʱ,����0x3D֮�����һ��0x00,��ʵ
//  �ʴ����ַ�Ϊ�����ֽ�:0x3D 0x00
//3.���ݽ����ߵ��յ�0x3Dʱ,�������ֽ��ӵ�,�ҽ����һ���ֽ���0x3D����
//  ���.


/**
 * 	The volatile of Serial
 */
uchar bStart[COMNUM] = { 0x00, 0x00 };
uchar bGet3D[COMNUM] = { 0x00, 0x00 };
uchar Pos[COMNUM] = { 0x00, 0x00 };
uchar RData_Buffer[COMNUM][COMLENGTH] = { { 0x00, 0x00, 0x00, 0x00, 0x00, }, {
		0x00 } };
uchar WData_Buffer[COMNUM][COMLENGTH] = { { 0x00 }, { 0x00 } };

#define CHANGETIME  't'
#define CHANGEPARAM  'p'
#define SHELL  's'

void ChangeName(void);
void ChangeTime(void);
void DoSomething();
void ShellControl(void);
void ChangeParam(void);
/*******************************************************
 ����һ�����ܻ����ת�������ַ���,����ֵΪת���Ӧ����
 �ӵĽ���ֵ
 *******************************************************/
uchar SendTransChar(uchar index, uchar c) {
	if (c == 0x3D) {
		SeriPushSend(index, 0x3D);
		SeriPushSend(index, 0x00);
		return 0x3D;
	} else if (c == 0x21) {
		SeriPushSend(index, 0x3D);
		SeriPushSend(index, 0x1C);
		return 0x21;
	} else {
		SeriPushSend(index, c);
		return c;
	}
}

/**
 * @Title:
 * @Description: Process the data
 * @Arguments:
 * @Return:
 * @author Berton
 * @date 2009-8-20
 */
void DoSomething(void) {

	switch (RData_Buffer[SERI1][1]) {
	case CHANGETIME:
		ChangeTime();
		break;
	case CHANGEPARAM:
		ChangeParam();
		break;
	case SHELL:
		ShellControl();
		break;
	default:
		break;
	}
}

void ShellControl() {
	shellEnable = TRUE;
	if (RData_Buffer[SERI1][2] == '1') {
		f_chdrive(1); //��SD��
	} else {
		f_chdrive(0); //��DATAFLASH
	}

}

//!t2010 10 10 10 10 10!
void ChangeTime(void) {
	struct tm now;
	int index;
	printf("\r\n���������:  %s", RData_Buffer);
	//������볤���ǶԵģ�������޸Ĳ���!t2010 10 10 10 10 10!
	//if(strlen(RData_Buffer[SERI0])==22){
	index = atoi(RData_Buffer[SERI1] + 2);
	if (index > 2037) {
		printf("\r\n���������������ȷʱ�䣬��: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_year = index;

	index = atoi(RData_Buffer[SERI1] + 7);
	if (index > 12) {
		printf("\r\n���������������ȷʱ�䣬��: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_mon = index - 1;

	index = atoi(RData_Buffer[SERI1] + 10);
	if (index > 31) {
		printf("\r\n���������������ȷʱ�䣬��: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_mday = index;

	index = atoi(RData_Buffer[SERI1] + 13);
	if (index > 23) {
		printf("\r\n���������������ȷʱ�䣬��: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_hour = index;

	index = atoi(RData_Buffer[SERI1] + 16);
	if (index > 59) {
		printf("\r\n���������������ȷʱ�䣬��: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_min = index;

	index = atoi(RData_Buffer[SERI1] + 19);
	if (index > 59) {
		printf("\r\n���������������ȷʱ�䣬��: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_sec = index;
	printf("\r\nʱ���޸ĳɹ�");
	timeFirstSet = 1;
	Time_SetCalendarTime(now); //���� ʱ��
	//}
	//else{
	//printf("\r\n���������������ȷʱ�䣬��: !t2010 10 10 10 10 10!");
	//}	
errorpoint:

}


void ChangeParam(void) {
	;
}

/**
 * @Title:
 * @Description: Get the data from the serial
 * @Arguments:
 * @Return:
 * @author Berton
 * @date 2009-8-20
 */
void ProcessProtocol(uchar index) {
	uchar c;
	if (SeriPopRece(index, &c)) {
		if (bStart[index]) {
			if (c != 0x3D) {
				if (bGet3D[index]) {
					if (c == 0x00)
						RData_Buffer[index][Pos[index]++] = 0x3D;
					else if (c == 0x1C)
						RData_Buffer[index][Pos[index]++] = 0x21;
					bGet3D[index] = FALSE;
				} else
					RData_Buffer[index][Pos[index]++] = c;
			} else
				bGet3D[index] = TRUE;
		}
		if (c == 0x21) {
			bGet3D[index] = FALSE;
			if (bStart[index]) {
				DoSomething();
				Pos[index] = 0x00;
				bStart[index] = FALSE;
			} else {
				Pos[index] = 0x01;
				bStart[index] = TRUE;
				RData_Buffer[index][0] = 0x21;
			}
		}
	}
}
