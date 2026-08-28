#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f10x.h"



/********************************** 用户需要设置的参数**********************************/
//#define      macUser_ESP8266_ApSsid                     "MZ"    // "OPPO A5"                //要连接的热点的名称
//#define      macUser_ESP8266_ApPwd                        "Di431122"           //要连接的热点的密钥
//#define      macUser_ESP8266_ApSsid                     "ChinaNet-KRWH"    // "OPPO A5"                //要连接的热点的名称
//#define      macUser_ESP8266_ApPwd                        "9cuakykr"           //要连接的热点的密钥
#define      macUser_ESP8266_ApSsid                     "OPPO A5"    // "OPPO A5"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "12345678"           //要连接的热点的密钥

#define      macUser_ESP8266_TcpServer_IP                 "39.107.239.44"      //要连接的服务器的 IP
#define      macUser_ESP8266_TcpServer_Port               "1883"               //要连接的服务器的端口
//#define      macUser_ESP8266_TcpServer_IP                 "39.107.239.44"      //要连接的服务器的 IP
//#define      macUser_ESP8266_TcpServer_Port               "1880"               //要连接的服务器的端口
//#define      macUser_ESP8266_TcpServer_IP                 "192.168.1.115"      //要连接的服务器的 IP
//#define      macUser_ESP8266_TcpServer_Port               "80"               //要连接的服务器的端口

/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;

extern uint32_t T_flag,numgh;

/********************************** 测试函数声明 ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );
void UP_SZ_data(char *shebei_id,uint32_t	messageid,uint32_t	timestamp,uint8_t paixunum);
int Find_string(char *pcBuf,char*left,char*right, char *pcRes);
uint32_t Str_To_ASC(char* a);
#endif

