/**
  ******************************************************************************
  * File Name          : USART.c
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "uart_tcp.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include <string.h>

//#include "ds1302.h"
#include "var.h"
#include "pcf8562.h"

Uart_Msg_TypeDef uPort;
uint8_t ucDataLength;
uint8_t ucAddress;
extern DEVICEDATA device;
osMessageQueueId_t xFromUpStreamMsg;
uint8_t ucProtocol = 0;
uint8_t Uart4ItOnOffFlag = 0, Uart4RstFlag = 0;
uint16_t Uart4RstCnt = 0;
char  SW_Version[] = {"DS-PF-JX-E0206N_V1.3.10_20240711"};

void UART_Init(uint32_t bps);

const unsigned char CRCHi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/***************** CRC浣庝綅瀛楄妭鍊艰〃********************/
const  unsigned char CRCLo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

char  SwVersion[35];
char  SwVersion_E0206[35] = {"//DS-RF20206N_V2.0.03_20250429//"};
char  SwVersion_E0106[35] = {"//DS-RF20106N_V2.0.03_20250429//"};
char  SwVersion_E0204[35] = {"//DS-RF20204N_V2.0.03_20250429//"};
char  SwVersion_E0104[35] = {"//DS-RF20104N_V2.0.03_20250429//"};
extern uint8_t firmware_vison[8];

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart6, (uint8_t*)&ch, 1, 0xFF);
    return ch;
}
/* USER CODE END 0 */

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart6;

/* UART4 init function */
void MX_UART4_Init(uint32_t bps)
{
    huart4.Instance = UART4;
    huart4.Init.BaudRate = bps;
    huart4.Init.WordLength = UART_WORDLENGTH_8B;
    huart4.Init.StopBits = UART_STOPBITS_1;
    huart4.Init.Parity = UART_PARITY_NONE;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;
    if(HAL_UART_Init(&huart4) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USART6 init function */

void MX_USART6_UART_Init(void)
{
    huart6.Instance = USART6;
    huart6.Init.BaudRate = 115200;
    huart6.Init.WordLength = UART_WORDLENGTH_8B;
    huart6.Init.StopBits = UART_STOPBITS_1;
    huart6.Init.Parity = UART_PARITY_NONE;
    huart6.Init.Mode = UART_MODE_TX_RX;
    huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart6.Init.OverSampling = UART_OVERSAMPLING_16;
    if(HAL_UART_Init(&huart6) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(uartHandle->Instance == UART4)
    {
        /* UART4 clock enable */
        __HAL_RCC_UART4_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**UART4 GPIO Configuration
        PC10     ------> UART4_TX
        PC11     ------> UART4_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* UART4 interrupt Init */
        HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(UART4_IRQn);
    }

    else if(uartHandle->Instance == USART6)
    {
        /* USART6 clock enable */
        __HAL_RCC_USART6_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**USART6 GPIO Configuration
        PC6     ------> USART6_TX
        PC7     ------> USART6_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    }
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
{
    if(huart->ErrorCode & HAL_UART_ERROR_ORE)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

    if(uartHandle->Instance == UART4)
    {
        /* Peripheral clock disable */
        __HAL_RCC_UART4_CLK_DISABLE();

        /**UART4 GPIO Configuration
        PC10     ------> UART4_TX
        PC11     ------> UART4_RX
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_11);

        /* UART4 interrupt Deinit */
        HAL_NVIC_DisableIRQ(UART4_IRQn);
    }

    else if(uartHandle->Instance == USART6)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART6_CLK_DISABLE();

        /**USART6 GPIO Configuration
        PC6     ------> USART6_TX
        PC7     ------> USART6_RX
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
    }
}

/* USER CODE BEGIN 1 */
unsigned int crc16(unsigned char* puchMsg, unsigned int usDataLen)
{
    unsigned char i;
    unsigned char uchCRCHL = 0x00 ;
    unsigned int  uIndex = 0x00;
    for(i = 0; i < usDataLen; i++)
    {
        uIndex = uIndex + *puchMsg++ ;
    }
    uchCRCHL = uIndex % 256;
    return (uchCRCHL) ;
}

unsigned int crc16_JS(unsigned char* puchMsg, unsigned int usDataLen)
{
    unsigned char uchCRCHi = 0xFF ; /* */
    unsigned char uchCRCLo = 0xFF ; /*  */
    unsigned int  uIndex ; /*  */
    while(usDataLen--)  /*  */
    {
        uIndex = uchCRCLo ^ *puchMsg++ ; /* 璁＄畻CRC */
        uchCRCLo = uchCRCHi ^ CRCHi[uIndex] ;
        uchCRCHi = CRCLo[uIndex] ;
    }
    return (uchCRCHi << 8 | uchCRCLo) ;
}

/*****************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    uint8_t ucCRCH, ucCRCL;
//    uint8_t ucCRCL;
    uint16_t uiCRC;
//    Uart_Msg_TypeDef *pxRecvMsg;

    if(huart->Instance == USART1)
    {
        HAL_UART1_RxCallBack();
    }
    else if(huart->Instance == UART4)
    {
//        pxRecvMsg = &uPort;
        Uart4RstCnt = 0;
        __HAL_UART_DISABLE_IT(&huart4, UART_IT_RXNE);
        switch(uPort.ucRxCnt)
        {
        case 0x00:
            if(uPort.ucRxData == 0xA3)
                uPort.ucMsgBuf[uPort.ucRxCnt++] = uPort.ucRxData ;
            else if(uPort.ucRxData == 0x7E)
                uPort.ucMsgBuf[uPort.ucRxCnt++] = uPort.ucRxData ;
            else
            {
                uPort.ucRxCnt = 0x00;
                uPort.ucMsgBuf[0] = 0x00;
            }
            break;

        case 0x01://local address
            if(ucAddress == uPort.ucRxData)
            {
                uPort.ucMsgBuf[uPort.ucRxCnt++] = uPort.ucRxData ;
            }
            else
            {
                uPort.ucRxCnt = 0x00;
                uPort.ucMsgBuf[1] = 0x00;
            }
            break;

        case 0x02:     //source address
            if((uPort.ucRxData == 0xFE) && (uPort.ucMsgBuf[0] == 0xA3))
            {
                uPort.ucMsgBuf[uPort.ucRxCnt++] = uPort.ucRxData ;
                ucProtocol = 1;//HK
            }
            else if((uPort.ucRxData >= 0xb0) && (uPort.ucRxData <= 0xbf) && (uPort.ucMsgBuf[0] == 0x7E))
            {
                uPort.ucMsgBuf[uPort.ucRxCnt++] = uPort.ucRxData ;
                ucProtocol = 0;//joosee
            }
            else
            {
                uPort.ucRxCnt = 0x00;
                uPort.ucMsgBuf[2] = 0x00;
            }
            break;

        case 0x03:     //command
            uPort.ucMsgBuf[uPort.ucRxCnt++] = uPort.ucRxData ;
            break;

        case 0x04://length
            if(ucProtocol == 1) //HK
            {
                ucDataLength = uPort.ucRxData;
            }
            else //joosee
            {
                ucDataLength = 10;//joosee
            }

            if(ucDataLength >= 16)
            {
                uPort.ucRxCnt = 0x00;
                uPort.ucRxData = 0x00;
                for(uint8_t i = 0; i < 16; i++)
                {
                    uPort.ucMsgBuf[i] = 0x00;
                }
            }
            else
            {
                uPort.ucMsgBuf[uPort.ucRxCnt++] = uPort.ucRxData ;
            }
            break;
        default:
            uPort.ucMsgBuf[uPort.ucRxCnt++] = uPort.ucRxData ;
            if(ucDataLength == uPort.ucRxCnt)
            {
                uPort.ucRxCnt = 0;
                if(ucProtocol == 1) //HK
                {
                    uiCRC = crc16(uPort.ucMsgBuf, ucDataLength - 1);
                    ucCRCL = uiCRC % 256;
                    if(uPort.ucMsgBuf[ucDataLength - 1] == ucCRCL) //CRC校验检测判断
                    {
                        osMessageQueuePut(xFromUpStreamMsg, &uPort, 0, NULL);
                        Uart4ItOnOffFlag = 1;
                    }
                    else
                    {
                        uint8_t ucxTxMsgBuf[10];
                        ucxTxMsgBuf[0] =  0xA3;
                        ucxTxMsgBuf[1] =  uPort.ucMsgBuf[2];
                        ucxTxMsgBuf[2] =  ucAddress;
                        ucxTxMsgBuf[3] =  uPort.ucMsgBuf[3];
                        ucxTxMsgBuf[4] =  0x07;
                        ucxTxMsgBuf[5] =  0x02;
                        ucxTxMsgBuf[6] =  crc16(ucxTxMsgBuf, 6);
                        UART_SendData(ucxTxMsgBuf, 7);
                        uPort.ucRxData = 0x00;
                        for(uint8_t i = 0; i < 16; i++)
                        {
                            uPort.ucMsgBuf[i] = 0x00;
                        }
                    }
                }
                else //joosee
                {
                    uiCRC = crc16_JS(uPort.ucMsgBuf, ucDataLength - 3);
                    ucCRCH = uiCRC / 256;
                    ucCRCL = uiCRC % 256;
                    if((uPort.ucMsgBuf[ucDataLength - 3] == ucCRCH) && (uPort.ucMsgBuf[ucDataLength - 2] == ucCRCL) && (uPort.ucRxData == 0x7F))
                    {
                        osMessageQueuePut(xFromUpStreamMsg, &uPort, 0, NULL);
                        Uart4ItOnOffFlag = 1;
                    }
                }
            }
            break;
        }
        if(Uart4ItOnOffFlag == 0)
            while(HAL_UART_Receive_IT(huart, (uint8_t*)&uPort.ucRxData, 1) == HAL_OK);
    }
//    if (HAL_UART_Receive_IT(huart, (uint8_t *)&uPort.ucRxData, 1) != HAL_OK)
//    {
//        HAL_UART_ErrorCallback(huart);
//    }
}

void UART_Init(uint32_t bps)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    MX_UART4_Init(bps);
    MX_USART6_UART_Init();

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

    HAL_UART_Receive_IT(&huart4, (uint8_t*)&uPort.ucRxData, 1);
#if Debug_flag
    printf("uart init success");
#endif
}

void UART_SendData(uint8_t* pData, uint8_t ucDataLen)
{
    led3_on();
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    for(unsigned int i = 0; i < 6000; i++)
    {
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
    }
    HAL_UART_Transmit(&huart4, pData, ucDataLen, 0xff);
    osDelay(2);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    led3_off();
}

uint8_t BCD_HEX(uint8_t dat)
{
    uint8_t a, b, c;
    a = dat / 10;
    b = dat % 10;
    c = ((a << 4) & 0xf0) | (b & 0x0f);
    return (c);
}
uint8_t HEX_BCD(uint8_t dat)
{
    uint8_t a, b;
    a = (dat >> 4) * 10;
    b = a + (dat & 0x0f);
    return (b);
}

void SelectSwVersion(void)
{
    memset(SwVersion, 0, 35);
    if(SIX_LINE_BOARD == 1)
    {
        if(DUAL_DEFENCE == 1)
        {
            memcpy(SwVersion, SwVersion_E0206, 35);
        }
        else
        {
            memcpy(SwVersion, SwVersion_E0106, 35);
        }
    }
    else
    {
        if(DUAL_DEFENCE == 1)
        {
            memcpy(SwVersion, SwVersion_E0204, 35);
        }
        else
        {
            memcpy(SwVersion, SwVersion_E0104, 35);
        }
    }
}


// UART任务函数，用于处理串口通信
void vUART_TASK(void* pvParameters)
{
    // 定义接收消息结构体并初始化为0
    Uart_Msg_TypeDef xRxMsgBuf = {0};
    // EEPROM写数据结构体
    EEprom_Msg_TypeDef xEEpromWriteData;
    // RTC时间/日期结构体
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    // RS485使能信号控制引脚(PA15)
    uint8_t ucCmd, i;         // 命令码和循环变量
    uint8_t ucxTxMsgBuf[32];  // 发送缓冲区

    // 获取设备数据结构指针
    DEVICEDATA* pd;
    pd = (DEVICEDATA*) pvParameters;  // 从任务参数转换设备数据结构
    uint8_t data_temp;  // 临时数据存储

    // CRC校验和发送长度相关变量
    uint16_t uiCRC;
    uint8_t ucTxLength;  // 发送数据长度
    uint8_t xTxDataBuf[48];  // 待发送数据缓冲区
    uint8_t xTxMsgBuf[48];  // 完整消息缓冲区

    // 创建消息队列（容量8，消息大小为Uart_Msg_TypeDef）
    xFromUpStreamMsg = osMessageQueueNew(8, sizeof(Uart_Msg_TypeDef), NULL);
    if(xFromUpStreamMsg == 0)  // 创建失败处理
    {
        // 调试模式下输出错误信息
#if Debug_flag
        printf("uart task have no enough heap");
#endif
    }
    else  // 初始化成功时初始化UART
    {
        UART_Init((pd->uiBaudrate));  // 使用设备结构中的波特率
    }

    // 任务主循环
    while(1)
    {
        // 外部看门狗喂狗信号（翻转PA1引脚）
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
        
        // 更新设备地址
        ucAddress = pd->ucAddress;

        // 从消息队列获取数据（等待时间0xff表示无限等待）
        if(osMessageQueueGet(xFromUpStreamMsg, &xRxMsgBuf, NULL, 0xff) == osOK)
        {
            Uart4RstFlag = 1;  // 设置UART4复位标志
            
            // 重置接收数据标志和缓冲区
            uPort.ucRxData = 0x00;
            for(i = 0; i < 16; i++)
            {
                uPort.ucMsgBuf[i] = 0x00;
            }

            // ================= 协议0处理逻辑（Joosee协议）================
            if(ucProtocol == 0) 
            {
                ucTxLength = 0x00;
                ucCmd = xRxMsgBuf.ucMsgBuf[2];  // 从消息中提取命令码
                
                // 命令分发处理
                switch(ucCmd)
                {
                case 0xB0:  // 轮询命令（获取设备状态）
                    // 填充电压、脉冲状态、报警类型数据
                    xTxDataBuf[0] = pd->ucVoltage[0];
                    xTxDataBuf[1] = pd->ucVoltage[1];
                    xTxDataBuf[2] = pd->ucPulseOn;
                    xTxDataBuf[3] = pd->ucAlarmType & 0x0f;
                    if(pd->ucAlarmType & 0x03)
                        xTxDataBuf[3] = 0xAA >> ((pd->ucAlarmType & 0x03) - 1);
                    else
                        xTxDataBuf[3] = 0x00;
                    // 处理报警类型（高4位）
                    if(pd->ucAlarmType & 0x30)
                        xTxDataBuf[4] = 0xAA >> (((pd->ucAlarmType & 0x30) >> 4) - 1);
                    else
                        xTxDataBuf[4] = 0x00;
                    // 报警状态处理
                    if(pd->ucAlarmStatus & 0x02)
                        xTxDataBuf[5] = 0xAA;
                    else
                        xTxDataBuf[5] = 0x00;
                    ucTxLength = 6;  // 设置响应数据长度
                    break;

                case 0xB1:  // 设置蜂鸣器时间
                    if((xRxMsgBuf.ucMsgBuf[3] < 51) && (xRxMsgBuf.ucMsgBuf[3] > 4)) 
                    {
                        xTxDataBuf[0] = 0x01;  // 成功标志
                        pd->ucBeepOnTime = xRxMsgBuf.ucMsgBuf[3] * 750;  // 计算蜂鸣时长
                        // 准备EEPROM写入数据
                        xEEpromWriteData.ucCmd = 0x06;
                        xEEpromWriteData.ucData[0] = xRxMsgBuf.ucMsgBuf[3];
                        xEEpromWriteData.ucData[1] = 0x00;
                        xEEpromWriteData.ucData[2] = 0x00;
                        xEEpromWriteData.ucData[3] = 0x00;
                        xEEpromWriteData.ucData[4] = 0x00;
                        // 发送到EEPROM写入队列
                        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xff);
                    }
                    else
                        xTxDataBuf[0] = 0x00;  // 失败标志
                    ucTxLength = 0x01;
                    break;

                case 0xB3:  // 设置输出电压
                    if(xRxMsgBuf.ucMsgBuf[3] < 0x0B) 
                    {
                        xTxDataBuf[0] = 0x01;  // 成功标志
                        // 计算并设置电压值（双通道）
                        pd->ucSetVoltage[0] = pd->ucSetVoltage[1] = (900 + xRxMsgBuf.ucMsgBuf[3] * 560) / 100;
                        pd->ucForceSync = 0x01;  // 设置强制同步标志
                    }
                    else
                        xTxDataBuf[0] = 0x00;  // 失败标志
                    ucTxLength = 0x01;
                    break;

                case 0xB6:  // 设置继电器开启时间
                    if((xRxMsgBuf.ucMsgBuf[3] > 0x01) && (xRxMsgBuf.ucMsgBuf[3] < 0x15)) 
                    {
                        xTxDataBuf[0] = 0x01;  // 成功标志
                        pd->ucRelayOnTime = xRxMsgBuf.ucMsgBuf[3] * 750;  // 计算继电器开启时间
                        // 准备EEPROM写入
                        xEEpromWriteData.ucCmd = 0x06;
                        xEEpromWriteData.ucData[0] = 0x00;
                        xEEpromWriteData.ucData[1] = 0x00;
                        xEEpromWriteData.ucData[2] = 0x00;
                        xEEpromWriteData.ucData[3] = 0x00;
                        xEEpromWriteData.ucData[4] = xRxMsgBuf.ucMsgBuf[3];
                        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xff);
                    }
                    else
                        xTxDataBuf[0] = 0x00;  // 失败标志
                    ucTxLength = 0x01;
                    break;

                case 0xB7:  // 开启脉冲输出
                    xTxDataBuf[0] = 0x00;  // 默认失败
                    ucTxLength =  0x01;
                    device.ucLcdOnTime = 120;  // LCD背光开启时间
                    lcd_backup_on();           // 开启LCD背光
                    // 处理主通道开启
                    if(xRxMsgBuf.ucMsgBuf[3] == 0xAA) 
                    {
                        xTxDataBuf[0] = 0x01;  // 成功标志
                        pd->ucPulseOn |= 0x01; // 设置主通道脉冲开启位
                    }
                    // 双防区设备处理副通道
                    if(DUAL_DEFENCE == 1) 
                    {
                        xTxDataBuf[1] = 0x00;  // 默认失败
                        if(xRxMsgBuf.ucMsgBuf[4] == 0xAA) 
                        {
                            xTxDataBuf[1] = 0x01;  // 成功标志
                            pd->ucPulseOn |= 0x02; // 设置副通道脉冲开启位
                        }
                        ucTxLength = 0x02;  // 双字节响应
                    }
                    break;

                case 0xB8:  // 关闭脉冲输出（逻辑与0xB7类似但取反）
                    xTxDataBuf[0] =  0x00;
                    ucTxLength =  0x01;
                    device.ucLcdOnTime = 120;
                    lcd_backup_on();
                    if(xRxMsgBuf.ucMsgBuf[3] == 0xAA)
                    {
                        xTxDataBuf[0] =  0x01;
                        pd->ucPulseOn &= 0xFE;
                    }
                    if(DUAL_DEFENCE == 1)
                    {
                        xTxDataBuf[1] =  0x00;
                        if(xRxMsgBuf.ucMsgBuf[4] == 0xAA)
                        {
                            xTxDataBuf[1] =  0x01;
                            pd->ucPulseOn &= 0xFD;
                        }
                        ucTxLength =  0x02;
                    }
                    break;

                case 0xB9:  // 多功能命令
                    switch(xRxMsgBuf.ucMsgBuf[3])  // 子命令
                    {
                    case 0x01:  // 查询运行时间
                        // 组织年份/日期/小时数据
                        xTxDataBuf[0] = pd->ucRunningYear;
                        xTxDataBuf[1] = pd->uiRunningDate / 256;  // 日期高字节
                        xTxDataBuf[2] = pd->uiRunningDate % 256;  // 日期低字节
                        xTxDataBuf[3] = pd->ucRunningHour;
                        ucTxLength = 4;
                        break;
                    case 0x02:  // 清除运行时间
                        // 重置所有运行时间参数
                        device.ucRunningYear = 0;
                        device.uiRunningDate = 0;
                        device.ucRunningHour = 0;
                        device.ucRunningMin = 0;
                        // 准备写入EEPROM的数据结构
                        xEEpromWriteData.ucCmd = 0x04;
                        xEEpromWriteData.ucData[0] = device.ucRunningMin;
                        xEEpromWriteData.ucData[1] = device.ucRunningHour;
                        xEEpromWriteData.ucData[2] = device.uiRunningDate / 256;
                        xEEpromWriteData.ucData[3] = device.uiRunningDate % 256;
                        xEEpromWriteData.ucData[4] = device.ucRunningYear;
                        // 发送到EEPROM队列
                        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
                        xTxDataBuf[0] = 0x01;  // 操作成功
                        ucTxLength = 1;
                        break;
                    case 0x03:  // 设置波特率为19200
                        device.uiBaudrate = 19200;  // 更新波特率
                        // 准备EEPROM写入
                        xEEpromWriteData.ucCmd = 0x03;
                        xEEpromWriteData.ucData[0] = device.uiBaudrate >> 8;  // 波特率高字节
                        xEEpromWriteData.ucData[1] = device.uiBaudrate & 0xff;  // 波特率低字节
                        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
                        xTxDataBuf[0] = 0x01;
                        ucTxLength = 1;
                        break;
                    case 0x04:  // 设置波特率为38400（类似0x03）
                        device.uiBaudrate = 38400;
                        xEEpromWriteData.ucCmd = 0x03;
                        xEEpromWriteData.ucData[0] = device.uiBaudrate >> 8;
                        xEEpromWriteData.ucData[1] = device.uiBaudrate & 0xff;
                        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
//							UART_Init(device.uiBaudrate);

                        xTxDataBuf[0] =  0x01;
                        ucTxLength = 1;
                        break;
                    case 0x05:  // 设置输出电压（双通道）
                        // 分别处理两个通道的电压设置
                        if((9 <= xRxMsgBuf.ucMsgBuf[4]) && (xRxMsgBuf.ucMsgBuf[4] <= 65)) 
                        {
                            device.ucSetVoltage[0] = xRxMsgBuf.ucMsgBuf[4];
                            pd->ucForceSync = 0x01;  // 需要同步
                            xTxDataBuf[0] = 0x01;    // 操作成功
                            ucTxLength = 1;
                        }
                        if((9 <= xRxMsgBuf.ucMsgBuf[5]) && (xRxMsgBuf.ucMsgBuf[5] <= 65)) 
                        {
                            device.ucSetVoltage[1] = xRxMsgBuf.ucMsgBuf[5];
                            pd->ucForceSync = 0x01;  // 需要同步
                            xTxDataBuf[0] = 0x01;    // 操作成功
                            ucTxLength = 1;
                        }
                        break;
                    case 0x06:  // 读取设置电压
                        xTxDataBuf[0] = device.ucSetVoltage[0];
                        xTxDataBuf[1] = device.ucSetVoltage[1];
                        ucTxLength = 2;
                        break;
                    case 0x5A:  // 读取软件版本
                        // 复制软件版本号到发送缓冲区
                        for(uint8_t i = 0; i < 32; i++)
                            xTxDataBuf[i] = SwVersion[i];
                        ucTxLength = 32;
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }  // end of switch(ucCmd)

                // 协议0的响应发送
                if(ucTxLength && (ucAddress != 254))  // 254为特殊地址
                {
                    // 构造消息头：起始符+地址+命令
                    xTxMsgBuf[0] = 0x7E;   // 帧头
                    xTxMsgBuf[1] = ucAddress;
                    xTxMsgBuf[2] = ucCmd;
                    // 填充数据段
                    for(uint8_t i = 0; i < ucTxLength; i++)
                        xTxMsgBuf[3 + i] = xTxDataBuf[i];
                    // 计算CRC校验（从帧头到数据结束）
                    uiCRC = crc16_JS(xTxMsgBuf, ucTxLength + 3);
                    // 添加CRC和帧尾
                    xTxMsgBuf[ucTxLength + 3] = uiCRC / 256;  // CRC高字节
                    xTxMsgBuf[ucTxLength + 4] = uiCRC % 256;  // CRC低字节
                    xTxMsgBuf[ucTxLength + 5] = 0x7F;        // 帧尾
                    // 发送完整消息
                    UART_SendData(xTxMsgBuf, ucTxLength + 6);
                }
            }  // end of if(ucProtocol==0)

            // ================= 协议1处理逻辑（HK协议）================
            if(ucProtocol == 1) 
            {
                ucCmd = xRxMsgBuf.ucMsgBuf[3];  // 从消息中获取命令码
                if(DUAL_DEFENCE == 1)  // 双防区设备处理
                {
                    switch(ucCmd)
                    {
                    case 0x10:  // 设备巡检命令
                        // 构造响应消息头
                        ucxTxMsgBuf[0] = 0xA3;
                        ucxTxMsgBuf[1] = xRxMsgBuf.ucMsgBuf[2];  // 序列号
                        ucxTxMsgBuf[2] = ucAddress;              // 设备地址
                        ucxTxMsgBuf[3] = ucCmd;                  // 命令码
                        ucxTxMsgBuf[4] = 17;                     // 数据长度
                        // 填充设备状态数据
                        ucxTxMsgBuf[5] = 0x01;  // 设备类型
                        ucxTxMsgBuf[6] = 0x02;  // 防区数量
                        ucxTxMsgBuf[7] = pd->ucVoltage[0];  // 主通道电压
                        ucxTxMsgBuf[8] = pd->ucVoltage[1];  // 副通道电压
                        // 主通道报警状态解析
                        if(pd->ucRunningStatus & 0x01) 
                        {
                            data_temp = pd->ucAlarmType & 0x0f;
                            if(data_temp == 0x00)
                                ucxTxMsgBuf[9] = 0x10;  // 正常
                            else if(data_temp == 0x01)    
                                ucxTxMsgBuf[9] = 0x12;  // 开路
                            else if(data_temp == 0x02)   
                                ucxTxMsgBuf[9] = 0x11;  // 短路
                            else
                                ucxTxMsgBuf[9] = 0x13;  // 其他故障
                        }
                        else
                            ucxTxMsgBuf[9] = 0x00;  // 无报警
                        // 副通道状态解析（类似主通道）
                        if(pd->ucRunningStatus & 0x02)
                        {
                            data_temp = (pd->ucAlarmType & 0xf0);
                            if(data_temp == 0x00)
                                ucxTxMsgBuf[ 10 ] = 0x10;
                            else if(data_temp == 0x10)
                                ucxTxMsgBuf[10] = 0x12;
                            else if(data_temp == 0x20)
                                ucxTxMsgBuf[10] = 0x11;
                            else
                                ucxTxMsgBuf[10] = 0x13;
                        }
                        else
                            ucxTxMsgBuf[ 10 ] = 0x00;
                        // 电源状态
                        ucxTxMsgBuf[11] = pd->ucPowerStatus;
                        // 防拆状态
                        if(pd->ucAlarmStatus == 0x00) 
                        {
                            ucxTxMsgBuf[12] = 0xF0;  // 正常
                        }
                        else if(pd->ucAlarmStatus == 0x01) 
                        {
                            ucxTxMsgBuf[12] = 0xFF;  // 防拆触发
                        }
                        // 红外使能状态
                        if(pd->ucIREn == 0xAA)
                            ucxTxMsgBuf[ 13 ] = 1;
                        else
                            ucxTxMsgBuf[ 13 ] = 0;
                        // 报警输出状态
                        if(pd->ucAlarmStatus & 0x02)
                            ucxTxMsgBuf[ 14 ] = 0xAA;
                        else
                            ucxTxMsgBuf[ 14 ] = 0x00;
                        ucxTxMsgBuf[15] = 0x00;  // 预留
                        // 计算并添加CRC
                        ucxTxMsgBuf[16] = crc16(ucxTxMsgBuf, 16);
                        // 发送响应
                        UART_SendData(ucxTxMsgBuf, 17);
                        // 清空接收缓冲区
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;

                    case 0x01:  // 设备注册命令
                        // 构造响应消息头
                        ucxTxMsgBuf[0] = 0xA3;
                        ucxTxMsgBuf[1] = xRxMsgBuf.ucMsgBuf[2];  // 序列号
                        ucxTxMsgBuf[2] = ucAddress;              // 设备地址
                        ucxTxMsgBuf[3] = ucCmd;                  // 命令码
                        ucxTxMsgBuf[4] = 25;                     // 数据长度
                        // 填充设备信息
                        ucxTxMsgBuf[5] = 0x01;  // 设备类型
                        ucxTxMsgBuf[6] = 0xF1;  // 厂商代码
                        ucxTxMsgBuf[7] = 0x02;  // 防区数量
                        // 复制固件版本信息
                        for(uint8_t i = 0; i < 8; i++)		//firmware version1.00 2021.0531
                        {
                            ucxTxMsgBuf[8 + i] = firmware_vison[i];
                        }
                        // 硬件版本信息
                        ucxTxMsgBuf[16] = 0x01;  // 硬件版本
                        ucxTxMsgBuf[17] = 0x00;  // 预留
                        ucxTxMsgBuf[18] = 0x00;
                        ucxTxMsgBuf[19] = 0x00;
                        ucxTxMsgBuf[20] = 0x00;
                        ucxTxMsgBuf[21] = 0x01;  // 设备能力
                        ucxTxMsgBuf[22] = 0x01;  // 通信能力
                        // 红外状态
                        if(pd->ucIREn == 0xAA)
                            ucxTxMsgBuf[ 23 ] = 1;
                        else
                            ucxTxMsgBuf[ 23 ] = 0;
                        ucxTxMsgBuf[ 24 ] =  crc16(ucxTxMsgBuf, 24);
                        UART_SendData(ucxTxMsgBuf, 25);
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;

                    case 0x02:  // 删除设备命令
                        // 构造简单响应
                        ucxTxMsgBuf[0] = 0xA3;
                        ucxTxMsgBuf[1] = xRxMsgBuf.ucMsgBuf[2];  // 序列号
                        ucxTxMsgBuf[2] = ucAddress;              // 设备地址
                        ucxTxMsgBuf[3] = ucCmd;                  // 命令码
                        ucxTxMsgBuf[4] = 7;                      // 数据长度
                        ucxTxMsgBuf[5] = 0x03;                   // 操作类型
                        // 计算CRC并发送
                        ucxTxMsgBuf[6] = crc16(ucxTxMsgBuf, 6);
                        UART_SendData(ucxTxMsgBuf, 7);
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;

                    case 0x03:  // 布防/撤防命令
                        if(pd->ucIREn == 0xAA)
                        {
                            if((xRxMsgBuf.ucMsgBuf[ 5 ] == 0x01) || (xRxMsgBuf.ucMsgBuf[ 5 ] == 0x02) || (xRxMsgBuf.ucMsgBuf[ 5 ] == 0x03))
                            {
                                if((pd->ucRunningStatus & 0x01) == 0)
                                    pd->ucPulseOn |= 0x01;
                            }
                            else if(xRxMsgBuf.ucMsgBuf[ 5 ] == 0x00)
                            {
                                if(pd->ucRunningStatus & 0x01)
                                    pd->ucPulseOn &= 0xFE;
                            }
                            if((xRxMsgBuf.ucMsgBuf[ 6 ] == 0x01) || (xRxMsgBuf.ucMsgBuf[ 6 ] == 0x02) || (xRxMsgBuf.ucMsgBuf[ 6 ] == 0x03))
                            {
                                if((pd->ucRunningStatus & 0x02) == 0)
                                    pd->ucPulseOn |= 0x02;
                            }
                            else if(xRxMsgBuf.ucMsgBuf[ 6 ] == 0x00)
                            {
                                if(pd->ucRunningStatus & 0x02)
                                    pd->ucPulseOn &= 0xFD;
                            }
                            if((xRxMsgBuf.ucMsgBuf[7] == 0x01) || (xRxMsgBuf.ucMsgBuf[7] == 0x02) || (xRxMsgBuf.ucMsgBuf[7] == 0x03))
                            {
                                pd->ucIREn = 0xAA;
                            }
                            else if(xRxMsgBuf.ucMsgBuf[ 7 ] == 0x00)
                            {
                                pd->ucIREn = 0x55;
                            }
                            ucxTxMsgBuf[ 0 ] =  0xA3;
                            ucxTxMsgBuf[ 1 ]  = xRxMsgBuf.ucMsgBuf[2];
                            ucxTxMsgBuf[ 2 ] =  ucAddress;
                            ucxTxMsgBuf[ 3 ] =  ucCmd;
                            ucxTxMsgBuf[ 4 ] =  10;
                            ucxTxMsgBuf[ 5 ] =  0x01;
                            ucxTxMsgBuf[ 6 ] =  xRxMsgBuf.ucMsgBuf[ 5 ];
                            ucxTxMsgBuf[ 7 ] =  xRxMsgBuf.ucMsgBuf[ 6 ];
                            ucxTxMsgBuf[ 8 ] =  xRxMsgBuf.ucMsgBuf[ 7 ];

                            ucxTxMsgBuf[ 9 ] =  crc16(ucxTxMsgBuf, 9);
                            UART_SendData(ucxTxMsgBuf, 10);
                            xRxMsgBuf.ucRxData = 0x00;
                            for(i = 0; i < 16; i++)
                            {
                                xRxMsgBuf.ucMsgBuf[i] = 0x00;
                            }
                        }
                        else
                        {
                            if((xRxMsgBuf.ucMsgBuf[ 5 ] == 0x01) || (xRxMsgBuf.ucMsgBuf[ 5 ] == 0x02) || (xRxMsgBuf.ucMsgBuf[ 5 ] == 0x03))
                            {
                                if((pd->ucRunningStatus & 0x01) == 0)
                                    pd->ucPulseOn |= 0x01;
                            }
                            else if(xRxMsgBuf.ucMsgBuf[ 5 ] == 0x00)
                            {
                                if(pd->ucRunningStatus & 0x01)
                                    pd->ucPulseOn &= 0xFE;
                            }

                            if((xRxMsgBuf.ucMsgBuf[ 6 ] == 0x01) || (xRxMsgBuf.ucMsgBuf[ 6 ] == 0x02) || (xRxMsgBuf.ucMsgBuf[ 6 ] == 0x03))
                            {
                                if((pd->ucRunningStatus & 0x02) == 0)
                                    pd->ucPulseOn |= 0x02;
                            }
                            else if(xRxMsgBuf.ucMsgBuf[ 6 ] == 0x00)
                            {
                                if(pd->ucRunningStatus & 0x02)
                                    pd->ucPulseOn &= 0xFD;
                            }
                            ucxTxMsgBuf[ 0 ] =  0xA3;
                            ucxTxMsgBuf[ 1 ]  =  xRxMsgBuf.ucMsgBuf[2];
                            ucxTxMsgBuf[ 2 ] =  ucAddress;
                            ucxTxMsgBuf[ 3 ] =  ucCmd;
                            ucxTxMsgBuf[ 4 ] =  9;
                            ucxTxMsgBuf[ 5 ] =  0x01;
                            ucxTxMsgBuf[ 6 ] =  xRxMsgBuf.ucMsgBuf[ 5 ];
                            ucxTxMsgBuf[ 7 ] =  xRxMsgBuf.ucMsgBuf[ 6 ];
                            ucxTxMsgBuf[ 8 ] =  crc16(ucxTxMsgBuf, 8);
                            UART_SendData(ucxTxMsgBuf, 9);
                            xRxMsgBuf.ucRxData = 0x00;
                            for(i = 0; i < 16; i++)
                            {
                                xRxMsgBuf.ucMsgBuf[i] = 0x00;
                            }
                        }
                        // 激活LCD背光
                        device.ucLcdOnTime = 120;
                        lcd_backup_on();
                        break;

                    case 0x20:  // 参数设置命令
                        // 各参数范围验证和设置：
                        // 1. 蜂鸣器时间 (5-50)
                        if((xRxMsgBuf.ucMsgBuf[5] > 0x01) && (xRxMsgBuf.ucMsgBuf[5] < 0x33)) 
                        {
                            xEEpromWriteData.ucData[0] = xRxMsgBuf.ucMsgBuf[5];
                            pd->ucBeepOnTime = (unsigned int)xRxMsgBuf.ucMsgBuf[5] * 900; //550
                            pd->ucForceSync = 1;
                        }
                        else
                        {
                            xEEpromWriteData.ucData[0] = 0x00;
                        }
                        if((xRxMsgBuf.ucMsgBuf[6] > 0x08) && (xRxMsgBuf.ucMsgBuf[6] < 0x42)) //输出电压设置
                        {
                            pd->ucSetVoltage[0]  = pd->ucSetVoltage[1] =  xRxMsgBuf.ucMsgBuf[6];
                            xEEpromWriteData.ucData[1] = xRxMsgBuf.ucMsgBuf[6];
                            pd->ucForceSync = 1;
                        }
                        else
                        {
                            xEEpromWriteData.ucData[1] = 0x00;
                        }
                        if((xRxMsgBuf.ucMsgBuf[7] >= 0x01) && (xRxMsgBuf.ucMsgBuf[7] < 0x04))  //输出脉冲个数设置
                        {
                            pd->ucAlarmCycle = xRxMsgBuf.ucMsgBuf[ 7 ] ;
                            xEEpromWriteData.ucData[2] = xRxMsgBuf.ucMsgBuf[7];
                            pd->ucForceSync = 1;
                        }
                        else
                        {
                            xEEpromWriteData.ucData[2] = 0x00;
                        }
                        if((xRxMsgBuf.ucMsgBuf[8] == 0x00) || (xRxMsgBuf.ucMsgBuf[8] == 0x01))  //集成开关量使能设置
                        {
                            xEEpromWriteData.ucData[3] = (0x55 << xRxMsgBuf.ucMsgBuf[8]);
                            pd->ucIREn = (0x55 << xRxMsgBuf.ucMsgBuf[8]);
                            pd->ucForceSync = 1;
                        }
                        else
                        {
                            xEEpromWriteData.ucData[3] = 0x00;
                        }
                        if((xRxMsgBuf.ucMsgBuf[9] > 0x01) && (xRxMsgBuf.ucMsgBuf[9] < 0x33)) //继电器输出保持时间设置
                        {
                            xEEpromWriteData.ucData[4] = xRxMsgBuf.ucMsgBuf[9];
                            pd->ucRelayOnTime = (unsigned int)xRxMsgBuf.ucMsgBuf[9] * 900; //550
                            pd->ucForceSync = 1;
                        }
                        else
                        {
                            xEEpromWriteData.ucData[4] = 0x00;
                        }
                        xEEpromWriteData.ucCmd = 0x06;
                        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xFF);

                        ucxTxMsgBuf[ 0 ] =  0xA3;
                        ucxTxMsgBuf[ 1 ] =  xRxMsgBuf.ucMsgBuf[2];
                        ucxTxMsgBuf[ 2 ] =  ucAddress;
                        ucxTxMsgBuf[ 3 ] =  ucCmd;
                        ucxTxMsgBuf[ 4 ] =  7;
                        ucxTxMsgBuf[ 5 ] =  0x01;
                        ucxTxMsgBuf[ 6 ] =  crc16(ucxTxMsgBuf, 6);
                        UART_SendData(ucxTxMsgBuf, 7);
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;

                    case 0x21:
                        if((xRxMsgBuf.ucMsgBuf[5] > 0x00) && (xRxMsgBuf.ucMsgBuf[5] < 0x04))
                        {
                            pd->ucAlarmCycle = xRxMsgBuf.ucMsgBuf[5] ;
                            pd->ucForceSync = 1;
                            xEEpromWriteData.ucData[2] = xRxMsgBuf.ucMsgBuf[5];    //to be comfirm what action
                        }
                        else
                        {
                            xEEpromWriteData.ucData[2] = 0x00;
                        }
                        xEEpromWriteData.ucCmd = 0x06;
                        xEEpromWriteData.ucData[0] = 0x00;
                        xEEpromWriteData.ucData[1] = 0x00;
                        xEEpromWriteData.ucData[3] = 0x00;
                        xEEpromWriteData.ucData[4] = 0x00;
                        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xFF);

                        ucxTxMsgBuf[0] =  0xA3;
                        ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];
                        ucxTxMsgBuf[2] =  ucAddress;
                        ucxTxMsgBuf[3] =  ucCmd;
                        ucxTxMsgBuf[4] =  0x07;
                        ucxTxMsgBuf[5] =  0x01;
                        ucxTxMsgBuf[6] =  crc16(ucxTxMsgBuf, 6);
                        UART_SendData(ucxTxMsgBuf, 7);
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;


                                        case 0x22:  // 未知命令的标准响应（可能用于测试或特定操作）
                        // 构造标准响应帧
                        ucxTxMsgBuf[0] = 0xA3;  // 协议头
                        ucxTxMsgBuf[1] = xRxMsgBuf.ucMsgBuf[2];  // 序列号
                        ucxTxMsgBuf[2] = ucAddress;  // 设备地址
                        ucxTxMsgBuf[3] = ucCmd;  // 原样返回接收到的命令码
                        ucxTxMsgBuf[4] = 7;  // 数据长度
                        ucxTxMsgBuf[5] = 0x03;  // 默认响应码
                        ucxTxMsgBuf[6] = crc16(ucxTxMsgBuf, 6);  // 计算CRC校验
                        // 发送响应数据
                        UART_SendData(ucxTxMsgBuf, 7);
                        // 清空接收缓冲区
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;

                    case 0xda:  // 设置RTC时间命令
                        // 注意：被注释的write_ds1302调用表明可能切换过RTC芯片
                        // 从接收消息中提取日期时间参数
                        sDate.Year = xRxMsgBuf.ucMsgBuf[4];   // 年
                        sDate.Month = xRxMsgBuf.ucMsgBuf[5];   // 月
                        sDate.Date = xRxMsgBuf.ucMsgBuf[6];    // 日
                        sTime.Hours = xRxMsgBuf.ucMsgBuf[7];   // 时
                        sTime.Minutes = xRxMsgBuf.ucMsgBuf[8]; // 分
                        sTime.Seconds = xRxMsgBuf.ucMsgBuf[9]; // 秒
                        // 设置硬件RTC
                        HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
                        HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
                        
                        // 构造返回消息（原样返回设置的时间）
                        ucxTxMsgBuf[0] = 0x06;  // 命令类型
                        ucxTxMsgBuf[1] = sDate.Year;    // 年
                        ucxTxMsgBuf[2] = sDate.Month;   // 月
                        ucxTxMsgBuf[3] = sDate.Date;    // 日
                        ucxTxMsgBuf[4] = sTime.Hours;   // 时
                        ucxTxMsgBuf[5] = sTime.Minutes;  // 分
                        ucxTxMsgBuf[6] = sTime.Seconds;  // 秒
                        // 发送响应
                        UART_SendData(ucxTxMsgBuf, 7);
                        // 清空接收缓冲区
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;

                    case 0xdb:  // 读取RTC时间命令
                        // 从硬件RTC获取当前时间
                        HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
                        HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
                        // 构造返回消息
                        ucxTxMsgBuf[0] = 0x06;  // 命令类型
                        ucxTxMsgBuf[1] = sDate.Year;    // 年
                        ucxTxMsgBuf[2] = sDate.Month;   // 月
                        ucxTxMsgBuf[3] = sDate.Date;    // 日
                        ucxTxMsgBuf[4] = sTime.Hours;   // 时
                        ucxTxMsgBuf[5] = sTime.Minutes;  // 分
                        ucxTxMsgBuf[6] = sTime.Seconds;  // 秒
                        // 发送响应
                        UART_SendData(ucxTxMsgBuf, 7);
                        // 清空接收缓冲区（注：原代码未在此处清空，后续添加）
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;
                    // 注：以下是已被注释的功能代码
                    // case 0x31: // 设置防区模式和线路板配置（需系统重启生效）
                    // case 0x32: // 读取特殊配置的版本号
                    
                    default:  // 未知命令默认响应
                        // 构造标准错误响应
                        ucxTxMsgBuf[0] = 0xA3;  // 协议头
                        ucxTxMsgBuf[1] = xRxMsgBuf.ucMsgBuf[2];  // 序列号
                        ucxTxMsgBuf[2] = ucAddress;  // 设备地址
                        ucxTxMsgBuf[3] = ucCmd;  // 原样返回接收到的命令码
                        ucxTxMsgBuf[4] = 7;  // 数据长度
                        ucxTxMsgBuf[5] = 0x03;  // 未知命令响应码
                        ucxTxMsgBuf[6] = crc16(ucxTxMsgBuf, 6);  // CRC校验
                        // 发送响应
                        UART_SendData(ucxTxMsgBuf, 7);
                        // 清空接收缓冲区
                        xRxMsgBuf.ucRxData = 0x00;
                        for(i = 0; i < 16; i++)
                        {
                            xRxMsgBuf.ucMsgBuf[i] = 0x00;
                        }
                        break;
                    }  // end of dual-defense HK protocol switch
                }  // end of if(DUAL_DEFENCE==1)

                // ================= 单防区处理逻辑(HK协议) ================
							else if(DUAL_DEFENCE == 0)  // 单防区设备协议处理逻辑
							{
									switch(ucCmd)  // 根据命令码进行分支处理
									{
									// =============== 设备注册命令（0x01） ===============
									case 0x01://注册命令
											// 构造响应消息头
											ucxTxMsgBuf[0] =  0xA3;  // 帧起始符
											ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];  // 流水号（从接收消息复制）
											ucxTxMsgBuf[2] =  ucAddress;  // 设备地址
											ucxTxMsgBuf[3] =  ucCmd;  // 命令码（0x01）
											ucxTxMsgBuf[4] =  25;  // 数据域长度（25字节）
											ucxTxMsgBuf[5] =  0x01;  // 设备类型（脉冲主机）
											ucxTxMsgBuf[6] =  0xF1;  // 厂商代码
											ucxTxMsgBuf[7] =  0x01;  // 防区数量（1防区）
											
											// 填充固件版本信息（8字节）
											for(uint8_t i = 0; i < 8; i++) {
													ucxTxMsgBuf[8 + i] = firmware_vison[i];  // 复制全局数组中的版本信息
											}
											
											// 填充硬件版本信息（7字节）
											ucxTxMsgBuf[16] =  0x01; // 主版本号
											ucxTxMsgBuf[17] =  0x00; // 次版本号
											ucxTxMsgBuf[18] =  0x00; // 保留字段
											ucxTxMsgBuf[19] =  0x00; // 保留字段
											ucxTxMsgBuf[20] =  0x00; // 保留字段
											ucxTxMsgBuf[21] =  0x01; // 设备能力标识
											ucxTxMsgBuf[22] =  0x01; // 通信能力标识
											
											// 填充红外使能状态
											if(pd->ucIREn == 0xAA)  // 使能状态
													ucxTxMsgBuf[23] = 1;  // 1表示红外功能开启
											else
													ucxTxMsgBuf[23] = 0;  // 0表示关闭
											
											// 计算CRC校验（从0-24字节）
											ucxTxMsgBuf[24] =  crc16(ucxTxMsgBuf, 24);
											
											// 发送完整响应（25字节）
											UART_SendData(ucxTxMsgBuf, 25);
											
											// 清空接收缓冲区和消息结构
											xRxMsgBuf.ucRxData = 0x00;
											for(i = 0; i < 16; i++) {
													xRxMsgBuf.ucMsgBuf[i] = 0x00;
											}
											break;

									// =============== 设备删除命令（0x02） ===============
									case 0x02://删除命令
											// 构造响应消息
											ucxTxMsgBuf[0] =  0xA3;  // 帧起始符
											ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];  // 流水号
											ucxTxMsgBuf[2] =  ucAddress;  // 设备地址
											ucxTxMsgBuf[3] =  ucCmd;  // 命令码（0x02）
											ucxTxMsgBuf[4] =  7;  // 数据长度（7字节）
											ucxTxMsgBuf[5] =  0x03;  // 响应状态（成功）
											ucxTxMsgBuf[6] =  crc16(ucxTxMsgBuf, 6);  // CRC校验（0-6字节）
											
											// 发送响应
											UART_SendData(ucxTxMsgBuf, 7);
											
											// 清空接收缓冲区和消息结构
											xRxMsgBuf.ucRxData = 0x00;
											for(i = 0; i < 16; i++) {
													xRxMsgBuf.ucMsgBuf[i] = 0x00;
											}
											break;

									// =============== 布撤防命令（0x03） ===============
									case 0x03://撤布防命令
											// 红外功能使能时的处理分支
											if(pd->ucIREn == 0xAA) {
													// 主防区布防/撤防处理
													if((xRxMsgBuf.ucMsgBuf[5] == 0x01) ||  // 布防指令
														 (xRxMsgBuf.ucMsgBuf[5] == 0x02) || 
														 (xRxMsgBuf.ucMsgBuf[5] == 0x03)) {
															if((pd->ucRunningStatus & 0x01) == 0)  // 当前主防区处于待布防状态
																	pd->ucPulseOn |= 0x01;  // 开启主防区脉冲输出
													}
													else if(xRxMsgBuf.ucMsgBuf[5] == 0x00) {  // 撤防指令
															if(pd->ucRunningStatus & 0x01)  // 当前主防区处于已布防状态
																	pd->ucPulseOn &= 0xFE;  // 关闭主防区脉冲输出
													}

													// 红外功能状态设置
													if((xRxMsgBuf.ucMsgBuf[6] == 0x01) ||  // 开启红外指令
														 (xRxMsgBuf.ucMsgBuf[6] == 0x02) || 
														 (xRxMsgBuf.ucMsgBuf[6] == 0x03)) {
															pd->ucIREn = 0xAA;  // 设置红外使能标志
													}
													else if(xRxMsgBuf.ucMsgBuf[6] == 0x00) {  // 关闭红外指令
															pd->ucIREn = 0x55;  // 清除红外使能标志
													}
													
													// 构造响应（9字节）
													ucxTxMsgBuf[0] =  0xA3;
													ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];
													ucxTxMsgBuf[2] =  ucAddress;
													ucxTxMsgBuf[3] =  ucCmd;
													ucxTxMsgBuf[4] =  9;  // 数据长度
													ucxTxMsgBuf[5] =  0x01;  // 操作成功
													ucxTxMsgBuf[6] =  xRxMsgBuf.ucMsgBuf[5];  // 返回主防区状态
													ucxTxMsgBuf[7] =  xRxMsgBuf.ucMsgBuf[6];  // 返回红外状态
													ucxTxMsgBuf[8] =  crc16(ucxTxMsgBuf, 8);  // CRC校验
													UART_SendData(ucxTxMsgBuf, 9);
													// 清空接收缓冲区
													xRxMsgBuf.ucRxData = 0x00;
													for(i = 0; i < 16; i++) {
															xRxMsgBuf.ucMsgBuf[i] = 0x00;
													}
											}
											else {  // 无红外功能的设备处理
													// 仅处理主防区布撤防
													if((xRxMsgBuf.ucMsgBuf[5] == 0x01) || 
														 (xRxMsgBuf.ucMsgBuf[5] == 0x02) || 
														 (xRxMsgBuf.ucMsgBuf[5] == 0x03)) {
															if((pd->ucRunningStatus & 0x01) == 0)
																	pd->ucPulseOn |= 0x01;
													}
													else if(xRxMsgBuf.ucMsgBuf[5] == 0x00) {
															if(pd->ucRunningStatus & 0x01)
																	pd->ucPulseOn &= 0xFE;
													}
													
													// 构造响应（8字节）
													ucxTxMsgBuf[0] =  0xA3;
													ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];
													ucxTxMsgBuf[2] =  ucAddress;
													ucxTxMsgBuf[3] =  ucCmd;
													ucxTxMsgBuf[4] =  8;  // 数据长度
													ucxTxMsgBuf[5] =  0x01;  // 操作成功
													ucxTxMsgBuf[6] =  xRxMsgBuf.ucMsgBuf[5];  // 返回主防区状态
													ucxTxMsgBuf[7] =  crc16(ucxTxMsgBuf, 7);  // CRC校验
													UART_SendData(ucxTxMsgBuf, 8);
													// 清空接收缓冲区
													xRxMsgBuf.ucRxData = 0x00;
													for(i = 0; i < 16; i++) {
															xRxMsgBuf.ucMsgBuf[i] = 0x00;
													}
											}
											// 激活LCD背光（120秒）
											device.ucLcdOnTime = 120;
											lcd_backup_on();
											break;

									// =============== 设备巡检命令（0x10） ===============
									case 0x10://巡检命令
											// 构造响应头
											ucxTxMsgBuf[0] =  0xA3;  // 起始符
											ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];  // 流水号
											ucxTxMsgBuf[2] =  ucAddress;  // 设备地址
											ucxTxMsgBuf[3] =  ucCmd;  // 命令码（0x10）
											ucxTxMsgBuf[4] =  15;  // 数据长度（15字节）
											ucxTxMsgBuf[5] =  0x01;  // 设备类型
											ucxTxMsgBuf[6] =  0x01;  // 单防区标志
											ucxTxMsgBuf[7] =  pd->ucVoltage[0];  // 主防区电压值

											// 主防区状态解析
											if(pd->ucRunningStatus & 0x01) {  // 防区处于激活状态
													data_temp = pd->ucAlarmType & 0x0f;  // 防区故障类型
													if(data_temp == 0x00)  // 正常
															ucxTxMsgBuf[8] = 0x10;
													else if(data_temp == 0x01)  // 开路
															ucxTxMsgBuf[8] = 0x12;
													else if(data_temp == 0x02)  // 短路
															ucxTxMsgBuf[8] = 0x11;
													else  // 其他故障
															ucxTxMsgBuf[8] = 0x13;
											}
											else  // 防区未激活
													ucxTxMsgBuf[8] = 0x00;

											// 设备状态信息
											ucxTxMsgBuf[9] = pd->ucPowerStatus;  // 电源状态
											// 防拆状态
											if(pd->ucAlarmStatus == 0x00) {  // 正常
													ucxTxMsgBuf[10] = 0xF0;
											}
											else if(pd->ucAlarmStatus == 0x01) {  // 防拆触发
													ucxTxMsgBuf[10] = 0xFF;
											}
											// 红外状态
											if(pd->ucIREn == 0xAA)
													ucxTxMsgBuf[11] = 1;  // 红外使能
											else
													ucxTxMsgBuf[11] = 0;  // 红外禁用
											// 报警输出状态
											if(pd->ucAlarmStatus & 0x02)
													ucxTxMsgBuf[12] = 0xAA;  // 报警输出激活
											else
													ucxTxMsgBuf[12] = 0x00;  // 报警输出关闭
											ucxTxMsgBuf[13] = 0x00;  // 预留字段
											
											// 计算CRC并发送
											ucxTxMsgBuf[14] = crc16(ucxTxMsgBuf, 14);
											UART_SendData(ucxTxMsgBuf, 15);
											
											// 清空接收缓冲区
											xRxMsgBuf.ucRxData = 0x00;
											for(i = 0; i < 16; i++) {
													xRxMsgBuf.ucMsgBuf[i] = 0x00;
											}
											break;

									// =============== 参数设置命令（0x20） ===============
									case 0x20://参数设置命令
											// 参数范围检查与处理
											// 1. 蜂鸣器时间设置 (1-50秒)
											if((xRxMsgBuf.ucMsgBuf[5] > 0x01) && (xRxMsgBuf.ucMsgBuf[5] < 0x33)) {
													xEEpromWriteData.ucData[0] = xRxMsgBuf.ucMsgBuf[5];
													pd->ucBeepOnTime = (unsigned int)xRxMsgBuf.ucMsgBuf[5] * 1000;
													pd->ucForceSync = 1;
											} else {
													xEEpromWriteData.ucData[0] = 0x00;  // 使用默认值
											}
											// 2. 输出电压设置 (9-65V)
											if((xRxMsgBuf.ucMsgBuf[6] > 0x08) && (xRxMsgBuf.ucMsgBuf[6] < 0x42)) {
													pd->ucSetVoltage[0] = xRxMsgBuf.ucMsgBuf[6];  // 单防区只设置主通道
													xEEpromWriteData.ucData[1] = xRxMsgBuf.ucMsgBuf[6];
													pd->ucForceSync = 1;
											} else {
													xEEpromWriteData.ucData[1] = 0x00;
											}
											// 3. 输出脉冲个数设置 (1-3个)
											if((xRxMsgBuf.ucMsgBuf[7] >= 0x01) && (xRxMsgBuf.ucMsgBuf[7] < 0x04)) {
													pd->ucAlarmCycle = xRxMsgBuf.ucMsgBuf[7];
													xEEpromWriteData.ucData[2] = xRxMsgBuf.ucMsgBuf[7];
													pd->ucForceSync = 1;
											} else {
													xEEpromWriteData.ucData[2] = 0x00;
											}
											// 4. 集成开关量使能设置
											if((xRxMsgBuf.ucMsgBuf[8] == 0x00) || (xRxMsgBuf.ucMsgBuf[8] == 0x01)) {
													xEEpromWriteData.ucData[3] = (0x55 << xRxMsgBuf.ucMsgBuf[8]);
													pd->ucIREn = (0x55 << xRxMsgBuf.ucMsgBuf[8]);
													pd->ucForceSync = 1;
											} else {
													xEEpromWriteData.ucData[3] = 0x00;
											}
											// 5. 继电器输出时间设置 (1-50秒)
											if((xRxMsgBuf.ucMsgBuf[9] > 0x01) && (xRxMsgBuf.ucMsgBuf[9] < 0x33)) {
													xEEpromWriteData.ucData[4] = xRxMsgBuf.ucMsgBuf[9];
													pd->ucRelayOnTime = (unsigned int)xRxMsgBuf.ucMsgBuf[9] * 1000;
													pd->ucForceSync = 1;
											} else {
													xEEpromWriteData.ucData[4] = 0x00;
											}
											
											// 写入EEPROM队列（异步执行）
											xEEpromWriteData.ucCmd = 0x06;  // EEPROM写操作命令
											osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xFF);
											
											// 构造响应消息
											ucxTxMsgBuf[0] =  0xA3;
											ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];
											ucxTxMsgBuf[2] =  ucAddress;
											ucxTxMsgBuf[3] =  ucCmd;
											ucxTxMsgBuf[4] =  7;  // 响应长度
											ucxTxMsgBuf[5] =  0x01;  // 操作成功
											ucxTxMsgBuf[6] =  crc16(ucxTxMsgBuf, 6);  // CRC校验
											UART_SendData(ucxTxMsgBuf, 7);
											
											// 清空接收缓冲区
											xRxMsgBuf.ucRxData = 0x00;
											for(i = 0; i < 16; i++) {
													xRxMsgBuf.ucMsgBuf[i] = 0x00;
											}
											break;

									// =============== 脉冲设置命令（0x21） ===============
									case 0x21:
											// 脉冲个数设置校验 (1-3个)
											if((xRxMsgBuf.ucMsgBuf[5] >= 0x01) && (xRxMsgBuf.ucMsgBuf[5] < 0x04)) {
													pd->ucAlarmCycle = xRxMsgBuf.ucMsgBuf[5];
													pd->ucForceSync = 1;
													xEEpromWriteData.ucData[2] = xRxMsgBuf.ucMsgBuf[5];
											} else {
													xEEpromWriteData.ucData[2] = 0x00;
											}
											
											// 准备EEPROM写数据（其他字段保持默认）
											xEEpromWriteData.ucCmd = 0x06;
											xEEpromWriteData.ucData[0] = 0x00;
											xEEpromWriteData.ucData[1] = 0x00;
											xEEpromWriteData.ucData[3] = 0x00;
											xEEpromWriteData.ucData[4] = 0x00;
											osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xFF);
											
											// 构造响应
											ucxTxMsgBuf[0] =  0xA3;
											ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];
											ucxTxMsgBuf[2] =  ucAddress;
											ucxTxMsgBuf[3] =  ucCmd;
											ucxTxMsgBuf[4] =  0x07;  // 数据长度
											ucxTxMsgBuf[5] =  0x01;  // 操作成功
											ucxTxMsgBuf[6] =  crc16(ucxTxMsgBuf, 6);  // CRC
											UART_SendData(ucxTxMsgBuf, 7);
											
											// 清空接收缓冲区
											xRxMsgBuf.ucRxData = 0x00;
											for(i = 0; i < 16; i++) {
													xRxMsgBuf.ucMsgBuf[i] = 0x00;
											}
											break;
									
									// =============== 测试命令（0x22） ===============
									case 0x22:
											// 构造标准化响应
											ucxTxMsgBuf[0] =  0xA3;
											ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];
											ucxTxMsgBuf[2] =  ucAddress;
											ucxTxMsgBuf[3] =  ucCmd;
											ucxTxMsgBuf[4] =  7;  // 数据长度
											ucxTxMsgBuf[5] =  0x03;  // 响应码
											ucxTxMsgBuf[6] =  crc16(ucxTxMsgBuf, 6);  // CRC
											UART_SendData(ucxTxMsgBuf, 7);
											
											// 清空接收缓冲区
											xRxMsgBuf.ucRxData = 0x00;
											for(i = 0; i < 16; i++) {
													xRxMsgBuf.ucMsgBuf[i] = 0x00;
											}
											break;

									// =============== 设置RTC时间（0xDA） ===============
									case 0xDA:
											// 从消息中提取时间参数
											sDate.Year = xRxMsgBuf.ucMsgBuf[4];  // 年
											sDate.Month = xRxMsgBuf.ucMsgBuf[5];  // 月
											sDate.Date = xRxMsgBuf.ucMsgBuf[6];  // 日
											sTime.Hours = xRxMsgBuf.ucMsgBuf[7];  // 时
											sTime.Minutes = xRxMsgBuf.ucMsgBuf[8];  // 分
											sTime.Seconds = xRxMsgBuf.ucMsgBuf[9];  // 秒
											
											// 更新硬件RTC
											HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
											HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
											
											// 构造响应（返回设置的时间）
											ucxTxMsgBuf[0] =  0x06;  // 命令类型
											ucxTxMsgBuf[1] = sDate.Year;  // 年
											ucxTxMsgBuf[2] = sDate.Month;  // 月
											ucxTxMsgBuf[3] = sDate.Date;  // 日
											ucxTxMsgBuf[4] = sTime.Hours;  // 时
											ucxTxMsgBuf[5] = sTime.Minutes;  // 分
											ucxTxMsgBuf[6] = sTime.Seconds;  // 秒
											UART_SendData(ucxTxMsgBuf, 7);
											
											// 清空接收缓冲区
											xRxMsgBuf.ucRxData = 0x00;
											for(i = 0; i < 16; i++) {
													xRxMsgBuf.ucMsgBuf[i] = 0x00;
											}
											break;

									// =============== 读取RTC时间（0xDB） ===============
									case 0xDB:
											// 从硬件RTC获取当前时间
											HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
											HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
											
											// 构造时间响应
											ucxTxMsgBuf[0] =  0x06;  // 命令类型
											ucxTxMsgBuf[1] = sDate.Year;  // 年
											ucxTxMsgBuf[2] = sDate.Month;  // 月
											ucxTxMsgBuf[3] = sDate.Date;  // 日
											ucxTxMsgBuf[4] = sTime.Hours;  // 时
											ucxTxMsgBuf[5] = sTime.Minutes;  // 分
											ucxTxMsgBuf[6] = sTime.Seconds;  // 秒
											UART_SendData(ucxTxMsgBuf, 7);
											break;

									// =============== 其他命令默认处理 ===============
									default:
											// 构造错误响应
											ucxTxMsgBuf[0] =  0xA3;  // 起始符
											ucxTxMsgBuf[1] =  xRxMsgBuf.ucMsgBuf[2];  // 流水号
											ucxTxMsgBuf[2] =  ucAddress;  // 设备地址
											ucxTxMsgBuf[3] =  ucCmd;  // 原始命令码
											ucxTxMsgBuf[4] =  7;  // 数据长度
											ucxTxMsgBuf[5] =  0x03;  // 未知命令响应码
											ucxTxMsgBuf[6] =  crc16(ucxTxMsgBuf, 6);  // CRC
											UART_SendData(ucxTxMsgBuf, 7);
											
											// 清空接收缓冲区
											xRxMsgBuf.ucRxData = 0x00;
											for(i = 0; i < 16; i++) {
													xRxMsgBuf.ucMsgBuf[i] = 0x00;
											}
											break;
                    }
                }
            }
            Uart4ItOnOffFlag = 0;
            // 重新使能UART中断接收
            HAL_UART_Receive_IT(&huart4, (uint8_t*)&uPort.ucRxData, 1);
        }  // end of if(osMessageQueueGet...)
        // 任务延时5ms（控制任务执行频率）
        osDelay(5);
    }  // end of while(1)
}  // end of vUART_TASK
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
