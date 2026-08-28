/**
 * **********************************************************************
 *             Copyright (c) 2016 temp. All Rights Reserved.
 * @file uaer_comm.h
 * @author 宋阳
 * @version V1.0
 * @date 2016.4.1
 * @brief 用户公用头文件.
 *
 * **********************************************************************
 * @note
 *
 * **********************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USER_COMM_H
#define _USER_COMM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"
#include "prjlib.h"
#include "console.h"
#include "protocol.h"
#include "SD.h"

/* Exported define -----------------------------------------------------------*/
/*调试串口号，为0时关闭打印输出*/
#define DEBUG               1

#define LOG_LEVEL_DYNAMIC   1

/*DEBUG
    信息默认等级,须设置为常量数字宏才能展开*/
#define LOG_LEVEL_DEF       4

#define PROJECT             "AB-12"
#define VERSION             "AC-12_FM_V0.1"
#define VERSION_HARDWARE    "AC-12_HD_V1.0"

#define UART1_RECEVIE_BUFFER_SIZE   0
#define UART2_RECEVIE_BUFFER_SIZE   1024


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define DBG_LEVEL_ERR           1
#define DBG_LEVEL_WAR           2
#define DBG_LEVEL_LOG           3
#define DBG_LEVEL_INFO          4
#define DBG_LEVEL_DEBUG         5
#define DBG_LEVEL_TEMP          6

/* Exported macro ------------------------------------------------------------*/

/*启用DEBUG信息*/
#if DEBUG > 0

/*DEBUG 信息等级动态*/
#if LOG_LEVEL_DYNAMIC > 0
#define DBG_ERR(format, ...)    CMD_Printf_Level(DBG_LEVEL_ERR, "error> file:%s line:%d "format"\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DBG_WAR(format, ...)    CMD_Printf_Level(DBG_LEVEL_WAR, "warring> "format"\r\n", ##__VA_ARGS__)
#define DBG_LOG(format, ...)    CMD_Printf_Level(DBG_LEVEL_LOG, "log> "format"\r\n", ##__VA_ARGS__)
#define DBG_INFO(format, ...)   CMD_Printf_Level(DBG_LEVEL_INFO, "inf> "format"\r\n", ##__VA_ARGS__)
#define DBG_DBG(format, ...)    CMD_Printf_Level(DBG_LEVEL_DEBUG, "dbg> "format"\r\n", ##__VA_ARGS__)
#define DBG_TEMP(format, ...)   CMD_Printf_Level(DBG_LEVEL_TEMP, "temp> "format"\r\n", ##__VA_ARGS__)

/*DEBUG 信息等级静态*/
#else
#if ((LOG_LEVEL_DEF > 0) && (LOG_LEVEL_DEF <= DBG_LEVEL_ERR))
#define DBG_ERR(format, ...)    CMD_Printf("error> "format"\r\n", ##__VA_ARGS__)
#else
#define DBG_ERR(format, ...)
#endif

#if ((LOG_LEVEL_DEF > 0) && (LOG_LEVEL_DEF <= DBG_LEVEL_WAR))
#define DBG_WAR(format, ...)    CMD_Printf("warring> "format"\r\n", ##__VA_ARGS__)
#else
#define DBG_WAR(format, ...)
#endif

#if ((LOG_LEVEL_DEF > 0) && (LOG_LEVEL_DEF <= DBG_LEVEL_LOG))
#define DBG_LOG(format, ...)    CMD_Printf("log> "format"\r\n", ##__VA_ARGS__)
#else
#define DBG_LOG(format, ...)
#endif

#if ((LOG_LEVEL_DEF > 0) && (LOG_LEVEL_DEF <= DBG_LEVEL_INFO))
#define DBG_INFO(format, ...)   CMD_Printf("inf> "format"\r\n", ##__VA_ARGS__)
#else
#define DBG_INFO(format, ...)
#endif

#if ((LOG_LEVEL_DEF > 0) && (LOG_LEVEL_DEF <= DBG_LEVEL_DEBUG))
#define DBG_DBG(format, ...)    CMD_Printf("dbg> "format"\r\n", ##__VA_ARGS__)
#else
#define DBG_DBG(format, ...)
#endif

#if ((LOG_LEVEL_DEF > 0) && (LOG_LEVEL_DEF <= DBG_LEVEL_TEMP))
#define DBG_TEMP(format, ...)   CMD_Printf("temp> "format"\r\n", ##__VA_ARGS__)
#else
#define DBG_TEMP(format, ...)
#endif
#endif

#define DBG_HEX(dat, len)                       CMD_HEX_Print(dat, len)
#define DBG_SEND(dat, len)                      CMD_SendData(dat, len)
#define THROW(str)                              UART_SendData_Direct(DEBUG, (uint8_t *)str, strlen_t(str))
#define THROW_PRINTF(format, ...)               CMD_Printf("> "format"\r\n",  ##__VA_ARGS__);

#define DBG_PRINT(level, format, ...)           CMD_Printf_Level(level, format"\r\n", ##__VA_ARGS__)
#define DBG_PRINTBUF(level, format, buf, len)   do \
                                                {\
                                                    CMD_Printf_Level(level, "%s\r\n", format);\
                                                    CMD_HEX_Print_Level(level, buf, len);\
                                                } while (0)
/*禁用DEBUG信息*/
#else
#define DBG_ERR(format, ...)
#define DBG_WAR(format, ...)
#define DBG_LOG(format, ...)
#define DBG_INFO(format, ...)
#define DBG_DBG(format, ...)
#define DBG_TEMP(format, ...)
#define DBG_SEND(dat, len)
#define THROW(str)
#define DBG_HEX(dat, len)
#define DBG_PRINT(level, format, ...)
#define DBG_PRINTBUF(level, format, buf, len)

#endif


/*IO操作*/
#define IO_READ(x)              ((x##_GPIO_Port->IDR & x##_Pin) ? 1 : 0)
#define IO_H(x)                 (x##_GPIO_Port->BSRR = x##_Pin)
#define IO_L(x)                 (x##_GPIO_Port->BSRR = (uint32_t)x##_Pin << 16)
#define IO_TOGGLE(x)            (x##_GPIO_Port->ODR ^= x##_Pin)
#define IO_WRITE(x, state)      (HAL_GPIO_WritePin(x##_GPIO_Port, x##_Pin, (GPIO_PinState)state))
#define IO_IS_OUT(x)            (x##_GPIO_Port->ODR & x##_Pin)

#define GPIO_H(port, pin)       (port->BSRR = pin)
#define GPIO_L(port, pin)       (port->BSRR = (uint32_t)pin << 16)

#define RS485_ENABLE(x)         IO_H(RS485_##x)
#define RS485_DISENABLE(x)      IO_L(RS485_##x)                                                  
#define LED_ON(x)               IO_L(##x)
#define LED_OFF(x)              IO_H(LED_##x)
#define LED_TOGGLE(x)           IO_TOGGLE(LED_##x)
#define LED_WRITE(x, state)     IO_WRITE(LED_##x, state)
#define LED_IS_ON(x)            IO_IS_OUT(LED_##x)
#define MOTOR_RUN(x)            IO_H(MOTOR_##x)
#define MOTOR_STOP(x)           IO_L(MOTOR_##x)                                                

/*TS*/
#define TS_INIT(ts)             do {ts = HAL_GetTick();}while(0)
#define TS_IS_OVER(ts, over)    (HAL_GetTick() - ts >= over)
#define TS_COUNT(ts)            (HAL_GetTick() - ts)
#define TS_DELAY(ts)            HAL_Delay(ts);



/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/



#endif
