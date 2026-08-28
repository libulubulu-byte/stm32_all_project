
#include "cmsis_os.h"
#include "pcf8562.h"
//#include "ds1302.h"
#include "var.h"
#include "keyboard.h"
#include "disp.h"
#include "eeprom.h"

//alarm status
#define ALARM_OPEN   0x00
#define ALARM_SHORT  0xFF
#define ALARM_NORMAL 0x80

//#define ARARM_BYPASS 0x20

extern void SelectSwVersion(void);
extern void UART_Init(uint32_t bps);
//defence area status
uint8_t record_num;

void HAL_LED_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6; //relay control gpio
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_7; //LED[0:3] BEEP

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);

}

void Relay_SetBit(uint8_t ucChannel, GPIO_PinState state)
{
    switch (ucChannel)
    {
        case 0x00:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, state);  //ctrl 3
            break;

        case 0x01:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, state);  //ctrl 3
            break;

        case 0x02:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, state);  //ctrl 2
            break;

        case 0x03:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, state);   //ctrl 1
            break;

        default:
            break;
    }
}


void ReadEEPROM(DEVICEDATA* pd)
{
    unsigned char temp;
    uint16_t date;
    pd->ucIREn = AT24CXX_ReadOneByte(EE_SWITCHEN);  //0xaa; // AT24CXX_ReadOneByte( EE_SWITCHEN )
    pd->ucTampEn = AT24CXX_ReadOneByte(EE_TAMPEN);

    pd->ucSetVoltage[0] = AT24CXX_ReadOneByte(EE_VOLTAGE1);
    pd->ucSetVoltage[1] = AT24CXX_ReadOneByte(EE_VOLTAGE2);
    pd->ucPulseOn = AT24CXX_ReadOneByte(EE_STATUS1);
    pd->ucAlarmCycle = AT24CXX_ReadOneByte(EE_ALARMCYCLES);

    pd->ucTouchEN = AT24CXX_ReadOneByte(EE_TOUCHEN);
    pd->ucRunningMin = AT24CXX_ReadOneByte(EE_MIN);
    pd->ucRunningHour = AT24CXX_ReadOneByte(EE_HOUR);
    date = AT24CXX_ReadOneByte(EE_DATE);
    date = (date << 8) + AT24CXX_ReadOneByte(EE_DATE + 1);
    pd->uiRunningDate = date;
    pd->ucRunningYear = AT24CXX_ReadOneByte(EE_YEAR);

    date = AT24CXX_ReadOneByte(EE_BPSH);
    date = (date << 8) + AT24CXX_ReadOneByte(EE_BPSL);
    pd->uiBaudrate = date;

    temp = AT24CXX_ReadOneByte(EE_BEEPTIME);

    if (DUAL_DEFENCE == 0)
        pd->ucBeepOnTime = temp * 1000;
    else if (DUAL_DEFENCE == 1)
        pd->ucBeepOnTime = temp * 900;

    temp = AT24CXX_ReadOneByte(EE_RELAYTIME);

    if (DUAL_DEFENCE == 0)
        pd->ucRelayOnTime = temp * 1000;
    else if (DUAL_DEFENCE == 1)
        pd->ucRelayOnTime = temp * 900;

    SIX_LINE_BOARD = AT24CXX_ReadOneByte(EE_SIX_LINE_BOARD);
    DUAL_DEFENCE = AT24CXX_ReadOneByte(EE_DUAL_DEFENCE);
    SIX_LINE_BOARD = 1;    //1: 6 Line
    DUAL_DEFENCE = 1;      //1: DUAL DEFENCE
    SelectSwVersion();
}

void init_EEPROM(void)
{
    AT24CXX_WriteOneByte(EE_INITIAL, 0x5A);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_BPSH, 38400 >> 8);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_BPSL, 38400 & 0xff);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(POWERON_CNT, 0x00);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_VOLTAGE1, 45);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_VOLTAGE2, 45);   //default set voltage is 3.0KV
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_STATUS1, 0);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_SWITCHEN, 0x55);   //the integrate switch is default on when power up
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_TAMPEN, 0xaa);   //the integrate switch is default on when power up
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_TOUCHEN, 0x00);   //the integrate switch is default on when power up
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_BEEPTIME, 10);   //set the beep on time is 10s by default
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_RELAYTIME, 10);   //set the relay on time is 10s by default
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_ALARMCYCLES, 2);  //set alarm trigger time to 3 pulse
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_RECORD, 0);   // init the alarm record time
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_MIN, 0);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_HOUR,  0);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_DATE,  0);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_DATE + 1,  0);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_YEAR, 0);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_SIX_LINE_BOARD, 0);
    __nop();
    __nop();
    __nop();
    __nop();
    AT24CXX_WriteOneByte(EE_DUAL_DEFENCE, 0);
}

void ProcessKeyBord(DEVICEDATA* pd)
{
    uint16_t baud_rate;
    EEprom_Msg_TypeDef xEEpromWriteData;

    if (pd->ucKeyValue == KEYMODE_SHORT)
    {
        pd->ucKeyValue = 0;

        pd->ucKeyStatus ++;

        if (pd->ucKeyStatus == 2)
            pd->ucKeyStatus = 3;

        pd->ucLcdOnTime = 120;
        lcd_backup_on();
    }
    else if (pd->ucKeyValue == KEYMODE_LONG)
    {
        pd->ucKeyValue = 0;
        pd->ucLcdOnTime = 120;
        lcd_backup_on();
    }

    if (pd->ucKeyStatus == 1)
    {
        pd->bOnSetting = 1 ;

        if (pd->ucKeyValue == KEYUP_SHORT)
        {
            pd->ucKeyValue = 0;
            pd->ucLcdOnTime = 120;
            lcd_backup_on();

            if (++ pd->ucSetVoltage[0] > 65)
                pd->ucSetVoltage[0] = 9;

            pd->ucSetVoltage[1] = pd->ucSetVoltage[0];
        }

        else if (pd->ucKeyValue == KEYDOWN_SHORT)
        {
            pd->ucKeyValue = 0;
            pd->ucLcdOnTime = 120;
            lcd_backup_on();

            if (pd->ucSetVoltage[0] > 9)
                pd->ucSetVoltage[0] -- ;
            else
                pd->ucSetVoltage[0] = 65;

            pd->ucSetVoltage[1] = pd->ucSetVoltage[0];

        }
        else if (pd->ucKeyValue == KEYDOWN_LIAN)
        {
            pd->ucKeyValue = 0;
            pd->ucLcdOnTime = 120;
            lcd_backup_on();

            if (pd->ucSetVoltage[0] > 9)
                pd->ucSetVoltage[0] -- ;
            else
                pd->ucSetVoltage[0] = 65;

            pd->ucSetVoltage[1] = pd->ucSetVoltage[0];
        }
        else if (pd->ucKeyValue == KEYUP_LIAN)
        {
            pd->ucKeyValue = 0;
            pd->ucLcdOnTime = 120;
            lcd_backup_on();

            if (++ pd->ucSetVoltage[0] > 65)
                pd->ucSetVoltage[0] = 9;

            pd->ucSetVoltage[1] = pd->ucSetVoltage[0];
        }
        else if (pd->ucKeyValue == 0xb4)
        {
            pd->ucKeyValue = 0;
            xEEpromWriteData.ucCmd = 0x04;
            xEEpromWriteData.ucData[0] = 0x00;
            xEEpromWriteData.ucData[1] = 0x00;
            xEEpromWriteData.ucData[2] = 0x00;
            xEEpromWriteData.ucData[3] = 0x00;
            xEEpromWriteData.ucData[4] = 0x00;
            osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
            pd->ucRunningYear = 0;
            pd->uiRunningDate = 0;
            pd->ucRunningHour = 0;
            pd->ucRunningMin = 0;
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
            pd->ucKeyStatus = 0;
            pd->ucForceSync = 0x01;
            pd->bOnSetting = 0 ;
            KeySetIpFlag = 1;
#if Debug_flag
            printf("\r\\KeySetIpFlag =1");
#endif
        }
    }
    else if (pd->ucKeyStatus == 3)
    {
        pd->ucKeyStatus = 0;
        pd->ucForceSync = 0x01;
        pd->bOnSetting = 0 ;
    }

    if (pd->ucKeyValue == 0xB2)
    {
        beep_on();
        osDelay(3);
        pd->ucKeyValue = 0;
        baud_rate = 19200;
        xEEpromWriteData.ucCmd = 0x03;
        xEEpromWriteData.ucData[0] = baud_rate >> 8;
        xEEpromWriteData.ucData[1] = baud_rate & 0xff;
        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);

        osDelay(50);
        beep_off();
#if Debug_flag

        printf("B2baud=%d", baud_rate);
#endif
        UART_Init(baud_rate);
    }
    else if (pd->ucKeyValue == 0xB3)
    {
        beep_on();
        osDelay(3);
        pd->ucKeyValue = 0;
        baud_rate = 38400;
        xEEpromWriteData.ucCmd = 0x03;
        xEEpromWriteData.ucData[0] = baud_rate >> 8;
        xEEpromWriteData.ucData[1] = baud_rate & 0xff;
        osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
        osDelay(50);
        beep_off();
		#if Debug_flag 

        printf("B3baud=%d", baud_rate);
		#endif
		
        UART_Init(baud_rate);
    }
}

void HAL_SW_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; // sw address
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

uint8_t get_addr(void)
{
    uint8_t addr = 0;

    if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0))
        addr |= 0x80;

    if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1))
        addr |= 0x40;

    if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2))
        addr |= 0x20;

    if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3))
        addr |= 0x10;

    if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4))
        addr |= 0x08;

    if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5))
        addr |= 0x04;

    if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6))
        addr |= 0x02;

    if (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7))
        addr |= 0x01;

    addr = ((addr & 0xf0) >> 4) * 10 + (addr & 0x0f);
    return addr;
}


