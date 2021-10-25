/*	 
 *	commands.c
 *	the genie shell command part for ucosII
 *	under skyeye
 *
 *	Bugs report:	 ganganwen@163.com
 *	Last modified:	 2005-04-25 
 *  changed by: wenjia
 */

#include	"config.h"



FIL myfile_fat;
char mybuffer_fat[0x200];

char CurDir_fat[400]="";		//show current dir
//char CurDir_fat[400]="0:";		//show current dir   先用于DataFlash中查看，以后补充
char TargetVal_fat[400] = "";  
//TargetVal数组第一功能功能：用于组合当前目录与输入数据如 
//CurDir_fat = "\mydir"   cat 1.txt 时组合成cat \mydir\1.txt
//第二功能：保留CurDir目录，因为如果cd参数第一个字符为\时就从&CurDir_fat[0] 
//开始复制，但若输入目录出错则将原来的CurDir目录还原。

UCHAR DirExist_fat(const char *DirName);
void RewindRoot_fat(void);


void CreateTargetVal_fat(const char *arg);

void shellClose(void)
{
	shellEnable=FALSE;
	f_chdrive(0); //打开DATAFLASH
	_log_Fat("close shell function!!");
}
//判断目录名是否存在
//1:目录存在  0:目录不存在
UCHAR DirExist_fat(const char *DirName)
{
    DIR dirp;
    if (FR_OK == f_opendir(&dirp,DirName))
    {
        return 1;
    } 
    else 
    {
		_error_Fat("unable to open dir: %s\r\n",DirName);
        return 0;
    }
} 
//--------------------------------------------
void _write_file_Fat(const char *name, const char *txt) 
{
  UINT x;
  
  /* create file */
  if (FR_OK == f_open(&myfile_fat,name,FA_CREATE_NEW|FA_WRITE)) 
  {
    /* write to file */
    if (FR_OK != f_write(&myfile_fat,txt,(UINT)strlen(txt),&x)) 
    {
      _error_Fat("Not all bytes written , the written data len is %d bytes.\r\n",x);
    }
    /* close file */
    f_close(&myfile_fat);
	_log_Fat("\r\n");
  }
  else 
  {
    _error_Fat("Unable to create file %s\r\n",name);
  }
}


void _dump_file_Fat(const char *name) 
{
  UINT x;

  /* open file */
  if (FR_OK == f_open(&myfile_fat,name,FA_READ)) 
  {
    /* read until EOF has been reached */
    do 
    {     
      if(FR_OK == f_read(&myfile_fat,mybuffer_fat,sizeof(mybuffer_fat), &x)) 
      {
        mybuffer_fat[x]=0;
        _log_Fat(mybuffer_fat);
      }
    } while (x);
    /* close file */
    f_close(&myfile_fat);
	_log_Fat("\r\n");
  }
  else 
  {
    _error_Fat("Unable to open file %s.\r\n",name);
  }
}



void _show_directory_Fat(const char* path)
{
    FILINFO finfo;
    DIR dirs;
    int i;

    _log_Fat("ls %s\r\n",path);
    if (f_opendir(&dirs, path) == FR_OK) 
    {
        i = strlen(path);
        while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0]) 
        {
//            _log_Fat(" %s %s Attributes: %s%s%s%s ", &finfo.fname[0],
            _log_Fat(" %s %s \tAttr: %s%s%s%s ", &finfo.fname[0],
                          (finfo.fattrib & AM_DIR) ? "\t(Dir)" : "   ",
                          (finfo.fattrib & AM_ARC) ? "A" : "-",
                          (finfo.fattrib & AM_RDO) ? "R" : "-",
                          (finfo.fattrib & AM_HID)    ? "H" : "-",
                          (finfo.fattrib & AM_SYS)    ? "S" : "-"
                          );
            if(finfo.fattrib & AM_DIR)
            {
               // _log_Fat("Size: %lu\r\n",0);
            }
            else
            {
                if(finfo.fsize<1024) // < 1KB
                    _log_Fat("  %lu bytes",finfo.fsize);
                else if(finfo.fsize<1024*1024)  // < 1Mb
                    _log_Fat("  %.2f KB",(finfo.fsize/1024.0));
                else //
                    _log_Fat("  %.2f MB",(finfo.fsize/1024.0/1024.0));
            } 
            _log_Fat("\r\n");
                        
#if 0   // 递归调用，遍历文件夹                       
            if (finfo.fattrib & AM_DIR) 
            {
                sprintf(&path[i], "/%s", &finfo.fname[0]);
                scan_files(path);
                path[i] = 0;                
            } 
            else
            {
                printf("%s/%s\r\n", path, &finfo.fname[0]);
            }
#endif            
        }
    }
}



void _show_free_Fat() 
{
    FATFS *fs;
    DWORD clust;
    
    // Get free clusters
    if (f_getfree("1:", &clust, &fs)== FR_OK) 
    {
    // Get free space
        _log_Fat("\r\n%lu KB total disk space.\r\n"
                 "%lu KB available on the disk.\r\n",
                 (DWORD)(fs->max_clust - 2) * fs->csize / 2,
                 clust * fs->csize / 2);        
    }
    else 
    {
        _error_Fat("\r\nInvalid drive specified\r\n");
    }
}


long ShowFunc_Fat(int argc,char **argv)
{

    _log_Fat("\r\n=========  TargetVal_fat =  ");
	_log_Fat(TargetVal_fat);
	_log_Fat("========\r\n");
    return 0;
}



long pwdFunc_Fat(int argc,char **argv)
{
	if (argc != 0)
	{
		_error_Fat("PWD is single arg\r\n");
		return 1;
	}
	_log_Fat(CurDir_fat);
	_log_Fat("\r\n");
	return 0;
}
//功能：设置成根目录
void RewindRoot_fat(void)
{
    CurDir_fat[0] = '\0';
    CurDir_fat[1] = '\0';
}


//更改当前目录命令
long cdFunc_Fat(int argc,char **argv)
{
	UCHAR DirCurPos = 0;
    UCHAR CmdLen    = 0;
	UCHAR TmpVal    = 0;
    UCHAR IsRoot    = 0;
    UCHAR IsBackup  = 0;
    UCHAR BackupLen = 0;
	DirCurPos = strlen(CurDir_fat);   //取得当前目录的字符长度
    if (argc < 1)
	{
		_error_Fat("please input command as:cd directoryname ");
        _error_Fat("\r\n cd \\ change to root\r\n cd .. \r\n cd \\MyDir\\Dir1\r\n");
		_error_Fat("\r\n");
		return 1;
	}
	if (strcmp(argv[0], "..") == 0)  //回到上一级
	{
		if (1 == DirCurPos)         //设定长度为1的为根目录
		{
			_error_Fat("This is root dir\r\n");
			return 1;
		} 
		else 
		{
			TmpVal = DirCurPos;   //取得当前目录的长度并从最后开始查找
		   //从最后开始查找将该级目录清除 
           //例：\my\dir1  从1开始找到r最后找到\dir的\清除
            while (TmpVal--) 
		   {
			   if (CurDir_fat[TmpVal] == '/')   //是否找到上一级目录
			   {
				   if (TmpVal == 0)         //根目录
				   {
					   RewindRoot_fat();
				   } else {
				      CurDir_fat[TmpVal] = '\0';   //将最后的\换成结束符
				   }
                   _log_Fat("\r\n CurDir_fat Len = %d\r\n", strlen(CurDir_fat));
				   break;    //退出循环
               } else {
                   CurDir_fat[TmpVal] = '\0';  //清除字符
               }
		   }
		}
    } 
    else if (strcmp(argv[0], "/") == 0)	
    {   //
		RewindRoot_fat();   //回到根目录
        _log_Fat("回到根目录  argc = %d\r\n",argc);
	}
	else 
	{
	    if (strcmp(CurDir_fat, "/") == 0)   //比较方法用于比较是否是当前目录
		{
            IsRoot = 1;
            if ('/' == argv[0][0])   //如果当输入参数是带\开始的，如cd \dir\dir1
            {
                DirCurPos = 0;   
            }
            _log_Fat("\r\ncd start at root! DirCurPos = %d\r\n", DirCurPos);
		} 
		else 
		{
            if ('/' == argv[0][0])   //如果当输入参数是带\开始的，如cd \dir\dir1
            {
                //因为当前目录不是根目录，所以为了防止出错保留原目录
                IsBackup  = 1;
                BackupLen = DirCurPos + 1;
                strncpy(&TargetVal_fat[0], &CurDir_fat[0], BackupLen);
                DirCurPos = 0;     //从头开始复制命令
            } 
            else 
            {
		        CurDir_fat[DirCurPos] = '/';
			    DirCurPos++;
            }
		}
        CmdLen    = strlen(argv[0]);
        strncpy(&CurDir_fat[DirCurPos], argv[0], CmdLen + 1);  //确保字符是以'\0'结束请看FS_STRNCPY源码
//        _log_Fat("\r\nafter strncpy() CurDir_fat= %s  DirCurPos = %d CmdLen = %d\r\n", CurDir_fat, DirCurPos, CmdLen);
        if (DirExist_fat(CurDir_fat) == 0)  //目录不存在
        {
			_log_Fat(argv[0]);
            _log_Fat("  Dir is not exist!\r\n" );
            if (1 == IsRoot)   //是否是根目录
            {
                RewindRoot_fat();               
            } 
            else 
            {
                if (1 == IsBackup)
                {
                    strncpy(&CurDir_fat[0], &TargetVal_fat[0], BackupLen);
                } 
                else 
                {
                    CurDir_fat[DirCurPos - 1] = '\0';   //将加入的目录清除
                }
            }
        } 
        else 
        {
//            _log_Fat("Open %s Exist!\r\n", CurDir_fat);  //目录存在
        }
	}
    _log_Fat("\r\nCurDir = %s \r\n", CurDir_fat);
	return 0;
}

long lsFunc_Fat(int argc,char **argv)
{			

	if(argc == 0)
    {
        _show_directory_Fat(CurDir_fat);
    } 
    else 
    {
        _show_directory_Fat(argv[0]);
    }
	return 0;
}
		

long rmFunc_Fat(int argc,char **argv)
{

	if(argc != 1)
	{
		_error_Fat("please input command as:rm filename");
		_error_Fat("\r\n\r\n");
		return 1;
	}
    CreateTargetVal_fat(argv[0]);
    if(FR_OK == f_unlink(TargetVal_fat))
	{
		_log_Fat("remove ");
		_log_Fat(argv[0]);
		_log_Fat(" successful\r\n\r\n");
	}
	else
	{
		_error_Fat("remove failure!\r\n\r\n");
	}
	return 0;
}

long writeFunc_Fat(int argc,char **argv)
{
	if(argc != 2)
	{
		_error_Fat("please input command as:create filename data");
		_error_Fat("\r\n\r\n");
		return 1;
	}
    CreateTargetVal_fat(argv[0]);

    _write_file_Fat(TargetVal_fat, argv[1]);

	return 0;
}


//用于将arg参数构造成CurDir + arg = TargetParam
void CreateTargetVal_fat(const char *arg)
{
    UCHAR CurDirLen = 0;

    CurDirLen = strlen(CurDir_fat);
    if ('/' == arg[0])
    {
        strncpy(&TargetVal_fat[0], arg, strlen(arg) + 1);
    } 
    else 
    {
        if (1 == CurDirLen)  //判断是否当前目录
        {
            strncpy(&TargetVal_fat[CurDirLen], arg, strlen(arg) + 1);
        } 
        else 
        {
            strncpy(TargetVal_fat, CurDir_fat, CurDirLen + 1);
            TargetVal_fat[CurDirLen++] = '/';
            _log_Fat("\r\n %s\r\n", TargetVal_fat);
            strncpy(&TargetVal_fat[CurDirLen], arg, strlen(arg) + 1);
        }
    }
    _log_Fat("\r\nTargetVal = %s\r\n", TargetVal_fat);
}

long catFunc_Fat(int argc,char **argv)
{
    if(argc != 1)
	{
		_error_Fat("please input command as:cat filename");
		_error_Fat("\r\n\r\n");
		return 1;
	}
    CreateTargetVal_fat(argv[0]);
    _dump_file_Fat(TargetVal_fat);
	return 0;
}



long mkdirFunc_Fat(int argc,char **argv)
{
	if(argc != 1)
	{
		_error_Fat("please input command as:mkdir filename");
		_error_Fat("\r\n\r\n");
		return 1;
	}
    CreateTargetVal_fat(argv[0]);

    if(FR_OK == f_mkdir(TargetVal_fat))
	{
		_log_Fat("make directory ");
		_log_Fat(argv[0]);
		_log_Fat(" successful\r\n\r\n");
	}
	else
	{
		_error_Fat("mkdir failure! please check disk full or unformated\r\n");
	}
	return 0;
}



long freeFunc_Fat(int argc,char **argv)
{
	_show_free_Fat();
	_log_Fat("\r\n");
	return 0;
}

FATFS mySDHandle;

long RegisterFunc_Fat(int argc,char **argv)
{
	f_mount(0,&mySDHandle);
	
	_log_Fat("f_mount(0,&mySDHandle)\r\n");
	return 0;
}




