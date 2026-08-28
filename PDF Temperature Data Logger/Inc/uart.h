#ifndef __UART_H
#define __UART_H

#include "stm32l0xx_hal.h"
#include <stdio.h>

#define USART1_CLK_ENABLE()              __USART1_CLK_ENABLE();
#define USART1_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE() 

#define USART1_FORCE_RESET()             __USART1_FORCE_RESET()
#define USART1_RELEASE_RESET()           __USART1_RELEASE_RESET()

/* Definition for USART1 Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA  
#define USART1_TX_AF                     GPIO_AF4_USART1
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA 
#define USART1_RX_AF                     GPIO_AF4_USART1

#define USART2_CLK_ENABLE()              __USART2_CLK_ENABLE();
#define USART2_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE() 

#define USART2_FORCE_RESET()             __USART2_FORCE_RESET()
#define USART2_RELEASE_RESET()           __USART2_RELEASE_RESET()

/* Definition for USART2 Pins */
#define USART2_TX_PIN                    GPIO_PIN_2
#define USART2_TX_GPIO_PORT              GPIOA  
#define USART2_TX_AF                     GPIO_AF4_USART2
#define USART2_RX_PIN                    GPIO_PIN_3
#define USART2_RX_GPIO_PORT              GPIOA 
#define USART2_RX_AF                     GPIO_AF4_USART2

#define LPUART1_CLK_ENABLE()              __LPUART1_CLK_ENABLE();
#define LPUART1_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define LPUART1_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE() 

#define LPUART1_FORCE_RESET()             __LPUART1_FORCE_RESET()
#define LPUART1_RELEASE_RESET()           __LPUART1_RELEASE_RESET()

/* Definition for LPUART1 Pins */
#define LPUART1_TX_PIN                    GPIO_PIN_4
#define LPUART1_TX_GPIO_PORT              GPIOC  
#define LPUART1_TX_AF                     GPIO_AF2_LPUART1
#define LPUART1_RX_PIN                    GPIO_PIN_5
#define LPUART1_RX_GPIO_PORT              GPIOC 
#define LPUART1_RX_AF                     GPIO_AF2_LPUART1

extern UART_HandleTypeDef Uart1Handle;
//extern UART_HandleTypeDef Uart2Handle;
//extern UART_HandleTypeDef Lpuart1Handle;

void USART1_Init(void);
void UART1_SendByte(uint8_t data);
void UART1_SendString(char * str);

void USART2_Init(void);
void UART2_SendByte(uint8_t data);
void UART2_SendString(char * str);

void LPUART1_Init(void);
void LPUART1_SendByte(uint8_t data);
void LPUART1_SendString(char * str);

#endif
