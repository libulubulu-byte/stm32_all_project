#ifndef TCP_H
#define TCP_H
#include "stdint.h"

#define Deal_data_MAX 200
//{ "statusCode": 1, "statusString": "OK", "subStatusCode": "OK" }

extern char Register_data_Tx[290];
extern char Register_data_Tx_1[290];
extern char Tcp_header[61];
extern char Heart_Tcp_Tx[104];

extern uint8_t pKey[16];
extern uint32_t byKeyLen;
//extern char Heart_data_Tx[35];
// extern char Heart_data_Tx_1[60];
extern unsigned char Register_flag;//机器注册结果标志，0：未注册；1：注册成功
extern unsigned char Heart_flag;
extern char Capabilities_TCP_Tx[232];

extern char Query_Tcp_Tx_1_1_1[213];
extern char Query_Tcp_Tx_1_1_0[213];
extern char Query_Tcp_Tx_1_0_0[213];

extern char Upload_Alarm_Tcp_Tx_1_1[169];//防区1和防区2短路报警

extern char Upload_Alarm_Tcp_Tx_2_1[169];//防区1和防区2报警恢复


extern char Upload_Alarm_Tcp_Tx_3_1[169];//防区1和防区2短路报警

extern char Upload_Alarm_Tcp_Tx_4_1[169];//防区1和防区2报警恢复


extern unsigned char Query_flag;
extern unsigned char Capabilities_flag;
extern unsigned char FQCF_flag;
extern unsigned char FQBF_flag;
extern unsigned char Fckg_flag;
extern unsigned char Clearalarm_flag;

extern unsigned char  Id_Adderss[2];

extern unsigned char FQBJ_flag_1;
extern unsigned char FQBJ_flag_2;
extern unsigned char FQBJ_flag_3;
extern unsigned char FQBJ_flag_4;
extern unsigned char FQBJ_flag_5;
extern unsigned char FQBJ_flag_6;
extern unsigned char FQBJ_flag_7;
extern unsigned char FQBJ_flag_8;
extern unsigned char FQBJ_flag_9;
extern unsigned char FQBJ_flag_A;
extern unsigned char FQBJ_flag_B;
extern unsigned char FQBJ_flag_C;

extern char Crc16_CCITT[2];//CRC校验值
extern char data[400];
//串口数据

//下位机主动发送给上位机的命令
extern char Command_Register[];//注册命令
extern char Command_Result[];
extern char Command_Heart[]; //心跳包
extern char Command_Upload[]; //设备状态上报

extern char Upload_FCBJ_Tcp_Tx[149];
extern char Upload_FCHF_Tcp_Tx[149];


//上位机主动发送给下位机的命令
extern char Command_Capabilities[]; //设备能力集
// extern char Command_Setuparm[]; //操作防区的撤防布防
extern char Command_Setuparm_BF[]; //操作防区的布防
extern char Command_Setuparm_CF[]; //操作防区的撤防



extern char Command_Enable[]; //防区启用操作使能
extern char Command_Clearalarm[]; //防区消警操作
extern char Command_Alarmtime[]; //持续报警时间设置
extern char Command_Query[]; //设备状态查询

extern char Mac_address[13];
extern char Mac_address_init[18];
extern char Ip_address[16];

extern char Deal_data_Header[60];
extern char Deal_data[Deal_data_MAX];
extern char Deal_data_out[Deal_data_MAX];
extern uint16_t Heart_time_Count;
extern uint16_t Register_data_Count;
extern uint16_t Register_init_Count;
extern char Register_data_flag;
extern uint16_t crctab[256];
extern char gATBuf[60];
extern char gComBuf[300];
extern char Receive_Flag;
extern uint16_t ucCom2RxCnt;      //串口接收数据计数
extern uint8_t  gPCommand_flag;

void Tcp_init( void );
void Information_data_init(void);
void Pkcs5padding_add(char *input);
void Enter_AT_model(void);
void Enter_Entm_model(void);
void Query_Mac(void);
void Query_Ip(void);
void Mac_init(void);
void Ip_init(void);
uint16_t CalcCRC( char *data, uint16_t size);
char *Base64encode( char *str, uint16_t str_length); //unsigned
char *Base64decode( char *code, uint16_t code_length);
void Mac_address_deal (void);
void Register_send(void);
void Heart_send(void);
void Capabilities_send(void);
void FCBJ_send(void);
void FCHF_send(void);
void Query_send_A(void);
void Query_send_B(void);
void Upload_Alarm_send(char *p);
void Query_send(char *p);
void Communicate_Network(void);
void Tcp_Header_init(void);

#endif

