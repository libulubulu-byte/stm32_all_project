#include "led.h"
#include "delay.h"
#include "key.h"
#include "Wenzheng_OTA.h"
#include "lcd.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h"
#define      GPIO_APBxClock_FUN                   						RCC_APB2PeriphClockCmd
#define      RS485_CLK																				RCC_APB2Periph_GPIOA
#define      RS485_PORT																				GPIOA
#define      RS485_PIN                             						GPIO_Pin_0
#define      Wifiled_PORT																			GPIOA
#define      Wifiled_PIN                             					GPIO_Pin_8
#define      NETled_PORT																			GPIOA
#define      NETled_PIN                             					GPIO_Pin_12
#define      ERRled_PORT																			GPIOA
#define      ERRled_PIN                             					GPIO_Pin_11
#define     Wifiled_Off()                 GPIO_SetBits ( Wifiled_PORT	,Wifiled_PIN )
#define     Wifiled_ON()               GPIO_ResetBits ( Wifiled_PORT	,Wifiled_PIN )
#define     NETled_Off()                 GPIO_SetBits ( NETled_PORT	,NETled_PIN )
#define     NETled_ON()               GPIO_ResetBits ( NETled_PORT	,NETled_PIN )
#define     ERRled_Off()                 GPIO_SetBits ( ERRled_PORT	,ERRled_PIN )
#define     ERRled_ON()               GPIO_ResetBits (ERRled_PORT	,ERRled_PIN )
 static void MX_GPIO_Init(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 配置 CH_PD 引脚*/

	GPIO_APBxClock_FUN ( RS485_CLK, ENABLE ); 
											   
	GPIO_InitStructure.GPIO_Pin = RS485_PIN;	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_Init ( RS485_PORT, & GPIO_InitStructure );	
	
	GPIO_InitStructure.GPIO_Pin = Wifiled_PIN|NETled_PIN|ERRled_PIN;	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_Init ( Wifiled_PORT, & GPIO_InitStructure );	
}
int main(void)
{		

	u16 oldcount=0;				//老的串口接收数据值
	u16 applenth=0;				//接收到的app代码长度
	uint8_t clearflag=0;
	uint8_t a=0;
	
	uint8_t app_bin = 0;
	uint8_t app_enter = 0;
	MX_GPIO_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	//串口初始化为115200
	delay_init();	   	 	//延时初始化 
//	KEY_Init();					//初始化按键
//		NETled_Off();
//		ERRled_Off();
//		Wifiled_Off();
		//Wifiled_ON() ;
	
	printf("kaishi!\r\n");
	while(1)
{
	Start_BootLoader();
	delay_ms(5000);
	
}
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
void MoveCode(unsigned int src_addr, unsigned int des_addr, uint32_t byte_size)
{
	    uint8_t temp[1024];int i=0;int j=0;
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
//		 ReadFlash((src_addr), temp, 20);
//					for(j =0;j<20;j++)
//				{
//					printf("%c *",temp[j]);
//				}
//    printf("> Copy down......\r\n");

    /*3.擦除源地址*/
    printf("> Start erase src flash......\r\n");
    Erase_page(src_addr, (byte_size/PageSize));
    printf("> Erase src flash down......\r\n");
}
	/* 程序跳转函数 */
typedef void (*Jump_Fun)(void);
void IAP_ExecuteApp (uint32_t App_Addr)
{
    Jump_Fun JumpToApp;

    if ( ( ( * ( __IO uint32_t * ) App_Addr ) & 0x2FFE0000 ) == 0x20000000 )	//检查栈顶地址是否合法.
    {
        JumpToApp = (Jump_Fun) * ( __IO uint32_t *)(App_Addr + 4);				//用户代码区第二个字为程序开始地址(复位地址)
        MSR_MSP( * ( __IO uint32_t * ) App_Addr );								//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
			printf("APP OK !!!!!!!!!!!!!\r\n");
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
			Set_Update_Down();
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
//    __disable_irq() ;  //很重要！经测试STM32F4必要！  貌似F105也需要   L431 裸机 却不需要  RTOS需要
    printf("> Start up......\r\n\r\n");
//    LCD_ShowCharStr(0, 210, 240, "> > Start up..", WHITE, BLUE, 12);
    IAP_ExecuteApp(Application_1_Addr);
}











