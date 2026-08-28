#ifndef __ADC_H
#define __ADC_H

#include "stm32l0xx_hal.h"

#define ADC1_STAR()   ADC1->CR |= ADC_CR_ADSTART
//#define ADC2_STAR()  ADC1->CR |= ADC_CR_ADSTART

//extern ADC_HandleTypeDef Adc1Handle;

extern int16_t  T_wendu,F_wendu; //温度变量
//extern float  T_wendu; //温度变量
#define PC1_ENABLE       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET)
#define PC1_DISABLE      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET)

#define PC3_ENABLE       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET)
#define PC3_DISABLE      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET)
 void DisableADC(void);
 void EnableADC(void);
void ADC1_Verf_Init(void);
void ADC1_Init(void);   //采集温度  初始化
void ADC2_Init(void);		//采集系统电压  初始化
//uint16_t ADC1_ConvertOnDiscMode(ADC_HandleTypeDef* Adc1Handle);
void ADC1_BaseConfig(ADC_HandleTypeDef* hadc);
void ADC1_ChannelConfig(uint16_t GPIO_PIN_N);
void ADC_GPIO_Init(void);
int16_t Temp_Coun(uint32_t Temp_Data);

//void ADC1_VER_Channel_Config(void);

//void MX_GPIO_Init(void);
void  ADC_Trave(void);
//int8_t ADC_Trave(void);

#endif
