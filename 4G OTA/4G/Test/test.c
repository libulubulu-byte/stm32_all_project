#include "test.h"
#include "bsp_esp8266.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "malloc.h"
#include <ctype.h>
#include <stdio.h>
#include "Wenzheng_OTA.h"
//#include <iostream>
//#include <string>
#include <time.h>
#include <test.h>
#include "delay.h"
volatile uint8_t ucTcpClosedFlag = 0;
void ESP8266_relink()
{
    uint8_t ucStatus;
    if ( ucTcpClosedFlag )                                             //检测是否失去连接
    {
        ESP8266_ExitUnvarnishSend ();                                    //退出透传模式

        do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
        while ( ! ucStatus );

        if ( ucStatus == 4 )                                             //确认失去连接后重连
        {
            printf ( "\r\n正在重连热点和服务器 ......\r\n" );

            while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );

            while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );

            printf ( "\r\n重连热点和服务器成功\r\n" );

        }

        while ( ! ESP8266_UnvarnishSend () );

    }

}
/***********************************************************
  函数名称：Find_string(char *pcBuf,char*left,char*right, char *pcRes)
  函数功能：寻找特定字符串
  入口参数：
           char *pcBuf 为传入的字符串
           char*left   为搜索字符的左边标识符  例如："["
           char*right  为搜索字符的右边标识符  例如："]"
					 char *pcRes 为输出转存的字符串数组
  返回值：用来校验是否成功，无所谓的。
  备注：
 服务器下发命令举例：+MQTTPUBLISH: 0,0,0,0,/device/NB/zx99999999999999_back,6,[reastrobot]
***********************************************************/
int Find_string(char *pcBuf,char*left,char*right, char *pcRes)
{
	char *pcBegin = NULL;
	char *pcEnd = NULL;
	
	pcBegin = strstr(pcBuf, left);
	pcEnd = strstr(pcBuf, right);
 
	if(pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd)
	{
		printf("Mail name not found!\n");
	}
	else
	{
		pcBegin += strlen(left);
		memcpy(pcRes, pcBegin, pcEnd-pcBegin);
	}
 
	return pcEnd-pcBegin;
}
void SmartConfig(void)
{	uint32_t i;char back_msg[100];char back_msg2[100];
	uint8_t bin_version=0,bin_version_last=0;
	/* 设置WiFi模式
	 * 1：Station 模式
	 * 2：SoftAP 模式
	 * 3：SoftAP+Station 模式
	 */
	printf("1. CWLAP\r\n");
	while(!ESP8266_Cmd ( "AT+CWLAP\r\n", "OK", NULL, 500 ))
	Delay_ms(500);
	printf("2. CWMODE\r\n");
	while(!ESP8266_Cmd ( "AT+CWMODE=1\r\n", "OK", NULL, 1500 ))
	Delay_ms(500);
	/* 开启SmartConfig */
	printf("2. CWSTARTSMART\r\n");
	ESP8266_Cmd3("AT+CWSTARTSMART\r\n", "smartconfig connected wifi", NULL);
	memset(back_msg,0x00,sizeof(back_msg));
	i=Find_string(strEsp8266_Fram_Record .Data_RX_BUF,"ssid:","\r\npassword:", back_msg);
	back_msg[i]='\0';
	ReadFlash(WIFI_Addr+104, &bin_version_last, 1);
	Erase_page(WIFI_Addr, 1);
	WriteFlash(WIFI_Addr+104,&bin_version_last, 1);
	WriteFlash(WIFI_Addr,back_msg, i+1);
	ReadFlash(WIFI_Addr, back_msg, 52);
	printf("%s",back_msg);	
	memset(back_msg2,0x00,sizeof(back_msg2));
	i=Find_string(strEsp8266_Fram_Record .Data_RX_BUF,"password:","\r\nWIFI CONNECTED", back_msg2);
	back_msg2[i]='\0';
	WriteFlash(WIFI_Addr+52,back_msg2, i+1);
	ReadFlash(WIFI_Addr+52, back_msg2, 52);
	printf("\r\n%s",back_msg2);

}
void CLOSE_SmartConfig(void)
{

	/* 关闭SmartConfig */
	printf(" CWSTOPSMART\r\n");
	while(!ESP8266_Cmd("AT+CWSTOPSMART\r\n", "OK", NULL, 1500 ))
		delay_ms(500);	
}
uint32_t numgh=0,T_flag=0,code_sum=0;
void WIFI_DOWNLOAD()
{
	uint32_t i=0,j=0,delay_num=0;uint8_t buff[2]; uint8_t bin_version=0,bin_version_last=0;
  code_sum=0;
    while(1)
    {
				Delay_ms(1);delay_num++;
        if (strEsp8266_Fram_Record .InfBit .FramFinishFlag==1 )
        {
            strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
        }
				if(delay_num>=6000)
				{
					for(i=0; i<strEsp8266_Fram_Record .InfBit .FramLength; i++)
					{
									if(strEsp8266_Fram_Record .Data_RX_BUF[i]=='c'&&strEsp8266_Fram_Record .Data_RX_BUF[i+1]=='l'&&strEsp8266_Fram_Record .Data_RX_BUF[i+2]=='o'&&strEsp8266_Fram_Record .Data_RX_BUF[i+3]=='s'&&strEsp8266_Fram_Record .Data_RX_BUF[i+4]=='e')
									{
										break;
									}
					}
					if(i>500)
					{
						i=0;
					}
					j=i+5+4;//两个回车换行4个字节
					printf("code---- %d\r\n", strEsp8266_Fram_Record .InfBit .FramLength-j);
					ReadFlash(WIFI_Addr+104, &bin_version_last, 1);
					Erase_page(Application_2_Addr, ((strEsp8266_Fram_Record .InfBit .FramLength-j)/PageSize));
					WriteFlash((Application_2_Addr),strEsp8266_Fram_Record .Data_RX_BUF+j, (strEsp8266_Fram_Record .InfBit .FramLength-j));
					ReadFlash(Application_2_Addr, buff, 2);
					WriteFlash(WIFI_Addr+104,&bin_version_last, 1);
					if(buff[0]!=0xff&&buff[1]!=0xff)
					{
						Set_Update_Down();
						Delay_ms(10);
						NVIC_SystemReset();
					}
					
					break;
				}
			}

}
uint32_t cifang(uint8_t n)
{uint8_t i=0;uint32_t sum=1;
		for(i=0;i<n;i++)
		{
			sum=sum*10;
		}

	return sum;
}
/***********************************************************
  函数名称：Str_To_ASC （适用于ME3616）
  函数功能：遍历字符串   逐个处理分析转为ASCII格式给NB
  入口参数：char* a
  出口参数：无
  备 注：
  ##0112QN=20190306152401;ST=32;CN=51;PW=CITC2018+;MN=112019015222;Flag=5 ;CP=&&LA=30;TE=25;HU=75;&&1C80\r\n
***********************************************************/
uint32_t Str_To_ASC(char* a)
{
  int i,k=0,hj=1,nk=0,j;
  uint8_t w;
	uint32_t sum=0;
  int x;
	for (i = 0; a[i] != '\0'; i++)
	{
//		if(a[i]=='-')
//		{
//			hj=1;
//		}
		if (a[i] >= '0' && a[i] <= '9'&&nk==0) //数字处理
    {
			k++;
		}else
		{
			if(k!=0)
			{
				nk=1;
				break;
			}
		}
	}
	j=i;
	i=k;
  while (i--)
  {

		if (a[j-k] >= '0' && a[j-k] <= '9') //数字处理
    {
      w = a[j-k] - '0'; //单个字符转  int
				sum+=w*cifang(i);
    }
		j++;
  }
  return sum;

}
/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */

void ESP8266_StaTcpClient_UnvarnishTest ( void )
{   char *cStr;// [ 100 ] = { 0 };
    struct tm *tn;char back_msg[100];char back_msg2[100];char*back_msg3;
    char shebei_id[30]="TS527A022";
//	char shebei_id[20]="MT419B5032";
    uint32_t TimingDelaynums=0,timestamp_AS=0,i=0,delay_count=0;
    uint8_t bin_version=0,bin_version_last=0;
    char buff[80];
    char buf[160];
    uint32_t	messageid=12133422;
    uint32_t	timestamp=1231323213;
    printf ( "\r\n正在配置 ESP8266 ......\r\n" );
    macESP8266_CH_ENABLE();

    ESP8266_AT_Test ();
		ESP8266_Net_Mode_Choose ( STA );
		ReadFlash(WIFI_Addr+52, back_msg2, 52);
		ReadFlash(WIFI_Addr, back_msg, 52);
		while ( ! ESP8266_JoinAP ( back_msg, back_msg2) )
		{
			delay_count++;
			if(delay_count>=6)//大于1min超时
			{
				SmartConfig();
				CLOSE_SmartConfig();
				break;
			}
		}
    ESP8266_Enable_MultipleId ( DISABLE );
    while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
    while ( ! ESP8266_UnvarnishSend () );
    printf ( "\r\n配置 ESP8266 完毕\r\n" );
//    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
////    cStr=(char*)mymalloc(8192);
// // http://39.107.239.44:1880/STM32_Version?imei=868540050007893
//		memset(back_msg2,0x00,sizeof(back_msg2));
//		ESP8266_Cmd ( "GET http://39.107.239.44:1880/STM32_Version?imei=868540050007893\r\n\r\n", "HTTP/1.1", NULL,2000 );
//		i=Find_string(strEsp8266_Fram_Record .Data_RX_BUF,"bin_version","update_log", back_msg2);
////		back_msg2[i]='\0';
//		bin_version=Str_To_ASC(back_msg2);
//		printf("bin_version %d\r\n",bin_version);
//		numgh=0;
//		memset(strEsp8266_Fram_Record .Data_RX_BUF,0x00,sizeof(strEsp8266_Fram_Record .Data_RX_BUF));
//		strEsp8266_Fram_Record .InfBit .FramLength = 0;
//    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
//		ReadFlash(WIFI_Addr+104, &bin_version_last, 1);
//		if(bin_version!=bin_version_last)
//		{
//			WriteFlash(WIFI_Addr+104,&bin_version, 1);
//			//	ESP8266_Cmd ( "GET http://39.107.239.44:1880/STM32OTA?imei=868540050007893\r\n\r\n", "HTTP/1.1", NULL,2000 ); //请求46K文件
//			ESP8266_Cmd ( "GET http://39.107.239.44:1880/STM32OTA?imei=lizhen\r\n\r\n", "HTTP/1.1", NULL,2000 ); //请求9K文件
//			WIFI_DOWNLOAD();
//		}
//    while(1)
//    {
//        if (strEsp8266_Fram_Record .InfBit .FramFinishFlag==1 )
//        {



//           
//            strEsp8266_Fram_Record .InfBit .FramLength = 0;
//            strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
//            memset(strEsp8266_Fram_Record .Data_RX_BUF,0x00,sizeof(strEsp8266_Fram_Record .Data_RX_BUF));
//        }
//				else
//        {
//            ESP8266_relink();

//        }
//    }


}

