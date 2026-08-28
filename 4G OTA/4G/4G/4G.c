#include "4G.h"
#include "Wenzheng_OTA.h"
#include "test.h"
#include "usart2.h"
void GPIO_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;   //GPIO结构体
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA,PC,PD端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB12 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //
 GPIO_SetBits(GPIOA,GPIO_Pin_7);
	Delay_ms(100);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}
char buff_bag[1200];
void Down_load()
{
	uint8_t bin_version=0,bin_version_last=0;u32 i,j,g=0,buff_length=0;char back_msg[100];char back_msg2[100];char*back_msg3;
	char buff[100];
		AT_Cmd ( "AT+HTTPINIT","OK", NULL, 1500);
		AT_Cmd ( "AT+HTTPPARA=\"URL\",\"http://39.107.239.44:1880/STM32_Version?imei=15514563357\"","OK", NULL, 1500);
		AT_Cmd ( "AT+HTTPACTION=0","OK", NULL, 1500);
		AT_Cmd ( "AT+HTTPREAD=0,1024","OK", NULL, 1500);
		i=Find_string(struct_usart2.USART_BUFF,"bin_version","update_log", back_msg2);
		back_msg2[i]='\0';
		bin_version=Str_To_ASC(back_msg2);
		printf("bin_version %d\r\n",bin_version);
//		ReadFlash(ADDR_4G+104, &bin_version_last, 1);
//		if(bin_version!=bin_version_last)
		{
//			WriteFlash(ADDR_4G+104,&bin_version, 1);
			AT_Cmd ( "AT+HTTPPARA=\"URL\",\"http://39.107.239.44:1880/STM32OTA?imei=15514563357\"", "OK", NULL,2000 ); //请求9K文件
			AT_Cmd ( "AT+HTTPACTION=0","OK", NULL, 2500);
			struct_usart2.USART_BUFF[struct_usart2.USART_Length]=0x36;
			struct_usart2.USART_BUFF[struct_usart2.USART_Length+1]=0x36;
			i=Find_string(struct_usart2.USART_BUFF,"200,","\r\n66", back_msg);
			back_msg[i]='\0';
			buff_length=Str_To_ASC(back_msg);
			printf("buff_length %d\r\n",buff_length);
			i=0;
			Erase_page(Application_2_Addr, ((buff_length)/PageSize));
				for(i=0;i<(buff_length/1024);i++)
				{
					AT_Cmd ( "AT+HTTPREAD=0,1024","OK", NULL, 500);
					for(j=0;j<(struct_usart2.USART_Length);j++)
					{
						if(struct_usart2.USART_BUFF[j]==0x0d&&struct_usart2.USART_BUFF[j+1]==0x0a)
						{
//								printf("%d \r\n",j+2);
							if(j>25)
							{
								break;
							}
						}
//						printf("%c", struct_usart2.USART_BUFF[j]);
					}
					WriteFlash((Application_2_Addr+i*1024),struct_usart2.USART_BUFF+j+2,1024);
//					for(g=0;g<(1024);g++)
//					{
//						printf("%x ", struct_usart2.USART_BUFF[j+2+g]);
//					}
				}
				
				if(buff_length%1024!=0)
				{
					sprintf(buff, "AT+HTTPREAD=0,%d",buff_length%1024);
					AT_Cmd (buff,"OK", NULL, 500);
//					sprintf(buff, "%d\r\n",buff_length%1024);
//					Find_string(struct_usart2.USART_BUFF,buff,"\r\n+HTTPREAD:0", buff_bag);
					for(j=0;j<(struct_usart2.USART_Length);j++)
					{
						if(struct_usart2.USART_BUFF[j]==0x0d&&struct_usart2.USART_BUFF[j+1]==0x0a)
						{
//								printf("%d \r\n",j+2);
							if(j>25)
							{
								break;
							}
						}
//						printf("%c", struct_usart2.USART_BUFF[j]);
					}
					WriteFlash((Application_2_Addr+i*1024),struct_usart2.USART_BUFF+j+2,buff_length%1024);
//					for(g=0;g<(buff_length%1024);g++)
//					{
//						printf("%x ", struct_usart2.USART_BUFF[j+2+g]);
//					}
				}
				Set_Update_Down();
				Delay_ms(10);
				NVIC_SystemReset();
					
					
		
			
		}

}
void OTA_4G(void)
{
	GPIO_init();
	while(!AT_Cmd ( "AT","OK", NULL, 500));
	AT_Cmd ( "ATE0","OK", NULL, 500);
	AT_Cmd ( "AT+CREG?","OK", NULL, 500);
	AT_Cmd ( "AT+CMQTTSTART","OK", NULL, 2500);
	AT_Cmd ( "AT+CMQTTACCQ=0,\"hello123456\"","OK", NULL, 2500);
	AT_Cmd ( "AT+CMQTTCONNECT=0,\"tcp://39.107.239.44:1883\",60,1,\"hello123456\",\"hello123456\"","OK", NULL, 2500);
	AT_Cmd ( "AT+CMQTTSUB=0,27,1","OK", NULL, 500);
	AT_Cmd ( "v1/devices/me/rpc/request/+","OK", NULL, 500);
	Down_load();
	while(1)
	{
		AT_Cmd ( "AT+CMQTTTOPIC=0,23","OK", NULL, 500);
		AT_Cmd ( "v1/devices/me/telemetry","OK", NULL, 500);
		AT_Cmd ( "AT+CMQTTPAYLOAD=0,14","OK", NULL, 500);
		AT_Cmd ( "{\"lizhen\":xin}","OK", NULL, 500);
		AT_Cmd ( "AT+CMQTTPUB=0,1,60","OK", NULL, 500);
		Delay_ms(1500);
	}
}