#ifndef __MQTT_8266_H__
#define __MQTT_8266_H__

//头文件
//#include "main.h"
#include "string.h"
#include <stdint.h>
#define Sever_IP 					"39.107.239.44"				//服务器域名
#define Sever_IP_Len			strlen(Sever_IP)													//域名长度
#define Sever_Port				1883																			//服务器端口号
#define User_Name					"hello123456"													//用户名
#define	User_Name_Len			strlen(User_Name)													//用户名长度
#define Secre							"hello123456"												//设备秘钥
#define Secre_Len					strlen(Secre)															//设备秘钥长度
#define Device_ID					"hello123456"																	//客户端ID（这个可以根据用户需要自由设置）
#define Device_ID_Len			strlen(Device_ID)													//客户端ID长度
#define SUB_Topical						"v1/devices/me/rpc/request/+"																		//主题名称
#define SUB_Topical_Len				strlen(SUB_Topical)														//主题名称长度
#define PUB_Topical						"v1/devices/me/telemetry"																		//主题名称
#define PUB_Topical_Len				strlen(Topical)														//主题名称长度

void Cliend_Send(uint8_t *dat,uint8_t dat_len);		//ESP8266向服务器发送数据
uint8_t MQTT_Connect(char *ClientID,char *Username,char *Password);											//MQTT CONNECT连接报文
void MQTT_DISCONNECT(void);												//MQTT DISCONNECT断开连接报文
void MQTT_PING(void);															//MQTT PING心跳报文
uint8_t MQTT_SubscribeTopic(char *topic,uint8_t qos,uint8_t whether);												//MQTT SUBSCRIBE订阅报文
uint8_t MQTT_PublishData(char *topic, char *message, uint8_t qos)	;								//MQTT PUBLISH发布消息报文
void MQTT_RxData(char *dat,uint8_t num);					//MQTT 接收消息处理

#endif
