#include "uart_tcp.h"
#include "AES.h"
#include "var.h"
#include "string.h"
#include "usart.h"
#include "gpio.h"
#include "SEGGER_RTT.h"
#include "pcf8562.h"
#include "keyboard.h"

#define BOOTLOADER_START_ADDR  0X08000000

const uint16_t crctab[256] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
const char tcp_header[61] = {"HK0100000000000000000000000000000000000000000000111100000000"};
const char command_register[] = {"CrBFXv2EfsBOnSa6pVu+QCxXHDOAZoOFNzt7PAbwFQOsroE0OjzLIfKs6m+e/h0JDBu9fhy4XUnSpyHMMtk/n+HsRuGz5B4utgKeaNFpWKQba5Z34acVRVenXu/RmwHR"};
const char heart_beats[44] = {"vju2cz2Oo5RwnsUAX6FMLdMGGiAJfdQ2DObTiBBE6FE="};
const char relaytime_ok[65] = {"wzCSbNo9nCgekK3pH4fKH+9QX+4kIl5V+HvMzZuvDDqrL+iMpqO8BJU6+xK/AFCu"};
const char relaytime_error[65] = {"wzCSbNo9nCgekK3pH4fKH+9QX+4kIl5V+HvMzZuvDDocO69uu/Hn5LRBbYvXeFM2"};

const char tamper_alarm_upload[89] = {"wzCSbNo9nCgekK3pH4fKHzODE4MfKOD5KNQS1aeiYrrseTJMYty/oKBMDwJuVRk6Jgn71uuw5pdFtcZYQVPvig=="}; //{"Command":"COM_UPLOAD_DEVICE_ALARM","deviceType":1}
const char tamper_alarm_remove[89] = {"wzCSbNo9nCgekK3pH4fKHzODE4MfKOD5KNQS1aeiYrrseTJMYty/oKBMDwJuVRk6CRNUHxL03bUjk/K4ew8UmQ=="}; //{"Command":"COM_UPLOAD_DEVICE_ALARM","deviceType":2}


char alarm_status_1_10[128]; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":1,"zoneStatus":"10"}]}}
char alarm_status_2_10[128]; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":2,"zoneStatus":"10"}]}}
char alarm_status_4_10[128]; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":4,"zoneStatus":"10"}]}}

char alarm_status_1_01[128]; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":1,"zoneStatus":"01"}]}}
char alarm_status_4_01[128]; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":4,"zoneStatus":"01"}]}}

char command_query_reply[220];
char command_query_reply_1[220];



const char alarm_status_1_11[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAmLOTxWgXTOkBpLd4scyFaV"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":1,"zoneStatus":"11"}]}}
//const char alarm_status_1_10[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAlA+FfCjVqc0iPqtjFMFcyb"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":1,"zoneStatus":"10"}]}}
//const char alarm_status_1_01[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAk5w4T5PQ0yMM5wm0iFnFq5"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":1,"zoneStatus":"01"}]}}

const char alarm_status_2_11[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLHBu0aYwG41mcVdKscVPsH6LOTxWgXTOkBpLd4scyFaV"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":2,"zoneStatus":"11"}]}}
//const char alarm_status_2_10[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLHBu0aYwG41mcVdKscVPsH5A+FfCjVqc0iPqtjFMFcyb"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":2,"zoneStatus":"10"}]}}
const char alarm_status_2_01[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLHBu0aYwG41mcVdKscVPsH45w4T5PQ0yMM5wm0iFnFq5"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":2,"zoneStatus":"01"}]}}

const char alarm_status_4_11[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFGLOTxWgXTOkBpLd4scyFaV"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":4,"zoneStatus":"11"}]}}
//const char alarm_status_4_10[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFFA+FfCjVqc0iPqtjFMFcyb"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":4,"zoneStatus":"10"}]}}
//const char alarm_status_4_01[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFE5w4T5PQ0yMM5wm0iFnFq5"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":4,"zoneStatus":"01"}]}}

const char alarm_status_1_100[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAmBwLBiaMZ1fGk3U3KJt8hK"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":1,"zoneStatus":"100"}]}}
const char alarm_status_2_100[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLHBu0aYwG41mcVdKscVPsH6BwLBiaMZ1fGk3U3KJt8hK"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":2,"zoneStatus":"100"}]}}
const char alarm_status_4_100[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFGBwLBiaMZ1fGk3U3KJt8hK"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":4,"zoneStatus":"100"}]}}

const char alarm_status_1_010[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAm05alBcr+Qe+B55MLbafZt"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":1,"zoneStatus":"010"}]}}
const char alarm_status_2_010[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLHBu0aYwG41mcVdKscVPsH605alBcr+Qe+B55MLbafZt"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":2,"zoneStatus":"010"}]}}
const char alarm_status_4_010[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFG05alBcr+Qe+B55MLbafZt"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":4,"zoneStatus":"010"}]}}

const char alarm_status_1_001[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAnSlhmOQb3kVRJiGsXV46l+"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":1,"zoneStatus":"001"}]}}
const char alarm_status_4_001[128] = {"wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFHSlhmOQb3kVRJiGsXV46l+"}; //{"Command":"COM_UPLOAD_ZONE_STATUS","Data":{"zoneAlarm":[{"deviceType":4,"zoneStatus":"001"}]}}

//const char command_query_reply[220] = {"{\"Command\":\"COM_QUERY_DEV_STATUS\",\"zoneEnable\":\"111\",\"zoneArmStatus\":\"111\",\"zoneAlarm\":\"000\",\"powerStatus\":1,\"tamperEnable\":1,\"tamperStatus\":0,\"infraredStatus\":0,\"commStatus\":0}"};
//const char command_query_reply_1[220] = {"{\"Command\":\"COM_QUERY_DEV_STATUS\",\"zoneEnable\":\"11\",\"zoneArmStatus\":\"11\",\"zoneAlarm\":\"00\",\"powerStatus\":1,\"tamperEnable\":1,\"tamperStatus\":0,\"infraredStatus\":0,\"commStatus\":0}"};


UART_HandleTypeDef huart1;
TCP_Msg_TypeDef xTCP_Msg;
TCP_Data_TypeDef xTCP_Data;
TCP_Addr_TypeDef xTCP_Addr;

extern DEVICEDATA device;

static uint8_t bTCPRecvOK;
static uint8_t bATRecvOK;

static uint8_t bTCPConnected;
static uint8_t ucUartTCPAlarmed;
static uint8_t connect_status;
static uint8_t ucTCPAlarmStatus;
static uint8_t ucTCPAlarmStatusLast;
static char xTCPHeader[60];
static char xTCPPacket[320];
static uint8_t repeat_time;
uint8_t TCP_AlarmTxFlag = 0;
uint8_t ucUartTCPAlarmedLast = 0;
uint8_t InitSendAlarmFlag = 0;
uint8_t TCP_AlarmTxFlagCnt = 0;

static uint8_t pKey[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0x01, 0x23, 0x45, 0x67, 0x89, 0x01, 0x23, 0x45, 0x67, 0x89, 0x12};
const char register_cmd[] = {"{\"Command\":\"COM_DEV_REGISTER\",\"Data\":{\"deviceID\":\"1003202000FF81992F49\",\"manufacturer\":\"1003\",\"ip\":\""};
static char register_cmd_end[120] = {"\",\"mac\":\"00:FF:81:99:2F:49\",\"version\":\"V2.0.03_20250429001\",\"userID\":\"000000001\",\"bSupExpandZone\":0,\"deviceType\":241}}"};
uint8_t firmware_vison[8]= {2, 0, 0, 3, 20, 25, 4, 29};
const char query_cmd[] = {"{\"Command\":\"COM_QUERY_DEV_STATUS\",\"Data\":{\"deviceID\":\"1003202000FF81992F49\"}}"};
const char ZONE_SETUP[] = {"{\"Command\":\"COM_ZONE_SETUPARM\",\"Result\":0}"};
static char register_data[250] = {0};
static char register_data_encrypt[320] = {0};

void IpAddrHexToAscii(uint8_t *str1, uint8_t *str2);
uint8_t MacHexToAscii(uint8_t data);

uint8_t Uart1RstFlag = 0;
uint16_t Uart1RstCnt = 0;

/* USART1 init function */
void HAL_UART_TCP_MspDeInit(void)
{
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
}

void HAL_UART_TCP_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void MX_USART_TCP_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

uint8_t upper_flag;         //上位机协议接受成功标志位
uint8_t upper_buf[20];
uint8_t IAP_flag=0;
uint8_t RX_buf[300];
extern uint8_t firmware_vison[8];
typedef void (*iapfun)(void);                   /* 定义一个函数类型的参数 */
iapfun jump2app;
uint16_t refresh_flag = 0;               //刷新标志

///**************************************************************************************
//延时函数ms
//入口参数:
//              num_ms：延时的毫秒数
//
//返回参数：
//        无
// **************************************************************************************/
void Delay_ms(uint16_t num_ms)
{
    uint16_t i;

    while (num_ms--)
    {
        i=20000;

        while (i--);
    }
}

/**
 * @brief       设置栈顶地址
 * @param       addr: 栈顶地址
 * @retval      无
 */
void sys_msr_msp(uint32_t addr)
{
    __set_MSP(addr);    /* 设置栈顶地址 */
}

/**
 * @brief       跳转到应用程序段(执行APP)
 * @param       appxaddr : 应用程序的起始地址

 * @retval      无
 */
void iap_load_app(uint32_t appxaddr)
{
    if (((*(volatile  uint32_t *)appxaddr) & 0x2FFE0000) == 0x20000000)     /* 检查栈顶地址是否合法.可以放在内部SRAM共64KB(0x20000000) */
    {
        /* 用户代码区第二个字为程序开始地址(复位地址) */
        jump2app = (iapfun) * (volatile uint32_t *)(appxaddr + 4);

        /* 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址) */
        sys_msr_msp(*(volatile uint32_t *)appxaddr);

        /* 跳转到APP */
        jump2app();
    }
}

void CMD_Deal_With(void)
{
    EEprom_Msg_TypeDef xEEpromWriteData;
    uint8_t updata_len=0;
    uint8_t rep_len=0;
    uint8_t send_buf[100];
    uint8_t type=0;

    upper_flag=0;
    updata_len=upper_buf[3];

    if ((upper_buf[updata_len-2]==CalcCRC((char*)upper_buf, updata_len-2)/256)&&(upper_buf[updata_len-1]==CalcCRC((char*)upper_buf, updata_len-2)%256))
    {
        send_buf[rep_len++]=0x55;
        send_buf[rep_len++]=0xAA;
        send_buf[rep_len++]=upper_buf[2];

        if (upper_buf[2]==0xA1)             //版本查询指令
        {
            send_buf[rep_len++]=16;     //帧长度

            for (uint8_t j=0; j<8; j++) //版本号
            {
                send_buf[rep_len++]=firmware_vison[j];
            }

            if (SIX_LINE_BOARD==1)  //6线
            {
                type=type|0x04;

                if (DUAL_DEFENCE==0)    //单防
                {
                    type=type|0x02;
                }
            }

            type=type|0x08;
            send_buf[rep_len++]=type;                       // 设备类型
            send_buf[rep_len++]=0xAA;       //  AA：app    BB：bootload
            send_buf[rep_len++]=CalcCRC((char*)send_buf, 14)/256;
            send_buf[rep_len++]=CalcCRC((char*)send_buf, 14)%256;
            HAL_UART_Transmit(&huart1, send_buf, 16, 1000);
        }

        else if (upper_buf[2]==0xA2)            //屏蔽注册
        {
            if (IAP_flag==0)
            {
                IAP_flag=1;
            }
            else
            {
                IAP_flag=0;
            }

            send_buf[rep_len++]=7;      //帧长度
            send_buf[rep_len++]=IAP_flag;           //
            send_buf[rep_len++]=CalcCRC((char*)send_buf, 5)/256;
            send_buf[rep_len++]=CalcCRC((char*)send_buf, 5)%256;
            HAL_UART_Transmit(&huart1, send_buf, 7, 1000);
        }

        else if (upper_buf[2]==0xB1)            //跳转到BOOTLOAD
        {
            uint8_t updata_flag[2]= {0xAA, 0xBB};
            uint8_t updata_read_flag[2]= {0};
            IAP_flag=1;

            if ((updata_len == 7)&&upper_buf[4]==0xBB)
            {
                if ((*((uint32_t *)BOOTLOADER_START_ADDR)!=0xFFFFFFFF))
                {
                    AT24CXX_Write(0X400, updata_flag, 2);
                    Delay_ms(10);
                    AT24CXX_Read(0X400, updata_read_flag, 2);

                    if (((updata_read_flag[0]==updata_flag[0])&&(updata_read_flag[1]==updata_flag[1])))     //校验成功，准备升级
                    {
                        HAL_NVIC_SystemReset();
                    }
                    else                                    //跳转失败
                    {
                        send_buf[rep_len++]=8;
                        send_buf[rep_len++]=0xAA;
                        send_buf[rep_len++]=1;
                        send_buf[rep_len++]=CalcCRC((char*)send_buf, 6)/256;
                        send_buf[rep_len++]=CalcCRC((char*)send_buf, 6)%256;
                        HAL_UART_Transmit(&huart1, send_buf, 8, 1000);
                    }
                }
                else                                    //跳转失败
                {
                    send_buf[rep_len++]=8;
                    send_buf[rep_len++]=0xAA;
                    send_buf[rep_len++]=2;
                    send_buf[rep_len++]=CalcCRC((char*)send_buf, 6)/256;
                    send_buf[rep_len++]=CalcCRC((char*)send_buf, 6)%256;
                    HAL_UART_Transmit(&huart1, send_buf, 8, 1000);
                }
            }
        }
        else if (upper_buf[2]==0xC1)    //脉冲主机设备信息查询与设置
        {
            if (updata_len==6)                  //查询
            {
                send_buf[rep_len++]=79;   //3 应答帧长度

                if (SIX_LINE_BOARD==1)  //6线
                {
                    type=type|0x04;

                    if (DUAL_DEFENCE==0)    //单防
                    {
                        type=type|0x02;
                    }
                }

                type=type|0x08;
                send_buf[rep_len++]=type;                       //4 设备类型
                send_buf[rep_len++]=0;                          //5 在线传感器数量
                send_buf[rep_len++] = 0;                        //6 离线传感器数量
                send_buf[rep_len++] = device.ucPulseOn;         //7 布撤防状态
                send_buf[rep_len++] = device.ucIREn;            //8 开关量布撤防状态
                send_buf[rep_len++] = 0;                        //9 开关量2布撤防状态
                send_buf[rep_len++] =device.ucSetVoltage[0];    //10 灵敏度/报警电压
                send_buf[rep_len++] =0;                         //11 报警阈值

                send_buf[rep_len++] = device.ucRelayOnTime /1000; // 550

                for (uint8_t j=0; j<8; j++)
                {
                    send_buf[j+rep_len] =firmware_vison[j];                     //13-20版本信息
                }

                rep_len=21;
                send_buf[rep_len++] =0;                                     //21 年
                send_buf[rep_len++] =0;                                     //22 月
                send_buf[rep_len++] =device.ucRunningYear;                  //23运行年
                send_buf[rep_len++] =device.uiRunningDate / 256;            //24 运行天数
                send_buf[rep_len++] =device.uiRunningDate % 256;             //25 运行天数
                send_buf[rep_len++] =device.ucRunningHour;                   //26 运行小时
                send_buf[rep_len++] =device.ucRunningMin;                    //27 日

                for (uint8_t j=0; j<6; j++)                                  //实时电压 28-75
                {
                    send_buf[j+rep_len++] =device.ucVoltage[0];
                    send_buf[j+rep_len++] =device.ucCurrent[0];
                    send_buf[j+rep_len++] =device.ucVoltage[1];
                    send_buf[j+rep_len++] =device.ucCurrent[0];
                    send_buf[j+rep_len++] =device.uiBaudrate/256;
                    send_buf[j+rep_len++] =device.uiBaudrate%256;
                    send_buf[j+rep_len++] =0;
                    send_buf[j+rep_len++] =0;
                }

                rep_len=76;
                send_buf[rep_len++] =   get_addr();
                send_buf[rep_len++]=CalcCRC((char*)send_buf, send_buf[3]-2)/256;
                send_buf[rep_len++]=CalcCRC((char*)send_buf, send_buf[3]-2)%256;
                HAL_UART_Transmit(&huart1, send_buf, 79, 1000);
            }
            else if (updata_len==17)
            {
                device.ucLcdOnTime = 120;
                lcd_backup_on();

                if (upper_buf[4]==1)
                {
                    device.ucPulseOn |= 0x03;
                }
                else
                {
                    device.ucPulseOn &= 0xfc;
                }

                if (upper_buf[5]==1)
                {
                    device.ucIREn = 0xAA;
                }
                else
                {
                    device.ucIREn = 0;
                }

                device.uiBaudrate=upper_buf[6]*256+upper_buf[9];    //保存波特率
                xEEpromWriteData.ucCmd = 0x03;
                xEEpromWriteData.ucData[0] = device.uiBaudrate >> 8;
                xEEpromWriteData.ucData[1] = device.uiBaudrate & 0xff;
                UART_Init(device.uiBaudrate);
                osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);

                device.ucSetVoltage[0] = upper_buf[7];      //保存设置电压
                device.ucSetVoltage[1] = upper_buf[7];      //保存设置电压

                device.ucRelayOnTime = upper_buf[8]*1000;   //保存设置的报警时长

                device.ucBeepOnTime=device.ucRelayOnTime;
                xEEpromWriteData.ucCmd = 0x06;
                xEEpromWriteData.ucData[0] = device.ucBeepOnTime/1000;
                xEEpromWriteData.ucData[1] = 0x00;
                xEEpromWriteData.ucData[2] = 0x00;
                xEEpromWriteData.ucData[3] = 0x00;
                xEEpromWriteData.ucData[4] = device.ucRelayOnTime/1000;
                osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xff);

                send_buf[rep_len++]=6;   //3 应答帧长度
                send_buf[rep_len++]=0X72;   //
                send_buf[rep_len++]=0X40;   //
                HAL_UART_Transmit(&huart1, send_buf, 6, 1000);
            }
        }
        else if (upper_buf[2]==0xC2)    //实时查询电压电流
        {
            if (updata_len==7)                  //查询
            {
                if (upper_buf[4]==1)
                {
                    send_buf[rep_len++]=7;
                    send_buf[rep_len++]=1;
                    send_buf[rep_len++]=0x64;
                    send_buf[rep_len++]=0X95;
                    refresh_flag=1;
                }
                else
                {
                    send_buf[rep_len++]=7;
                    send_buf[rep_len++]=0;
                    send_buf[rep_len++]=0x74;
                    send_buf[rep_len++]=0XB4;
                    refresh_flag=0;
                }

                HAL_UART_Transmit(&huart1, send_buf, 7, 1000);
            }
        }
        else if (upper_buf[2]==0xC3)    //撤布防
        {
            if (updata_len==7)
            {
                if (upper_buf[4]==1)
                {
                    send_buf[rep_len++]=7;
                    send_buf[rep_len++]=1;
                    send_buf[rep_len++]=0x53;
                    send_buf[rep_len++]=0XA5;
                    device.ucPulseOn |= 0x03;
                }
                else
                {
                    send_buf[rep_len++]=7;
                    send_buf[rep_len++]=0;
                    send_buf[rep_len++]=0x43;
                    send_buf[rep_len++]=0X84;
                    device.ucPulseOn &= 0xfc;
                }

                HAL_UART_Transmit(&huart1, send_buf, 7, 1000);
            }
        }
        else if (upper_buf[2]==0xC4)    //开关量1
        {
            if (updata_len==7)
            {
                if (upper_buf[4]==1)
                {
                    send_buf[rep_len++]=7;
                    send_buf[rep_len++]=1;
                    send_buf[rep_len++]=0xD6;
                    send_buf[rep_len++]=0X35;
                    device.ucIREn = 0xAA;
                }
                else
                {
                    send_buf[rep_len++]=7;
                    send_buf[rep_len++]=0;
                    send_buf[rep_len++]=0xC6;
                    send_buf[rep_len++]=0X14;
                    device.ucIREn = 0;
                }

                HAL_UART_Transmit(&huart1, send_buf, 7, 1000);
            }
        }
        else if (upper_buf[2]==0xD1)    //恢复出厂
        {
            if (updata_len==6)
            {
                xEEpromWriteData.ucCmd = 0x04;
                xEEpromWriteData.ucData[0] = 0x00;
                xEEpromWriteData.ucData[1] = 0x00;
                xEEpromWriteData.ucData[2] = 0x00;
                xEEpromWriteData.ucData[3] = 0x00;
                xEEpromWriteData.ucData[4] = 0x00;
                osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
                device.ucRunningYear = 0;
                device.uiRunningDate = 0;
                device.ucRunningHour = 0;
                device.ucRunningMin = 0;
                beep_on();
                osDelay(100);
                beep_off();

                device.ucSetVoltage[0] = 45;
                AT24CXX_WriteOneByteN(EE_VOLTAGE1, device.ucSetVoltage[0]);
                osDelay(20);
                device.ucSetVoltage[1] = 45;
                AT24CXX_WriteOneByteN(EE_VOLTAGE2, device.ucSetVoltage[1]);
                osDelay(20);
                device.ucIREn = 0x55;
                AT24CXX_WriteOneByte(EE_SWITCHEN, 0x55);
                osDelay(20);
                device.ucForceSync = 0x01;
                device.bOnSetting = 0 ;
                KeySetIpFlag = 1;

                send_buf[rep_len++]=6;
                send_buf[rep_len++]=0x71;
                send_buf[rep_len++]=0X33;
                HAL_UART_Transmit(&huart1, send_buf, 6, 1000);


            }
        }
        else if (upper_buf[2]==0xD4)    //重启设备
        {
            if (updata_len==6)
            {
                NVIC_SystemReset();
            }
        }
    }

    memset(upper_buf, 0, 100);
}


void UART_TCP_init(void)
{
    HAL_UART_DeInit(&huart1);
    HAL_UART_TCP_MspInit();
    MX_USART_TCP_Init();
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_IT(&huart1, RX_buf, 300);
}

void DefenceAreaSwitchTcpAck(void)
{
    if (DUAL_DEFENCE == 0)
    {
        memcpy(alarm_status_1_10, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAlA+FfCjVqc0iPqtjFMFcyb", 128);
        memcpy(alarm_status_2_10, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLHBu0aYwG41mcVdKscVPsH5A+FfCjVqc0iPqtjFMFcyb", 128);
        memcpy(alarm_status_4_10, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFFA+FfCjVqc0iPqtjFMFcyb", 128);

        memcpy(alarm_status_1_01, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAk5w4T5PQ0yMM5wm0iFnFq5", 128);
        memcpy(alarm_status_4_01, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFE5w4T5PQ0yMM5wm0iFnFq5", 128);

        memcpy(command_query_reply, "{\"Command\":\"COM_QUERY_DEV_STATUS\",\"zoneEnable\":\"11\",\"zoneArmStatus\":\"11\",\"zoneAlarm\":\"00\",\"powerStatus\":1,\"tamperEnable\":1,\"tamperStatus\":0,\"infraredStatus\":1,\"commStatus\":0}", 220);
        memcpy(command_query_reply_1, "{\"Command\":\"COM_QUERY_DEV_STATUS\",\"zoneEnable\":\"1\",\"zoneArmStatus\":\"1\",\"zoneAlarm\":\"0\",\"powerStatus\":1,\"tamperEnable\":1,\"tamperStatus\":0,\"infraredStatus\":1,\"commStatus\":0}", 220);
    }
    else
    {
        memcpy(alarm_status_1_10, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAlA+FfCjVqc0iPqtjFMFcyb", 128);
        memcpy(alarm_status_2_10, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLHBu0aYwG41mcVdKscVPsH5A+FfCjVqc0iPqtjFMFcyb", 128);
        memcpy(alarm_status_4_10, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFFA+FfCjVqc0iPqtjFMFcyb", 128);

        memcpy(alarm_status_1_01, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLN5A37zdm4AFZQtHYFZSAAk5w4T5PQ0yMM5wm0iFnFq5", 128);
        memcpy(alarm_status_4_01, "wzCSbNo9nCgekK3pH4fKH7LQKm56t6+CUeLJdIkd9yhXOzQo0F9m9LndofD1nK99c8tRPDiDv1G3V2u63fqKLNW1B+8t/7ht24dvWW0GpFE5w4T5PQ0yMM5wm0iFnFq5", 128);

        memcpy(command_query_reply, "{\"Command\":\"COM_QUERY_DEV_STATUS\",\"zoneEnable\":\"111\",\"zoneArmStatus\":\"111\",\"zoneAlarm\":\"000\",\"powerStatus\":1,\"tamperEnable\":1,\"tamperStatus\":0,\"infraredStatus\":0,\"commStatus\":0}", 220);
        memcpy(command_query_reply_1, "{\"Command\":\"COM_QUERY_DEV_STATUS\",\"zoneEnable\":\"11\",\"zoneArmStatus\":\"11\",\"zoneAlarm\":\"00\",\"powerStatus\":1,\"tamperEnable\":1,\"tamperStatus\":0,\"infraredStatus\":0,\"commStatus\":0}", 220);
    }
}

void HAL_UART1_RxCallBack(void)
{
    //    TCP_Msg_TypeDef *pxTCPMsg;
    //    pxTCPMsg = &xTCP_Msg;
    //    TCP_Data_TypeDef *pxTCPData;
    //    pxTCPData = &xTCP_Data;
    //    switch (pxTCPMsg->uiRxCnt)
    //    {
    //    case 0x00:
    //      if(bTCPConnected > 0)
    //      {
    //          if (pxTCPMsg->ucRxData == 'H') // 0x48
    //          {
    //              pxTCPMsg->ucMsgBuf[0] = pxTCPMsg->ucRxData;
    //              pxTCPMsg->uiRxCnt++;
    //          }
    //          else
    //              pxTCPMsg->uiRxCnt = 0x00;
    //      }
    //       else  //bTCPConnected = 0
    //     {
    //         if(connect_status == 0)
    //         {
    //             if (pxTCPMsg->ucRxData == 0xa5)
    //             {
    //                 pxTCPMsg->ucMsgBuf[0] = pxTCPMsg->ucRxData;
    //                 pxTCPMsg->uiRxCnt = 0x00;
    //                 bATRecvOK = 0x01;
    //             }
    //             else
    //             {
    //                  pxTCPMsg->uiRxCnt = 0x00;
    //             }
    //         }
    //         else if(connect_status >= 3)
    //         {
    //             if (pxTCPMsg->ucRxData == 0xAA)
    //             {
    //                 pxTCPMsg->ucMsgBuf[0] = pxTCPMsg->ucRxData;
    //                 pxTCPMsg->uiRxCnt = 0x00;
    //                 bATRecvOK = 0x01;
    //             }
    //             else
    //             {
    //                  pxTCPMsg->uiRxCnt = 0x00;
    //             }
    //         }
    //         else if((connect_status == 1) || (connect_status == 2))
    //         {
    //             pxTCPMsg->ucMsgBuf[0] = pxTCPMsg->ucRxData;
    //             pxTCPMsg->uiRxCnt++;
    //         }
    //
    //     }
    //        break;
    //    case 0x01:
    //        if ((pxTCPMsg->ucRxData == 'K') && (pxTCPMsg->ucMsgBuf[0] == 'H'))
    //        {
    //            pxTCPMsg->ucMsgBuf[1] = pxTCPMsg->ucRxData;
    //            pxTCPMsg->ucProtocol = 1;
    //            pxTCPMsg->uiRxCnt++;
    //        }
    //        else if(bTCPConnected == 0)
    //        {
    //            pxTCPMsg->ucMsgBuf[1] = pxTCPMsg->ucRxData;
    //            pxTCPMsg->ucProtocol = 2;
    //            pxTCPMsg->uiRxCnt++;
    //        }
    //        else
    //            pxTCPMsg->uiRxCnt = 0x00;
    //        break;
    //    default:
    //        if (pxTCPMsg->ucProtocol == 1)
    //        {
    //            if (pxTCPMsg->uiRxCnt < 60)
    //            {
    //                pxTCPMsg->ucMsgBuf[pxTCPMsg->uiRxCnt++] = pxTCPMsg->ucRxData;
    //                if (pxTCPMsg->uiRxCnt == 56)
    //                    pxTCPData->uiLength = pxTCPMsg->ucMsgBuf[55];
    //            }
    //            else
    //            {
    //                pxTCPData->ucDataBuf[pxTCPMsg->uiRxCnt - 60] = pxTCPMsg->ucRxData;
    //                pxTCPMsg->uiRxCnt++;
    //                if (pxTCPMsg->uiRxCnt >= (pxTCPData->uiLength + 60))
    //                {
    //                    bTCPRecvOK = 1;
    //                    pxTCPMsg->uiRxCnt = 0;
    //                }
    //                if (pxTCPMsg->uiRxCnt >= 300)
    //                {
    //                    pxTCPMsg->uiRxCnt = 0;
    //                }
    //            }
    //        }
    //        else if (pxTCPMsg->ucProtocol == 2)
    //        {
    //            pxTCPMsg->ucMsgBuf[pxTCPMsg->uiRxCnt++] = pxTCPMsg->ucRxData;
    //            if ((connect_status == 1) && (pxTCPMsg->uiRxCnt == 4))//ip addr
    //            {
    //                pxTCPMsg->uiRxCnt = 0x00;
    //                bATRecvOK = 0x01;
    //            }
    //          if ((connect_status == 2) && (pxTCPMsg->uiRxCnt == 6))//ip addr
    //            {
    //                pxTCPMsg->uiRxCnt = 0x00;
    //                bATRecvOK = 0x01;
    //            }
    //            if (pxTCPMsg->uiRxCnt >= 60)
    //                pxTCPMsg->uiRxCnt = 0x00;
    //        }
    //        else
    //        {
    //            pxTCPMsg->ucMsgBuf[0] = 0x00;
    //            pxTCPMsg->ucMsgBuf[1] = 0x00;
    //            pxTCPMsg->uiRxCnt = 0x00;
    //        }
    //        break;
    //    }
    //    HAL_UART_Receive_IT(&huart1, (uint8_t *)&pxTCPMsg->ucRxData, 1);
}


void HAL_UART1_Rx(void)     //非上位机解析
{
    TCP_Msg_TypeDef *pxTCPMsg;
    pxTCPMsg = &xTCP_Msg;
    TCP_Data_TypeDef *pxTCPData;
    pxTCPData = &xTCP_Data;

    if (bTCPConnected > 0)
    {
        if ((RX_buf[0] == 'H')&&(RX_buf[1] == 'K')) // 0x48
        {
            pxTCPMsg->ucProtocol = 1;

            for (uint8_t i; i<60; i++)
            {
                pxTCPMsg->ucMsgBuf[i] = RX_buf[i];
            }

            pxTCPData->uiLength = RX_buf[55];

            if (pxTCPData->uiLength<240)
            {
                for (uint8_t j=0; j<pxTCPData->uiLength; j++)
                {
                    pxTCPData->ucDataBuf[j]=RX_buf[j+60];
                }

                bTCPRecvOK = 1;
            }
        }
    }
    else  //bTCPConnected = 0       //未连接
    {
        if (connect_status == 0)
        {
            if (RX_buf[0] == 0xa5)
            {
                pxTCPMsg->ucMsgBuf[0] = RX_buf[0];
                bATRecvOK = 0x01;
            }
        }
        else if (connect_status >= 3)
        {
            if (RX_buf[0] == 0xAA)
            {
                pxTCPMsg->ucMsgBuf[0] = RX_buf[0];
                bATRecvOK = 0x01;
            }
        }
        else if ((connect_status == 1) || (connect_status == 2))
        {
            pxTCPMsg->ucMsgBuf[0] = RX_buf[0];
            pxTCPMsg->ucMsgBuf[1] = RX_buf[1];
            pxTCPMsg->ucMsgBuf[2] = RX_buf[2];
            pxTCPMsg->ucMsgBuf[3] = RX_buf[3];

            if (connect_status == 2)
            {
                pxTCPMsg->ucMsgBuf[4] = RX_buf[4];
                pxTCPMsg->ucMsgBuf[5] = RX_buf[5];
            }

            bATRecvOK = 0x01;
        }
    }
}

void vUART_TCP_TASK(void *pvParameters)
{
    DEVICEDATA *pd;
    pd = (DEVICEDATA *)pvParameters;
    EEprom_Msg_TypeDef xEEpromWriteData;
    static uint16_t uiHeartBeatCnt;
    int value;

    TCP_Data_TypeDef *pxTCPData;
    uint16_t tcp_data_length;
    pxTCPData = &xTCP_Data;
    char xTCPPacketTmp[320] = {0};
    char xTCPMsgJson[220] = {0};
    char *substr, *substr1;

    HAL_SW_MspInit();
    pd->ucAddress = get_addr();
    UART_TCP_init();
    bTCPConnected = 0x00;

    // set the tcp communicate address
    while (1)
    {
        if (upper_flag==1)
        {
            CMD_Deal_With();
        }
        else if (IAP_flag==0)
        {
            if (bTCPConnected)
            {
                uiHeartBeatCnt++;

                if (bTCPConnected == 0x01) // 60s 3000
                {
                    if ((uiHeartBeatCnt % 500) == 0)
                    {
                        //                vUartTCPRegister();
                        vUartTCPConnect(pd->ucAddress);
                        //                bTCPConnected = 0;
                    }

                    if (uiHeartBeatCnt >= 3000)
                    {
                        uiHeartBeatCnt = 0;
                        bTCPConnected = 0x00;
                        connect_status = 0;
                        InitSendAlarmFlag = 0;
                    }
                }

                if ((uiHeartBeatCnt >= 5000) && (bTCPConnected == 0x02)) // 60s
                {
                    uiHeartBeatCnt = 0;
                    bTCPConnected = 0x00;
                    connect_status = 0;
                    InitSendAlarmFlag = 0;
                }

                if (bTCPRecvOK)
                {
                    Uart1RstCnt = 0;
                    Uart1RstFlag = 1;
                    bTCPRecvOK = 0x00;
                    // tcp_data_length = strlen((char *)pxTCPData->ucDataBuf);
                    tcp_data_length = pxTCPData->uiLength;
                    Base64decode((char *)pxTCPData->ucDataBuf, xTCPPacketTmp, tcp_data_length);
                    aesDecrypt(pKey, 0x10, (unsigned char *)xTCPPacketTmp, (unsigned char *)xTCPMsgJson, tcp_data_length);

                    memset(pxTCPData->ucDataBuf, 0, 250);
                    int id, relaytime;
                    uint8_t bReplyFlag = 1;

                    if (strstr(xTCPMsgJson, "COM_DEV_REGISTER") != NULL)
                    {
                        bTCPConnected = 0x02;
                        uiHeartBeatCnt = 0;

                        //                  InitSendAlarmFlag = 1;
                        TCP_AlarmTxFlag = 0;
                        ucUartTCPAlarmedLast = 0;
                        ucTCPAlarmStatus = 0;
                        ucTCPAlarmStatusLast = 0;
                        ucUartTCPAlarmed = 0;
                    }

                    if (bTCPConnected == 0x02) //
                    {
                        if (strstr(xTCPMsgJson, "COM_RELAY_CONTINUE_TIME") != NULL)
                        {
                            substr = strstr(xTCPMsgJson, "\"id\":");
                            substr1 = strstr(substr, "\"relayTime\":");

                            if ((sscanf(substr, "\"id\":%d", &id) == 1) && (sscanf(substr1, "\"relayTime\":%d", &relaytime) == 1))
                            {
                                if ((relaytime >= 2) && (relaytime <= 50))
                                {
                                    xEEpromWriteData.ucData[4] = relaytime;

                                    if (DUAL_DEFENCE == 0)
                                        pd->ucRelayOnTime = (unsigned int)relaytime * 1000; // 550

                                    else if (DUAL_DEFENCE == 1)
                                        pd->ucRelayOnTime = (unsigned int)relaytime * 900; // 550

                                    pd->ucForceSync = 1;
                                    xEEpromWriteData.ucData[0] = 0x00;
                                    xEEpromWriteData.ucData[1] = 0x00;
                                    xEEpromWriteData.ucData[2] = 0x00;
                                    xEEpromWriteData.ucData[3] = 0x00;
                                    xEEpromWriteData.ucCmd = 0x06;
                                    osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xFF);
                                    memset(xTCPMsgJson, 0, 220);
                                    strcpy(xTCPMsgJson, "{\"Command\":\"COM_RELAY_CONTINUE_TIME\",\"Result\":\"ok\"}");
                                }
                                else
                                {
                                    memset(xTCPMsgJson, 0, 220);
                                    strcpy(xTCPMsgJson, "{\"Command\":\"COM_RELAY_CONTINUE_TIME\",\"Result\":\"error\"}");
                                }
                            }
                            else
                            {
                                memset(xTCPMsgJson, 0, 220);
                                strcpy(xTCPMsgJson, "{\"Command\":\"COM_RELAY_CONTINUE_TIME\",\"Result\":1,\"ErrorCode\":2}");
                            }
                        }
                        else if (strstr(xTCPMsgJson, "COM_ZONE_SETUPARM") != NULL)
                        {
                            value = 0;
                            char value1[5] = {0};
                            substr = strstr(xTCPMsgJson, "\"setupType\":");
                            substr1 = strstr(xTCPMsgJson, "\"armZoneChan\":");

                            if (sscanf(substr, "\"setupType\":%d", &value) == 1)
                            {
                                device.ucLcdOnTime = 120;
                                lcd_backup_on();

                                if (value == 0x01)
                                {
                                    if (sscanf(substr1, "\"armZoneChan\":%s", value1) == 1)
                                    {
                                        if (value1[1] == '1')
                                        {
                                            pd->ucPulseOn |= 0x01;
                                        }

                                        if (value1[2] == '1')
                                        {
                                            pd->ucPulseOn |= 0x02;
                                        }
                                    }
                                }
                                else
                                {
                                    if (sscanf(substr1, "\"armZoneChan\":%s", value1) == 1)
                                    {
                                        if (value1[1] == '1')
                                        {
                                            pd->ucPulseOn &= 0xFE;
                                        }

                                        if (value1[2] == '1')
                                        {
                                            pd->ucPulseOn &= 0xFD;
                                        }
                                    }
                                }
                            }

                            memset(xTCPMsgJson, 0, 220);
                            strcpy(xTCPMsgJson, "{\"Command\":\"COM_ZONE_SETUPARM\",\"Result\":0}");
                        }
                        else if (strstr(xTCPMsgJson, "COM_HOST_CAPABILITIES") != NULL)
                        {
                            memset(xTCPMsgJson, 0, 220);

                            if (DUAL_DEFENCE == 0)
                                strcpy(xTCPMsgJson, "{\"Command\":\"COM_HOST_CAPABILITIES\",\"Data\":{\"zoneNum\":{\"@min\":0001,\"@max\":0002},\"alarmTime\":{\"@min\":0002,\"@max\":0050}}}");
                            else if (DUAL_DEFENCE == 1)
                                strcpy(xTCPMsgJson, "{\"Command\":\"COM_HOST_CAPABILITIES\",\"Data\":{\"zoneNum\":{\"@min\":0002,\"@max\":0003},\"alarmTime\":{\"@min\":0002,\"@max\":0050}}}");
                        }
                        else if (strstr(xTCPMsgJson, "COM_QUERY_DEV_STATUS") != NULL)
                        {
                            memset(xTCPMsgJson, 0, 220);

                            if (DUAL_DEFENCE == 1)
                            {
                                if (pd->ucIREn == 0xaa)
                                {
                                    memcpy(xTCPMsgJson, command_query_reply, 220);

                                    if (pd->ucRunningStatus & 0x01)
                                    {
                                        xTCPMsgJson[48] = '1';
                                        xTCPMsgJson[70] = '1';

                                        if (pd->ucAlarmStatus & 0x04)
                                            xTCPMsgJson[88] = '1';
                                        else
                                            xTCPMsgJson[88] = '0';
                                    }
                                    else
                                    {
                                        xTCPMsgJson[48] = '0';
                                        xTCPMsgJson[70] = '0';
                                        xTCPMsgJson[88] = '0';
                                    }

                                    if (pd->ucRunningStatus & 0x02)
                                    {
                                        xTCPMsgJson[49] = '1';
                                        xTCPMsgJson[71] = '1';

                                        if (pd->ucAlarmStatus & 0x08)
                                            xTCPMsgJson[89] = '1';
                                        else
                                            xTCPMsgJson[89] = '0';
                                    }
                                    else
                                    {
                                        xTCPMsgJson[49] = '0';
                                        xTCPMsgJson[71] = '0';
                                        xTCPMsgJson[89] = '0';
                                    }

                                    if (pd->ucPowerStatus)
                                        xTCPMsgJson[107] = '1';
                                    else
                                        xTCPMsgJson[107] = '0';

                                    xTCPMsgJson[124] = '1';

                                    if (pd->ucAlarmStatus & 0x01)
                                        xTCPMsgJson[141] = '1';
                                    else
                                        xTCPMsgJson[141] = '0';

                                    if (pd->ucIREn == 0xaa)
                                    {
                                        xTCPMsgJson[160] = '1';
                                        xTCPMsgJson[50] = '1';
                                        xTCPMsgJson[72] = '1';

                                        if (pd->ucAlarmStatus & 0x02)
                                        {
                                            xTCPMsgJson[90] = '1';
                                        }
                                        else
                                        {
                                            xTCPMsgJson[90] = '0';
                                        }
                                    }
                                    else
                                    {
                                        xTCPMsgJson[50] = '0';
                                        xTCPMsgJson[72] = '0';
                                        xTCPMsgJson[90] = '0';
                                        xTCPMsgJson[160] = '0';
                                    }
                                }
                                else // if ( pd->ucIREn == 0x55)
                                {
                                    memcpy(xTCPMsgJson, command_query_reply_1, 220);

                                    if (pd->ucRunningStatus & 0x01)
                                    {
                                        xTCPMsgJson[48] = '1';
                                        xTCPMsgJson[69] = '1';

                                        if (pd->ucAlarmStatus & 0x04)
                                            xTCPMsgJson[86] = '1';
                                        else
                                            xTCPMsgJson[86] = '0';
                                    }
                                    else
                                    {
                                        xTCPMsgJson[48] = '0';
                                        xTCPMsgJson[69] = '0';
                                        xTCPMsgJson[86] = '0';
                                    }

                                    if (pd->ucRunningStatus & 0x02)
                                    {
                                        xTCPMsgJson[49] = '1';
                                        xTCPMsgJson[70] = '1';

                                        if (pd->ucAlarmStatus & 0x08)
                                            xTCPMsgJson[87] = '1';
                                        else
                                            xTCPMsgJson[87] = '0';
                                    }
                                    else
                                    {
                                        xTCPMsgJson[49] = '0';
                                        xTCPMsgJson[70] = '0';
                                        xTCPMsgJson[87] = '0';
                                    }

                                    if (pd->ucPowerStatus)
                                        xTCPMsgJson[104] = '1';
                                    else
                                        xTCPMsgJson[104] = '0';

                                    xTCPMsgJson[121] = '1';

                                    if (pd->ucAlarmStatus & 0x01)
                                        xTCPMsgJson[138] = '1';
                                    else
                                        xTCPMsgJson[138] = '0';
                                }
                            }
                            else if (DUAL_DEFENCE == 0)
                            {
                                if (pd->ucIREn == 0xaa)
                                {
                                    memcpy(xTCPMsgJson, command_query_reply, 220);

                                    if (pd->ucRunningStatus & 0x01)
                                    {
                                        xTCPMsgJson[48] = '1';
                                        xTCPMsgJson[69] = '1';

                                        if (pd->ucAlarmStatus & 0x04)
                                            xTCPMsgJson[86] = '1';
                                        else
                                            xTCPMsgJson[86] = '0';
                                    }
                                    else
                                    {
                                        xTCPMsgJson[48] = '0';
                                        xTCPMsgJson[69] = '0';
                                        xTCPMsgJson[86] = '0';
                                    }

                                    if (pd->ucPowerStatus)
                                        xTCPMsgJson[104] = '1';
                                    else
                                        xTCPMsgJson[104] = '0';

                                    xTCPMsgJson[121] = '1';

                                    if (pd->ucAlarmStatus & 0x01)
                                        xTCPMsgJson[138] = '1';
                                    else
                                        xTCPMsgJson[138] = '0';

                                    if (pd->ucIREn == 0xaa)
                                    {
                                        xTCPMsgJson[157] = '1';
                                        xTCPMsgJson[49] = '1';
                                        xTCPMsgJson[70] = '1';

                                        if (pd->ucAlarmStatus & 0x02)
                                        {
                                            xTCPMsgJson[87] = '1';
                                        }
                                        else
                                        {
                                            xTCPMsgJson[87] = '0';
                                        }
                                    }
                                    else
                                    {
                                        xTCPMsgJson[49] = '0';
                                        xTCPMsgJson[70] = '0';
                                        xTCPMsgJson[87] = '0';
                                        xTCPMsgJson[157] = '0';
                                    }
                                }
                                else // if ( pd->ucIREn == 0x55)
                                {
                                    memcpy(xTCPMsgJson, command_query_reply_1, 220);

                                    if (pd->ucRunningStatus & 0x01)
                                    {
                                        xTCPMsgJson[48] = '1';
                                        xTCPMsgJson[68] = '1';

                                        if (pd->ucAlarmStatus & 0x04)
                                            xTCPMsgJson[84] = '1';
                                        else
                                            xTCPMsgJson[84] = '0';
                                    }
                                    else
                                    {
                                        xTCPMsgJson[48] = '0';
                                        xTCPMsgJson[68] = '0';
                                        xTCPMsgJson[84] = '0';
                                    }

                                    if (pd->ucPowerStatus)
                                        xTCPMsgJson[101] = '1';
                                    else
                                        xTCPMsgJson[101] = '0';

                                    xTCPMsgJson[118] = '1';

                                    if (pd->ucAlarmStatus & 0x01)
                                        xTCPMsgJson[135] = '1';
                                    else
                                        xTCPMsgJson[135] = '0';

                                    xTCPMsgJson[154] = '0';
                                }

                                //                    }
                            }
                        }
                        else if (strstr(xTCPMsgJson, "COM_ZONE_ENABLE") != NULL)
                        {
                            value = 0;
                            substr = strstr(xTCPMsgJson, "\"setupType\":");

                            if (sscanf(substr, "\"setupType\":%d", &value) == 1)
                            {
                                if (value == 0x00) // bypass
                                    ;
                                else // enable
                                    ;
                            }

                            memset(xTCPMsgJson, 0, 220);
                            strcpy(xTCPMsgJson, "{\"Command\":\"COM_ZONE_ENABLE\",\"Result\":0}");
                        }
                        else if (strstr(xTCPMsgJson, "COM_ZONE_PARAM_SET") != NULL)
                        {
                            value = 0;
                            int value1 = 0;
                            substr = strstr(xTCPMsgJson, "\"zoneVol\":");
                            substr1 = strstr(substr, "\"zoneSensi\":");

                            if ((sscanf(substr, "\"zoneVol\":%d", &value) == 1) && (sscanf(substr1, "\"zoneSensi\":%d", &value1) == 1))
                            {
                                pd->ucSetVoltage[0] = pd->ucSetVoltage[1] = value;
                                pd->ucForceSync = 1;

                                if ((value1 >= 1) && (value1 <= 10))
                                {
                                    pd->ucAlarmCycle = value1;
                                    xEEpromWriteData.ucData[0] = 0x00;
                                    xEEpromWriteData.ucData[1] = value;
                                    xEEpromWriteData.ucData[2] = value1;
                                    xEEpromWriteData.ucData[3] = 0x00;
                                    xEEpromWriteData.ucData[4] = 0x00;
                                    xEEpromWriteData.ucCmd = 0x06;
                                    osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xFF);
                                }

                                memset(xTCPMsgJson, 0, 220);
                                strcpy(xTCPMsgJson, "{\"Command\":\"COM_ZONE_PARAM_SET\",\"Result\":0}");
                            }
                            else
                            {
                                memset(xTCPMsgJson, 0, 220);
                                strcpy(xTCPMsgJson, "{\"Command\":\"COM_ZONE_PARAM_SET\",\"Result\":1}");
                            }
                        }
                        else if (strstr(xTCPMsgJson, "COM_BUZZER_CONTINUE_TIME") != NULL)
                        {
                            value = 0;
                            substr = strstr(xTCPMsgJson, "\"buzzerTime\":");

                            if (sscanf(substr, "\"buzzerTime\":%d", &value) == 1)
                            {
                                if ((value >= 2) && (value <= 50))
                                {
                                    if (DUAL_DEFENCE == 0)
                                        pd->ucBeepOnTime = (unsigned int)value * 1000; // 550
                                    else if (DUAL_DEFENCE == 1)
                                        pd->ucBeepOnTime = (unsigned int)value * 900; // 550

                                    pd->ucForceSync = 1;
                                    xEEpromWriteData.ucData[0] = value;
                                    xEEpromWriteData.ucData[1] = 0x00;
                                    xEEpromWriteData.ucData[2] = 0x00;
                                    xEEpromWriteData.ucData[3] = 0x00;
                                    xEEpromWriteData.ucData[4] = 0x00;
                                    xEEpromWriteData.ucCmd = 0x06;
                                    osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xFF);
                                    memset(xTCPMsgJson, 0, 220);
                                    strcpy(xTCPMsgJson, "{\"Command\":\"COM_BUZZER_CONTINUE_TIME\",\"Result\":\"ok\"}");
                                }
                                else
                                {
                                    memset(xTCPMsgJson, 0, 220);
                                    strcpy(xTCPMsgJson, "{\"Command\":\"COM_BUZZER_CONTINUE_TIME\",\"Result\":\"error\"}");
                                }
                            }
                        }
                        else if (strstr(xTCPMsgJson, "COM_IO_SET") != NULL)
                        {
                            value = 0;
                            substr = strstr(xTCPMsgJson, "\"enable\":");

                            if (sscanf(substr, "\"enable\":%d", &value) == 1)
                            {
                                if (value == 0x01)
                                {
                                    pd->ucIREn = 0xaa;
                                    xEEpromWriteData.ucData[3] = 0xaa;
                                }
                                else
                                {
                                    pd->ucIREn = 0x00;
                                    xEEpromWriteData.ucData[3] = 0x55;
                                }

                                xEEpromWriteData.ucData[0] = 0x00;
                                xEEpromWriteData.ucData[1] = 0x00;
                                xEEpromWriteData.ucData[2] = 0x00;
                                //                            xEEpromWriteData.ucData[3] = pd->ucIREn;
                                xEEpromWriteData.ucData[4] = 0x00;
                                xEEpromWriteData.ucCmd = 0x06;
                                osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, 0xFF);
                            }

                            memset(xTCPMsgJson, 0, 220);
                            strcpy(xTCPMsgJson, "{\"Command\":\"COM_IO_SET\",\"Result\":\"ok\"}");
                        }
                        else if (strstr(xTCPMsgJson, "COM_CONTLEVER_ANTICLIMB") != NULL)
                        {
                            memset(xTCPMsgJson, 0, 220);
                            strcpy(xTCPMsgJson, "{\"Command\":\"COM_ZONE_ENABLE\",\"Result\":1}");
                        }
                        else if (strstr(xTCPMsgJson, "COM_UPLOAD_ZONE_STATUS") != NULL)
                        {
							#if Debug_flag 
                            printf("\nCRC\n");
							#endif
                            TCP_AlarmTxFlag = 2;
                            bReplyFlag = 0;
                        }
                        else if (strstr(xTCPMsgJson, "COM_UPLOAD_DEVICE_ALARM") != NULL)
                        {
							#if Debug_flag 
                            printf("\nCRC\n");
							#endif
                            TCP_AlarmTxFlag = 2;
                            bReplyFlag = 0;
                        }
                        else if (strstr(xTCPMsgJson, "statusCode") != NULL)
                        {
                            bReplyFlag = 0;
                            InitSendAlarmFlag = 1;
                            osDelay(1000);
                        }
                        else
                            bReplyFlag = 0;

                        if (bReplyFlag)
                        {
                            Pkcs5padding_add(xTCPMsgJson);
                            uint16_t tmp_length;
                            tmp_length = strlen(xTCPMsgJson);
                            aesEncrypt(pKey, 0x10, (unsigned char *)xTCPMsgJson, (unsigned char *)xTCPPacketTmp, tmp_length);
                            Base64encode(xTCPPacketTmp, xTCPPacket, tmp_length);
                            tmp_length = strlen(xTCPPacket);
                            vUartTCPSend(tmp_length, xTCPPacket);
                            uiHeartBeatCnt = 0;
                        }
                    } //
                }

                // short alarmtype is 0x02 and open alarmtype is 0x01
                if (InitSendAlarmFlag == 1)
                {
                    InitSendAlarmFlag = 2;

#if DUAL_DEFENCE == 1
                    memcpy(xTCPPacket, alarm_status_4_100, 128);
                    vUartTCPSend(128, xTCPPacket);
                    osDelay(1000);
                    memcpy(xTCPPacket, alarm_status_4_010, 128);
                    vUartTCPSend(128, xTCPPacket);
                    osDelay(1000);
                    memcpy(xTCPPacket, alarm_status_4_001, 128);
                    vUartTCPSend(128, xTCPPacket);
                    osDelay(1000);
                    memcpy(xTCPPacket, tamper_alarm_remove, 88);
                    vUartTCPSend(88, xTCPPacket);
                    osDelay(1000);
#else
                    memcpy(xTCPPacket, alarm_status_4_10, 128);
                    vUartTCPSend(128, xTCPPacket);
                    osDelay(1000);
                    memcpy(xTCPPacket, alarm_status_4_01, 128);
                    vUartTCPSend(128, xTCPPacket);
                    osDelay(1000);
                    memcpy(xTCPPacket, tamper_alarm_remove, 88);
                    vUartTCPSend(88, xTCPPacket);
                    osDelay(1000);
#endif
                    osDelay(100);
					#if Debug_flag 
                    printf("\n 0 \n");
					#endif
                }

                TCP_AlarmTxFlagCnt ++;

                if (TCP_AlarmTxFlagCnt > 250)
                {
                    TCP_AlarmTxFlagCnt = 0;
                }

                if (InitSendAlarmFlag > 0)
                {
                    ucTCPAlarmStatusLast = ucTCPAlarmStatus;
                    ucTCPAlarmStatus = (pd->ucAlarmStatus & 0x0F); // ucTCPAlarmStatus = (pd->ucAlarmStatus & 0x0d);

                    if (TCP_AlarmTxFlagCnt > 10)
                    {
                        if (TCP_AlarmTxFlag == 2) //有应答
                        {
                            TCP_AlarmTxFlag = 0;

                            if (ucUartTCPAlarmedLast == 0) //上次发送的是有报警的信息
                            {
                                if (ucTCPAlarmStatus == 0) //无报警
                                {
                                    ;
									#if Debug_flag 
                                    printf("\n1\n");
									#endif
									
                                }
                                else                      //有报警
                                {
                                    ;
									#if Debug_flag 
                                    printf("\n2\n");
									#endif
                                }
                            }

                            if (ucUartTCPAlarmedLast != 0) //上次发送的是无报警的信息
                            {
                                if (ucTCPAlarmStatus == 0) //无报警
                                {
                                    ;
									#if Debug_flag 
                                    printf("\n3\n");
									#endif
                                }
                                else                      //有报警
                                {
                                    ucUartTCPAlarmed = 0;
									#if Debug_flag 
                                    printf("\n4\n");
									#endif
                                }
                            }
                        }
                        else if (TCP_AlarmTxFlag == 1)  //发送报警信息，无应答
                        {
                            ucTCPAlarmStatusLast = 0xff;

                            if (ucUartTCPAlarmedLast == 0) //上次发送的是有报警的信息
                            {
                                if (ucTCPAlarmStatus == 0) //无报警
                                {
                                    ;
									#if Debug_flag 
                                    printf("\n5\n");
									#endif
                                }
                                else                      //有报警
                                {
                                    ucUartTCPAlarmed = 0;
									#if Debug_flag 
                                    printf("\n6\n");
									#endif
                                }
                            }

                            if (ucUartTCPAlarmedLast != 0) //上次发送的是无报警的信息
                            {
                                if (ucTCPAlarmStatus == 0) //无报警
                                {
                                    ucUartTCPAlarmed = ucUartTCPAlarmedLast;
									#if Debug_flag 
                                    printf("\n7\n");
									#endif
                                }
                                else                      //有报警
                                {
                                    ucUartTCPAlarmed = 0;
									#if Debug_flag 
                                    printf("\n8\n");
									#endif
                                }
                            }
                        }
                    }

                    if (ucTCPAlarmStatusLast ^ ucTCPAlarmStatus)
                    {
                        TCP_AlarmTxFlag = 1;
                        TCP_AlarmTxFlagCnt = 0;

                        if (DUAL_DEFENCE == 1)
                        {
                            switch (ucUartTCPAlarmed)
                            {
                                case 0x00:
                                    if ((ucTCPAlarmStatus & 0x04) == 0x04) //防区1报警
                                    {
                                        if ((pd->ucAlarmType & 0x01) == 0x01)
                                        {
                                            memcpy(xTCPPacket, alarm_status_2_100, 128); //防区1断路报警
                                            vUartTCPSend(128, xTCPPacket);
                                            ucUartTCPAlarmed = 0x01;
                                        }
                                        else if ((pd->ucAlarmType & 0x02) == 0x02)
                                        {
                                            memcpy(xTCPPacket, alarm_status_1_100, 128); //防区1短路报警
                                            vUartTCPSend(128, xTCPPacket);
                                            ucUartTCPAlarmed = 0x01;
											#if Debug_flag 
                                            printf("\n0-1\n");
											#endif
                                        }
                                    }

                                    if ((ucTCPAlarmStatus & 0x08) == 0x08) //防区2报警
                                    {
                                        if ((pd->ucAlarmType & 0x10) == 0x10)
                                        {
                                            memcpy(xTCPPacket, alarm_status_2_010, 128); //防区2断路路报警
                                            vUartTCPSend(128, xTCPPacket);
                                            ucUartTCPAlarmed = 0x02;
                                        }
                                        else if ((pd->ucAlarmType & 0x20) == 0x20)
                                        {
                                            memcpy(xTCPPacket, alarm_status_1_010, 128); //防区2短路报警
                                            vUartTCPSend(128, xTCPPacket);
                                            ucUartTCPAlarmed = 0x02;
                                        }
										#if Debug_flag 
                                        printf("Area2 Alarm\n");
										#endif
                                    }

                                    if ((ucTCPAlarmStatus & 0x02) == 0x02) //开关量报警
                                    {
                                        memcpy(xTCPPacket, alarm_status_1_001, 128); //防区2短路报警
                                        vUartTCPSend(128, xTCPPacket);
                                        ucUartTCPAlarmed = 0x03;
                                    }

                                    if (ucTCPAlarmStatus & 0x01) //防拆报警01
                                    {
                                        memcpy(xTCPPacket, tamper_alarm_upload, 88); //防拆报警
                                        vUartTCPSend(88, xTCPPacket);
                                        ucUartTCPAlarmed = 0x04;
                                    }

                                    if ((ucTCPAlarmStatus & 0x0E) == 0x0E) // 1110
                                    {
                                        ucUartTCPAlarmed = 0x05;
                                    }

                                    if ((ucTCPAlarmStatus & 0x0C) == 0x0C) // 1100
                                    {
                                        ucUartTCPAlarmed = 0x06;
                                    }

                                    if ((ucTCPAlarmStatus & 0x0A) == 0x0A) // 1010
                                    {
                                        ucUartTCPAlarmed = 0x07;
                                    }

                                    if ((ucTCPAlarmStatus & 0x06) == 0x06) // 0110
                                    {
                                        ucUartTCPAlarmed = 0x08;
                                    }

                                    ucUartTCPAlarmedLast = 0;
                                    break;

                                case 0x01:
                                    memcpy(xTCPPacket, alarm_status_4_100, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
								#if Debug_flag 
                                    printf("\n1\n");
								#endif
                                    ucUartTCPAlarmedLast = 1;
                                    break;

                                case 0x02:
                                    memcpy(xTCPPacket, alarm_status_4_010, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
								#if Debug_flag 
                                    printf("Area2 ok\n");
								#endif
                                    ucUartTCPAlarmedLast = 2;
                                    break;

                                case 0x03:
                                    memcpy(xTCPPacket, alarm_status_4_001, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    ucUartTCPAlarmedLast = 3;
                                    break;

                                case 0x04:
                                    memcpy(xTCPPacket, tamper_alarm_remove, 88);
                                    vUartTCPSend(88, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    ucUartTCPAlarmedLast = 4;
                                    break;

                                case 0x05: // 1110
                                    memcpy(xTCPPacket, alarm_status_4_100, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    memcpy(xTCPPacket, alarm_status_4_010, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    memcpy(xTCPPacket, alarm_status_4_001, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    break;

                                case 0x06: // 1100
                                    memcpy(xTCPPacket, alarm_status_4_100, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    memcpy(xTCPPacket, alarm_status_4_010, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    break;

                                case 0x07: // 1010
                                    memcpy(xTCPPacket, alarm_status_4_100, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    memcpy(xTCPPacket, alarm_status_4_001, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    break;

                                case 0x08: // 0110
                                    memcpy(xTCPPacket, alarm_status_4_010, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    memcpy(xTCPPacket, alarm_status_4_001, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    break;
                            }
                        }
                        else
                        {
                            switch (ucUartTCPAlarmed)
                            {
                                case 0x00:
                                    if ((ucTCPAlarmStatus & 0x04) == 0x04) //防区1报警
                                    {
                                        if ((pd->ucAlarmType & 0x01) == 0x01)
                                        {
                                            memcpy(xTCPPacket, alarm_status_2_10, 128); //防区1断路报警
                                            vUartTCPSend(128, xTCPPacket);
                                            ucUartTCPAlarmed = 0x01;
                                        }
                                        else if ((pd->ucAlarmType & 0x02) == 0x02)
                                        {
                                            memcpy(xTCPPacket, alarm_status_1_10, 128); //防区1短路报警
                                            vUartTCPSend(128, xTCPPacket);
                                            ucUartTCPAlarmed = 0x01;
                                        }
										#if Debug_flag 
                                        printf("Area1 Alarm\n");
										#endif
                                    }

                                    if ((ucTCPAlarmStatus & 0x02) == 0x02) //开关量报警
                                    {
                                        memcpy(xTCPPacket, alarm_status_1_01, 128); //防区2短路报警
                                        vUartTCPSend(128, xTCPPacket);
                                        ucUartTCPAlarmed = 0x03;
                                    }

                                    if (ucTCPAlarmStatus & 0x01) //防拆报警01
                                    {
                                        memcpy(xTCPPacket, tamper_alarm_upload, 88); //防拆报警
                                        vUartTCPSend(88, xTCPPacket);
                                        ucUartTCPAlarmed = 0x04;
                                    }

                                    if ((ucTCPAlarmStatus & 0x06) == 0x06) // 0110
                                    {
                                        ucUartTCPAlarmed = 0x08;
                                    }

                                    ucUartTCPAlarmedLast = 0;
                                    break;

                                case 0x01:
                                    memcpy(xTCPPacket, alarm_status_4_10, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
								#if Debug_flag 
                                    printf("Area1 ok\n");
								#endif
                                    ucUartTCPAlarmedLast = 1;
                                    break;

                                case 0x03:
                                    memcpy(xTCPPacket, alarm_status_4_01, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    ucUartTCPAlarmedLast = 3;
                                    break;

                                case 0x04:
                                    memcpy(xTCPPacket, tamper_alarm_remove, 88);
                                    vUartTCPSend(88, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    ucUartTCPAlarmedLast = 4;
                                    break;

                                case 0x08: // 0110
                                    memcpy(xTCPPacket, alarm_status_4_10, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    memcpy(xTCPPacket, alarm_status_4_01, 128);
                                    vUartTCPSend(128, xTCPPacket);
                                    ucUartTCPAlarmed = 0x00;
                                    break;

                                default:
                                    break;
                            }
                        }
                    }
                }
            }
            else
            {
                vUartTCPConnect(pd->ucAddress);
            }

            if (KeySetIpFlag == 1)
            {
                KeySetIpFlag = 2;
                uiHeartBeatCnt = 0;
                bTCPConnected = 0x00;
                connect_status = 0;
            }
        }

        if (refresh_flag>0)
        {
            refresh_flag++;

            if (refresh_flag>100)
            {
                uint8_t refresh_buf[10];

                refresh_flag=1;
                uint8_t len=0;
                refresh_buf[len++]=0x55;
                refresh_buf[len++]=0xAA;
                refresh_buf[len++]=0xC2;
                refresh_buf[len++]=10;
                refresh_buf[len++] =device.ucVoltage[0];
                refresh_buf[len++] =device.ucCurrent[0];
                refresh_buf[len++] =device.ucVoltage[1];
                refresh_buf[len++] =device.ucCurrent[0];

                refresh_buf[len++]=CalcCRC((char*)refresh_buf, 8)/256;
                refresh_buf[len++]=CalcCRC((char*)refresh_buf, 8)%256;
                HAL_UART_Transmit(&huart1, refresh_buf, 10, 1000);
            }
        }

        osDelay(10);
    }
}

void vUartTCPRegister(void)
{
    uint16_t crc16;
    uint16_t uiLength;

    uiLength = strlen(register_data_encrypt);
    crc16 = CalcCRC((char *)register_data_encrypt, uiLength);
    memcpy(xTCPHeader, tcp_header, 60);

    for (uint8_t i = 0; i < 20; i++)
    {
        xTCPHeader[i + 4] = register_data[50 + i];
    }

    xTCPHeader[52] = (uiLength >> 24);
    xTCPHeader[53] = (uiLength >> 16);
    xTCPHeader[54] = (uiLength >> 8);
    xTCPHeader[55] = (uiLength & 0x00ff);
    xTCPHeader[56] = '0';
    xTCPHeader[57] = '0';
    xTCPHeader[58] = (crc16 >> 8);
    xTCPHeader[59] = (crc16 & 0x00ff);
    HAL_UART_Transmit(&huart1, (uint8_t *)xTCPHeader, 60, 0xff);
    HAL_UART_Transmit(&huart1, (uint8_t *)register_data_encrypt, uiLength, 0xff);
}

void vUartTCPSend(uint16_t uiLength, char *pData)
{
    uint16_t crc16;
    memcpy(xTCPHeader, tcp_header, 60);

    for (uint8_t i = 0; i < 20; i++)
    {
        xTCPHeader[i + 4] = register_data[50 + i];
    }

    crc16 = CalcCRC((char *)pData, uiLength);
    xTCPHeader[52] = 0x00;
    xTCPHeader[53] = 0x00;
    xTCPHeader[54] = (uiLength >> 8);
    xTCPHeader[55] = (uiLength & 0x00ff);
    xTCPHeader[58] = (crc16 >> 8);
    xTCPHeader[59] = (crc16 & 0x00ff);
    HAL_UART_Transmit(&huart1, (uint8_t *)xTCPHeader, 60, 0xff);
    HAL_UART_Transmit(&huart1, (uint8_t *)pData, uiLength, 0xff);
}

void vUartTCPConnect(uint8_t ucAddress)
{
    char at_cmd_buff[60];
    static uint16_t tmp_length;
    unsigned char flag = 0x00;

    TCP_Msg_TypeDef *pxTCPMsg;
    pxTCPMsg = &xTCP_Msg;

    char xTCPPacketTmp[250] = {0};
    uint8_t i, j;

    switch (connect_status)
    {
        case 0x00: //进入串口配置模式
            osDelay(2000);
            at_cmd_buff[0] = 0x55;
            at_cmd_buff[1] = 0xaa;
            at_cmd_buff[2] = 0x5a;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 3, 0xff);
            osDelay(100);
            bATRecvOK = 0x00;
            at_cmd_buff[0] = 0xa5;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 1, 0xff);
            osDelay(100);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;
                connect_status++;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)     //退出串口配置模式
                {
                    repeat_time = 0x00;
                    bATRecvOK = 0x00;
                    at_cmd_buff[0] = 0x57;
                    at_cmd_buff[1] = 0xAB;
                    at_cmd_buff[2] = 0x5E;
                    HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 3, 0xff);
                }
            }

            if (KeySetIpFlag == 2)
            {
                connect_status = 0x10;
            }

            osDelay(50);
            break;

        case 0x10: //写本机IP
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xab;
            at_cmd_buff[2] = 0x11;
            at_cmd_buff[3] = 0xc0;
            at_cmd_buff[4] = 0x00;
            at_cmd_buff[5] = 0x00;
            at_cmd_buff[6] = 0x40;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 7, 0xff);
            osDelay(50);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;

                KeySetIpFlag = 0;
                connect_status = 0x11;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x11: //写子网掩码
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xab;
            at_cmd_buff[2] = 0x12;
            at_cmd_buff[3] = 0xff;
            at_cmd_buff[4] = 0xff;
            at_cmd_buff[5] = 0xff;
            at_cmd_buff[6] = 0x00;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 7, 0xff);
            osDelay(50);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;

                KeySetIpFlag = 0;
                connect_status = 0x12;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x12: //写网关
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xab;
            at_cmd_buff[2] = 0x13;
            at_cmd_buff[3] = 0xc0;
            at_cmd_buff[4] = 0x00;
            at_cmd_buff[5] = 0x00;
            at_cmd_buff[6] = 0x01;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 7, 0xff);
            osDelay(50);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;

                KeySetIpFlag = 0;
                connect_status = 0x13;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x13: //写远程IP
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xab;
            at_cmd_buff[2] = 0x15;
            at_cmd_buff[3] = 0xc0;
            at_cmd_buff[4] = 0x00;
            at_cmd_buff[5] = 0x00;
            at_cmd_buff[6] = 0xc9;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 7, 0xff);
            osDelay(50);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;

                KeySetIpFlag = 0;
                connect_status = 0x14;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x14: //写远程端口号
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xab;
            at_cmd_buff[2] = 0x16;
            at_cmd_buff[3] = 0x41;
            at_cmd_buff[4] = 0x9c;

            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 5, 0xff);
            osDelay(50);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;

                KeySetIpFlag = 0;
                connect_status = 0x15;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x15: //保存eeprom
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xab;
            at_cmd_buff[2] = 0x0d;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 3, 0xff);
            osDelay(50);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;

                KeySetIpFlag = 0;
                connect_status = 0x16;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x16: //执行配置，复位 9121
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xab;
            at_cmd_buff[2] = 0x0e;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 3, 0xff);
            osDelay(50);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;

                KeySetIpFlag = 0;
                connect_status = 0x00;
                repeat_time = 0x00;
                osDelay(100);
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x01: //读取模块IP
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xAB;
            at_cmd_buff[2] = 0x61;
            bATRecvOK = 0x00;
            pxTCPMsg->uiRxCnt = 0x00;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 3, 0xff);
            osDelay(100);
            memset(xTCP_Addr.ip_address, 0, 18); //clear

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;
                IpAddrHexToAscii(pxTCPMsg->ucMsgBuf, (uint8_t*)xTCP_Addr.ip_address);
                connect_status++;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;
                connect_status = 0x00;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x02://读取模块MAC
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xAB;
            at_cmd_buff[2] = 0x81;
            pxTCPMsg->uiRxCnt = 0x00;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 3, 0xff);
            osDelay(100);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;
                j = 0;
                xTCP_Addr.mac_address[0] = MacHexToAscii(pxTCPMsg->ucMsgBuf[0]/16);
                xTCP_Addr.mac_address[1] = MacHexToAscii(pxTCPMsg->ucMsgBuf[0]%16);
                xTCP_Addr.mac_address[2] = MacHexToAscii(pxTCPMsg->ucMsgBuf[1]/16);
                xTCP_Addr.mac_address[3] = MacHexToAscii(pxTCPMsg->ucMsgBuf[1]%16);
                xTCP_Addr.mac_address[4] = MacHexToAscii(pxTCPMsg->ucMsgBuf[2]/16);
                xTCP_Addr.mac_address[5] = MacHexToAscii(pxTCPMsg->ucMsgBuf[2]%16);
                xTCP_Addr.mac_address[6] = MacHexToAscii(pxTCPMsg->ucMsgBuf[3]/16);
                xTCP_Addr.mac_address[7] = MacHexToAscii(pxTCPMsg->ucMsgBuf[3]%16);
                xTCP_Addr.mac_address[8] = MacHexToAscii(pxTCPMsg->ucMsgBuf[4]/16);
                xTCP_Addr.mac_address[9] = MacHexToAscii(pxTCPMsg->ucMsgBuf[4]%16);
                xTCP_Addr.mac_address[10] = MacHexToAscii(pxTCPMsg->ucMsgBuf[5]/16);
                xTCP_Addr.mac_address[11] = MacHexToAscii(pxTCPMsg->ucMsgBuf[5]%16);

                for (i = 0; i < 12; i++)
                {
                    if ((j + 1) % 3)
                        xTCP_Addr.mac_address_format[j++] = xTCP_Addr.mac_address[i];
                    else
                    {
                        xTCP_Addr.mac_address_format[j++] = ':';
                        xTCP_Addr.mac_address_format[j++] = xTCP_Addr.mac_address[i];
                    }
                }

                connect_status++;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x03:
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xab;
            at_cmd_buff[2] = 0x0e;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 3, 0xff);
            osDelay(100);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;
                connect_status++;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x04: //推出串口配置模式
            at_cmd_buff[0] = 0x57;
            at_cmd_buff[1] = 0xAB;
            at_cmd_buff[2] = 0x5E;
            bATRecvOK = 0x00;
            HAL_UART_Transmit(&huart1, (uint8_t *)at_cmd_buff, 3, 0xff);
            osDelay(100);

            if (bATRecvOK)
            {
                bATRecvOK = 0x00;
                connect_status++;
                repeat_time = 0x00;
            }
            else
            {
                repeat_time++;

                if (repeat_time > 5)
                {
                    connect_status = 0x00;
                    repeat_time = 0x00;
                }
            }

            break;

        case 0x05:
            memset(register_data, 0, 240);
            strcpy(register_data, register_cmd);

            for (i = 0; i < 12; i++)
                register_data[58 + i] = xTCP_Addr.mac_address[i];

            for (i = 0; i < 17; i++)
                register_cmd_end[9 + i] = xTCP_Addr.mac_address_format[i];

            register_cmd_end[77] = (((ucAddress / 10) & 0x0f) + 0x30); // Id_Adderss[0];
            register_cmd_end[78] = (((ucAddress % 10) & 0x0f) + 0x30); // Id_Adderss[1];

            flag = AT24CXX_ReadOneByte(EE_SWITCHEN);

            if (flag == 0xAA)
                register_cmd_end[98] = '1';
            else
                register_cmd_end[98] = '0';

            tmp_length = strlen(xTCP_Addr.ip_address);
            strncat(register_data, xTCP_Addr.ip_address, tmp_length);
            strncat(register_data, register_cmd_end, 120);
            Pkcs5padding_add(register_data);
            tmp_length = strlen(register_data);
            aesEncrypt(pKey, 0x10, (unsigned char *)register_data, (unsigned char *)xTCPPacketTmp, tmp_length);
            Base64encode(xTCPPacketTmp, register_data_encrypt, tmp_length);
            vUartTCPRegister();
            bTCPConnected = 0x01;

            break;

        default:
            break;
    }
}

uint16_t CalcCRC(char *pbuf, uint16_t len)
{
    unsigned int crc = 0xFFFF;

    while (len-- > 0)
    {
        crc = crctab[(crc >> 8 ^ *pbuf++) & 0xff] ^ (crc << 8);
    }

    return crc;
}

void Pkcs5padding_add(char *input)
{
    unsigned char length;
    unsigned char i;
    char tmp[16] = {0};
    length = 16 - (strlen(input) % 16);

    for (i = 0; i < length; i++)
    {
        tmp[i] = length;
    }

    strncat(input, tmp, length);
}

void Base64encode(char *str, char *res, uint16_t str_length)
{
    uint16_t len;
    uint16_t str_len;
    //    char *res;
    int i, j;
    const unsigned char base64_table[65] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
    str_len = str_length; // strlen(str); //32;//strlen(str);

    if (str_len % 3 == 0)
    {
        len = str_len / 3 * 4;
    }
    else
        len = (str_len / 3 + 1) * 4;

    res[len] = '\0';

    for (i = 0, j = 0; i < len - 2; j += 3, i += 4)
    {
        res[i] = base64_table[str[j] >> 2];
        res[i + 1] = base64_table[(str[j] & 0x3) << 4 | (str[j + 1] >> 4)];
        res[i + 2] = base64_table[(str[j + 1] & 0xf) << 2 | (str[j + 2] >> 6)];
        res[i + 3] = base64_table[str[j + 2] & 0x3f];
    }

    switch (str_len % 3)
    {
        case 1:
            res[i - 2] = '=';
            res[i - 1] = '=';
            break;

        case 2:
            res[i - 1] = '=';
            break;
    }
}

void Base64decode(char *code, char *res, uint16_t code_length)
{
    int table[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63, 52, 53, 54,
                   55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                   10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0,
                   0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                   45, 46, 47, 48, 49, 50, 51
                  };
    uint16_t len;
    uint16_t str_len;
    //    char *res;//unsigned
    int i, j;
    len = code_length; // strlen(code);

    if (strstr(code, "=="))
        str_len = len / 4 * 3 - 2;
    else if (strstr(code, "="))
        str_len = len / 4 * 3 - 1;
    else
        str_len = len / 4 * 3;

    //    res = malloc(sizeof(unsigned char) * str_len + 1);
    res[str_len] = '\0';

    for (i = 0, j = 0; i < len - 2; j += 3, i += 4)
    {
        res[j] = ((unsigned char)table[code[i]]) << 2 | (((unsigned char)table[code[i + 1]]) >> 4);
        res[j + 1] = (((unsigned char)table[code[i + 1]]) << 4) | (((unsigned char)table[code[i + 2]]) >> 2);
        res[j + 2] = (((unsigned char)table[code[i + 2]]) << 6) | ((unsigned char)table[code[i + 3]]);
    }

    //   return res;
}

void IpAddrHexToAscii(uint8_t *str1, uint8_t *str2)
{
    uint8_t dataCnt = 0;

    if (str1[0] >= 100) //
    {
        str2[dataCnt++] = (str1[0] / 100) + 0x30; //百位
        str2[dataCnt++] = ((str1[0] % 100) / 10)+ 0x30;//十位
        str2[dataCnt++] = ((str1[0] % 100) % 10)+ 0x30;//个位
    }
    else if (str1[0] >= 10) //
    {
        str2[dataCnt++] = ((str1[0] % 100) / 10)+ 0x30;//十位
        str2[dataCnt++] = ((str1[0] % 100) % 10)+ 0x30;//个位
    }
    else
    {
        str2[dataCnt++] = ((str1[0] % 100) % 10)+ 0x30;//个位
    }

    str2[dataCnt++] = 0x2E;

    if (str1[1] >= 100) //
    {
        str2[dataCnt++] = (str1[1] / 100) + 0x30; //百位
        str2[dataCnt++] = ((str1[1] % 100) / 10)+ 0x30;//十位
        str2[dataCnt++] = ((str1[1] % 100) % 10)+ 0x30;//个位
    }
    else if (str1[1] >= 10) //
    {
        str2[dataCnt++] = ((str1[1] % 100) / 10)+ 0x30;//十位
        str2[dataCnt++] = ((str1[1] % 100) % 10)+ 0x30;//个位
    }
    else
    {
        str2[dataCnt++] = ((str1[1] % 100) % 10)+ 0x30;//个位
    }

    str2[dataCnt++] = 0x2E;

    if (str1[2] >= 100) //
    {
        str2[dataCnt++] = (str1[2] / 100) + 0x30; //百位
        str2[dataCnt++] = ((str1[2] % 100) / 10)+ 0x30;//十位
        str2[dataCnt++] = ((str1[2] % 100) % 10)+ 0x30;//个位
    }
    else if (str1[2] >= 10) //
    {
        str2[dataCnt++] = ((str1[2] % 100) / 10)+ 0x30;//十位
        str2[dataCnt++] = ((str1[2] % 100) % 10)+ 0x30;//个位
    }
    else
    {
        str2[dataCnt++] = ((str1[2] % 100) % 10)+ 0x30;//个位
    }

    str2[dataCnt++] = 0x2E;

    if (str1[3] >= 100) //
    {
        str2[dataCnt++] = (str1[3] / 100) + 0x30; //百位
        str2[dataCnt++] = ((str1[3] % 100) / 10)+ 0x30;//十位
        str2[dataCnt++] = ((str1[3] % 100) % 10)+ 0x30;//个位
    }
    else if (str1[3] >= 10) //
    {
        str2[dataCnt++] = ((str1[3] % 100) / 10)+ 0x30;//十位
        str2[dataCnt++] = ((str1[3] % 100) % 10)+ 0x30;//个位
    }
    else
    {
        str2[dataCnt++] = ((str1[3] % 100) % 10)+ 0x30;//个位
    }
}

uint8_t MacHexToAscii(uint8_t data)
{
    if (data <= 9)
    {
        data = data + 0x30;
    }
    else
    {
        data = data + 0x37;
    }

    return data;
}
