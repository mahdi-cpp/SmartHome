#ifndef SYSDEBUG_FILE_H_
#define SYSDEBUG_FILE_H_

#define SYS_DEBUG_STACK_SIZE 60 //����¼10�������ĵ���.


#define SYS_OUTPUT_DEBUG  0x00

struct SYS_DEBUG
{
  uint  Index;
  uchar Name[10];
};

typedef struct SYS_DEBUG DEBUG_INFO;

extern void Debug_In(uchar * pInfo,uchar len);

//�Ƿ������Զ���ĵ�����Ϣ.���ֳ���������һ��ʱ��֮��ͻ�����,������ָ�������
//��,���������еĺ���ǰ���ּ�һ���������.

#if (SYS_OUTPUT_DEBUG)
  #define OUTPUT_DEBUG(a,b)   Debug_In(a,b)
#else
  #define OUTPUT_DEBUG(a,b)
#endif

extern DEBUG_INFO Debug_Info[SYS_DEBUG_STACK_SIZE];

#endif


