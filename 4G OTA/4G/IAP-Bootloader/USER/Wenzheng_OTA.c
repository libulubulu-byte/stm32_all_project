#include "Wenzheng_OTA.h"
#include "stm32f10x_flash.h"
#include "stmflash.h"
#define FLASH_BASE            (0x08000000UL) /*!< FLASH(up to 128KB) base address */
#define FLASH_END             (0x0801FFFFUL) /*!< FLASH END address                */

//#define FLASH_SIZE_DATA_REGISTER ((uint32_t)0x1FFF75E0)

//#define FLASH_SIZE               (((((*((uint32_t *)FLASH_SIZE_DATA_REGISTER)) & (0x0000FFFFU)) == 0x0000FFFFU)) ? (0x100U << 10U) : \
//                                  (((*((uint32_t *)FLASH_SIZE_DATA_REGISTER)) & (0x0000FFFFU)) << 10U))
//#define FLASH_BANK_SIZE                    (FLASH_SIZE)

/* 标记升级完成 */
void Set_Update_Down(void)
{
    uint8_t update_flag = 0xff;				///< 对应bootloader的启动步骤
    WriteFlash((Application_2_Addr + Application_Size - 8), &update_flag,1 );
}

//static uint32_t GetPage(uint32_t Addr)
//{
//    uint32_t page = 0;

//    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
//    {
//        /* Bank 1 */
//        page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
//        printf("Bank 1 \r\n");
//    }
//    else
//    {
//        /* Bank 2 */
//        page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
//        printf("Bank 2 \r\n");
//    }

//    return page;
//}
int Erase_page2(uint32_t pageaddr)
{
    FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Page_Address));

#ifdef STM32F10X_XL
  if(Page_Address < FLASH_BANK1_END_ADDRESS)  
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    { 
      /* if the previous operation is completed, proceed to erase the page */
      FLASH->CR|= CR_PER_Set;
      FLASH->AR = Page_Address; 
      FLASH->CR|= CR_STRT_Set;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank1Operation(EraseTimeout);

      /* Disable the PER Bit */
      FLASH->CR &= CR_PER_Reset;
    }
  }
  else
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    { 
      /* if the previous operation is completed, proceed to erase the page */
      FLASH->CR2|= CR_PER_Set;
      FLASH->AR2 = Page_Address; 
      FLASH->CR2|= CR_STRT_Set;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(EraseTimeout);
      
      /* Disable the PER Bit */
      FLASH->CR2 &= CR_PER_Reset;
    }
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = pageaddr; 
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    /* Disable the PER Bit */
    FLASH->CR &= CR_PER_Reset;
  }
#endif /* STM32F10X_XL */

  /* Return the Erase Status */
  return status;
}
#define FLASH_SECTOR_SIZE   1024
// addr:地址  count:块数量
int FlashErase(uint32_t addr, uint8_t count)
{
  uint8_t i;
 
  FLASH_Unlock();
 
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
 
  for(i = 0; i < count; ++i)
  {
    if(FLASH_ErasePage(addr + i * FLASH_SECTOR_SIZE) != FLASH_COMPLETE)
    {
      return 0;
    }
  }
 
  FLASH_Lock();
 
  return 1;
}

/**
 * @bieaf 擦除页
 *  L431每页2K  共127页  256K
 * @param pageaddr  起始地址
 * @param num       擦除的页数
 * @return 1
 */
int Erase_page(uint32_t pageaddr, uint32_t num)
{
//	uint32_t i=0;
//	for(i=0;i<num;i++)
//	{
//		Erase_page2(pageaddr+i*PageSize);
//	}
	FlashErase(pageaddr, num);
}

/**
 * @bieaf 写若干个数据
 *
 * @param addr       写入的地址
 * @param buff       写入数据的起始地址
 * @param word_size  长度
 L4 只能双字节写入
 * @return
 */
/**
 * @bieaf 写若干个数据
 *
 * @param addr       写入的地址
 * @param buff       写入数据的起始地址
 * @param word_size  长度
 L4 只能双字节写入
 * @return
 */
uint32_t FlashWrite(uint32_t addr, uint8_t *buffer, uint32_t length)
{
  uint16_t i, data = 0;
 
  FLASH_Unlock();
 
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
 
  for(i = 0; i < length; i += 2)
  {
    data = (*(buffer + i + 1) << 8) + (*(buffer + i));
    if(FLASH_ProgramHalfWord((uint32_t)(addr + i), data) != FLASH_COMPLETE)
    {
      return i;
    }
  }
  
  FLASH_Lock();
 
  return length;
}
uint8_t backups[FLASH_PAGE_SIZE]; 
FLASH_Status stm32_flash_write(uint32_t addr, const uint8_t* buf, uint32_t len)
{
 uint32_t offs = addr % FLASH_PAGE_SIZE; 
 uint32_t flash_ptr = addr - offs; 
 uint32_t* buf_ptr = (uint32_t*)backups; 
 uint16_t count;


 /*Unlock the flash to enable the flash control register access*/
 FLASH_Unlock();
 /* Clear pending flags (if any) */ 
 FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);


 /*Backup the data from falsh memory*/
 memset(backups, 0, FLASH_PAGE_SIZE); 
 memcpy(backups, (uint8_t*)flash_ptr, FLASH_PAGE_SIZE);
 /*Copy my data to backup buffer*/
 memcpy(backups + offs, buf, len);
 /*Erase the whole page*/
 FLASH_ErasePage(flash_ptr);


 /*Write the data into flash memory again*/
 for(count = 0; count < (FLASH_PAGE_SIZE / 4); count++)
 {
 if(FLASH_ProgramWord(flash_ptr, *buf_ptr++) == FLASH_COMPLETE)
 {
 flash_ptr += 4;
 }
 else
 {
FLASH_Lock();
return FLASH_ERROR_PG;
}
}
FLASH_Lock();
 return FLASH_COMPLETE;
}

void WriteFlash(uint32_t addr, uint8_t * buff, uint32_t buf_len)
{
	FlashWrite(addr, buff, buf_len);
}

/**
 * @bieaf 读若干个数据
 *
 * @param addr       读数据的地址
 * @param buff       读出数据的数组指针
 * @param word_size  长度
 * @return
 */
void ReadFlash(uint32_t dest_addr, uint8_t * buff, int buf_len)
{
    uint32_t i;
    for(i = 0; i < buf_len; i++)
    {
        buff[i] = *(__IO uint8_t*)(dest_addr + i);
    }
    return;
}



/* 读取启动模式 */
unsigned int Read_Start_Mode(void)
{
    uint8_t mode = 0;
    ReadFlash((Application_2_Addr + Application_Size - 8), &mode, 1);
    return mode;
}

/* 判断是否有APP */
unsigned int Read_APP_None(void)
{
    uint8_t mode = 0;
    ReadFlash((Application_1_Addr), &mode, 1);
    return mode;
}




/* 采用汇编设置栈的值 */
__asm void MSR_MSP (uint32_t ulAddr)
{
    MSR MSP, r0 			                   //set Main Stack value
    BX r14
}




#if Enable_W25Q
///**
// * @bieaf 每次升级前备份APP1至外部Flash
// * @param App_Addr APP1地址
// * @param W25Q_Addr 外部Flash地址
// * @param App_size  APP大小
// * @return none
// */
//uint8_t Mov_code_W25Q(uint32_t App_Addr,uint32_t W25Q_Addr,unsigned int App_size)
//{
//    uint8_t temp[256];
//	  uint8_t read_buf[1024];
//    printf("> Start copy.to W25Q64.....\r\n");
//	   /* 擦除该扇区 4k*/
//	   for(int i = 0; i < App_size/4096; i++)
//    {
//			printf("-------- erase sector %d -----------\r\n",i);
//      W25QXX_Erase_Sector(i);
//    }
//    for(int i = 0; i < App_size/256; i++)
//    {
//        ReadFlash((App_Addr + i*256), temp, 256);
//			  W25QXX_Page_Program(temp, (W25Q_Addr+i*256), 256);
//    }
//		W25QXX_Read(read_buf, W25Q_Addr, 1024);
//		for(int i = 0; i < 1024; i++)
//    {
//     printf("%x ",read_buf[i]);
//    }
//		return 0;
//}
///**
// * @bieaf 回滚上次备份的APP1程序
// * @param W25Q_Addr 外部Flash地址
// * @param des_addr APP1地址
// * @param byte_size  APP大小
// * @return none
// */
//uint8_t rollback_APP(uint32_t W25Q_Addr,unsigned int des_addr, unsigned int byte_size)
//{
//    uint8_t temp[1024];
//	  printf("> rollback Start rollback......\r\n");
//	  /*1.擦除目的地址*/
//    printf("> rollback Start erase des flash......\r\n");
//    Erase_page(des_addr, (byte_size/PageSize));
//    printf("> rollback Erase des flash down......\r\n");
//    for(int i = 0; i < byte_size/1024; i++)
//    {
//				W25QXX_Read(temp, W25Q_Addr+i*1024, 1024);
//        WriteFlash((des_addr + i*1024), temp, 1024);
//    }
//		return 0;
//}
#endif

