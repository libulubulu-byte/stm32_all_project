#ifndef __STREAM1_H
#define __STREAM1_H
#include "stm32l0xx_hal.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "fatfs_flash_spi.h"
#include "ff.h"
#include "LCD.H"
#include "sht20.h"
#include "pwr.h"
#include "zlib.h"
#include "calendar.h"
#include "button.h"
#include "uart.h"
#include "stm32l0xx_it.h"
#include "stream1.h"
void PDF_Creat();
void CSV_Creat();
void DataEEPROM_Write(uint32_t Address,uint32_t data);
void DataEEPROM_Writeint32(uint32_t Address,int32_t data);
void DataEEPROM_Erase(uint32_t Page_Address);
uint32_t DataEEPROM_Read(uint32_t Address);
#define Period               (uint32_t) 65535
/* Set the Timeout value */
#define Timeout              (uint32_t) (32768 - 1)
//#define EN_INT          __enable_irq();     //???????  
//#define DIS_INT         __disable_irq();    //???????  
static	FATFS fst;



/*************DataEEPROM*****************/
#define PEKEY1  0x89ABCDEF      //FLASH_PEKEYR  
#define PEKEY2  0x02030405      //FLASH_PEKEYR 

//#define High_alarm  8
//#define Low_alarm  2
//#define High_alarm_RH  60
//#define Low_alarm_RH  30
#endif


