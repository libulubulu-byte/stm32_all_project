#include "MQTT_8266.H"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "bsp_esp8266.h"
#include "test.h"
#include "usart2.h"
#include "4G.h"
/************************************************
 ALIENTEK精英STM32开发板实验4
 串口 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
 C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin --output .\OBJ\wifi.bin .\OBJ\*.axf
************************************************/
 int main(void)
 {		
	
 	u32 i;  
//	u16 len;	
//	u16 times=0;
	 SCB->VTOR = FLASH_BASE | 0x8003000U	; 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	usart2_init(115200);
	 printf("4G..........\r\n");
	OTA_4G();
 	while(1)
	{
		Delay_ms(500);
		if (struct_usart2.flag==1 )
     {


//						for(i=0;i<struct_usart2.USART_Length;i++)
//						{
//           printf("%c",struct_usart2.USART_BUFF[i]);
//						}
            struct_usart2.USART_Length = 0;
            struct_usart2.flag= 0;
            memset(struct_usart2.USART_BUFF,0x00,sizeof(struct_usart2.USART_BUFF));
      }


	}	 
 }

