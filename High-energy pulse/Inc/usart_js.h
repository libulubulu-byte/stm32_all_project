/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stdint.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

typedef struct
{
	uint8_t ucRxData;    //receive data
	uint8_t ucRxCnt;     //receive package cnt
    uint8_t ucProtocol;
	uint8_t ucMsgBuf[16];
}Uart_Msg_TypeDef;

typedef struct
{
	uint8_t ucID;
	uint8_t ucProtocol;    //receive data
	uint8_t ucCmd;     //receive package cnt
    uint8_t ucLength;
	uint8_t ucDataBuf[12];
}Uart_Data_TypeDef;


extern osMessageQueueId_t xFromUpStreamMsg;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_UART4_Init(uint32_t bps);
void MX_USART1_UART_Init(void);
void MX_USART6_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void vUART_TASK(void *pvParameters);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
