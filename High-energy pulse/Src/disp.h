#ifndef _DISP_H__
#define _DISP_H__

#include "stdint.h"
#include "cmsis_os.h"
#include "pcf8562.h"


typedef struct
{
    uint8_t bOnSetting;
    uint8_t ucActiveSetting;    //1: active setting defence area 1, 2:active setting defence area 2
    uint8_t ucRunningStatus;
    uint8_t ucAlarmFlag;    //bit 0 IR alarm  bit 1 tamper  Defence area 1 alarm. bit 2 defence area 2
	uint8_t ucAlarmType;   //bit 0 tamper bit 1 IR bit2:bit3 area1  bit3:bit4 area2
	
    uint8_t ucPowerStatus;
    uint8_t ucRunningCnt;

    uint8_t ucSetVoltage[2];
    uint8_t ucVoltage[2];
    uint8_t ucMovingPosition;
    uint8_t ucCurrent[2];
    uint8_t bOnBlink;
	
	uint8_t ucRunningHour;
    uint16_t uiRunningDate;
    uint8_t ucRunningYear;
    
    uint8_t bDeviceFlag;

}DISPLAY;

extern osMessageQueueId_t xDisplayMsg;
void vDisplayTask(void *argument);

#endif

