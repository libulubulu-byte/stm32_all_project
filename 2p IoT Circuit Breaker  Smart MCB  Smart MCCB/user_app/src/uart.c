/***************************************************************************
* NAME:         eeprom.c
*----------------------------------------------------------------------------
* Copyright (c)
*----------------------------------------------------------------------------
* DATE:         2018-06-07
* REVISION:         V1.0
*----------------------------------------------------------------------------
* NOTE:
*
* Modified:
*****************************************************************************/

/*---------------------C O M P L I E R  C O M M A N D----------------*/

/*---------------------I N C L U D  F I L E S------------------------*/

#include "main.h"
#include "stm32f0xx_it.h"
#include "bord.h"
#include "proc.h"
#include "prjlib.h"
#include "user_comm.h"
#include "uart.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef DEBUG_AFU
#endif
#define UART_FIFO_BUF_SIZE          128
/*---------------------M A C R O  D E F I N I T I O N S--------------*/
/*---------------------V A R  D E F I N I T I O N S------------------*/
#define UPDATE_FLAG_ADDRESS  0x08000000
#define UART_PORT_MAX 2

extern UART_HandleTypeDef huart1;
static uint32_t tmsuart = 0;
static FIFO_t   uartrecfifo;
static uint8_t  uartrecbuffer[UART_FIFO_BUF_SIZE];
FIFO_t  uartsendfifo;
static uint8_t  uartsendbuffer[UART_FIFO_BUF_SIZE];
static  uint8_t buf[100]={0};
void uart_fifoinit(void) {
  FIFO_Init(&uartrecfifo, uartrecbuffer, UART_FIFO_BUF_SIZE);
  FIFO_Init(&uartsendfifo, uartsendbuffer, UART_FIFO_BUF_SIZE);
}
void UART_WriteBlocking_ex(uint8_t* pdata, uint16_t len) {
 HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
   TS_DELAY(50); 
  HAL_UART_Transmit_IT(&huart1, pdata, len);
}

extern  struct workpara_t workpara;
//CMD_device_type=100
#define CMD_1 "CMD_type="
#define CMD_2 "CMD_addr="
#define CMD_3 "CMD_ssn="
#define CMD_4 "CMD_type?"
#define CMD_5 "CMD_addr?"
#define CMD_6 "CMD_ssn?"
#define CMD_7 "CMD_mode="
#define CMD_8 "CMD_mode?"

#define PRINT_STR(str) UART_WriteBlocking_ex((uint8_t*)str, strlen(str))

void AmendCommand(char* data) {
  char* tmp = (char*)data;
  uint32_t num = 0;
  char str[20] = "";
  if (!strncmp(tmp, CMD_1, strlen(CMD_1))) {
    tmp = data + strlen(CMD_1);
    sscanf(tmp, "%d", &num);
    if (num != 1 && num != 2) {
      UART_WriteBlocking_ex((uint8_t*)"input 1 or 2", strlen("input 1 or 2"));
    } else {
      workpara.type = num;
      saveparam();
      PRINT_STR("set ok\r\n");
    }
  } else if (!strncmp(tmp, CMD_2, strlen(CMD_2))) {
    tmp = data + strlen(CMD_2);
    sscanf(tmp, "%d", &num);
    if (num >=  65536) {
      UART_WriteBlocking_ex((uint8_t*)"The input value is too large", strlen("The input value is too large"));
    } else {
      workpara.address = num;
      saveparam();
      PRINT_STR("set ok\r\n");
    }
  } else if (!strncmp(tmp, CMD_3, strlen(CMD_3))) {
    tmp = data + strlen(CMD_3);
    sscanf(tmp, "%d", &num);
    if (num >=  4294967295) {
      UART_WriteBlocking_ex((uint8_t*)"The input value is too large", strlen("The input value is too large"));
    } else {
      workpara.sn = num;
      saveparam();
      PRINT_STR("set ok\r\n");
    }
  }else if (!strncmp(tmp, CMD_7, strlen(CMD_7))) {
    tmp = data + strlen(CMD_7);
    sscanf(tmp, "%d", &num);
    if (num >=  65536) {
      UART_WriteBlocking_ex((uint8_t*)"The input value is too large", strlen("The input value is too large"));
    } else {
      workpara.mode = num;
      workpara.limit_amp=num*1000;
      saveparam();
      PRINT_STR("set ok\r\n");
    }
  } else if (!strncmp(tmp, CMD_4, strlen(CMD_4))) {
    PRINT_STR("TYPE:");
    sprintf(str, "%d", workpara.type);
    UART_WriteBlocking_ex((uint8_t*)str, strlen(str));
    PRINT_STR("\r\n");
  } else if (!strncmp(tmp, CMD_5, strlen(CMD_5))) {
    PRINT_STR("ADDR:");
    sprintf(str, "%d", workpara.address);
    UART_WriteBlocking_ex((uint8_t*)str, strlen(str));
    PRINT_STR("\r\n");
  } else if (!strncmp(tmp, CMD_6, strlen(CMD_6))) {
    PRINT_STR("SSN:");
    sprintf(str, "%d", workpara.sn);
    UART_WriteBlocking_ex((uint8_t*)str, strlen(str));
    PRINT_STR("\r\n");
  }else if (!strncmp(tmp, CMD_8, strlen(CMD_8))) {
    PRINT_STR("MODE:");
    sprintf(str, "%d", workpara.mode);
    UART_WriteBlocking_ex((uint8_t*)str, strlen(str));
    PRINT_STR("\r\n");
  }

}


/**
 * 接收处理命令
 */
void uartdataproc(void) {
  uint8_t len = 0;
  static uint8_t fifolen = 0;

  fifolen=FIFO_Length(&uartrecfifo);
  if (TMS_IS_OVER(tmsuart,20) && fifolen > 0) {
   if ((FIFO_Query(&uartrecfifo, 0) == 0x7E)&&(fifolen >= 10))
    {
      FIFO_Read(&uartrecfifo,buf,8);
      len = buf[7] + 8;
      len += 3;      
      FIFO_Read(&uartrecfifo,&buf[8],len-8);
      if((buf[6]&0x80) == 0){
        cmddataproc(buf, len);
      }
    } else if (fifolen >= 6 && fifolen < 100) { //Bootloader update use
      FIFO_Read(&uartrecfifo, buf, fifolen);
      buf[fifolen - 1] = 0;
      if (STR_EQUAL(buf, "update")) {
        IO_L(ON_LED);
        IO_L(OFF_LED);
        __disable_irq();
        HAL_FLASH_Unlock();
        
/*        FLASH_EraseSector2(UPDATE_FLAG_ADDRESS);
//        FLASH_Program2(UPDATE_FLAG_ADDRESS, &up_flag, 1);*/
        HAL_FLASH_Lock();
        __enable_irq();
        NVIC_SystemReset();
      } else {
        AmendCommand((char*)buf);
      }
    }
    else{
      FIFO_Read(&uartrecfifo, buf, 1);
    }
  } else if (TMS_IS_OVER(tmsuart, 2000) || FIFO_Length(&uartrecfifo) >= UART_FIFO_BUF_SIZE - 1) {
    if (FIFO_Length(&uartrecfifo) > 0) {
//      FIFO_Flush(&uartrecfifo);
    }
  }
}


BOOL UART_ReceiveByte(UART_HandleTypeDef *pHandle)
{
    uint8_t rd = 0;
    if (pHandle != NULL && ((__HAL_UART_GET_FLAG(pHandle, UART_FLAG_RXNE) != RESET && __HAL_UART_GET_IT_SOURCE(pHandle, UART_IT_RXNE) != RESET)
                            || __HAL_UART_GET_FLAG(pHandle, UART_FLAG_ORE) != RESET)) {
        rd = (uint8_t)(pHandle->Instance->RDR);

        TMS_INIT(tmsuart);
        if(tmsuart>(tmsuart+2000)){
          tmsuart=0;
        }
        FIFO_Put(&uartrecfifo, rd);
        return TRUE;
    }
    return FALSE;
}



/**
 * 串口接收到新的数据
 *
 * @param data   新接收到的数据
 */
void UART_NewData(uint8_t data) {
  FIFO_Put(&uartrecfifo, data);
  TMS_INIT(tmsuart);
}

void PritR(void)
{
    static char str[20] = "";
  
    PRINT_STR("REC:");
    sprintf(str, "%d", workpara.address);
    UART_WriteBlocking_ex((uint8_t*)str, strlen(str));
    PRINT_STR("\r\n");
}

/*---------------------------------------------------------------------------------
Name:         write_eeprom
Funtion:      write 'len' bytes to the eeprom of mcu.
IN:           start_adr, eeprom address.
              wr_buf, write data buffer.
              len: number of write data. must be equal or less than 256.
OUT:          0,success; -1, eeprom error; -2, len is greater than 256.
---------------------------------------------------------------------------------*/


