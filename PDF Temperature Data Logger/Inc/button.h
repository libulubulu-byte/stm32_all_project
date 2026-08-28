#ifndef __BUTTON_H
#define __BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

#define KEY_BUTTON_PIN                          GPIO_PIN_8                       //开机按键
#define KEY_BUTTON_GPIO_PORT                    GPIOC
#define KEY_BUTTON_GPIO_CLK_ENABLE()            __GPIOC_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()           __GPIOC_CLK_DISABLE()
#define KEY_BUTTON_EXTI_LINE                    GPIO_PIN_8
#define KEY_BUTTON_EXTI_IRQn                    EXTI4_15_IRQn
#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)      (KEY_BUTTON_GPIO_CLK_ENABLE())
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)     (KEY_BUTTON_GPIO_CLK_DISABLE())

#define KEY_STOP_BUTTON_PIN                          GPIO_PIN_7                       //关机按键
#define KEY_STOP_BUTTON_GPIO_PORT                    GPIOC
#define KEY_STOP_BUTTON_GPIO_CLK_ENABLE()            __GPIOC_CLK_ENABLE()
#define KEY_STOP_BUTTON_GPIO_CLK_DISABLE()           __GPIOC_CLK_DISABLE()
#define KEY_STOP_BUTTON_EXTI_LINE                    GPIO_PIN_7
#define KEY_STOP_BUTTON_EXTI_IRQn                    EXTI4_15_IRQn




#define USB_POWERON_PIN                          GPIO_PIN_9                         //USB上电检测按键
#define USB_POWERON_GPIO_PORT                    GPIOC
#define USB_POWERON_GPIO_CLK_ENABLE()            __GPIOC_CLK_ENABLE()
#define USB_POWERON_GPIO_CLK_DISABLE()           __GPIOC_CLK_DISABLE()
#define USB_POWERON_EXTI_LINE                    GPIO_PIN_9
#define USB_POWERON_EXTI_IRQn                    EXTI4_15_IRQn
#define USB_PWON_GPIO_CLK_ENABLE(__INDEX__)      (USB_POWERON_GPIO_CLK_ENABLE())
#define USB_PWON_GPIO_CLK_DISABLE(__INDEX__)     (USB_POWERON_GPIO_CLK_DISABLE())

/***********************/             // 开机按键
typedef enum
{
  BUTTON_KEY = 0
} Button_TypeDef;

typedef enum        //按键状态
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;
/*******************************/
/*************************/          //关机按键   
typedef enum
{
  STOP_BUTTON_KEY = 0
} STOP_Button_TypeDef;

typedef enum                  
{
  STOP_BUTTON_MODE_GPIO = 0,
  STOP_BUTTON_MODE_EXTI = 1
} STOP_ButtonMode_TypeDef;


/********************************/

/***********************************/ // 上电检测
typedef enum
{
  USB_PWON= 0          //上电之前状态为0
} USBPWON_TypeDef;

typedef enum
{
  USBPWON_MODE_GPIO = 0,  // USB 上电状态为1
  USBPWON_MODE_EXTI = 1   // USB 掉电状态为0
} USBPWONMode_TypeDef;
/************************************/
/********************开机按键函数*****************/
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void BSP_PB_DISable(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
uint32_t BSP_PB_GetState(Button_TypeDef Button);
void EXTILine0_1_Config(void);
void EXTILine2_3_Config(void);
/*******************USB上电检测******************/
void BSP_USB_POWERON_Init(USBPWON_TypeDef Button, USBPWONMode_TypeDef USB_PWONMode);

/********************关机按键函数*****************/
void BSP_STOP_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void BSP_STOP_DISable(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
uint32_t BSP_STOP_GetState(Button_TypeDef Button);
void BSP_USB_POWERON_DISable(USBPWON_TypeDef Button, USBPWONMode_TypeDef USB_PWONMode);

#endif 
