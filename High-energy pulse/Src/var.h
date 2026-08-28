#ifndef __VAR_H
#define __VAR_H

#include "stdint.h"
#include "stdio.h"
#include "main.h"
#include "eeprom.h"
#include "rtc.h"


#define beep_on()   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET)
#define beep_off()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET)

#define led1_on()   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)
#define led1_off()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET)
#define led2_on()   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET)
#define led2_off()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET)
#define led3_on()   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET)
#define led3_off()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET)

//#define SCAN_EN   1
//#define Debug_flag   1

#define   EE_FLAG         0xA5               //hk version

typedef struct
{
    uint8_t ucAlarmStatus;// current status  00 normal  01 short 02 open
    uint8_t ucAlarmType;
    uint8_t ucSetVoltage[2];//lcd display also need this
    uint8_t ucVoltage[2];
    uint8_t ucCurrent[2];

//pulse  control parameter
    uint8_t ucPulseOn;
    uint8_t ucForceSync;
    uint8_t ucTouchEN;
    uint8_t ucRunningStatus;        //bit 0 : first defence area  bit 1: second defence area
//    uint8_t ucStopStatus[2];

//parameter set
    uint8_t ucIREn;//∫ÏÕ‚
//    uint8_t ucSwitchAlarm;
    uint8_t ucPowerStatus;      // bit 0 :battery  bit 1: vpower
    uint8_t ucKeyValue;
    uint8_t ucKeyStatus;
    uint8_t ucBatteryLow;
    uint8_t ucPowerFlag;
    uint8_t ucSetAlarm[2];
    uint8_t ucPowerOnTime;

    uint16_t ucBeepOnTime;
    uint16_t ucRelayOnTime;
    uint8_t ucAlarmCycle;

//lcd relate status
    uint8_t bOnSetting;
    uint8_t ucLcdOnTime;
    uint8_t ucRunningMs;
    uint8_t ucRunningMin;
    uint8_t ucRunningHour;
    uint16_t uiRunningDate;
    uint8_t ucRunningYear;

//device flag hk or joose
    uint8_t bDeviceFlag;
    uint16_t uiBaudrate;

    uint8_t ucAddress;
    uint8_t ucTampEn;

} DEVICEDATA;


extern uint8_t DUAL_DEFENCE,SIX_LINE_BOARD;
uint8_t get_addr(void);
void HAL_SW_MspInit(void);
void HAL_LED_MspInit(void);
void ReadEEPROM(DEVICEDATA *pd);
void init_EEPROM(void);
void Wrtie_AlarmRecord(void);
void ProcessKeyBord(DEVICEDATA *pd);
void Relay_SetBit(uint8_t ucChannel, GPIO_PinState state);
extern void AlarmVoltageValueSelect(void);
extern void DefenceAreaSwitchTcpAck(void);
#endif

