#include "adc.h"
#include "bord.h"
#include "ntc.h"
#include "proc.h"
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  32)

uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
uint16_t   aADCxLeakgeData[ADC_CONVERTED_DATA_BUFFER_SIZE];
uint16_t   aADCxjiaoliuData[ADC_CONVERTED_DATA_BUFFER_SIZE];
uint16_t adcntcvalue;
uint16_t adcleakgevalue;
extern ADC_HandleTypeDef hadc;
extern ADC_HandleTypeDef hadc_1;

void ADC_start(void)
{
  if (HAL_ADC_Start_DMA(&hadc,
                        (uint32_t *)aADCxConvertedData,
                        ADC_CONVERTED_DATA_BUFFER_SIZE
                       ) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_Delay(5);
  
  if (HAL_ADC_Start_DMA(&hadc_1,
                        (uint32_t *)aADCxLeakgeData,
                        ADC_CONVERTED_DATA_BUFFER_SIZE
                       ) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_Delay(5);
}

void ADC_ntcvalue(uint16_t *Data)
{
  adcntcvalue=0;
  for(uint8_t i=5;i<15;i++){
    adcntcvalue+=Data[i];
  }
  adcntcvalue=adcntcvalue/10;
}

void ADC_leekgevalue(uint16_t *Data)
{
  adcleakgevalue=0;
  for(uint8_t i=0;i<10;i++){
    adcleakgevalue+=Data[i];
  }
  adcleakgevalue=adcleakgevalue/10;
}
void ADC_GetNtcvalue(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

  ADC1->CHSELR = 0;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
  __HAL_ADC_CLEAR_FLAG(&hadc, ADC_FLAG_EOC);

  for(int i=0; i<22; i++)
  {
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 10);
    
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc), HAL_ADC_STATE_REG_EOC))
    {
      aADCxConvertedData[i] = HAL_ADC_GetValue(&hadc);
    }

  }
   HAL_ADC_Stop(&hadc);
  /*if (HAL_ADC_Start_DMA(&hadc,
                        (uint32_t *)aADCxConvertedData,
                        ADC_CONVERTED_DATA_BUFFER_SIZE
                       ) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_Delay(5);
  HAL_ADC_Stop_DMA(&hadc);*/
  ADC_ntcvalue(aADCxConvertedData);
}

void ADC_GetLeakgevalue(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

  ADC1->CHSELR = 0;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
  __HAL_ADC_CLEAR_FLAG(&hadc, ADC_FLAG_EOC);

  for(int i=0; i<22; i++)
  {
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 10);
    
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc), HAL_ADC_STATE_REG_EOC))
    {
      aADCxLeakgeData[i] = HAL_ADC_GetValue(&hadc);
    }

  }
   HAL_ADC_Stop(&hadc); 

  ADC_leekgevalue(aADCxLeakgeData);
}
///* ????:??channel?ADC???
//??:channel
//??:??channel??
//*/
//uint16_t Get_ADC(unsigned char channel)
//{
// uint16_t result=0;
//   ADC_ChannelConfTypeDef sConfig;
//   /**Configure Regular Channel 
//    */
//  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
// if(channel==1)
// {
//  sConfig.Channel = ADC_CHANNEL_5;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
// }
//   if(channel==2)
// {
//  sConfig.Channel = ADC_CHANNEL_6;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
// }
//  if(channel==3)
// {
//  sConfig.Channel = ADC_CHANNEL_3;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
// }
//  if(channel==4)
// {
//  sConfig.Channel = ADC_CHANNEL_4;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
// }
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
//  for(uint8_t i=0;i<8;i++)
// {
//  HAL_ADC_Start(&hadc1);
//  HAL_ADC_PollForConversion(&hadc1, 1); 
//  AD_VAL[channel] = HAL_ADC_GetValue(&hadc1);
//  
//  float ADC_TOG=set_value.Filter_Level/100;
//  
//  AD_VAL[channel]=AD_VAL[channel+3]*ADC_TOG+AD_VAL[channel]*(1- ADC_TOG);
//   AD_VAL[channel+3]=AD_VAL[channel];
// } 
// result=AD_VAL[channel];
//  return result;
//}
uint16_t ADC_Getvalue(uint16_t  ADC_CHANNEL_num)
{
	uint32_t sumb=0;
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_num;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5 ;

  ADC1->CHSELR = 0;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
  __HAL_ADC_CLEAR_FLAG(&hadc, ADC_FLAG_EOC);
	HAL_ADC_Start(&hadc);
  for(uint8_t i=0; i<2; i++)
  {
    
    HAL_ADC_PollForConversion(&hadc, 1);
    
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc), HAL_ADC_STATE_REG_EOC))
    {
      HAL_ADC_GetValue(&hadc);
    }

  }
   HAL_ADC_Stop(&hadc); 
	HAL_ADC_Start(&hadc);
  for(uint8_t i=0; i<30; i++)
  {
    
    HAL_ADC_PollForConversion(&hadc, 1);
    
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc), HAL_ADC_STATE_REG_EOC))
    {
      aADCxLeakgeData[i] = HAL_ADC_GetValue(&hadc);
    }

  }
   HAL_ADC_Stop(&hadc); 

//  ADC_leekgevalue(aADCxLeakgeData);

  for(uint8_t i=0;i<30;i++){
    sumb+=aADCxLeakgeData[i];
  }
  sumb=sumb/30;
	return sumb;
}
uint16_t adc_Jiaoliu(uint16_t  ADC_CHANNEL_num)
{
	uint16_t adc_jiaoliu=0,adc_max=0,adc_min=0,i,flag=0,k_f=0,t_f=0;
	uint32_t sum=0,time_out=0,k=0;
adc_jiaoliu=ADC_Getvalue(ADC_CHANNEL_num);
	for(i=0;i<30;i++)
	{
			time_out=gettickms();
		while(1)
		{
					if(gettickms()-time_out>20)
					{
						t_f=1;
						break;
					
					}
				 adc_jiaoliu=ADC_Getvalue(ADC_CHANNEL_num);
					if(adc_jiaoliu>20)
					{
						if(flag<3)
						{
						flag++;
						}
						if(flag==3)
						{
							k_f=1;
						}
					}
					if(adc_jiaoliu<=20)
					{
						flag=0;
						if(k_f==1)
						{
							k_f=0;
							break;
						}
					}
					if(k_f==1)
					{
						k++;
						sum+=adc_jiaoliu;
					}
	
		}
		if(t_f==1)
		{
			i++;
			break;
		}
//			if(adc_jiaoliu>adc_max)
//			adc_max=adc_jiaoliu;
//				if(adc_jiaoliu<adc_min)
//			adc_min=adc_jiaoliu;
	}
	if(k!=0)
	{
		adc_max=sum/k;

	}
	adc_max=(12.25*adc_max+107.03);
	if(adc_max<=107)
	{
		adc_max=0;
	
	}
	adc_max=(adc_max+50)/100;
	return (adc_max);


}
uint16_t get_adcvalue(void){
	return adcntcvalue;
}

uint16_t get_adcleakgevalue(void){
	return adcleakgevalue;
}
/**************
uint16_t get_adcvalue(void){
	return adcvalue;
}*************/