#ifndef _BOARD_H_
#define _BOARD_H_


#include "main.h"
#include "user_comm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The board name */
#define BOARD_NAME "afu-smart"
//#define DEBUG_AFU						
	 
/*! @brief The UART to use for debug messages. */

#define BOARD_USER_UART 	                    huart1



#define   SPI_CS_CLEAR() \
IO_L(SPI1_CS)
   
#define   SPI_CS_SET() \
IO_H(SPI1_CS)   

#define   LED_RED_ON() \
IO_L(OFF_LED)

#define   LED_RED_OFF() \
IO_H(OFF_LED)

#define   LED_GREEN_ON() \
IO_L(ON_LED)

#define   LED_GREEN_OFF() \
IO_H(ON_LED)

#define   LED_YELLOW_ON() \
IO_L(FAULT_LED)

#define   LED_YELLOW_OFF() \
IO_H(FAULT_LED)

//#define   LED_RED_ON() \
//HAL_GPIO_WritePin(OFF_LED_GPIO_Port, OFF_LED_Pin, GPIO_PIN_RESET)

//#define   LED_RED_OFF() \
//HAL_GPIO_WritePin(OFF_LED_GPIO_Port, OFF_LED_Pin, GPIO_PIN_SET)
////IO_H(OFF_LED)

//#define   LED_GREEN_ON() \
//HAL_GPIO_WritePin(ON_LED_GPIO_Port, ON_LED_Pin, GPIO_PIN_RESET)

//#define   LED_GREEN_OFF() \
//HAL_GPIO_WritePin(ON_LED_GPIO_Port, ON_LED_Pin, GPIO_PIN_SET)
//#define   LED_YELLOW_ON() \
//HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_RESET)

//#define   LED_YELLOW_OFF() \
//HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_SET)

#define   MOTOR_IS_RUN()  do{ \
    IO_H( MOTOR_DRIVE); \
}while(0)

#define   MOTOR_IS_STOP()  do{ \
    IO_L( MOTOR_DRIVE); \
}while(0)

//#define   IS_SWITCH_OFF() \
//(adcvaluep3_avr*baifen_he>ADC_Getvalue(2)-adcvaluep3_m)
////!IO_READ(SW_ON_DETE)

//#define   IS_SWITCH_ON() \
//!IO_READ(SW_OFF_DETE)

//#define   IS_SWITCH_LOCK() \
//!IO_READ(TRIP_DETE2)

//#define   IS_SWITCH_FREE() \
//!IO_READ(TRIP_DETE1)

#define   IS_SWITCH_NOTREMOTE() \
IO_READ(ATT7053_PF)

#define   RS485_DE_TX()  do{ \
    RS485_ENABLE(EN); \
}while(0)

#define   RS485_DE_RX()  do{ \
    RS485_DISENABLE(EN); \
}while(0)

#define   SORT_IN() \
IO_READ(SET_NUMBER_IN)
#define   SORT_INs() \
IO_READ(SET_NUMBER_IN2)
#define   SORT_OUT_LOW() \
IO_H(SET_NUMBER_OUT)
#define   SORT_OUT_LOWs() \
IO_H(SET_NUMBER_OUT2)
#define   SORT_OUT_HIGH() \
IO_L(SET_NUMBER_OUT)
#define   SORT_OUT_HIGHs() \
IO_L(SET_NUMBER_OUT2)
#define   IS_OVER_V() \
IO_READ(OV_DETE)

#define   IS_UNDER_V() \
IO_READ(UV_DETE)

#define   IS_LEAKAGE_TEST() \
IO_READ(LEAKAGE_SW_DETE)

//#define   IS_LEAKAGE_STATUS() \
IO_READ(LEAKAGE_SW_DETE)

#define   IS_SW_TEST_STATUS() \
IO_READ(SW_TEST)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef DEBUG_AFU
void BOARD_InitDebugConsole(void);
#endif

void init_peripherals(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
