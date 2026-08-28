/**
  ******************************************************************************
  * File Name          : USART_TCP.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************

*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_tcp_H
#define __usart_tcp_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"


typedef struct
{
	uint8_t ucRxData;    //receive data
	uint16_t uiRxCnt;     //receive package cnt
    uint8_t ucProtocol;
	uint8_t ucMsgBuf[100];
}TCP_Msg_TypeDef;

typedef struct
{
    uint16_t uiLength;
	uint8_t ucDataBuf[250];
}TCP_Data_TypeDef;

typedef struct
{
    char ip_address[18];
    char mac_address[13];
    char mac_address_format[18];
}TCP_Addr_TypeDef;

extern osMessageQueueId_t xTCPMsgID;
extern UART_HandleTypeDef huart1;


void MX_USART1_UART_Init(void);
void HAL_UART1_RxCallBack(void);
void HAL_UART_TCP_MspInit(void);
void MX_USART_TCP_Init(void);

/* USER CODE BEGIN Prototypes */
void vUART_TCP_TASK(void *pvParameters);
void vUartTCPSend(uint16_t ucLength,char *pData);
void vUartTCPConnect(uint8_t ucAddress);
uint16_t CalcCRC( char *pbuf, uint16_t len );
void vUartTCPRegister(void);
void Pkcs5padding_add(char *input);
void Base64encode( char *str, char *res,uint16_t str_length);
void Base64decode( char *code, char *res,uint16_t code_length);
void UART_Init(uint32_t bps);
void Delay_ms(uint16_t num_ms);
void HAL_UART1_Rx(void);     //非上位机解析

/* USER CODE END Prototypes */


#endif /*__ usart_tcp_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
