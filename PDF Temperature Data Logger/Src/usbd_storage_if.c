/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @brief          : Memory management layer
  ******************************************************************************
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/
#include "malloc.h"
#include "stream1.h"
#include "uart.h"
/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"
/* USER CODE BEGIN INCLUDE */
#include "fatfs_flash_spi.h"
/* USER CODE END INCLUDE */
uint8_t usb_write_flag=0;
/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_STORAGE 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_TYPES */
/* USER CODE END PRIVATE_TYPES */ 
/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_Private_Defines
  * @{
  */ 
//#define STORAGE_LUN_NBR                  1  
//#define STORAGE_BLK_NBR                  0x10000  
//#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */

#define STORAGE_LUN_NBR                  1  
//#define STORAGE_BLK_NBR                  (4096-8) //预留 8*512 最后一个sector 4K作为备份区域   flash容量  2M
#define STORAGE_BLK_NBR                  (2048-8) //预留 8*512 最后一个sector 4K作为备份区域   flash容量  2M
#define STORAGE_BLK_SIZ                  512//4096//0x200

/* USER CODE END PRIVATE_DEFINES */
  
/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_Private_Macros
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_MACRO */
/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */ 

/** @defgroup USBD_STORAGE_IF_Private_Variables
  * @{
  */
/* USER CODE BEGIN INQUIRY_DATA_FS */ 
/* USB Mass storage Standard Inquiry Data */
const int8_t  STORAGE_Inquirydata_FS[] = {/* 36 */
  
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1',                     /* Version      : 4 Bytes */
}; 
/* USER CODE END INQUIRY_DATA_FS */ 

/* USER CODE BEGIN PRIVATE_VARIABLES */
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */ 
 
/** @defgroup USBD_STORAGE_IF_Exported_Variables
  * @{
  */ 
  extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */ 
  
/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @{
  */
static int8_t STORAGE_Init_FS (uint8_t lun);
static int8_t STORAGE_GetCapacity_FS (uint8_t lun, 
                           uint32_t *block_num, 
                           uint16_t *block_size);
static int8_t  STORAGE_IsReady_FS (uint8_t lun);
static int8_t  STORAGE_IsWriteProtected_FS (uint8_t lun);
static int8_t STORAGE_Read_FS (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);
static int8_t STORAGE_Write_FS (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS (void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */ 
  
USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS,
};

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : STORAGE_Init_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Init_FS (uint8_t lun)
{
  /* USER CODE BEGIN 2 */ 
//		uint16_t status = USBD_OK;

//	TM_FATFS_FLASH_SPI_disk_initialize();
////	if(SPI_FLASH_ReadID()==sFLASH_ID)		
//	{
//		//printf("flash init succsee\n");				
//		status = USBD_OK;		
//	}			
//	else 			
//		status = USBD_FAIL;
	
	
  return (USBD_OK);
  /* USER CODE END 2 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_GetCapacity_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */   
  *block_num  = STORAGE_BLK_NBR;
  *block_size = STORAGE_BLK_SIZ;
  return (USBD_OK);
  /* USER CODE END 3 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_IsReady_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t  STORAGE_IsReady_FS (uint8_t lun)
{
  /* USER CODE BEGIN 4 */ 
  return (USBD_OK);
  /* USER CODE END 4 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_IsWriteProtected_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t  STORAGE_IsWriteProtected_FS (uint8_t lun)
{
  /* USER CODE BEGIN 5 */ 
  return (USBD_OK);
  /* USER CODE END 5 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_Read_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Read_FS (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,                       
                        uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */ 
		//改为块大小为 4096   blk_add参数传进来就是 地址偏移 blk_len参数传进来就是 读取字节个数大小

	SPI_FLASH_BufferRead((uint8_t *)buf, blk_addr, blk_len);
  return (USBD_OK);
  /* USER CODE END 6 */ 
}

/*******************************************************************************
* Function Name  : STORAGE_Write_FS
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_Write_FS (uint8_t lun, 
                         uint8_t *buf, 
                         uint32_t blk_addr,
                         uint16_t blk_len)
{
	//改为块大小为 4096   blk_add参数传进来就是 地址偏移 blk_len参数传进来就是 读取字节个数大小
	static uint32_t First_Enter=0;
	
	uint16_t Sector_Len;
	uint32_t Sector;
		uint32_t i=0,Set_Interl_time=0,j=0;
//	uint32_t	PC_Set_YMD=0,PC_Set_HMS=0;
//	int16_t data[60];	uint8_t Send_data[18];
//uint8_t dayuxiaoyu[8];	
//		uint16_t kaiji_delay=0;
//	uint8_t alarm_flag=0;
//uint8_t dayuxiaoyu_flag=0;
//	uint8_t xiaoshu[8],k=0;
//		uint16_t setInterval=0;
//	uint16_t delay_start=0;
//	int16_t Alarm1_below=0;
//	uint16_t	Alarm1_Accumulative=0;
//	int16_t Alarm2_below=0;
//	uint16_t	Alarm2_Accumulative=0;
//	int16_t Alarm3_above=0;
//	uint16_t	Alarm3_Accumulative=0;
//	int16_t Alarm4_above=0;
//	uint16_t	Alarm4_Accumulative=0;
//	int16_t Alarm5_above=0;
//	uint16_t	Alarm5_Accumulative=0;
//	int16_t Alarm6_above=0;
//	uint16_t	Alarm6_Accumulative=0;
//	int16_t Alarm7_above=0;
//	uint16_t	Alarm7_Accumulative=0;
//	int16_t Alarm8_above=0;
//	uint16_t	Alarm8_Accumulative=0;
	uint8_t lt=0,kly=0;
//	uint8_t	*Tempe_Data_wendu;
	Sector = blk_addr/STORAGE_BLK_SIZ;
	Sector_Len = blk_len / STORAGE_BLK_SIZ;

	if(DataEEPROM_Read(0x0808003C)<2)
		{
					if(DataEEPROM_Read(0x08080044)==0)
					{
							if((blk_addr/512)==76)
							{	
								SPI_FLASH_SectorErase(blk_addr);
										SPI_FLASH_BufferWrite((uint8_t *)buf,blk_addr,blk_len);
							}
							if((blk_addr/512)<76)
							{
								TM_FATFS_FLASH_SPI_disk_write(buf,  blk_addr/512, blk_len/512 );
							
							}
					}
							if((blk_addr/512)>=108)
							{
									if(DataEEPROM_Read(0x08080044)==1)
									{
											if((blk_addr/512)>110)
											{
													return (USBD_OK);
											
											}
									}
										uint8_t	Read_Buf[256];
										SPI_FLASH_BufferRead(Read_Buf, blk_addr,256);
										for(i=0;i<256;i++)
										{
											if(Read_Buf[i]!=0xFF)
											{		
													kly=1;break;
											}
										}
										if(kly==1)
										{
											SPI_FLASH_SectorErase(blk_addr);kly=0;
										}
										SPI_FLASH_BufferWrite((uint8_t *)buf,blk_addr,blk_len);
								
							}
				
			if((blk_addr/512)==477&&buf[511]==0xff&&buf[510]==0xff)
			{
				DataEEPROM_Write(0x08080044,1);
			
			}
			if(DataEEPROM_Read(0x08080044)==1)
			{
						if(usb_write_flag==0)
						{
							if(Sector>=108)
							{
									if(buf[509]==0&&buf[510]==0&&buf[508]==0&&buf[507]==0)	
									{
										usb_write_flag=1;
										
									}
							}
						
						}
			}
		}
  return (USBD_OK);
  /* USER CODE END 7 */ 
}


/*******************************************************************************
* Function Name  : STORAGE_GetMaxLun_FS
* Description    : 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int8_t STORAGE_GetMaxLun_FS (void)
{
  /* USER CODE BEGIN 8 */ 
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */ 
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */ 

/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
