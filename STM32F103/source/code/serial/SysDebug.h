#ifndef SYSDEBUG_FILE_H_
#define SYSDEBUG_FILE_H_

#define SYS_DEBUG_STACK_SIZE 60 //最多记录10个函数的调用.


#define SYS_OUTPUT_DEBUG  0x00

struct SYS_DEBUG
{
  uint  Index;
  uchar Name[10];
};

typedef struct SYS_DEBUG DEBUG_INFO;

extern void Debug_In(uchar * pInfo,uchar len);

//是否输入自定义的调试信息.因发现程序在运行一段时间之后就会死机,可能与指针操作有
//关,所以在所有的函数前部分加一个调试输出.

#if (SYS_OUTPUT_DEBUG)
  #define OUTPUT_DEBUG(a,b)   Debug_In(a,b)
#else
  #define OUTPUT_DEBUG(a,b)
#endif

extern DEBUG_INFO Debug_Info[SYS_DEBUG_STACK_SIZE];

#endif


