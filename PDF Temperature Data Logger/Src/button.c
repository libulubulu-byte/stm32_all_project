
#include"button.h"


#define BUTTONn                                1
#define STOP_BUTTONn                           1
#define USBONn                                 1

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY_BUTTON_GPIO_PORT};  //开机按键
const uint16_t BUTTON_PIN[BUTTONn] = {KEY_BUTTON_PIN};
const uint8_t BUTTON_IRQn[BUTTONn] = {KEY_BUTTON_EXTI_IRQn};


GPIO_TypeDef* STOP_BUTTON_PORT[STOP_BUTTONn] = {KEY_STOP_BUTTON_GPIO_PORT};  //关机按键
const uint16_t STOP_BUTTON_PIN[STOP_BUTTONn] = {KEY_STOP_BUTTON_PIN};
const uint8_t STOP_BUTTON_IRQn[STOP_BUTTONn] = {KEY_STOP_BUTTON_EXTI_IRQn};



GPIO_TypeDef*  USB_POWER_ON_PORT[USBONn] = {USB_POWERON_GPIO_PORT};   //USB 上电
const uint16_t USB_POWER_ON_PIN[USBONn]  = {USB_POWERON_PIN};
const uint8_t  USB_POWER_ON_IRQn[USBONn] = {USB_POWERON_EXTI_IRQn};


/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_KEY
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability
  * @retval None
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON Clock */
  __GPIOC_CLK_ENABLE();
  
//  if (ButtonMode == BUTTON_MODE_GPIO)  //Button will be used as simple IO
//  {
//    /* Configure Button pin as input */
//    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
//  }
//  
//  if (ButtonMode == BUTTON_MODE_EXTI)        //Button will be connected to EXTI line with interrupt generation capability
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
   // GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;             //// GPIO_MODE_IT_RISING 上升沿
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 3, 0);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}
void BSP_PB_DISable(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON Clock */
  __GPIOC_CLK_ENABLE();
  
//  if (ButtonMode == BUTTON_MODE_GPIO)  //Button will be used as simple IO
//  {
//    /* Configure Button pin as input */
//    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
//  }
  
//  if (ButtonMode == BUTTON_MODE_EXTI)        //Button will be connected to EXTI line with interrupt generation capability
//  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
   // GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;             //// GPIO_MODE_IT_RISING 上升沿
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 3, 0);
    HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
//  }
}

void BSP_STOP_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON Clock */
  __GPIOC_CLK_ENABLE();
  
//  if (ButtonMode == BUTTON_MODE_GPIO)  //Button will be used as simple IO
//  {
//    /* Configure Button pin as input */
//    GPIO_InitStruct.Pin = STOP_BUTTON_PIN[Button];
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//    HAL_GPIO_Init(STOP_BUTTON_PORT[Button], &GPIO_InitStruct);
//  }
  
//  if (ButtonMode == BUTTON_MODE_EXTI)        //Button will be connected to EXTI line with interrupt generation capability
//  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = STOP_BUTTON_PIN[Button];
   // GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;             //// GPIO_MODE_IT_RISING 上升沿
    HAL_GPIO_Init(STOP_BUTTON_PORT[Button], &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(STOP_BUTTON_IRQn[Button]),1, 0);
    HAL_NVIC_EnableIRQ((IRQn_Type)(STOP_BUTTON_IRQn[Button]));
//  }
}
void BSP_STOP_DISable(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON Clock */
  __GPIOC_CLK_ENABLE();
  
//  if (ButtonMode == BUTTON_MODE_GPIO)  //Button will be used as simple IO
//  {
//    /* Configure Button pin as input */
//    GPIO_InitStruct.Pin = STOP_BUTTON_PIN[Button];
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//    HAL_GPIO_Init(STOP_BUTTON_PORT[Button], &GPIO_InitStruct);
//  }
//  
//  if (ButtonMode == BUTTON_MODE_EXTI)        //Button will be connected to EXTI line with interrupt generation capability
//  {
//    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = STOP_BUTTON_PIN[Button];
   // GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;             //// GPIO_MODE_IT_RISING 上升沿
    HAL_GPIO_Init(STOP_BUTTON_PORT[Button], &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(STOP_BUTTON_IRQn[Button]),1, 0);
    HAL_NVIC_DisableIRQ((IRQn_Type)(STOP_BUTTON_IRQn[Button]));
//  }
}
void BSP_USB_POWERON_DISable(USBPWON_TypeDef Button, USBPWONMode_TypeDef USB_PWONMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON Clock */
  USB_POWERON_GPIO_CLK_ENABLE();
//  
//  if (USB_PWONMode == USBPWON_MODE_GPIO)  //
//  {
//    /* Configure Button pin as input */
//    GPIO_InitStruct.Pin = USB_POWERON_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//		//GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//  }
  
//  if (USB_PWONMode == USBPWON_MODE_EXTI)
//  {
//    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = USB_POWERON_PIN;
   // GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Pull = GPIO_NOPULL;

   // GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;             //// GPIO_MODE_IT_RISING 上升沿
		
		GPIO_InitStruct.Mode =GPIO_MODE_ANALOG;             //// GPIO_MODE_IT_RISING 上升沿
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(USB_POWER_ON_IRQn[Button]), 2, 0);
    HAL_NVIC_DisableIRQ((IRQn_Type)(USB_POWER_ON_IRQn[Button]));
//	
//  }
}

void BSP_USB_POWERON_Init(USBPWON_TypeDef Button, USBPWONMode_TypeDef USB_PWONMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON Clock */
  USB_POWERON_GPIO_CLK_ENABLE();
  
  if (USB_PWONMode == USBPWON_MODE_GPIO)  //
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = USB_POWERON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		//GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  
  if (USB_PWONMode == USBPWON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = USB_POWERON_PIN;
   // GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;

   // GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;             //// GPIO_MODE_IT_RISING 上升沿
		
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;             //// GPIO_MODE_IT_RISING 上升沿
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(USB_POWER_ON_IRQn[Button]), 2, 0);
    HAL_NVIC_EnableIRQ((IRQn_Type)(USB_POWER_ON_IRQn[Button]));
	
  }
}


/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_KEY
  * @retval The Button GPIO pin value.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_KEY
  * @retval The Button GPIO pin value.
  */
uint32_t BSP_STOP_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(STOP_BUTTON_PORT[Button], STOP_BUTTON_PIN[Button]);
}

/**
  * @brief  Configures EXTI Line13 (connected to PC13 pin) in interrupt mode.
  * @param  None
  * @retval None
  */
void EXTILine2_3_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __GPIOA_CLK_ENABLE();
  
  /* Configure PA3 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_3;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable and set EXTI4_15 Interrupt to the lowest priority */

	  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
}
/**
  * @brief EXTI line detection callback.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == KEY_BUTTON_PIN)
  {
    /* Toggle LED2 */
  // LED2_On();
  }
}
