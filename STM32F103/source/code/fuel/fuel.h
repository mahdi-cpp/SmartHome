#ifndef _FUEL_H
  #define _FUEL_H

  //#include <ctype.h>
  //#include <errno.h>
  //#include <math.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ff.h"
  //#include <time.h>
  //#include <wchar.h>
  //#include <wctype.h>



  /* Default configuration filename. */
  #define DEFAULT_CONFIG_FILE "system.txt"
  #define DEFAULT_SD_CONFIG_FILE "1:/machine/machine1/sd.txt"


  #define BUF_SIZ 150
  #define LINE 19
  #define NUM 1
  #define PWD_LEN 6
  #define TRUE 1
  #define FALSE 0
  #define OK  0
  #define ERR  -1
  #define VERSION "1.0.1"

  /*SD卡配置文件action，决定此次配置的动作*/
  enum action
  {
    W_CONFIG = 0,  /*更新配置文件*/
    R_CONFIG,  /*读取配置文件*/
    COPY_TO_SD,  /*拷贝数据到SD卡*/
  };

  /*油箱的形状*/
  enum shape_t
  {
    CYLINDER = 0, CUBE,
  };

  typedef struct cuber_s
  {
    unsigned short height;
    unsigned short length;
    unsigned short width;
  } cube_t;

  typedef struct cylinder_s
  {
    unsigned short len;
    unsigned short radius;
  } cylinder_t;

  union parameter
  {
    cube_t cube;
    cylinder_t cylinder;
  };


  typedef struct config_s
  {
    char name[LINE + 1];
    char pwd[PWD_LEN];
    unsigned short shape;
    unsigned int capacity;
    union parameter u;
  }
  config_t;

  extern config_t g_data;
  extern int load_config(void);
  extern int load_config_from_sd(int *action);
  extern void write_file_tmp(void);

#endif
