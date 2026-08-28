#ifndef __WENZHENG_OTA_H_
#define __WENZHENG_OTA_H_


#include "stdint.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>




#define FLASH_PAGE_SIZE                    ((uint32_t)0x800)
#define PageSize		FLASH_PAGE_SIZE			//2K
/*=====用户配置(根据自己的分区进行配置)=====*/
#define BootLoader_Size 		0x3000U			  ///< BootLoader的大小 12K 6 页
#define Application_Size		0x0D000U	    ///< 应用程序的大小 52K    26页
#define Application_1_Addr		0x08003000U		///< 应用程序1的首地址  6页
#define Application_2_Addr		0x08010000U		///< 应用程序2的首地址  32页
/*==========================================*/
#define WIFI_Addr		0x0801D000U		///< 应用程序wifi的首地址  58页
#define ADDR_4G			0x0801D000U		///< 应用程序wifi的首地址  58页

/* 启动的步骤 */
#define Startup_Normol 0xFF	      ///< 正常启动
#define Startup_Update 0xAA	      ///< 升级再启动
#define Startup_Reset  0xBB       ///< ***恢复出厂 目前没使用***


/* 存储介质 */

#define Enable_W25Q 1
 

int Erase_page(uint32_t pageaddr, uint32_t num);
void Set_Update_Down(void);
unsigned int Read_Start_Mode(void);
unsigned int Read_APP_None(void);
void Start_BootLoader(void);

void ReadFlash(uint32_t dest_addr, uint8_t * buff, int buf_len);
void WriteFlash(uint32_t addr, uint8_t * buff, uint32_t buf_len);
uint8_t Mov_code_W25Q(uint32_t App_Addr,uint32_t W25Q_Addr,unsigned int App_size);
uint8_t rollback_APP(uint32_t W25Q_Addr,unsigned int des_addr, unsigned int byte_size);

#endif



