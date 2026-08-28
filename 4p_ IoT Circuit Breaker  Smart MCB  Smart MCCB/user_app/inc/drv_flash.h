#ifndef _DRV_FLASH_H__
#define _DRV_FLASH_H__

#include "stm32f0xx_hal.h"


/* Base address of the Flash sectors */
#define ADDR_FLASH_PAGE_61   ((uint32_t)0x0800F400) /* Base @ of Page 61, 1 Kbytes */
#define ADDR_FLASH_PAGE_62   ((uint32_t)0x0800F800) /* Base @ of Page 62, 1 Kbytes */
#define ADDR_FLASH_PAGE_63   ((uint32_t)0x0800FC00) /* Base @ of Page 63, 1 Kbytes */



void Flash_PageErase(uint32_t Pageaddr);
void FlashWrite(uint32_t pageaddr,uint8_t *databuffer,uint16_t len);
void FlashRead(uint32_t pageaddr,uint8_t *databuffer,uint16_t len);


#endif
