
#ifndef __USART2_H
#define	__USART2_H
#include "stm32f10x.h"
#include <stdio.h>  
#include <stdbool.h>
//extern char back_msg[800];

void usart2_init(u32 band);//串口2 初始化函数
void usart2_SendStringByLen(unsigned char * data,u8 len);//
void usart2_Sendchar(char * data);//
void usart2_SendString(unsigned char * data);
void usart2_Send_Data(u8 *buf,u16 len);
void USART2SendByte(unsigned char SendData);
bool AT_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime );


//定义全局变量结构体用来存储接收数据
typedef struct {
    char USART_BUFF[1600];
    int USART_Length;
    int flag;
}Usart_Struct;
extern  Usart_Struct struct_usart2; 
 
#endif

