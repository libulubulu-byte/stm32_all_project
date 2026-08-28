#include "pwr.h"

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
//#include "stm32l0538_discovery.h"
#include"stm32l0xx_hal_pwr.h"
#include"stm32l0xx_hal_pwr_ex.h"
#include "stm32l0xx_hal_rtc.h"
#include <stdio.h>
#include "stm32l0xx_it.h"

//uint8_t YMD_RTC_Y=0,YMD_RTC_M=0,YMD_RTC_D=0,HMS_RTC_H=0,HMS_RTC_M=0,HMS_RTC_S=0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */

// RTC_HandleTypeDef RTCHandle;
//RTC_DateTypeDef sdatestructureget;
//RTC_TimeTypeDef stimestructureget;
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
 void RTC_CalendarConfig(void)
{
 // RTC_HandleTypeDef RTCHandle;
//	RTC_DateTypeDef sdatestructure;
//  RTC_TimeTypeDef stimestructure;

//	
//  /*##-1- Configure the Date #################################################*/
//  /* Set Date: Monday May 30th 2014 */
//  sdatestructure.Year = 0x15;
//  sdatestructure.Month = RTC_MONTH_SEPTEMBER;
//  sdatestructure.Date = 0x07;
//  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
//  
//  if(HAL_RTC_SetDate(&RTCHandle,&sdatestructure,FORMAT_BCD) != HAL_OK)
//  {
//    /* Error */
//    Error_Handler(); 
//  } 
//  
//  /*##-2- Configure the Time #################################################*/
//  /* Set Time: 02:00:00 */
//  stimestructure.Hours = 0x09;
//  stimestructure.Minutes = 0x45;
//  stimestructure.Seconds = 0x01;
//  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
//	//stimestructure.TimeFormat = RTC_HOURFORMAT24;
//  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
//  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
//  
//  if(HAL_RTC_SetTime(&RTCHandle,&stimestructure,FORMAT_BCD) != HAL_OK)
//  {
//    /* Error */
//    Error_Handler(); 
//  }
//  
//  /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
//  HAL_RTCEx_BKUPWrite(&RTCHandle,RTC_BKP_DR0,0x32F2);  
}
/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
//void RTC_CalendarShow(uint8_t* showtime, uint8_t* showdate)
//{
//	RTC_HandleTypeDef RTCHandle;
//	RTC_DateTypeDef sdatestructureget;
//  RTC_TimeTypeDef stimestructureget;
//  
//  /* Get the RTC current Time */
//  HAL_RTC_GetTime(&RTCHandle, &stimestructureget, FORMAT_BIN);
//  /* Get the RTC current Date */
//  HAL_RTC_GetDate(&RTCHandle, &sdatestructureget, FORMAT_BIN);
//  
//#ifdef DISPLAY_ON_DUBUGGER
//  /* Display time Format : hh:mm:ss */
//  sprintf((char*)showtime,"%02d:%02d:%02d",stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
//  /* Display date Format : mm-dd-yy */
//  sprintf((char*)showdate,"%02d-%02d-%02d",sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
//#endif
//}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
//void Error_Handler(void)
//{
//  while(1)
//  {
//    /* Turn LED3 on */
//  //  BSP_LED_Toggle(LED3);
//    /* Add a 100ms Delay */
//    HAL_Delay(100);
//  }
//}
/**
  * @brief  System Power Configuration
  *         The system Power is configured as follow : 
  *            + Regulator in LP mode
  *            + VREFINT OFF, with fast wakeup enabled
  *            + HSI as SysClk after Wake Up
  *            + No IWDG
  *            + Automatic Wakeup using RTC clocked by LSI (after ~4s)
  * @param  None
  * @retval None
  */
  void SystemPower_Config(void)
{
	//	GPIO_InitTypeDef GPIO_InitStructure;
//	

//  /* Enable Ultra low power mode */
//  HAL_PWREx_EnableUltraLowPower();
//  
//  /* Enable the fast wake up from Ultra low power mode */
//  HAL_PWREx_EnableFastWakeUp();

//  /* Select HSI as system clock source after Wake Up from Stop mode */
//  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_StopWakeUpClock_HSI);
//	
//  
//  /* Enable GPIOs clock */
//  __GPIOA_CLK_ENABLE();
//  __GPIOB_CLK_ENABLE();
//  __GPIOC_CLK_ENABLE();
//  __GPIOD_CLK_ENABLE();
////  __GPIOH_CLK_ENABLE();

//  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
//  GPIO_InitStructure.Pin = GPIO_PIN_All;
//  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStructure.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
//  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

//  /* Disable GPIOs clock */
//  __GPIOA_CLK_DISABLE();
//  __GPIOB_CLK_DISABLE();
//  __GPIOC_CLK_DISABLE();
//  __GPIOD_CLK_DISABLE();
//  RTC_HandleTypeDef RTCHandle;

	GPIO_InitTypeDef GPIO_InitStructure;
	

  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPower();
  
  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableFastWakeUp();

  /* Select HSI as system clock source after Wake Up from Stop mode */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_StopWakeUpClock_HSI);
	
  
  /* Enable GPIOs clock */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
	
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
	
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
/************************************************/
/**********************¶àÓàµÄB¿Ú**********/
  GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Disable GPIOs clock */
  __GPIOA_CLK_DISABLE();
  __GPIOB_CLK_DISABLE();
  __GPIOC_CLK_DISABLE();
  __GPIOD_CLK_DISABLE();
  __GPIOH_CLK_DISABLE();
	

}
/**
  * @brief  Configures system clock after wake-up from STOP: enable HSI, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void SystemClockConfig_STOP(void)
{
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//  RCC_OscInitTypeDef RCC_OscInitStruct;
//  
//  /* Enable Power Control clock */
//  __PWR_CLK_ENABLE();

//  /* The voltage scaling allows optimizing the power consumption when the device is 
//     clocked below the maximum system frequency, to update the voltage scaling value 
//     regarding system frequency refer to product datasheet.  */
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

//  /* Get the Oscillators configuration according to the internal RCC registers */
//  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

//  /* After wake-up from STOP reconfigure the system clock: Enable HSI and PLL */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
//  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
//  RCC_OscInitStruct.HSICalibrationValue = 0x10;
//  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
////    Error_Handler();
//  }

//  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
//     clocks dividers */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
//  {
////    Error_Handler();
//  }
//	  /* Enable GPIOs clock */
//	
//  __GPIOA_CLK_ENABLE();
//  __GPIOB_CLK_ENABLE();
//  __GPIOC_CLK_ENABLE();
//  __GPIOD_CLK_ENABLE();
//  __GPIOH_CLK_ENABLE();
}
/**
  * @brief  RTC Wake Up callback
  * @param  None
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Clear Wake Up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
