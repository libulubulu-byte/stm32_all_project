#include "uart2.h"

//extern const uint8_t CRCHi[];
//extern const uint8_t CRCLo[];

typedef struct
{
    uint8_t Year;
    uint8_t Month;
    uint8_t Date;
    uint8_t Hour;
    uint8_t Min;
    uint8_t Sec;
} __TIME;

char gATBuf[60] = {0};
char gComBuf[300] = {0};
uint16_t ucCom2RxCnt = 0;    //接收数据计数
uint8_t  gPCommand_flag = 0x00;

uint16_t ucCom2DataLength;  //接收的数据长度
uint8_t ucCom2Data[300];  //接收数据缓冲区



//uint8_t ucCom2Cmd;  //接收的命令
//uint8_t ucRecvDataCnt;
//uint8_t COM2LED_FLAG;
uint8_t bCom2RecvOK;

//extern uint8_t Record_ciCollect; //记录指针 记录总指针

//extern uint8_t POWER_FIRSTON;
//extern uint16_t Zone1_TimeJs, Zone2_TimeJs;
//extern const uint16_t Zone1_Time[];


void UART2_IRQHandler(void);

/*********************************************************************************************************
** Function name:       UARTInit
** Descriptions:        串口初始化，设置为8位数据位，1位停止位，无奇偶校验，波特率为115200
** input parameters:    uiDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART2Init( void )
{
    uint32_t usFdiv;
    PCONP = PCONP | (1 << 24);   /* 开启串口1功能模块            */
    U2LCR = 0x83;                /* 允许设置波特率               */
    usFdiv = (FPCLK / 16) / 115200; /* 设置波特率                */
    U2DLM = usFdiv / 256;
    U2DLL = usFdiv % 256;
    U2LCR = 0x03; /* 锁定波特率                   */
    U2FCR = 0x07; /* 使能FIFO，设置8个字节触发点  */
    U2IER = 0x01;
    zyIsrSet(NVIC_UART2, (unsigned long)UART2_IRQHandler, PRIO_ONE);
}

/***************************************************************************************************
** Function name:       uart2SendByte
** Descriptions:        向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart2SendByte(INT8U ucDat)
{
    U2THR = ucDat; // 写入数据
    while ((U2LSR & 0x20) == 0)
        ; // 等待数据发送完
}



void UART2_IRQHandler(void)
{
    //  unsigned int intCRC;
    unsigned char IIR, temp;
    while (((IIR = U2IIR) & 0x01) == 0) // 为0有效挂起的中断被通过
    {
        switch (IIR & 0x0E)
        {
        case 0x04:        //接收数据可用
            temp = U2RBR; //接收数据缓冲区的数据
            switch (ucCom2RxCnt)
            {
            case 0x00:
                if (temp == 'H' ) //0x48
                {
                    gComBuf[ucCom2RxCnt] = temp;
                    gPCommand_flag = 1;
                    ucCom2RxCnt++;
                }
                else if (temp == '\r' )
                {
                    gATBuf[ucCom2RxCnt] = temp;
                    gPCommand_flag = 3;
                    ucCom2RxCnt++;
                }
                else
                {
                    ucCom2RxCnt = 0;
                }
                break;
            case 0x01:
                if ((temp == 'K') && (gPCommand_flag == 0x01)) //0x4B
                {
                    gComBuf[ucCom2RxCnt] = temp;
                    gPCommand_flag = 2;
                    ucCom2RxCnt++;
                }
                else if ((temp == '\n' ) && (gPCommand_flag == 0x03))
                {
                    gATBuf[ucCom2RxCnt] = temp;
                    gPCommand_flag = 4;
                    ucCom2RxCnt++;
                }
                else
                {
                    ucCom2RxCnt = 0;
                    gATBuf[ucCom2RxCnt] = 0x00;
                    gComBuf[ucCom2RxCnt] = 0x00;
                }
                break;

            default:
                if (gPCommand_flag == 2)
                {
                    gComBuf[ucCom2RxCnt] = temp;
                    if (ucCom2RxCnt == 55)
                    {
                        ucCom2DataLength = gComBuf[54] * 256 + gComBuf[ucCom2RxCnt];
                    }
                    ucCom2RxCnt++;
                    if (ucCom2RxCnt > 60)
                    {
                        if (ucCom2RxCnt >= (ucCom2DataLength + 60))
                        {
                            bCom2RecvOK = 1;
                            ucCom2RxCnt = 0;
                        }
                        if (ucCom2RxCnt >= 300)
                        {
                            ucCom2RxCnt = 0;
                        }
                    }
                }
                else if (gPCommand_flag == 4)
                {
                    gATBuf[ucCom2RxCnt] = temp;
                    ucCom2RxCnt++;
                    if (ucCom2RxCnt > 60)
                    {
                        ucCom2RxCnt = 0x00;
                    }
                }
                else
                {
                    gComBuf[0] = 0x00;
                    gComBuf[1] = 0x00;
                    gATBuf[0] = 0x00;
                    gATBuf[1] = 0x00;
                    ucCom2RxCnt = 0;
                }
                break;
            }
            break;
        case     0x06:
            temp = U1RBR;
            break;

        case  0x0C:
            temp = U1RBR;
            break;

        default:
            break;
        }
    }
}


