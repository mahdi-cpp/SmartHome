#include "config.h"
/////////////////////////////////////////////////////////////
//外设协议基本格式:
//包头 +包类型 + 数据内容 + 包尾
//!n=machine1!
//!t=20100312202020
//整个协议总长度为256,包头,包尾,设备类型,包类型,包长度均为一
//字节字段,所以此协议最多能传输内容长度为249个字节的数据包.
//包头与包尾均为0x21,即"!"号
//较验为两个字节,负责较验从设备类型到较验字节前的内容,较验方
//式为检验和
//设备类型为二进制

//转义处理:
//从设备类型开始,直到数据内容结束,都有可能出现与协议头/尾相同的字符
//当出现此字符时,需要进行转义处理,方法如下:
//1.当数据包长度或数据内容中出现0x21时,则先在些字符之前插入一个将其
//  与0x3D进行异或(0x1C)的值,即当出现0x21时,实际传输字符为两个字节:
//   0x3D 0x1C
//2.当数据包长度或数据内容中出现0x3D时,则在0x3D之后插入一个0x00,即实
//  际传输字符为两个字节:0x3D 0x00
//3.数据接收者当收到0x3D时,即将此字节扔掉,且将其后一个字节与0x3D进行
//  异或.


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
 发送一个可能会存在转义机会的字符串,返回值为转义后应该增
 加的较验值
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
		f_chdrive(1); //打开SD卡
	} else {
		f_chdrive(0); //打开DATAFLASH
	}

}

//!t2010 10 10 10 10 10!
void ChangeTime(void) {
	struct tm now;
	int index;
	printf("\r\n你输入的是:  %s", RData_Buffer);
	//如果输入长度是对的，则进行修改操作!t2010 10 10 10 10 10!
	//if(strlen(RData_Buffer[SERI0])==22){
	index = atoi(RData_Buffer[SERI1] + 2);
	if (index > 2037) {
		printf("\r\n输入错误！请输入正确时间，如: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_year = index;

	index = atoi(RData_Buffer[SERI1] + 7);
	if (index > 12) {
		printf("\r\n输入错误！请输入正确时间，如: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_mon = index - 1;

	index = atoi(RData_Buffer[SERI1] + 10);
	if (index > 31) {
		printf("\r\n输入错误！请输入正确时间，如: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_mday = index;

	index = atoi(RData_Buffer[SERI1] + 13);
	if (index > 23) {
		printf("\r\n输入错误！请输入正确时间，如: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_hour = index;

	index = atoi(RData_Buffer[SERI1] + 16);
	if (index > 59) {
		printf("\r\n输入错误！请输入正确时间，如: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_min = index;

	index = atoi(RData_Buffer[SERI1] + 19);
	if (index > 59) {
		printf("\r\n输入错误！请输入正确时间，如: !t2010 10 10 10 10 10!");
		goto errorpoint;
	}
	now.tm_sec = index;
	printf("\r\n时间修改成功");
	timeFirstSet = 1;
	Time_SetCalendarTime(now); //设置 时间
	//}
	//else{
	//printf("\r\n输入错误！请输入正确时间，如: !t2010 10 10 10 10 10!");
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
