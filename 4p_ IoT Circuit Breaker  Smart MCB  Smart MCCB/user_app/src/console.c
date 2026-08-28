/**
 * *********************************************************************
 *             Copyright (c) 2016 temp. All Rights Reserved.
 * @file conslole.c
 * @version V1.1
 * @date 2016.4.1
 * @brief 控制台函数文件.
 *
 * *********************************************************************
 * @note v1.1 CMD 接收缓存改为FIFO
 *
 * *********************************************************************
 * @author 宋阳
 */



/* Includes ------------------------------------------------------------------*/
#include "user_comm.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static BOOL         CMD_EchoEnable;
static uint8_t      CMD_RecBuffer[CMD_BUF_SIZE];
static uint8_t      CMD_SendBuffer[CMD_BUF_SIZE];
static uint16_t     CMD_RecBufferIndex = 0, CMD_SendIndex = 0, CMD_SendDoneIndex = 0;
static uint8_t      g_ucDbgLevel = LOG_LEVEL_DEF;
extern UART_HandleTypeDef huart1;
extern __IO ITStatus UartReady;

/* Private function prototypes -----------------------------------------------*/
static void System_Console(int argc, char* argv[]);

/* Exported functions --------------------------------------------------------*/
/**
 * 控制台初始化
 */
void CMD_Init(void) {
  CMD_SetEchoEnable((BOOL)CMD_ECHO_DEF);

  CMD_ENT_DEF(system, System_Console);
  Cmd_AddEntrance(CMD_ENT(system));

  DBG_LOG("Console init.");
}

/**
 * 控制台收到新的byte
 */
void CMD_NewByte(uint8_t ch) {
  if (CMD_RecBufferIndex < CMD_BUF_SIZE - 1) {
    CMD_RecBuffer[CMD_RecBufferIndex++] = ch;
  } else {
    /*溢出归零*/
    CMD_RecBufferIndex = 0;
  }
}

/**
 * 控制台发送数据
 * @param dat  待发送的数据指针
 * @param len  数据的长度
 */
void CMD_SendData(uint8_t* dat, uint16_t len) {

  if (len < CMD_BUF_SIZE - CMD_SendIndex) {
    memcpy(&CMD_SendBuffer[CMD_SendIndex], dat, len);
    CMD_SendIndex += len;
    if (CMD_SendDoneIndex == 0) {
      CMD_SendDoneIndex = CMD_SendIndex;
      HAL_UART_Transmit_DMA(&huart1, CMD_SendBuffer, CMD_SendIndex);
    }
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
  HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);
  if (CMD_SendDoneIndex == CMD_SendIndex) {
    CMD_SendIndex = 0;
    CMD_SendDoneIndex = 0;
    UartReady = SET;
  } else {
   // HAL_UART_Transmit_DMA(&huart1, &CMD_SendBuffer[CMD_SendDoneIndex], CMD_SendIndex - CMD_SendDoneIndex);
    CMD_SendDoneIndex = CMD_SendIndex;
  }
}

/**
  * 控制台打印可变参数字符串.
  * @param  fomat: 参数列表.
  * @param  ...:可变参数
  */
void CMD_Printf(char* format, ...) {
  uint16_t sent = 0;
  char pBuf[CMD_PRINTF_BUFF_MAX];
  va_list args;

  va_start(args, format);
  sent = vsnprintf(pBuf, CMD_PRINTF_BUFF_MAX - 1, format, args);
  va_end(args);
  if (sent > 0) {
    CMD_SendData((uint8_t*)pBuf, sent);
  }
}

/**
 * 控制台打印HEX数据
 * @param dat  数据指针
 * @param len  数据的长度
 */
void CMD_HEX_Print(uint8_t* dat, uint16_t len) {
  uint16_t line, rem, i, pos = 0;
  uint8_t pBuf[50];

  if (dat != NULL && len > 0) {
    line = (len % 16) ? 1 : 0;
    line += len / 16;

    for (i = 0; i < line; i++) {
      rem = len - pos;
      /*一行最多16个字*/
      if (rem > 16) {
        rem = 16;
      }
      Array2Hex(dat, pBuf, rem);
      CMD_Printf("0x%04X:%s", pos, pBuf);
      dat += rem;
      pos += rem;
    }
  }
}

/**
  * 控制台打印可变参数字符串,根据优先等级决定是否打印
  * @param  fomat: 参数列表.
  * @param  ...:可变参数
  */
void CMD_Printf_Level(uint8_t level, char* format, ...) {
  uint16_t sent = 0;
  static char pBuf[CMD_PRINTF_BUFF_MAX];
  va_list args;

  if (level > 0 && level <= g_ucDbgLevel) {
    va_start(args, format);
    sent = vsnprintf(pBuf, CMD_PRINTF_BUFF_MAX - 1, format, args);
    va_end(args);
    if (sent > 0) {
      CMD_SendData((uint8_t*)pBuf, sent);
    }
  }
}

/**
 * 控制台打印HEX数据,根据优先等级决定是否打印
 * @param dat  数据指针
 * @param len  数据的长度
 */
void CMD_HEX_Print_Level(uint8_t level, uint8_t* dat, uint16_t len) {
  if (level > 0 && level <= g_ucDbgLevel) {
    CMD_HEX_Print(dat, len);
  }
}

/**
 * 控制台设置回显开关
 * @param en 回显使能开关
 */
void CMD_SetEchoEnable(BOOL en) {
  if (en != CMD_EchoEnable) {
    CMD_EchoEnable = en;
  }
}

/**
 * 控制台设置打印信息等级
 * @param level
 */
void CMD_SetDebugLevel(uint8_t level) {
  if (level != g_ucDbgLevel) {
    g_ucDbgLevel = level;
  }
}

/**
 * 命令处理轮询.
 */
void CMD_Proc_Poll(void) {
  char c = 0;

  c = CMD_RecBuffer[CMD_RecBufferIndex - 1];
  if (c == '\n' && CMD_RecBuffer[CMD_RecBufferIndex - 2] == '\r') {
    if (CMD_EchoEnable) {
      CMD_SendData(CMD_RecBuffer, CMD_RecBufferIndex);
    }
    CMD_RecBuffer[CMD_RecBufferIndex - 2] = 0;
    Cmd_Handle((char*)CMD_RecBuffer);
    CMD_RecBufferIndex = 0;
  }
}

/* Private function prototypes -----------------------------------------------*/

/**
 * 系统调试命令
 * @param argc 参数项数量
 * @param argv 参数列表
 */
static void System_Console(int argc, char* argv[]) {
  uint32_t d = 0;

  argv++;
  argc--;

  if (strcmp(*argv, "reset") == 0) {
    d = uatoi(argv[1]);
    DBG_LOG("System Will Reset %dms latter.", d);
    HAL_Delay(d + 5);
    NVIC_SystemReset();
  } else if (strcmp(*argv, "version") == 0) {
    DBG_LOG("Firmware version:%s", VERSION);
  } else if (strcmp(*argv, "echo") == 0) {
    if (strcmp(argv[1], "on") == 0) {
      CMD_SetEchoEnable(TRUE);
    } else if (strcmp(argv[1], "off") == 0) {
      CMD_SetEchoEnable(FALSE);
    }
    DBG_LOG("Console set echo:%s.", argv[1]);
  } else if (strcmp(*argv, "loglevel") == 0) {
    d = uatoi(argv[1]);
    CMD_SetDebugLevel(d);
    DBG_WAR("System log level set to:%d.", d);
  }
}


