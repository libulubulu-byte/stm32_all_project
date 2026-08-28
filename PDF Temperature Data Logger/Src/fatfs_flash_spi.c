#include "fatfs_flash_spi.h"
#include "malloc.h"
//#define  Erase_back_Sector    8192 //flash  8M 为例
#define CR1_SPE_Set          ((uint16_t)0x0040)
#define CR1_SPE_Reset        ((uint16_t)0xFFBF)

//#define FLASH_SECTOR_SIZE      512

//#define FLASH_SECTOR_COUNT    16384

//#define  Erase_back_Sector    (2048-4) //flash  以2M 为例
#define  Erase_back_Sector   (1024-4)   //flash  以2M 为例255*4096

#define FLASH_SECTOR_SIZE      512

#define FLASH_SECTOR_COUNT    (2048-8) //

//  extern SPI_HandleTypeDef hspi1;
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   
static volatile uint8_t TM_FATFS_FLASH_SPI_Stat = 1;	/* Physical drive status */

//static uint16_t SPI_TIMEOUT_UserCallback(void);

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters */
//  assert_param(IS_SPI_ALL_PERIPH(SPIx));
//  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral */
    SPIx->CR1 |= CR1_SPE_Set;
  }
  else
  {
    /* Disable the selected SPI peripheral */
    SPIx->CR1 &= CR1_SPE_Reset;
  }
}
uint8_t	TM_FATFS_FLASH_SPI_disk_initialize(void)
{	
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
//	if(sFLASH_ID == SPI_FLASH_ReadID())			/*检测FLASH是否正常工作*/
	{
		return TM_FATFS_FLASH_SPI_Stat = 0;	/* Clear STA_NOINIT flag */
	}
//	else
//	{
//		return TM_FATFS_FLASH_SPI_Stat = 1;
//	}
  #if 0
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

	
  FLASH_DEBUG_FUNC();
  /* Enable FLASH_SPI and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);

  /*!< SPI_FLASH_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI2, ENABLE);
   
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* FLASH_SPI configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPI, &SPI_InitStructure);

  /* Enable FLASH_SPI  */
  SPI_Cmd(SPI2, ENABLE);
	
	if(sFLASH_ID == SPI_FLASH_ReadID())			/*检测FLASH是否正常工作*/
	{
		return TM_FATFS_FLASH_SPI_Stat = 0;	/* Clear STA_NOINIT flag */
	}else
	{
		return TM_FATFS_FLASH_SPI_Stat = 1;
	}
#endif
}


DSTATUS TM_FATFS_FLASH_SPI_disk_status(void)
{
//	if(sFLASH_ID == SPI_FLASH_ReadID())			/*检测FLASH是否正常工作*/
	{
		return TM_FATFS_FLASH_SPI_Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
	}
//	else
//	{
//		return TM_FATFS_FLASH_SPI_Stat |= STA_NOINIT;
//	}

}


DRESULT TM_FATFS_FLASH_SPI_disk_ioctl(BYTE cmd, char *buff)
{
  int a;	  
	switch (cmd) 
	{
		case GET_SECTOR_SIZE :     // Get R/W sector size (WORD)
			*(WORD * )buff = FLASH_SECTOR_SIZE;   //4096;		//flash最小写单元为页，256字节，此处取2页为一个读写单位
		break;
		case GET_BLOCK_SIZE :      // Get erase block size in unit of sector (DWORD)
			*(DWORD * )buff = 1;		 //flash以4k为最小擦除单位
		break;
		case GET_SECTOR_COUNT:
		//	*(DWORD * )buff = 1536;		//sector数量1536*4096=6兆
		*(DWORD * )buff =  FLASH_SECTOR_COUNT;//4096;//16300;//16384;//2048;		//sector数量  2048*4096=8兆
		break;
		case CTRL_SYNC :
			for(a=0;a<100000;a++);
		break;
		default:break;
	}

	return RES_OK;
}

DRESULT TM_FATFS_FLASH_SPI_disk_read(BYTE *buff, DWORD sector, UINT count)
{
//	// FLASH_DEBUG_FUNC();
//	if ((TM_FATFS_FLASH_SPI_Stat & STA_NOINIT)) {
//		return RES_NOTRDY;
//	}

	sector+=0;//扇区偏移，外部Flash文件系统空间放在外部Flash后面8M空间
	if(count>0)
	{

		SPI_FLASH_BufferRead(buff, sector*FLASH_SECTOR_SIZE, count*FLASH_SECTOR_SIZE);

	}

	return RES_OK;
}

//#define write_size   256


//#define stack_size   256  //使用局部变量 
//#define ram_size     (4096-256)  //使用全局变量 

//#define stack_size   256  //使用局部变量 
//#define ram_size     256  //使用全局变量 

//static uint8_t Read_Buf[ram_size];
	

DRESULT TM_FATFS_FLASH_SPI_disk_write(BYTE *buff, DWORD sector, UINT count)
{
//	uint8_t Last_Read_Buf[stack_size]; //全局变量不够  

	uint32_t addr_offset;
	uint32_t secpos;
	uint16_t secoff;
	uint32_t secremain;	   
	uint32_t i; 
	
	uint32_t WriteAddr=0, Adress_to_write=0;
	uint16_t NumByteToWrite;
		//uint8_t	*Tempe_Data_wendu;	
	uint8_t lt=0,kly=0;
	uint8_t Read_Buf[266];
//	if(sector!=64&&sector!=70&&sector!=76)
//				{
////					Tempe_Data_wendu=mymalloc(512);
//					SPI_FLASH_BufferRead(Read_Buf, sector*512,256);
//							for(i=0;i<256;i++)
//							{
//								if(Read_Buf[i]!=0xFF)
//								{		
//										kly=1;break;
//								}
//							}
//							if(kly==1)
//							{
//								SPI_FLASH_SectorErase(sector*512);kly=0;
//							}else
//							{
//									SPI_FLASH_BufferRead(Read_Buf, sector*512+256,256);
//									for(i=0;i<256;i++)
//									{
//										if(Read_Buf[i]!=0xFF)
//										{		
//												kly=1;break;
//										}
//									}
//									if(kly==1)
//									{
//										SPI_FLASH_SectorErase(sector*512);kly=0;
//									}
//							
//							}
//							
//							SPI_FLASH_BufferWrite((uint8_t *)buff,sector*512,count*512);
////							myfree(Tempe_Data_wendu);
//							myfree(Read_Buf);
//							return RES_OK;
//				}else
{
#if 1//256zijei
	uint16_t k;

	uint32_t Start_addr,End_addr;	 
	uint32_t write_addr,read_addr;

	uint32_t write_count;	
	uint16_t Read_Cycle;
	uint8_t Erase_Flag;

	 uint16_t Start_Sector;
	
 // FLASH_DEBUG_FUNC();
	Erase_Flag = 0;
	
	read_addr = sector * FLASH_SECTOR_SIZE;  //;(sector<<9);    //* 512	  
	write_count = count * FLASH_SECTOR_SIZE; //(count<<9);
	
	Read_Cycle = write_count / 256;  //计算读取周期数  缓存只定义了256个字节 
	
	/*--------------------------写之前先判断 数据区是否是0xff状态 不是的话 需要先做擦除算法---------------------------*/		
	Erase_Flag = 0;
	for(i=0;i<Read_Cycle;i++) // 
	{
		if(Erase_Flag==1)
		{
			break;
		}
		if(i!=0)
			{
				read_addr = read_addr + 256;
			}
	  SPI_FLASH_BufferRead((uint8_t*)Read_Buf, read_addr, 256);
		for(k=0;k<256;k++)
		{
		  if(Read_Buf[k]!=0xff)
			{
				Erase_Flag = 1;
				break;
			}
		}
	}
	if(Erase_Flag==1)
	{
		write_count = count * FLASH_SECTOR_SIZE;//(count<<9);
		read_addr = sector * FLASH_SECTOR_SIZE;//(sector<<9); 
			
		Start_Sector =  read_addr / 4096;	
	 
		read_addr = Start_Sector*4096;
		write_addr = Erase_back_Sector*1024;// 最后一块sector4K 作为 存储缓存  存储容量一共2048K 
//	write_addr = Erase_back_Sector*4096;// 最后一块sector4K 作为 存储缓存  存储容量一共2048K
				
		
	  //最后一页作为   缓存区 
		SPI_FLASH_SectorErase(write_addr); // 最后一块sector4K 作为 存储缓存  存储容量一共2048K 
		 
		for(i=0;i<16;i++) //擦除最后一页之后 把待写入扇区数据先做备份 再擦除
		{
			if(i!=0)
				{
					write_addr = write_addr + 256;
					read_addr = read_addr + 256;
				}				
			SPI_FLASH_BufferRead((uint8_t*)Read_Buf,read_addr,256);  
			SPI_FLASH_BufferWrite((uint8_t*)Read_Buf,write_addr,256);	
		}		
		/*--------------------------擦除需要写的扇区---------------------------*/		
		write_addr = sector* FLASH_SECTOR_SIZE;  //(sector<<9); 
		SPI_FLASH_SectorErase(write_addr);     //擦除需要写的那块 地址快  				
	}
//	else //不需要做擦除 直接写入
//	{
//		/*--------------------------写入目前所有写的缓存区---------------------------*/	
//		write_addr =  (sector<<9); 	
//		write_count = (count<<9);
//		SPI_FLASH_BufferWrite(buff,write_addr,write_count);	
//		return RES_OK;
//	}
//		
	/*--------------------------写入目前所有写的缓存区---------------------------*/			
  write_addr = sector* FLASH_SECTOR_SIZE;//(sector<<9); 			
	write_count = count * FLASH_SECTOR_SIZE;//(count<<9);		
	SPI_FLASH_BufferWrite(buff,write_addr,write_count);	
  if(Erase_Flag==0) //如果之前没做擦除动作 写好之后直接返回即可
	{	
		return RES_OK;
	}
	
	/*---------------------已经做擦除备份的---Step 之前的buf区域读取重新写入---------------*/			
  //write_count = (count<<9);
	write_addr = sector * FLASH_SECTOR_SIZE;//(sector<<9); 
		
	Start_Sector =  write_addr / 4096;	
 
	write_addr = Start_Sector*4096;
	read_addr = Erase_back_Sector*1024;// 最后一块sector4K 作为 存储缓存  存储容量一共2048K 
		 
	Start_addr = sector*FLASH_SECTOR_SIZE;//(sector<<9); 
	End_addr = Start_addr + count * FLASH_SECTOR_SIZE;//(count<<9); 
	End_addr = End_addr -1;
	
	for(i=0;i<16;i++)
	{
			if(i!=0)
			{
				read_addr = read_addr + 256;
				write_addr = write_addr + 256;
			}
		if((write_addr<Start_addr)||(write_addr>End_addr)) //之前区域内的数据已经做写入  只要填充返回其他块数据区
		{				
			SPI_FLASH_BufferRead((uint8_t*)Read_Buf,read_addr,256);  
		  SPI_FLASH_BufferWrite((uint8_t*)Read_Buf,write_addr,256);	
		}	  
	}
	//return RES_OK;
#endif
}
	
	return RES_OK;
}

//DRESULT TM_FATFS_FLASH_SPI_disk_writel(BYTE *buff, DWORD sector, UINT count)
//{
//	uint32_t secAddr=((uint32_t)sector)<<9;
////	BYTE buf[32]={0x46,0x52,0x45,0x53,0x48,0x20,0x20,0x20,0x50,0x44,0x46,0x20,0,0,0,
////								0,0,0,0,0,0,0,0,0,0,0,0,2,0,0x0a,0,};
//	uint16_t i=0,j=0;
////	
////	disf_write++;
////	if(disf_write!=15&&disf_write<46)
////	{
////	SPI_FLASH_BufferWrite((uint8_t*)buff,secAddr,512);
////	}else if(disf_write==15)
////	{
////		SPI_FLASH_PageWrite((uint8_t*)buf,secAddr,32);
////	}else
////	{
////for(i=0;i<512;i++)
////	{
////	if(buff[i]!=0xff)
////	{
////		SPI_FLASH_PageWrite((uint8_t*)buff+i,secAddr+i,1);
////	}else if(buff[i+1]==0xff&&buff[i+2]==0xff&&buff[i+3]==0xff)
////	{break;
////	}

//////	}
////}
//SPI_FLASH_BufferWrite((uint8_t*)buff,secAddr,512);
//	return RES_OK;
//}


/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	//	__disable_irq();
	
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  SPI_FLASH_WaitForWriteEnd();
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Sector Erase instruction */
  SPI_FLASH_SendByte(W25X_SectorErase);
  /* Send SectorAddr high nibble address byte */
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
	
//	__enable_irq();
	
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(void)
{		
//	__disable_irq();
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Bulk Erase instruction  */
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
	//	__enable_irq();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	//	__disable_irq();
	
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send "Write to Memory " instruction */
  SPI_FLASH_SendByte(W25X_PageProgram);
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)	//只写一页
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     //printf("\n\r Err: SPI_FLASH_PageWrite too large!");
  }

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
	
	//	__enable_irq();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
//	__disable_irq();
	
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
	
//	 __enable_irq();
}
void SPI_FLASH_BufferWritel(uint8_t* Tempe_Data_wendu, uint32_t Data_Temperat_Adress, uint16_t NumByteToWrite)
{
	uint32_t	j=0,i=0,LL=0;uint8_t Tempe_Data_wendu2[256];
	j=0;LL=1;
															while(1)
															{
																		if(j!=LL)
																		{
																			SPI_FLASH_BufferWrite(Tempe_Data_wendu,Data_Temperat_Adress,256);
																			j=0;
																									for(i=0;i<256;i++)                                          //????   ??????????
																									{
																										if(Tempe_Data_wendu[i]==255)
																										{
																											j++;
																										}
																									}
																			SPI_FLASH_BufferRead(Tempe_Data_wendu2, Data_Temperat_Adress, 256);
																									LL=0;
																									for(i=0;i<256;i++)                                          //????   ??????????
																									{
																										if(Tempe_Data_wendu2[i]==255)
																										{
																											LL++;
																										}
																									}
																							
																									
																			}
																		else
																			{
																				break;
																			}
																		
																			
												}
}
/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/

//static uint8_t RecceTemp[4]; 
//static uint8_t SendBufTemp[4] = {W25X_JedecDeviceID,0xff,0xff,0xff};
//uint32_t SPI_FLASH_ReadID(void)
//{
//  uint8_t Temp0 = 0, Temp1 = 0, Temp2 = 0;
//  uint32_t Temp = 0;
//	
//	
//  /* Select the FLASH: Chip Select low */
//  SPI_FLASH_CS_LOW();



//#if 1
//  /* Send "RDID " instruction */
//  SPI_FLASH_SendByte(W25X_JedecDeviceID);

//  /* Read a byte from the FLASH */
//  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

//  /* Read a byte from the FLASH */
//  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

//  /* Read a byte from the FLASH */
//  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);
//#endif
//  /* Deselect the FLASH: Chip Select high */
//  SPI_FLASH_CS_HIGH();

//  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

//  return Temp;
//}
/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;
	
//	__disable_irq();

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
//	__enable_irq();	
  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
	SPI_FLASH_CS_HIGH();

}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{	
// 	while((SPI1->SR&SPI_SR_TXE) == RESET);//等待发送寄存器空
//  SPI1->DR = (uint8_t)byte;
//  while((SPI1->SR&SPI_SR_RXNE) == RESET);//等待接收寄存器非空
//  return (uint8_t)SPI1->DR;
		
	 	while((SPI2->SR&SPI_SR_TXE)==RESET);//等待发送寄存器空                               //去掉分号        2016.09.08
		SPI2->DR = (uint8_t)byte;
  while((SPI2->SR&SPI_SR_RXNE)==RESET);//等待接收寄存器非空                               
  return (uint8_t)SPI2->DR;
	

}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
//	 	while((SPI1->SR&SPI_SR_TXE) != SPI_SR_TXE);//等待发送寄存器空
//    SPI1->DR = HalfWord;
//    while((SPI1->SR&SPI_SR_RXNE) != SPI_SR_RXNE);//等待接收寄存器非空
//    return SPI1->DR;
		 	while((SPI2->SR&SPI_SR_TXE) != SPI_SR_TXE);//等待发送寄存器空
    SPI2->DR = HalfWord;
    while((SPI2->SR&SPI_SR_RXNE) != SPI_SR_RXNE);//等待接收寄存器非空
    return SPI2->DR;
	
#if  0  
 // SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
 //   if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback();
   }

  /* Send Half Word through the FLASH_SPI peripheral */
  SPI_I2S_SendData(FLASH_SPI, HalfWord);

//  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
 //   if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback();
   }
  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(FLASH_SPI);
#endif
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Write Enable" instruction */
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

 // SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

//    {
      if((SPITimeout--) == 0) 
      {
				  SPI_FLASH_CS_HIGH();
        return;
      }
//    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */


  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}


//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Power Down" instruction */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}   

//唤醒
void SPI_Flash_WAKEUP(void)   
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Power Down" instruction */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();                   //等待TRES1
}   


///**
//  * @brief  Basic management of the timeout situation.
//  * @param  None.
//  * @retval None.
//  */
//static  uint16_t SPI_TIMEOUT_UserCallback(void)
//{
  /* Block communication and all processes */
 // FLASH_ERROR("SPI Timeout error!");
 // return 0;
//}
/*******************************************************************************
* Function Name  : GetGBKCode_from_FLASH
*******************************************************************************/ 
int GetGBKCode_from_EXFlash(unsigned char* pBuffer,const unsigned char * c)
{ 
    unsigned char High8bit,Low8bit;
    unsigned int pos;
    High8bit=*c;     
    Low8bit=*(c+1);  
	  pos = ((High8bit-0xa0-16)*94+Low8bit-0xa0-1)*2*16;
		SPI_FLASH_BufferRead(pBuffer,4096+pos,32);
		return 0;  
}
