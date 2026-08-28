#ifndef __EEPROM_H
    #define __EEPROM_H
		
#include "stdint.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "main.h"



/********************************************
#define   EE_FQ1alarm      0x01
#define   EE_FQ2alarm      0x02
#define   EE_FQstatus1     0x04
#define   EE_FQstatus2     0x05
#define   EE_VOLCLK        0x13
#define   EE_CLOCKNUM      0x14
********************************************/

#define   EE_INITIAL       0x00
#define   POWERON_CNT      0x01

#define   EE_STATUS1       0x10    //pulse status

#define   EE_VOLTAGE1      0x11    //setvol 1
#define   EE_VOLTAGE2      0x12    //setvol 2
#define   EE_SWITCHEN      0x13  //0x13  
#define   EE_BEEPTIME      0x14    //beep on time
#define   EE_RELAYTIME     0x15    //relay on time
#define   EE_ALARMCYCLES   0x16    //alarm cycle
#define   EE_TOUCHEN       0x17    //touch alarm enable
#define   EE_TAMPEN       0x18    //touch alarm enable

#define   EE_MIN          0x30   //device time 
#define   EE_HOUR         0x31
#define   EE_DATE         0x32
#define   EE_YEAR         0x34

#define   EE_TIME         0x30

#define   EE_BPSH          0x20   //baud rate
#define   EE_BPSL          0x21

#define   EE_SIX_LINE_BOARD   0x25 
#define   EE_DUAL_DEFENCE     0x26

#define   EE_RECORD        0x4f
#define   EE_RECORD_BASE   0x50


typedef struct{ 
	uint8_t ucCmd;
	uint8_t ucData[8];
}EEprom_Msg_TypeDef;


extern osMessageQueueId_t xEEWriteMsg;


void vI2C_init(void);
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite);
void vWriteEETask(void *argument);
void AT24CXX_WriteOneByteN(uint16_t WriteAddr, uint8_t DataToWrite);
void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);
void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);


#endif

