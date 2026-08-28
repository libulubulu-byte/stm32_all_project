/**************************************************************************************
 *                    (c) Copyright 20xx, Company Name, City, State
 *                               All Rights Reserved
 *
 *
 * FileName       : AT24C02.h
 * Version        : V1.0
 * Programmer(s)  :
 * Parameters     : ARM LPC2132  11.0952MHz
 * DebugTools     : JLINK V8.0 And Realview MDK V4.11
 * Description    :
 *
 *
 **************************************************************************************/

#ifndef __PCF85162_H
#define __PCF85162_H

#include "stdint.h"
#define u8 uint8_t
#define u16 uint16_t

#define CHKBIT(x, y) (x & (1 << y))

#define PCF8562_ADDR 0x70

#define PCF8562_SDA_IN()        \
  {                             \
    GPIOB->MODER &= 0XFFFFFF3F; \
  }
#define PCF8562_SDA_OUT()       \
  {                             \
    GPIOB->MODER &= 0xFFFFFF3F; \
    GPIOB->MODER |= (1 << 6);   \
  }
#define PCF8562_SDA_READ HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)

#define PCF8562_SCL_HIGH HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)
#define PCF8562_SCL_LOW HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET)
#define PCF8562_SDA_HIGH HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET)
#define PCF8562_SDA_LOW HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET)

//#define lcd_backup_on()   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET)
//#define lcd_backup_off()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET)
#if 0
#define lcd_backup_on() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET)
#define lcd_backup_off() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET)
#else
#define lcd_backup_on() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET)
#define lcd_backup_off() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET)
#endif

// HT16C22A I2C BY TERRY
#define HT16C22_SCL_H() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET) // PD13
#define HT16C22_SCL_L() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET)

#define HT16C22_SDA_H() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET) // PD14
#define HT16C22_SDA_L() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)

#define HT16C22_SDA_IN()        \
  {                             \
    GPIOD->MODER &= 0XCFFFFFFF; \
  }
#define HT16C22_SDA_OUT()       \
  {                             \
    GPIOD->MODER &= 0xCFFFFFFF; \
    GPIOD->MODER |= (1 << 28);  \
  }
#define HT16C22_SDA_READ() HAL_GPIO_ReadPin(GPIOD, 14)

#define HT16C22_OK 0
#define HT16C22_FAIL 1
#define HT16C22_LONG_TIMEOUT 0x5000 //���ʱʱ��?
#define HT16C22_I2C_ADDR 0X7F
#define HT16C22_I2C_ADDW 0X7E

/**************************************************************************************
 *                                Function stated
 **************************************************************************************/
// void  I2C_DelayMs(uint16_t t);
// void  I2C_SomeNOP(uint16_t t);
// void  I2C_Start(void);
// void  I2C_Stop(void);
// void  I2C_WriteByte(uint8_t dat);
// uint8_t I2C_ReadByte(uint8_t ic);
// void  I2C_SendAck(uint8_t a);
// uint8_t I2C_GetAck(void);

typedef struct
{
  uint8_t DisBuf1[11]; /*Display data 1,2,3,4*/
  uint8_t DisBuf2[11];
  uint8_t gVar_Buf[4];
} DTA_BUF;

extern DTA_BUF DataBuf;

extern uint8_t DisBuf[11];
extern uint8_t DisBuf1[11];
extern uint8_t const Lcd_Seg[10];
extern uint8_t const Lcd_Seg1[10];
extern uint8_t const Lcd_Seg2[10];

// extern void  I2CInit(void);
// extern uint8_t I2CWriteByte(uint8_t ic, uint8_t addr, uint8_t dat);
// extern uint8_t I2CReadByte(uint8_t ic, uint8_t addr);
// extern uint8_t I2CWriteNByte(uint8_t ic, uint8_t addr, uint8_t *p, uint8_t num);
// extern uint8_t I2CReadNByte(uint8_t ic, uint8_t addr, uint8_t *p, uint8_t num);

void LCD_Init(void);
uint8_t PCF8562_Init(uint8_t addr, uint8_t mode, uint8_t glitter);
void Disp(void);

void HT16C22_Init(void);
void HT16C22_Write_RAM(u8 *pBuffer, u8 RamAdd, u8 NumByte);
void HT16C22_disD_chg(void);
void HT16C22_DisAll(unsigned char dat);

/*************************************************************************************/
#endif
