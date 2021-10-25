#ifndef SERIAL_FILE_H_
#define SERIAL_FILE_H_


#define COMNUM 2

#define SERI1 		0
#define SERI2 		1
#define SERI3 		2

#define COMLENGTH 0xfc

extern uchar logEnable;
extern volatile uchar Serial_Tail[COMNUM];

extern volatile uchar Serial_Count[COMNUM];

extern volatile uchar Serial_Buffer[COMNUM][COMLENGTH];

extern void SeriPushSend(uchar index, uchar c);
extern void PushSend_String(uchar index, uchar *str);
extern void Serial_BufCls(uchar index);
extern unsigned char SeriPopRece(uchar index, uchar *c);

extern void Log_PushSend_String(uchar index, uchar *str);
extern void Log_SeriPushSend(uchar index, uchar c);



#endif
