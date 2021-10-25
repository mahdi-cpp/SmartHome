#include "config.h"

int i_global;
char key_global;
char command_global[MAX_CMD_LEN];

void ShellPutChar(char data)
{

  if (data == '\n')
  {
    Seri0PushSend('\r');
  }
  Seri0PushSend(data);

}



long Help(int argc, char *argv[]);


typedef long(*cmdproc)(int argc, char *argv[]);

typedef struct
{
  char *cmd;
  char *hlp;
  cmdproc proc;
} CMD_STRUC;


CMD_STRUC CMD_INNER[] =
{
  {
    "help", "show help", Help
  }
  ,
  {
    "?", "= help", Help
  }
  ,
  {
    "------", "------", NULL
  }
  ,
  {
    "mkdir", "mkdir  - ����Ŀ¼", mkdirFunc_Fat
  }
  ,
  {
    "write", "write  - �����ļ�", writeFunc_Fat
  }
  ,
  {
    "ls", "ls     - ��ʾĿ¼", lsFunc_Fat
  }
  ,
  {
    "cat", "cat    - ���ļ�", catFunc_Fat
  }
  ,
  {
    "rm", "rm     - ɾ���ļ�", rmFunc_Fat
  }
  ,
  {
    "free", "free   - ��ʾ�ռ�", freeFunc_Fat
  }
  ,
  {
    "cd", "cd - ���ĵ�ǰĿ¼", cdFunc_Fat
  }
  ,
  {
    "show", "show   - ��ʾTargetValֵ", ShowFunc_Fat
  }
  ,
  {
    "pwd", "pwd    - ��ʾ��ǰĿ¼", pwdFunc_Fat
  }
  ,
  {
    "mount", "mount    - sdcard", RegisterFunc_Fat
  }
  ,
  {
    "close", "close    - close shell function", shellClose
  }
  ,
  {
    NULL, NULL, NULL
  }
};




long Help(int argc, char *argv[])
{
  int i;

  for (i = 0; CMD_INNER[i].cmd != NULL; i++)
  {
    if (CMD_INNER[i].hlp != NULL)
    {
      Shell_OutTXT("%s	------	%s\r\n", CMD_INNER[i].cmd, CMD_INNER[i].hlp);
    }
  }

  return 0;
}

/*********************************************************/
void ParseArgs(char *cmdline, int *argc, char **argv)
{
  #define STATE_WHITESPACE	0
  #define STATE_WORD			1

  char *c;
  int i;
  int state = STATE_WHITESPACE;

  *argc = 0;

  if (strlen(cmdline) == 0)
    return ;

  /* convert all tabs into single spaces */
  c = cmdline;
  while (*c != '\0')
  {
    if (*c == '\t')
      *c = ' ';
    c++;
  }

  c = cmdline;
  i = 0;

  /* now find all words on the command line */
  while (*c != '\0')
  {
    if (state == STATE_WHITESPACE)
    {
      if (*c != ' ')
      {
        argv[i] = c; //��argv[i]ָ��c
        i++;
        state = STATE_WORD;
      }
    }
    else
    {
       /* state == STATE_WORD */
      if (*c == ' ')
      {
        *c = '\0';
        state = STATE_WHITESPACE;
      }
    }
    c++;
  }

  *argc = i;
  #undef STATE_WHITESPACE
  #undef STATE_WORD
}

int GetCmdMatche(char *cmdline)
{
  int i;

  for (i = 0; CMD_INNER[i].cmd != NULL; i++)
  {
    if (strncmp(CMD_INNER[i].cmd, cmdline, strlen(CMD_INNER[i].cmd)) == 0)
      return i;
  }

  return  - 1;
}

int ParseCmd(char *cmdline, int cmd_len)
{
  int argc;
  int num_commands;
  char *argv[MAX_ARGS];

  ParseArgs(cmdline, &argc, argv);

  /* only whitespace */
  if (argc == 0)
    return 0;

  num_commands = GetCmdMatche(argv[0]);
  if (num_commands < 0)
    return  - 1;

  argc--;

  if (CMD_INNER[num_commands].proc != NULL)
    CMD_INNER[num_commands].proc(argc, &argv[1]);

  return 0;
}



void ShellProcess(char ch)
{
  int tmp;
  key_global = ch;
  //Backspace���ó����ն˲������֣���������һЩ�ر��ַ�
  if (key_global == BACK_KEY)
  {
    if (i_global)
    {
      Shell_OutChar(key_global);
      Shell_OutChar(' ');
      Shell_OutChar(key_global);
    }
    i_global -= i_global ? 1 : 0;
  }
  else if (key_global == ENTER_KEY)
  {
    command_global[i_global] = 0;
    next_line();
    tmp = ParseCmd(command_global, i_global);
    if (tmp < 0)
      Shell_OutTXT("Bad command\n");
    prompt();

    i_global = 0;
  }
  else
  {
    if (i_global < MAX_CMD_LEN - 1)
    {
      command_global[i_global++] = key_global;
      Shell_OutChar(key_global);
    }
  }
}
