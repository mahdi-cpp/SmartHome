/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
#include "config.h"
#include "diskio.h"
#include "SD_driver.h"
//#include "dataflash.h"
DSTATUS disk_initialize(BYTE drv /* Physical drive nmuber (0..) */
)
{
  switch (drv)
  {
    case 0:
      //
	  //SPI_FLASH_Init();
      // translate the reslut code here

      return 0;

    case 1:
      SPI_Configuration();
      bw = SD_Init();
      // translate the reslut code here

      return 0;

    case 2:
      return STA_NOINIT;
  }
  return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status(BYTE drv /* Physical drive nmuber (0..) */
)
{
  switch (drv)
  {
    case 0:

      // translate the reslut code here

      return 0;

    case 1:
      // translate the reslut code here

      return 0;

    case 2:
      // translate the reslut code here

      return 0;

    default:
      break;
  }
  return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read(BYTE drv,  /* Physical drive nmuber (0..) */
BYTE *buff,  /* Data buffer to store read data */
DWORD sector,  /* Sector address (LBA) */
BYTE count /* Number of sectors to read (1..255) */
)
{

  BYTE *pBuf = buff;
  u8 res = 0;
  if (!count)
  {
    return RES_PARERR; //仅支持单磁盘操作，count不能等于0，否则返回参数错误
  }

  switch (drv)
  {
    case 0:
      while (count)
      {
        //DataFlash_ReadSector(sector, pBuf);
        //sector++;
        //pBuf += 512; // SD卡扇区大小为512字节
        //count--;
      }
      return RES_OK;
    case 1:
      if (count == 1)
      //1个sector的读操作
      {
        res = SD_ReadSingleBlock(sector, buff);
      }
      else
      //多个sector的读操作
      {
        res = SD_ReadMultiBlock(sector, buff, count);
      }
      //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
      if (res == 0x00)
      {
        return RES_OK;
      }
      else
      {
        return RES_ERROR;
      }

    case 2:
      break;
    default:
      break;
  }
  return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
  DRESULT disk_write(BYTE drv,  /* Physical drive nmuber (0..) */
  const BYTE *buff,  /* Data to be written */
  DWORD sector,  /* Sector address (LBA) */
  BYTE count /* Number of sectors to write (1..255) */
  )
  {
    BYTE *pBuf = (BYTE*)buff;

    u8 res;
    if (!count)
    {
      return RES_PARERR; //仅支持单磁盘操作，count不能等于0，否则返回参数错误
    }

    switch (drv)
    {
      case 0:
        while (count)
        {
          //DataFlash_WriteSector(sector, buff);
          sector++;
          pBuf += 512; // SD卡扇区大小为512字节
          count--;
        }
        return RES_OK;
      case 1:
        if (count == 1)
        //1个sector的读操作
        {
          res = SD_WriteSingleBlock(sector, buff);
        }
        else
        //多个sector的读操作
        {
          res = SD_WriteMultiBlock(sector, buff, count);
        }
        //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
        if (res == 0x00)
        {
          return RES_OK;
        }
        else
        {
          return RES_ERROR;
        }
      case 2:
        break;
      default:
        break;
    }
    return RES_ERROR;

  }
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl(BYTE drv,  /* Physical drive nmuber (0..) */
BYTE ctrl,  /* Control code */
void *buff /* Buffer to send/receive control data */
)
{
  if (drv)
  {
    return RES_PARERR; //仅支持单磁盘操作，否则返回参数错误
  }
  switch (ctrl)
  {
    case CTRL_SYNC:
      return 0;
    case GET_SECTOR_COUNT:
      *((DWORD*)buff) = 0x1000; // 2Mbyte
      return RES_OK;

    case GET_BLOCK_SIZE:
      *((DWORD*)buff) = 512;
      return RES_OK;
    case CTRL_POWER:
      break;
    case CTRL_LOCK:
      break;
    case CTRL_EJECT:
      break;
      /* MMC/SDC command */
    case MMC_GET_TYPE:
      break;
    case MMC_GET_CSD:
      break;
    case MMC_GET_CID:
      break;
    case MMC_GET_OCR:
      break;
    case MMC_GET_SDSTAT:
      break;
  }
  return RES_PARERR;

}


/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
DWORD get_fattime(void)
{

  return 0;
}

//得到文件Calendar格式的建立日期,是DWORD get_fattime (void) 逆变换
