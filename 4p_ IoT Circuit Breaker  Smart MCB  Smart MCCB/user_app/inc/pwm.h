#ifndef __PWM_H
#define __PWM_H
#include "stdint.h"
extern uint16_t Pwm_Cycle;
extern uint8_t Duty_ratio;
#define  PERIOD_VALUE       (uint32_t)(Pwm_Cycle - 1)  /* Period Value 666 */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       (uint32_t)(PERIOD_VALUE*Duty_ratio/100) /* Capture Compare 2 Value 37.5 */
#define  PULSE3_VALUE       (uint32_t)(PERIOD_VALUE/4)        /* Capture Compare 3 Value  */
#define  PULSE4_VALUE       (uint32_t)(PERIOD_VALUE*12.5/100) /* Capture Compare 4 Value  */



void MX_TIM15_Init(void);

#endif 