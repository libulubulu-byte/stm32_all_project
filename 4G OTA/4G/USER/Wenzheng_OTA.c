#include "Wenzheng_OTA.h"
#include "stm32f10x_flash.h"
//#include "stmflash.h"
//#include "bsp_usart1.h"

//#define FLASH_BASE            (0x08000000UL) /*!< FLASH(up to 128KB) base address */
//#define FLASH_END             (0x0801FFFFUL) /*!< FLASH END address                */

//#define FLASH_SIZE_DATA_REGISTER ((uint32_t)0x1FFF75E0)

//#define FLASH_SIZE               (((((*((uint32_t *)FLASH_SIZE_DATA_REGISTER)) & (0x0000FFFFU)) == 0x0000FFFFU)) ? (0x100U << 10U) : \
//                                  (((*((uint32_t *)FLASH_SIZE_DATA_REGISTER)) & (0x0000FFFFU)) << 10U))
//#define FLASH_BANK_SIZE                    (FLASH_SIZE)

/* 标记升级完成 */
void Set_Update_Down(void)
{
    uint8_t update_flag = 0xAA;				///< 对应bootloader的启动步骤
		 Erase_page((Application_2_Addr + Application_Size - 8), ((Application_2_Addr + Application_Size - 8)-FLASH_BASE)/PageSize);
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


/**
 * @bieaf 进行程序的覆盖
 * @detail 1.擦除目的地址
 *         2.源地址的代码拷贝到目的地址
 *         3.擦除源地址
 *
 * @param  搬运的源地址
 * @param  搬运的目的地址
 * @return 搬运的程序大小
 */
void MoveCode(unsigned int src_addr, unsigned int des_addr, unsigned int byte_size)
{
	    uint8_t temp[1024];int i=0;
    /*1.擦除目的地址*/
    printf("> Start erase des flash......\r\n");
    Erase_page(des_addr, (byte_size/PageSize));
    printf("> Erase des flash down......\r\n");

    /*2.开始拷贝*/


    printf("> Start copy......\r\n");
    for(i = 0; i < byte_size/1024; i++)
    {
        ReadFlash((src_addr + i*1024), temp, 1024);
        WriteFlash((des_addr + i*1024), temp, 1024);
    }
    printf("> Copy down......\r\n");

    /*3.擦除源地址*/
    printf("> Start erase src flash......\r\n");
    Erase_page(src_addr, (byte_size/PageSize));
    printf("> Erase src flash down......\r\n");
}


///* 采用汇编设置栈的值 */
//__asm void MSR_MSP (uint32_t ulAddr)
//{
//    MSR MSP, r0 			                   //set Main Stack value
//    BX r14
//}


/* 程序跳转函数 */
typedef void (*Jump_Fun)(void);
void IAP_ExecuteApp (uint32_t App_Addr)
{
    Jump_Fun JumpToApp;

    if ( ( ( * ( __IO uint32_t * ) App_Addr ) & 0x2FFE0000 ) == 0x20000000 )	//检查栈顶地址是否合法.
    {
        JumpToApp = (Jump_Fun) * ( __IO uint32_t *)(App_Addr + 4);				//用户代码区第二个字为程序开始地址(复位地址)
        MSR_MSP( * ( __IO uint32_t * ) App_Addr );								//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        JumpToApp();															                //跳转到APP.
    } else {
        if(Read_APP_None()==0xFF)
        {
//            LCD_ShowCharStr(0, 210, 240, "> App--NONE!!!", WHITE, RED, 24);
            printf("> App--NONE!!!\r\n");
							//从SPI flash回滚搬运备份程序到APP1
//					  rollback_APP(0,Application_1_Addr,Application_Size);
        }
        else
        {
//            LCD_ShowCharStr(0, 210, 240, "> JumpToApp--ERROR!!!", WHITE, RED, 24);
            printf("JumpToApp--ERROR!!!\r\n");
					  //从SPI flash回滚搬运备份程序到APP1
//					  rollback_APP(0,Application_1_Addr,Application_Size);
        }

    }
}


/**
 * @bieaf 进行BootLoader的启动
 *
 * @param none
 * @return none
 */
void Start_BootLoader(void)
{
    /*==========打印消息==========*/
    printf("\r\n");
    printf("***********************************\r\n");
    printf("*                                 *\r\n");
    printf("*    Wenzheng's BootLoader        *\r\n");
    printf("*                                 *\r\n");
    printf("***********************************\r\n");
    printf("> Choose a startup method......\r\n");
    switch(Read_Start_Mode())									///< 读取是否启动应用程序 */
    {
    case Startup_Normol:										///< 正常启动 */
    {
        printf("> Normal start......\r\n");
//        LCD_ShowCharStr(0, 210, 240, "> > Normal start.", WHITE, BLUE, 12);
        break;
    }
    case Startup_Update:										///< 升级再启动 */
    {
        printf("> Start update......\r\n");
			  //将APP备份到W25Q64中 便于回滚
//			  LCD_ShowCharStr(0, 210, 240, "> > Start backup..", WHITE, BLUE, 12);
//			  Mov_code_W25Q(Application_1_Addr,0,Application_Size);
//        LCD_ShowCharStr(0, 210, 240, "> > Start update..", WHITE, BLUE, 12);
        MoveCode(Application_2_Addr, Application_1_Addr, Application_Size);
        printf("> Update down......\r\n");
//        LCD_ShowCharStr(0, 210, 240, "> > Update down.", WHITE, BLUE, 12);
        break;
    }
    case Startup_Reset:										///<   目前没使用 */
    {
        printf("> Restore to factory program......\r\n");
        break;
    }
    default:														///< 启动失败
    {
        printf("> Error:%X!!!......\r\n", Read_Start_Mode());
//        LCD_ShowCharStr(0, 210, 240, "> > Error...", WHITE, BLUE, 12);
        return;
    }
    }

//    for(int i=0; i<100; i++)
//    {
//        Display_Process_Bar(i,1);
//        HAL_Delay(15);
//    }
    /* 跳转到应用程序 */
    __disable_irq() ;  //很重要！经测试STM32F4必要！  貌似F105也需要   L431 裸机 却不需要  RTOS需要
    printf("> Start up......\r\n\r\n");
//    LCD_ShowCharStr(0, 210, 240, "> > Start up..", WHITE, BLUE, 12);
    IAP_ExecuteApp(Application_1_Addr);
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

