#ifndef __PWER_H
#define __PWER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
//#include "stm32l0538_discovery.h"
#include"stm32l0xx_hal_pwr.h"
#include"stm32l0xx_hal_pwr_ex.h"
#include "stm32l0xx_hal_rtc.h"
#include "stm32l0xx_hal_rtc_ex.h"

#include <stdio.h>

//extern uint8_t YMD_RTC_Y,YMD_RTC_M,YMD_RTC_D,HMS_RTC_H,HMS_RTC_M,HMS_RTC_S;
//extern RTC_HandleTypeDef RTCHandle;

//extern RTC_DateTypeDef sdatestructureget;
//extern RTC_TimeTypeDef stimestructureget;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment to enable the adaquate RTC Clock Source */
// #define RTC_CLOCK_SOURCE_LSI 
#define RTC_CLOCK_SOURCE_LSE

/* Comment to disable displaying the time on the debugger */
#define DISPLAY_ON_DUBUGGER
        
#ifdef RTC_CLOCK_SOURCE_LSI
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x00FF
#endif
void RTC_CalendarConfig(void);
void RTC_CalendarShow(uint8_t* showtime, uint8_t* showdate);
void Error_Handler(void);
void SystemPower_Config(void);
void SystemClockConfig_STOP(void);
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc);
void HAL_SYSTICK_Callback(void);
void RTC_Config(void);
void LSE_ClockEnable(void);

#endif
