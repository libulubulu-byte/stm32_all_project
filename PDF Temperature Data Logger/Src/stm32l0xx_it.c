/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "stm32l0xx_it.h"
#include "button.h"
#include "uart.h"

/* USER CODE BEGIN 0 */
uint16_t Wake_up_10s=0,wake_light_10s=0;
uint8_t  Button_Flag=0;
uint8_t  Usb_Power_On_Flag=0;
uint8_t  Rev_Flag=0,Rece_count=0,i=0,Comm_OK_flag=0,Write_set_flag=0,Write_barcode_flag=0;
uint8_t Rec_data[18],Send_data[18]; 	
uint8_t flag = 0,flag_delay=0,flag_delay_count=0;

uint8_t	Key_stop_Flag=0;

void USB_GPIO_Init(void);
void USB_Clock_Config(void);  //USB时钟重新设置

uint8_t get_crc(uint8_t *pdata, uint8_t num);
uint8_t set_crc(uint8_t *pdata, uint8_t num);
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
// extern I2C_HandleTypeDef hi2c2;
 extern RTC_HandleTypeDef RtcHandle;//用于RTC处理的结构体
/******************************************************************************/
/*            Cortex-M0+ Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles I2C2 interrupt.
*/
void I2C2_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_IRQn 0 */

  /* USER CODE END I2C2_IRQn 0 */
//  if (hi2c2.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
//    HAL_I2C_ER_IRQHandler(&hi2c2);
//  } else {
//    HAL_I2C_EV_IRQHandler(&hi2c2);
//  }
  /* USER CODE BEGIN I2C2_IRQn 1 */

  /* USER CODE END I2C2_IRQn 1 */
}

/**
* @brief This function handles USB event interrupt / USB wake-up interrupt through EXTI line 18.
*/
void USB_IRQHandler(void)
{
  /* USER CODE BEGIN USB_IRQn 0 */

  /* USER CODE END USB_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
  /* USER CODE BEGIN USB_IRQn 1 */

  /* USER CODE END USB_IRQn 1 */
}

//extern uint16_t	Bat_Day;
void RTC_IRQHandler(void)
{
  HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
Wake_up_10s++;                              //以10秒唤醒为单位，进行累计。
//	wake_light_10s++;
}

/* USER CODE BEGIN 1 */
/**
  * @brief  This function handles External lines 2 to 3 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)   
{
  HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_EXTI_LINE);
	Button_Flag=1;
}

/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   
  */

void LPUART1_IRQHandler(void)
{
     uint8_t Data_temp=0;
	
	if((LPUART1->ISR&UART_FLAG_ORE) != 0)
    { 
        LPUART1->ICR = UART_CLEAR_OREF;
		}
        /* do something */
        	Data_temp=LPUART1->RDR;
			Rec_data[Rece_count++]=Data_temp;
			if(Rec_data[0]==0xAA)
			{
				switch(Rec_data[1])
				{
					case 9: 
						    if(Rec_data[5]==0x55)        //建立通讯连接命令码	
									{
										if(Rec_data[4]==get_crc(Rec_data,6))
											{
												Send_data[0]=Rec_data[0];
												Send_data[1]=Rec_data[1];
												Send_data[2]=Rec_data[2]+0x01;
												Send_data[3]=Rec_data[3];
											  Send_data[4]=set_crc(Send_data, 6);
												Send_data[5]=Rec_data[5];
												Comm_OK_flag=1;
												
  										}
								 }
						break;
					case 10:
							if(Rec_data[13]==0x55)					//写入条形码信息 命令码	
								{
										if(Rec_data[12]==get_crc(Rec_data,14))
											{
												for(i=0;i<12;i++)
													{
													 if(i==10)
														Send_data[i]=Rec_data[i]+0x01;
														else
														Send_data[i]=Rec_data[i];
													}
												Send_data[12]=set_crc(Send_data, 14);
												Send_data[13]=Rec_data[13];
												Write_barcode_flag=1;
  										}
									
								}
						break;
					case 11:
							if(Rec_data[17]==0x55)					//写入设定信息 命令码	
								{
									if(Rec_data[16]==get_crc(Rec_data,18))
											{
												for(i=0;i<16;i++)
													{
													 if(i==14)
														Send_data[i]=Rec_data[i]+0x01;
													 else
														Send_data[i]=Rec_data[i];
													}
												Send_data[16]=set_crc(Send_data, 18);
												Send_data[17]=Rec_data[17];
												Write_set_flag=1;
  										}
								}
						break;
					default:
						
						break;

		
		 }
    }
}



/**
  * @brief  This function handles External lines 4 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
uint8_t K_station_start=0,K_station_stop=0,K_station_usb=0;
	
	K_station_start=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);
	K_station_stop=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);
	K_station_usb=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9);
	if(K_station_start==0)
	{
	  HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_EXTI_LINE);
	  Button_Flag=1;
	}
	if(K_station_stop==0)
	{
	  HAL_GPIO_EXTI_IRQHandler(KEY_STOP_BUTTON_EXTI_LINE);
		Key_stop_Flag=1;
	}
  if(K_station_usb==1)
	{
		HAL_GPIO_EXTI_IRQHandler(USB_POWERON_EXTI_LINE);
		Usb_Power_On_Flag=1;
	}
}

/*****************??????? ************************/
//void set_crc(uint8_t *pdata, uint8_t num)
uint8_t set_crc(uint8_t *pdata, uint8_t num)
{
    uint8_t i = 0, sum = 0;
    for(i = 0; i < num -2; i++)
        sum += *(pdata+i);
   // *(pdata+num-1) = sum;
	return  sum;
}
/*****************??????? ************************/
uint8_t get_crc(uint8_t *pdata, uint8_t num)
{
    uint8_t i = 0, sum = 0;
    for(i = 0; i < num -2; i++)
        sum += *(pdata+i);
    return sum;
}

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
