/***************************************************************************
* NAME:         eeprom.h
*----------------------------------------------------------------------------
* Copyright (c) 
*----------------------------------------------------------------------------
* DATE:			2018-06-07
* REVISION:     V1.0
*
*----------------------------------------------------------------------------
* NOTE:
*
*****************************************************************************/
#ifndef __UART__H__
#define __UART__H__
#include "prjlib.h"
#ifdef __cplusplus
extern "C"
{
#endif
/*---------------------C O M P L I E R  C O M M A N D----------------*/
/*---------------------I N C L U D  F I L E S------------------------*/

extern FIFO_t	uartsendfifo;
void uart_fifoinit(void);
void UART_NewData(uint8_t data);
void UART_WriteBlocking_ex(uint8_t* pdata, uint16_t len);
void uartdataproc(void);
BOOL UART_ReceiveByte(UART_HandleTypeDef *pHandle/*uint8_t num*/);
#ifdef __cplusplus
}
#endif

#endif
