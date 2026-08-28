#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "string.h"
#include "delay.h"

//char back_msg[800];
Usart_Struct struct_usart2;
void usart2_init(u32 band)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  //使能USART2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //

  //重新定义管脚
  //GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
  //USART2_TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // 引脚不能更改
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

  //USART2_RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

  //Usart NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级3 原3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //子优先级3     原2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

  //USART 初始化设置

  USART_InitStructure.USART_BaudRate = band;//串口波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启串口空闲中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2
}
void usart2_Send_Data(u8 *buf, u16 len)
{
  u16 t;

  for (t = 0; t < len; t++) //循环发送数据
  {
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, buf[t]);
  }

  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

}
bool AT_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	uint32_t i=0,delay_count=0;char buff[100];
	struct_usart2.USART_Length = 0;               //从新开始接收新的数据包
	printf ( "AT——send:%s\r\n", cmd );
	sprintf (buff, "%s\r\n", cmd );
	usart2_Send_Data(buff,strlen(buff));

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;

	Delay_ms ( waittime );                 //延时
	
	struct_usart2.USART_BUFF[ struct_usart2.USART_Length ]  = '\0';

	printf ( "%s\r\n", struct_usart2.USART_BUFF );

	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( struct_usart2.USART_BUFF, reply1 ) || 
						 ( bool ) strstr ( struct_usart2.USART_BUFF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( struct_usart2.USART_BUFF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( struct_usart2.USART_BUFF, reply2 ) );
	
}
/*发送一个字节数据*/
void USART2SendByte(unsigned char SendData)
{

  USART_SendData(USART2, SendData);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

/*
  根据长度向串口2 发送数据
*/
void usart2_SendStringByLen(unsigned char * data, u8 len)
{
  u16 i = 0;
  for (i = 0; i < len; i++)
  {
    USART_SendData(USART2, data[i]);         //向串口1发送数据
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET); //等待发送结束
    //i++;
  }
  //i++;
}
/*串口3 发送数据到上位机 data 以\0结尾*/
void usart2_SendString(unsigned char * data)
{
  u16 i = 0;
  while (data[i] != '\0')
  {
    USART_SendData(USART2, data[i]);         //向串口1发送数据
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET); //等待发送结束
    i++;
  }
  i++;
}
/*******************************************************************************
  发送字节
*******************************************************************************/
void uart2_send_char(u8 temp)
{
  USART_SendData(USART2, (u8)temp);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
  发送字符串
*******************************************************************************/
void uart2_send_buff(u8 buf[], u32 len)
{
  u32 i;
  for (i = 0; i < len; i++)
    uart2_send_char(buf[i]);
  // 可设置换行符   NB串口AT指令不需要换行
  uart2_send_char('\r');
  uart2_send_char('\n');
}
void usart2_Sendchar(char * data)
{
  u16 i = 0;
  while (data[i] != '\0')
  {
    USART_SendData(USART2, data[i]);         //向串口2发送数据
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET); //等待发送结束
    i++;
  }
  i++;
}

/***********************************************************
  函数名称：USART2_IRQHandler(void)
  函数功能：串口2中断服务程序
  入口参数：无
  返回值：无
  备注：
   实现了数据接收中断缓存，以及空闲中断处理，实现长字符数据的接收
   同时检测是否有下发命令的数据回传。
***********************************************************/
void USART2_IRQHandler(void)                  //串口2中断服务程序
{
  uint8_t ch;
  uint8_t Clear = Clear;
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    USART_ClearITPendingBit(USART2, USART_IT_RXNE); //接收中断
    ch = USART_ReceiveData(USART2);
    struct_usart2.USART_BUFF[struct_usart2.USART_Length++] = ch;
  }
  if ( USART_GetITStatus( USART2, USART_IT_IDLE ) == SET ) //空闲总线中断
  {
    USART_ClearITPendingBit(USART2, USART_IT_IDLE);
    ch = USART_ReceiveData(USART2);
    if (struct_usart2.USART_Length != 0) //返回值不为空
    {
			struct_usart2.flag=1;
//      if (strstr((char*)struct_usart2.USART_BUFF, "MQTTPUBLISH")) //是否包含回传数据
//      {
//        //printf("\r\nBACK!!!!!%s\r\n",struct_usart2.USART_BUFF);
//        struct_usart2.flag = 2;
//        sprintf(back_msg, "%s", struct_usart2.USART_BUFF); //转存为局部变量（这样）
//        //memset(&struct_usart2.USART_BUFF,0,sizeof(struct_usart2.USART_BUFF));//清空结构体数组(此处不能清空，因为有可能数据正在处理的时候，服务器下发了数据)
//      }
					
    }
  }

}


/*
  USART_IT_PE 奇偶错误中断
  USART_IT_TXE 发送中断
  USART_IT_TC 传输完成中断
  USART_IT_RXNE 接收中断
  USART_IT_IDLE 空闲总线中断
  USART_IT_LBD LIN 中断检测中断
  USART_IT_CTS CTS 中断
  USART_IT_ERR 错误中断

  USART_FLAG_CTS CTS 标志位
  USART_FLAG_LBD LIN 中断检测标志位
  USART_FLAG_TXE 发送数据寄存器空标志位
  USART_FLAG_TC 发送完成标志位
  USART_FLAG_RXNE 接收数据寄存器非空标志位
  USART_FLAG_IDLE 空闲总线标志位
  USART_FLAG_ORE 溢出错误标志位
  USART_FLAG_NE 噪声错误标志位
  USART_FLAG_FE 帧错误标志位
  USART_FLAG_PE 奇偶错误标志位

*/
