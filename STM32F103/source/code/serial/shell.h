/* 这份源代码文件已被未注册的SourceFormatX格式化过 */
/* 如果您想不再添加此类信息，请您注册这个共享软件  */
/* 更多相关信息请访问网站: http://cn.textrush.com  */

/****************************************************************
 **       Copyright (c) 2009-2010
 **       All Right Reserved.
 **	
 **       $Index     http://blog.ednchina.com/walnutcy/
 **       $Author    walnutcy
 **       $Email     walnutcy@163.com
 **       $Version   v1.0
 **       $          使用请保持文件的完整性，包括这个注释
 **                  代码可免费商业使用，但作者不对软件的bug负责
 *****************************************************************/

#ifndef _SHELL_H
  #define _SHELL_H

  #ifdef _SHELL_H_GLOBAL_
    #define SHELL_EXT
  #else
    #define SHELL_EXT	extern
  #endif

  void ShellPutChar(char data);
  void ShellProcess(char ch);


  //===========================================================
  #define Shell_OutTXT    printf
  #define Shell_OutChar   ShellPutChar
  #define next_line()	    do {Shell_OutChar('\r');Shell_OutChar('\n');}while(0)
  #define prompt()   	    Shell_OutTXT("\\>")

  #define MAX_CMD_LEN	64
  #define MAX_ARGS	MAX_CMD_LEN/4

  #define ENTER_KEY	0x0d
  #define BACK_KEY	0x08
  #define ESC_KEY		0x1b

  extern int i_global;
  extern char key_global;
  extern char command_global[MAX_CMD_LEN];


#endif
