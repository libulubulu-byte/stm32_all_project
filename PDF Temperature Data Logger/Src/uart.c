/*******************************************************************************
** 版权:		    利尔达科技有限公司
** 文件名: 		  uart.c
** 版本：  	    V1.00   
** 工作环境: 	  RealView MDK-ARM V5.11
** 作者: 		    ydc
** 功能:		    UART驱动						
** 日期: 	      2014-08-12
** 修改日志：	  2014-08-20
                  1、增加串口时钟源配置代码
                  2、增加对C标准库打印函数的支持，当前配置为USART2
                2014-08-25    
                  1、串口RX引脚配置增加内部上拉电阻
                2014-12-04
                  1、整理、添加注释
                2014-01-15
                  1、整理、添加注释
** 版权所有     (C)2014-2015 利尔达科技有限公司
*******************************************************************************/

#include "uart.h"
//#include "rcc.h"
#include "stm32l0xx_hal_uart_ex.h"

//UART_HandleTypeDef Uart1Handle;//用于UART1处理的结构体
//UART_HandleTypeDef Uart2Handle;//用于UART2处理的结构体
//UART_HandleTypeDef LpUart1Handle;//用于LPUART处理的结构体

/*******************************************************************************
以下内容为printf函数重定向至串口的代码，重定向至USART2,可修改
*******************************************************************************/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  while((LPUART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//等待发送寄存器为空
  LPUART1_SendByte(ch);
  return ch;
}
/*******************************************************************************
以上内容为printf函数重定向至串口的代码
*******************************************************************************/






void LPUART1_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* 使能LSE时钟 */  
 // LSE_ClockEnable();
	
	  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* 选择USART时钟源 */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  RCC_PeriphCLKInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;//USART时钟源配置选择为LSE
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);    

  /* Enable GPIO TX/RX clock */
  LPUART1_TX_GPIO_CLK_ENABLE();
  LPUART1_RX_GPIO_CLK_ENABLE();

  /* Enable LPUART1 clock */
  LPUART1_CLK_ENABLE(); 

//  __HAL_UART_DISABLE(&LpUart1Handle);//失能USART

  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUART1_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = LPUART1_TX_AF;
  HAL_GPIO_Init(LPUART1_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUART1_RX_PIN;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = LPUART1_RX_AF;
  HAL_GPIO_Init(LPUART1_RX_GPIO_PORT, &GPIO_InitStruct);

//  LpUart1Handle.Instance        = LPUART1;
//  LpUart1Handle.Init.BaudRate   = 9600;
//  LpUart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
//  LpUart1Handle.Init.StopBits   = UART_STOPBITS_1;
//  LpUart1Handle.Init.Parity     = UART_PARITY_NONE;
//  LpUart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
//  LpUart1Handle.Init.Mode       = UART_MODE_TX_RX;

//  if(HAL_UART_Init(&LpUart1Handle) != HAL_OK)
//  {
//      
//  }
//  __HAL_UART_DISABLE(&LpUart1Handle);
//  LPUART1->RQR = 1<<3;//clears the RXNE flag
//  LPUART1->CR1 |= 1<<5;//enable the RXNE interrupt
//  HAL_NVIC_SetPriority(LPUART1_IRQn, 3, 0);//设置USART中断优先级:抢占优先级3；子优先级0（M0+内核无子优先级）
//  HAL_NVIC_EnableIRQ(LPUART1_IRQn);//使能USART中断
//  __HAL_UART_ENABLE(&LpUart1Handle);//使能USART

}

/*******************************************************************************
** 功能	      LPUSART1发送一字节数据
** 参数       data：将要发送的字节
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void LPUART1_SendByte(uint8_t data)
{
  while((LPUART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//等待发送寄存器为空
  LPUART1->TDR = data;
}

/*******************************************************************************
** 功能	      LPUSART1发送字符串
** 参数       str：将要发送的字符串
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void LPUART1_SendString(char * str)
{
  while((*str)!=0)
  {
    while((LPUART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//等待发送寄存器为空
    LPUART1->TDR = *str++;       
  }
}



