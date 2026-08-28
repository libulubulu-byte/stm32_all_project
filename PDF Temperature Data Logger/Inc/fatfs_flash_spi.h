//#ifndef __SPI_FLASH_H
//#define __SPI_FLASH_H

#ifndef __FATFS_FLASH_SPI_H
#define __FATFS_FLASH_SPI_H

#include "stm32l0xx_hal.h"//#include "stm32f10x.h"



//#include <stdio.h>

#include "diskio.h"

//#include "integer.h"

//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
//#define  sFLASH_ID                0XEF4017    //W25Q64  华邦

//#define  sFLASH_ID                0xC84015//  0XEF4017    //W25Q64  GD

//#define  sFLASH_ID                SPI_FLASH_ReadID()//  0XEF4017    //W25Q64  GD

/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF



/*SPI接口*/
//#define FLASH_SPI                           SPI1
//#define FLASH_SPI_CLK                       RCC_APB2Periph_SPI1

//#define FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
//#define FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
//#define FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA

//#define FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
//#define FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
//#define FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA

//#define FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
//#define FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
//#define FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA

//#define FLASH_CS_PIN                        GPIO_Pin_4                 /* PA.04 */
//#define FLASH_CS_GPIO_PORT                  GPIOA                       /* GPIOA */
//#define FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA


#define SPI_FLASH_CS_LOW()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)   //GPIO_ResetBits(GPIOA, GPIO_Pin_4)				//输出低电平 GPIO_ResetBits(FLASH_CS_GPIO_PORT, FLASH_CS_PIN)
#define SPI_FLASH_CS_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)  // GPIO_SetBits(GPIOA, GPIO_Pin_4)			  //设置为高电平 GPIO_SetBits(FLASH_CS_GPIO_PORT, FLASH_CS_PIN)

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         0
#define FLASH_DEBUG_FUNC_ON    0

#define FLASH_INFO(fmt,arg...)            0 //printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)           0//printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
//#define FLASH_DEBUG(fmt,arg...)           0
//do{\
//                                       	if(FLASH_DEBUG_ON)\
//                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                          }while(0)

#define FLASH_DEBUG_FUNC()                0 
//do{\
//                                         if(FLASH_DEBUG_FUNC_ON)\
//                                         printf("<<-FLASH-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
//                                       }while(0)

//uint8_t SPI_FLASH_Init(void);
DSTATUS TM_FATFS_FLASH_SPI_disk_initialize(void);
DSTATUS TM_FATFS_FLASH_SPI_disk_status(void) ;
DRESULT TM_FATFS_FLASH_SPI_disk_ioctl(BYTE cmd, char *buff) ;
DRESULT TM_FATFS_FLASH_SPI_disk_read(BYTE *buff, DWORD sector, UINT count) ;
DRESULT TM_FATFS_FLASH_SPI_disk_write(BYTE *buff, DWORD sector, UINT count) ;
//DRESULT TM_FATFS_FLASH_SPI_disk_writel(BYTE *buff, DWORD sector, UINT count) ;
//DRESULT TM_FATFS_FLASH_SPI_disk_writew(BYTE *buff, DWORD sector, UINT count);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
//uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_BufferWritel(uint8_t* Tempe_Data_wendu, uint32_t Data_Temperat_Adress, uint16_t NumByteToWrite);

uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
#endif /* __SPI_FLASH_H */

