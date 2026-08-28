#include "MQTT_8266.H"
#include "usart.h"
#include "bsp_esp8266.h"
#include "delay.h"
//打印重定向
extern uint8_t USART1_TX_BUF[200];
#define u1_printf(...)  HAL_UART_Transmit(&huart1,USART1_TX_BUF,sprintf((char *)USART1_TX_BUF,__VA_ARGS__),0xffff)
																					
extern uint8_t USART2_TX_BUF[200];
#define u2_printf(...)  HAL_UART_Transmit(&huart2,USART2_TX_BUF,sprintf((char *)USART2_TX_BUF,__VA_ARGS__),0xffff)

uint8_t MQTT_Buf[200];				//存放报文缓存区（随着发送报文的不同，内部数据也将改变）
uint8_t MQTT_Rx_Buf[100];			//接收消息缓存区（这里面的消息是经过处理的完整消息）
uint8_t Rx_Flag;							//判断是否接收到消息。0：没有接受到消息    1：接收到消息
uint8_t Connect_Flag;
uint16_t MQTT_PING_Time;			//PING报文计数变量
//usart1发送和接收数组
uint8_t usart1_txbuf[256];
volatile uint16_t MQTT_TxLen;
#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
const uint8_t parket_connetAck[] = {0x20,0x02,0x00,0x00};
const uint8_t parket_subAck[] = {0x90,0x03};

void usart3_Send_Data(u8 *buf, u16 len)
{
  u16 tp;

  for (tp = 0; tp < len; tp++) //循环发送数据
  {
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    USART_SendData(USART3, buf[tp]);
  }

  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);

}

/*----------------------------------------------------------*/
/*函数名：ESP8266向服务器发送数据                           */
/*参  数：dat:ESP客户端需要发送的具体数据                   */
/*        dat_len：ESP客户端发送数据的长度									*/
/*返回值：无                                                */
/*----------------------------------------------------------*/
void Cliend_Send(uint8_t *dat,uint8_t dat_len)
{
	uint8_t i;char buff[100];	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包
//	sprintf(buff,"AT+CIPSEND=%d\r\n",dat_len);					//ESP8266向服务器发送数据 dat_len 是发送数据长度
//	macESP8266_Usart ( "%s\r\n", buff);
//	Delay_ms(5000);
	usart3_Send_Data(dat,dat_len);
	for(i=0;i<dat_len;i++)
	{
//		macESP8266_Usart ("%c",dat[i]);	//发送的具体数据
		printf("%c",dat[i]);
	}
	printf("\r\n");
	Delay_ms(1000);
}

//MQTT连接服务器的打包函数
uint8_t MQTT_Connect(char *ClientID,char *Username,char *Password)
{
	int ClientIDLen = strlen(ClientID);
	int UsernameLen = strlen(Username);
	int PasswordLen = strlen(Password);
	int DataLen;
	uint8_t cnt=2;
	uint8_t wait=0;
	MQTT_TxLen=0;
	//可变报头+Payload  每个字段包含两个字节的长度标识
  DataLen = 10 + (ClientIDLen+2) + (UsernameLen+2) + (PasswordLen+2);
	
	//固定报头
	//控制报文类型
  usart1_txbuf[MQTT_TxLen++] = 0x10;		//MQTT Message Type CONNECT
	//剩余长度(不包括固定头部)
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		usart1_txbuf[MQTT_TxLen++] = encodedByte;
	}while ( DataLen > 0 );
    	
	//可变报头
	//协议名
	usart1_txbuf[MQTT_TxLen++] = 0;        		// Protocol Name Length MSB    
	usart1_txbuf[MQTT_TxLen++] = 4;        		// Protocol Name Length LSB    
	usart1_txbuf[MQTT_TxLen++] = 'M';        	// ASCII Code for M    
	usart1_txbuf[MQTT_TxLen++] = 'Q';        	// ASCII Code for Q    
	usart1_txbuf[MQTT_TxLen++] = 'T';        	// ASCII Code for T    
	usart1_txbuf[MQTT_TxLen++] = 'T';        	// ASCII Code for T    
	//协议级别
	usart1_txbuf[MQTT_TxLen++] = 4;        		// MQTT Protocol version = 4    
	//连接标志
	usart1_txbuf[MQTT_TxLen++] = 0xc2;        	// conn flags 
	usart1_txbuf[MQTT_TxLen++] = 0;        		// Keep-alive Time Length MSB    
	usart1_txbuf[MQTT_TxLen++] = 60;        	// Keep-alive Time Length LSB  60S心跳包  

	usart1_txbuf[MQTT_TxLen++] = BYTE1(ClientIDLen);// Client ID length MSB    
	usart1_txbuf[MQTT_TxLen++] = BYTE0(ClientIDLen);// Client ID length LSB  	
	memcpy(&usart1_txbuf[MQTT_TxLen],ClientID,ClientIDLen);
	MQTT_TxLen += ClientIDLen;
	
	if(UsernameLen > 0)
	{   
		usart1_txbuf[MQTT_TxLen++] = BYTE1(UsernameLen);		//username length MSB    
		usart1_txbuf[MQTT_TxLen++] = BYTE0(UsernameLen);    	//username length LSB    
		memcpy(&usart1_txbuf[MQTT_TxLen],Username,UsernameLen);
		MQTT_TxLen += UsernameLen;
	}
	
	if(PasswordLen > 0)
	{    
		usart1_txbuf[MQTT_TxLen++] = BYTE1(PasswordLen);		//password length MSB    
		usart1_txbuf[MQTT_TxLen++] = BYTE0(PasswordLen);    	//password length LSB  
		memcpy(&usart1_txbuf[MQTT_TxLen],Password,PasswordLen);
		MQTT_TxLen += PasswordLen; 
	}    
	

	while(cnt--)
	{
		memset(strEsp8266_Fram_Record .Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record .Data_RX_BUF));
		Cliend_Send(usart1_txbuf,MQTT_TxLen);
		wait=80;//等待3s时间
		while(wait--)
		{
			//CONNECT
			if(strEsp8266_Fram_Record .Data_RX_BUF[0]==parket_connetAck[0] && strEsp8266_Fram_Record .Data_RX_BUF[1]==parket_connetAck[1]) //连接成功			   
			{
				printf("连接成功\r\n");
				return 1;//连接成功
			}
			Delay_ms(100);			
		}
	}
	return 0;
}
//void MQTT_SUBSCRIBE(void)
//{
//	u32 i=0;
//	MQTT_Buf[0] = 0x82;				//第1个字节 ：固定0x82
//	MQTT_Buf[1] = 2 + 2 + SUB_Topical_Len + 1;	//第2个字节 ：剩余长度(可变报头+有效负荷的长度)
//	MQTT_Buf[2] = 0x30;				//第3个字节 ：报文标识符高字节（可根据用后自行改变，用于判断服务器返回消息是对哪个主题返回）
//	MQTT_Buf[3] = 0x31;				//第4个字节 ：报文标识符低字节（可根据用后自行改变，用于判断服务器返回消息是对哪个主题返回）
//	
//	/* 主题 */
//	MQTT_Buf[4] =SUB_Topical_Len/256;						//第5个字节 ：主题名称长度高字节
//	MQTT_Buf[5] = SUB_Topical_Len%256;						//第6个字节 ：主题名称长度低字节
//	memcpy(&MQTT_Buf[6],SUB_Topical,SUB_Topical_Len);	//将主题写入 MQTT_Buf 缓存区
//	
//	MQTT_Buf[6 + SUB_Topical_Len] = 0x00;					//消息等级报文Qos:0
//	Cliend_Send(MQTT_Buf,MQTT_Buf[1] + 2);		//发送订阅报文
//	printf("主题：%s  订阅成功\r\n",SUB_Topical);
//		while(1)
//	 {
//		 if (strEsp8266_Fram_Record .InfBit .FramFinishFlag==1 )
//     {


//						for(i=0;i<strEsp8266_Fram_Record .InfBit .FramLength;i++)
//						{
//           printf("%c",strEsp8266_Fram_Record .Data_RX_BUF[i]);
//						}
//            strEsp8266_Fram_Record .InfBit .FramLength = 0;
//            strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
//            memset(strEsp8266_Fram_Record .Data_RX_BUF,0x00,sizeof(strEsp8266_Fram_Record .Data_RX_BUF));
//						break;
//      }
//	 
//	 }
//	Connect_Flag = 1;
//}
//MQTT订阅/取消订阅数据打包函数
//topic       主题 
//qos         消息等级 
//whether     订阅/取消订阅请求包
uint8_t MQTT_SubscribeTopic(char *topic,uint8_t qos,uint8_t whether)
{    
		uint8_t cnt=2;
	uint8_t wait=0;u32 i=0;
		int topiclen = strlen(topic);
	
	int DataLen = 2 + (topiclen+2) + (whether?1:0);//可变报头的长度（2字节）加上有效载荷的长度
	MQTT_TxLen=0;

	//固定报头
	//控制报文类型
	if(whether) usart1_txbuf[MQTT_TxLen++] = 0x82; //消息类型和标志订阅
	else	usart1_txbuf[MQTT_TxLen++] = 0xA2;    //取消订阅

	//剩余长度
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		usart1_txbuf[MQTT_TxLen++] = encodedByte;
	}while ( DataLen > 0 );	
	
	//可变报头
	usart1_txbuf[MQTT_TxLen++] = 0;				//消息标识符 MSB
	usart1_txbuf[MQTT_TxLen++] = 0x01;           //消息标识符 LSB
	//有效载荷
	usart1_txbuf[MQTT_TxLen++] = BYTE1(topiclen);//主题长度 MSB
	usart1_txbuf[MQTT_TxLen++] = BYTE0(topiclen);//主题长度 LSB   
	memcpy(&usart1_txbuf[MQTT_TxLen],topic,topiclen);
	MQTT_TxLen += topiclen;

	if(whether)
	{
		usart1_txbuf[MQTT_TxLen++] = qos;//QoS级别
	}
	

	while(cnt--)
	{
		memset(strEsp8266_Fram_Record .Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record .Data_RX_BUF));
		Cliend_Send(usart1_txbuf,MQTT_TxLen);;
		wait=80;//等待3s时间
		while(wait--)
		{
			//CONNECT
//			if(strEsp8266_Fram_Record .Data_RX_BUF[0]==parket_subAck[0]  && strEsp8266_Fram_Record .Data_RX_BUF[1]==parket_subAck[1]) //连接成功		
		
			if (strEsp8266_Fram_Record .InfBit .FramFinishFlag==1 )
     {
							
						for(i=0;i<strEsp8266_Fram_Record .InfBit .FramLength;i++)
						{
           printf("%x ",strEsp8266_Fram_Record .Data_RX_BUF[i]);
						}
						printf("\r\n");
						if(strEsp8266_Fram_Record .Data_RX_BUF[4]!=0x80) //连接成功		
							{
								printf("订阅成功\r\n");
								return 1;//连接成功
							}
            strEsp8266_Fram_Record .InfBit .FramLength = 0;
            strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
            memset(strEsp8266_Fram_Record .Data_RX_BUF,0x00,sizeof(strEsp8266_Fram_Record .Data_RX_BUF));
      }
			Delay_ms(100);			
		}
		
	}
	if(cnt) return 1;	//订阅成功
	return 0;
}
//MQTT发布数据打包函数
//topic   主题 
//message 消息
//qos     消息等级 
uint8_t MQTT_PublishData(char *topic, char *message, uint8_t qos)
{  
	int topicLength = strlen(topic); uint8_t encodedByte;   
	int messageLength = strlen(message);     
	static uint16_t id=0;
	int DataLen;
	MQTT_TxLen=0;
	//有效载荷的长度这样计算：用固定报头中的剩余长度字段的值减去可变报头的长度
	//QOS为0时没有标识符
	//数据长度             主题名   报文标识符   有效载荷
	if(qos)	DataLen = (2+topicLength) + 2 + messageLength;       
	else	DataLen = (2+topicLength) + messageLength;   

    //固定报头
	//控制报文类型
	usart1_txbuf[MQTT_TxLen++] = 0x30;    // MQTT Message Type PUBLISH  

	//剩余长度
	do
	{
	 encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		usart1_txbuf[MQTT_TxLen++] = encodedByte;
	}while ( DataLen > 0 );	
	
	usart1_txbuf[MQTT_TxLen++] = BYTE1(topicLength);//主题长度MSB
	usart1_txbuf[MQTT_TxLen++] = BYTE0(topicLength);//主题长度LSB 
	memcpy(&usart1_txbuf[MQTT_TxLen],topic,topicLength);//拷贝主题
	MQTT_TxLen += topicLength;
        
	//报文标识符
	if(qos)
	{
			usart1_txbuf[MQTT_TxLen++] = BYTE1(id);
			usart1_txbuf[MQTT_TxLen++] = BYTE0(id);
			id++;
	}
	memcpy(&usart1_txbuf[MQTT_TxLen],message,messageLength);
  MQTT_TxLen += messageLength;
        
	Cliend_Send(usart1_txbuf,MQTT_TxLen);
  return MQTT_TxLen;
}

/*----------------------------------------------------------*/
/*函数名：MQTT DISCONNECT断开连接报文                       */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MQTT_DISCONNECT(void)
{
	MQTT_Buf[0] = 0xE0;				//第1个字节 ：固定0xE0
	MQTT_Buf[1] = 0x00;				//第2个字节 ：固定0x00
	Cliend_Send(MQTT_Buf,2);
	
	Connect_Flag = 0;
}

/*----------------------------------------------------------*/
/*函数名：MQTT PING心跳报文    		                          */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MQTT_PING(void)
{
	MQTT_Buf[0] = 0xC0;				//第1个字节 ：固定0xC0
	MQTT_Buf[1] = 0x00;				//第2个字节 ：固定0x00
	Cliend_Send(MQTT_Buf,2);
}

