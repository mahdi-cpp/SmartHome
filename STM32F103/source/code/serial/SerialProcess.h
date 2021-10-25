#ifndef _H_SERIAL0PROCESS
  #define _H_SERIAL0PROCESS



  extern uchar bStart[];
  extern uchar bGet3D[];
  extern uchar Pos[];
  extern uchar RData_Buffer[COMNUM][COMLENGTH];


  extern uchar SendTransChar(uchar index, uchar c);
  extern void SendData(uchar index, uchar *str, uchar ilen);
  extern void SendTransparentData(uchar index);
  extern void DoSomething(void);
  extern void ProcessProtocol(uchar index);

#endif
