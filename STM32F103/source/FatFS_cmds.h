/* ���Դ�����ļ��ѱ�δע���SourceFormatX��ʽ���� */
/* ������벻�����Ӵ�����Ϣ������ע�������������  */
/* ���������Ϣ�������վ: http://cn.textrush.com  */

/*	
 *	commands.h
 *	the genie shell commands part for ucosII
 *	under skyeye
 *
 *	Bugs report:	 Yang Ye  ( yangye@163.net )
 *	Last modified:	 2003-05-1
 *  changed by:		wenjia
bugs report:	ganganwen@163.com
 */
#ifndef FATFS_CMDS_H_
  #define FATFS_CMDS_H_


  #include <stdio.h>
  #include <string.h>
  #include <ctype.h>
  #include <stdlib.h>




  #define _error_Fat      printf
  #define _log_Fat        printf


  long lsFunc_Fat(int argc, char **argv);
  long rmFunc_Fat(int argc, char **argv);
  long catFunc_Fat(int argc, char **argv);

  long writeFunc_Fat(int argc, char **argv);
  long mkdirFunc_Fat(int argc, char **argv);
  long freeFunc_Fat(int argc, char **argv);

  long cdFunc_Fat(int argc, char **argv);
  long pwdFunc_Fat(int argc, char **argv);
  long ShowFunc_Fat(int argc, char **argv);

  long RegisterFunc_Fat(int argc, char **argv);



  void _show_directory_Fat(const char *path);

  void _write_file_Fat(const char *name, const char *txt);
  void _dump_file_Fat(const char *name);
  void _show_free_Fat(void);

  //�ر�shell����
  void shellClose(void);



#endif