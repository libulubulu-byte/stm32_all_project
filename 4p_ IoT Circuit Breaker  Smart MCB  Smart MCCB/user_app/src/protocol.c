/*
 * Copyright © 2019 copyright ShenZhen AFU Smart Internet Technology Co.,Ltd.
 * All Right Reserved.
 *
 * Version Logs:
 * Date         Author         Notes
 * 2019.5.30    Yang Song      first version.
 */

/* Includes ------------------------------------------------------------------*/
#include "user_comm.h"
#include "fatfs.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_hal_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/

static uint32_t const table[] =
{
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FATFS SDFs;
static FIL  mFile;
static DIR hdir;
static FILINFO fileinf;

static uint8_t proc_cnt = 0, tran_buf[1024];
static char proc_buf[48], path[32];
static uint32_t send_index = 0, send_done = 0, file_crc = 0;

ProcStatusEnum status;

extern UART_HandleTypeDef huart1;
extern CRC_HandleTypeDef hcrc;
extern IWDG_HandleTypeDef hiwdg;
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static BOOL ScanFiles(void);
static void ProcConsole(int argc, char* argv[]);

/* External functions --------------------------------------------------------*/

/**
 * 协议处理初始化
 */
void ProtocolInit(void) {
  CMD_ENT_DEF(proc, ProcConsole);
  Cmd_AddEntrance(CMD_ENT(proc));

  if (f_mount(&SDFs, "0:", 1) != FR_OK) {
    DBG_LOG("fs mount failed.");
  }

  DBG_LOG("protocol init.");
}

unsigned int crc32(unsigned int crc, char* buff, int len) {
  int i;

  crc = ~crc;
  for (i = 0; i < len; i++)
    crc = (crc >> 8) ^ table[(crc ^ buff[i]) & 0xff];
  return ~crc;
}

/**
 * 串口接收到新数据
 *
 * @param newch  新接收到的字节
 */
void ProtocolNewData(char newch) {
  uint16_t cmd = 0;
  uint32_t bdr = 0;

  if (proc_cnt == 0 && newch != 0x26) {
    return;
  }
  if (proc_cnt == 1 && newch != 0x27) {
    return;
  }
  if (proc_cnt < 48) {
    proc_buf[proc_cnt++] = newch;
    if (proc_cnt == 48) {
      cmd = proc_buf[7];
      // 查询文件
      if (cmd == 0x02) {
        status = proc_scan;
      }
      // 是否允许发送文件
      else if (cmd == 0x01) {
        if (proc_buf[11] == 1) {
          send_index = 0;
          file_crc = 0;
          status = proc_trans_begin;
#if 0 // 客户要求固定波特率 2019.7.9
          bdr = proc_buf[12] << 24;
          bdr |= (proc_buf[13] << 16);
          bdr |= (proc_buf[14] << 8);
          bdr |= (proc_buf[15] << 0);
          LL_USART_SetBaudRate(huart1.Instance,
                               48000000, LL_USART_OVERSAMPLING_16, bdr);
#endif
        } else {
          status = proc_cancel;
        }
      }
      // 传输应答
      else if (cmd == 0x03) {
        if (proc_buf[15] == 3) {
          status = proc_done;
        } else if (proc_buf[15] == 2) {
          send_index = (proc_buf[8] << 24);
          send_index |= (proc_buf[9] << 16);
          send_index |= (proc_buf[10] << 8);
          send_index |= (proc_buf[11] << 0);
          status = proc_trans_resend;
        } else if (proc_buf[15] == 1) {
          status = proc_trans;
        } else if (proc_buf[15] == 4) {
          status = proc_resume;
        }
      }
      proc_cnt = 0;
    }
  } else {
    proc_cnt = 0;
  }
}

/**
 * 协议处理轮询
 */
void ProcPolling(void) {
  static UINT rb;
  uint32_t crc;

  switch (status) {
    case proc_idle:
      break;
    case proc_scan:
      if (ScanFiles() && f_open(&mFile, path, FA_READ) == FR_OK) {
        tran_buf[0] = 0x26;
        tran_buf[1] = 0x26;
        tran_buf[2] = 0x00;
        tran_buf[3] = 0x30;
        tran_buf[4] = 0x00;
        tran_buf[5] = 0x01;
        tran_buf[6] = 0x00;
        tran_buf[7] = 0x01;

        tran_buf[8] = (mFile.fsize) >> 24;
        tran_buf[9] = (mFile.fsize) >> 16;
        tran_buf[10] = (mFile.fsize) >> 8;
        tran_buf[11] = (mFile.fsize) >> 0;

        strcpy((char*)&tran_buf[16], path);
        status = proc_scan_wait_rsp;
      } else {
        tran_buf[0] = 0x26;
        tran_buf[1] = 0x26;
        tran_buf[2] = 0x00;
        tran_buf[3] = 0x30;
        tran_buf[4] = 0x00;
        tran_buf[5] = 0x01;
        tran_buf[6] = 0x00;
        tran_buf[7] = 0x01;

        memset(&tran_buf[8], 0, 40);
        status = proc_idle;
      }
      HAL_UART_Transmit_DMA(&huart1, tran_buf, 48);
      f_close(&mFile);
      break;
    case proc_scan_wait_rsp:
      break;
    case proc_trans_begin:
      if (f_open(&mFile, path, FA_READ) == FR_OK) {
        status = proc_trans;
        HAL_Delay(100);
      } else {
        status = proc_cancel;
      }

      memset(&tran_buf[4], 0, 1016);
      f_read(&mFile, &tran_buf[4], 1016, &rb);
      crc = crc32(0, (char*)&tran_buf[4], 1016);

      tran_buf[1020] = crc >> 24;
      tran_buf[1021] = crc >> 16;
      tran_buf[1022] = crc >> 8;
      tran_buf[1023] = crc >> 0;
      break;
    case proc_trans_resend:
      f_lseek(&mFile, (send_index - 1) * 1016);

      memset(&tran_buf[4], 0, 1016);
      f_read(&mFile, &tran_buf[4], 1016, &rb);

      crc = crc32(0, (char*)&tran_buf[4], 1016);
      tran_buf[1020] = crc >> 24;
      tran_buf[1021] = crc >> 16;
      tran_buf[1022] = crc >> 8;
      tran_buf[1023] = crc >> 0;

      tran_buf[0] = send_index >> 24;
      tran_buf[1] = send_index >> 16;
      tran_buf[2] = send_index >> 8;
      tran_buf[3] = send_index >> 0;

      send_done = 0;
      HAL_UART_Transmit_DMA(&huart1, tran_buf, 1024);

      status = proc_trans_reload;
      break;
    case proc_trans:
      send_index++;

      tran_buf[0] = send_index >> 24;
      tran_buf[1] = send_index >> 16;
      tran_buf[2] = send_index >> 8;
      tran_buf[3] = send_index >> 0;

      send_done = 0;
      HAL_UART_Transmit_DMA(&huart1, tran_buf, 1024);

      // 文件读出成功且非重传时计算文件总的CRC值
      if (rb > 0) {
        file_crc = crc32(file_crc, (char*)&tran_buf[4], rb);
      }

      status = proc_trans_reload;
      break;
    case proc_trans_reload:
      while (send_done == 0) {
        continue;
      }
      memset(&tran_buf[4], 0, 1016);
      f_read(&mFile, &tran_buf[4], 1016, &rb);

      crc = crc32(0, (char*)&tran_buf[4], 1016);
      tran_buf[1020] = crc >> 24;
      tran_buf[1021] = crc >> 16;
      tran_buf[1022] = crc >> 8;
      tran_buf[1023] = crc >> 0;
      status = proc_trans_wait_ack;
      break;
    case proc_trans_wait_ack:
      break;
    case proc_done:
      tran_buf[0] = 0x26;
      tran_buf[1] = 0x26;
      tran_buf[2] = 0x00;
      tran_buf[3] = 0x30;
      tran_buf[4] = 0x00;
      tran_buf[5] = 0x01;
      tran_buf[6] = 0x00;
      tran_buf[7] = 0x04;

      tran_buf[8] = file_crc >> 24;
      tran_buf[9] = file_crc >> 16;
      tran_buf[10] = file_crc >> 8;
      tran_buf[11] = file_crc >> 0;

      memset(&tran_buf[12], 0, 36);
      HAL_UART_Transmit_DMA(&huart1, tran_buf, 48);

      status = proc_trans_wait_ack;
      break;
    case proc_resume:
    case proc_cancel:
      HAL_Delay(100);
      LL_USART_SetBaudRate(huart1.Instance,
                           48000000, LL_USART_OVERSAMPLING_16, 921600);
      f_close(&mFile);
      status = proc_idle;
      break;
    default:
      break;
  }
}

/**
 * UART发送完成
 */
void UartSendDone(void) {
  send_done = 1;
}


/* Private functions ---------------------------------------------------------*/

/**
 * 搜索文件
 *
 * @return 搜索成功返回TRUE
 */
static BOOL ScanFiles(void) {
  BOOL ret = FALSE;
  char buf[32];

  fileinf.lfname = buf;
  fileinf.lfsize = 31;

  if (CardInfo.Capacity == 0) {
    return FALSE;
  }
  if (f_opendir(&hdir, "/") == FR_OK) {
    while (f_readdir(&hdir, &fileinf) == FR_OK) {
      if (!fileinf.fname[0]) {
        break;
      }
      if (STR_NEQUAL(fileinf.lfname, "Ellipal_ec02_") && strstr(fileinf.lfname, ".zip")) {
        if (strlen(fileinf.lfname) < 32
            && strcmp(fileinf.lfname, path) >= 0) {
          strcpy(path, fileinf.lfname);
          ret = TRUE;
        }
      }
    }
    f_closedir(&hdir);
  }
  return ret;
}

/**
 * 协议调试命令
 *
 */
static void ProcConsole(int argc, char* argv[]) {
  FRESULT ret;

  argv++;
  argc--;
  if (ARGV_EQUAL("fslist")) {
    ret = f_opendir(&hdir, argv[1]);
    if (ret == FR_OK) {
      while (1) {
        ret = f_readdir(&hdir, &fileinf);
        if (ret == FR_OK) {
          if (!fileinf.fname[0]) {
            DBG_LOG("list done.");
            break;
          }
          DBG_LOG("%s/%s", argv[1], fileinf.fname);
        } else {
          DBG_LOG("read dir fail:%d", ret);
          break;
        }
      }
      f_closedir(&hdir);
    } else {
      DBG_LOG("open dir fail:%d", ret);
    }
  } else if (ARGV_EQUAL("f_open")) {
    ret = f_open(&mFile, argv[1], uatoix(argv[2]));
    DBG_LOG("f_open ret:%d.", ret);
  } else if (ARGV_EQUAL("f_close")) {
    ret = f_close(&mFile);
    DBG_LOG("f_close ret:%d.", ret);
  } else if (ARGV_EQUAL("crc")) {
    DBG_LOG("crc test:%#x.", crc32(uatoix(argv[1]), argv[2], strlen(argv[2])));
  }
}

