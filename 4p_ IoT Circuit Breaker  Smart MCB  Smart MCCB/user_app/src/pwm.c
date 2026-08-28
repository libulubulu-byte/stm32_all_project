#include "main.h"
#include "pwm.h"
#include "stm32f0xx_hal_flash.h"

extern uint8_t Duty_ratio;
extern TIM_HandleTypeDef htim15;
void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */
  TIM_OC_InitTypeDef sConfigOC = {0};
  
  
  htim15.Instance = TIM15;

  htim15.Init.Prescaler         = 0;
  htim15.Init.Period            = PERIOD_VALUE;
  htim15.Init.ClockDivision     = 0;
  htim15.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim15) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channels #########################################*/
  /* Common configuration for all channels */
  sConfigOC.OCMode       = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;

  /* Set the pulse value for channel 2 */
  sConfigOC.Pulse = PULSE2_VALUE;
  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }


  /* Start channel 2 */
//  if (HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2) != HAL_OK)
//  {
    /* PWM Generation Error */
//    Error_Handler();
//  }

  /* USER CODE END TIM15_Init 0 */



  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
}
/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM15_Init2(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */
  TIM_OC_InitTypeDef sConfigOC = {0};
  
  
  htim15.Instance = TIM15;

  htim15.Init.Prescaler         = 0;
  htim15.Init.Period            = PERIOD_VALUE;
  htim15.Init.ClockDivision     = 0;
  htim15.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim15) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channels #########################################*/
  /* Common configuration for all channels */
  sConfigOC.OCMode       = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;

  /* Set the pulse value for channel 2 */
  sConfigOC.Pulse = PULSE2_VALUE;
  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }


  /* Start channel 2 */
//  if (HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2) != HAL_OK)
//  {
    /* PWM Generation Error */
//    Error_Handler();
//  }

  /* USER CODE END TIM15_Init 0 */



  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
}