#ifndef __ADC__H__
#define __ADC__H__
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif
/*---------------------C O M P L I E R  C O M M A N D----------------*/
/*---------------------I N C L U D  F I L E S------------------------*/
uint16_t ADC_Getvalue(uint16_t  ADC_CHANNEL_num);
void ADC_start(void);  
void init_adc(void);
void ADC_GetLeakgevalue(void);
void ADC_GetNtcvalue(void);
void ADC_value(uint16_t *Data);
uint16_t get_adcvalue(void);
uint16_t adc_Jiaoliu(uint16_t  ADC_CHANNEL_num);
#ifdef __cplusplus
}
#endif

#endif