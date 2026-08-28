/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "keyboard.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "disp.h"
#include "keyboard.h"
#include "var.h"
#include "powerpulse.h"
#include "stdio.h"
//#include "ds1302.h"
#include "uart_tcp.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern uint8_t RunTimeStartSaveFlag;

DEVICEDATA device;
static uint8_t ucBeepAlarmFlag;
static uint8_t ucRelayAlarmFlag;
static uint8_t ucEEAlarmFlag;
static uint16_t uiRunningTime;
static uint16_t beepon_delay_cnt[4];
static uint16_t relayon_delay_cnt[4];
static uint16_t led_period_cnt[3];
static uint8_t ucPulseOnLast;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t logicTaskHandle;
const osThreadAttr_t logicTask_attributes = {
    .name = "logicTask",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 128 * 4};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osThreadId_t uartTaskHandle;
osThreadId_t uTCPTaskHandle;
osThreadId_t displayTaskHandle;
osThreadId_t dinputTaskHandle;
osThreadId_t eepromTaskHandle;
const osThreadAttr_t uartTask_attributes = {
    .name = "uartTask",
    .priority = (osPriority_t)osPriorityNormal5,
    .stack_size = 128 * 8};
const osThreadAttr_t uartTCP_attributes = {
    .name = "uartTCPTask",
    .priority = (osPriority_t)osPriorityNormal4,
    .stack_size = 256 * 32};
const osThreadAttr_t displayTask_attributes = {
    .name = "displayTask",
    .priority = (osPriority_t)osPriorityNormal2,
    .stack_size = 128 * 4};
const osThreadAttr_t dinputTask_attributes = {
    .name = "dinputTask",
    .priority = (osPriority_t)osPriorityNormal3,
    .stack_size = 128 * 4};
const osThreadAttr_t eepromTask_attributes = {
    .name = "eepromTask",
    .priority = (osPriority_t)osPriorityNormal1,
    .stack_size = 128 * 4};
/* USER CODE END FunctionPrototypes */

void StartLogicTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */
    //  uint8_t ee_init_flag;
    //  AT24CXX_WriteOneByte (EE_INITIAL, 0xAA );
    device.bDeviceFlag = AT24CXX_ReadOneByte(EE_INITIAL);

    if (device.bDeviceFlag == 0x5A)
        ReadEEPROM(&device);
    else
    {
        init_EEPROM();
        ReadEEPROM(&device);
    }
	AlarmVoltageValueSelect();
	DefenceAreaSwitchTcpAck();
    //  osMessageQueueId_t xDisplayData;

    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    logicTaskHandle = osThreadNew(StartLogicTask, NULL, &logicTask_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    uartTaskHandle = osThreadNew(vUART_TASK, &device, &uartTask_attributes);
    displayTaskHandle = osThreadNew(vDisplayTask, &device, &displayTask_attributes);
    dinputTaskHandle = osThreadNew(vDinputTask, NULL, &displayTask_attributes);
    eepromTaskHandle = osThreadNew(vWriteEETask, NULL, &eepromTask_attributes);
    uTCPTaskHandle = osThreadNew(vUART_TCP_TASK, &device, &uartTCP_attributes);

    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartLogicTask(void *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    uint8_t last_status = 0;     // 用于存储上次的电源状态
    uint8_t bCheckEnable;        // 运行时间检查使能标志
    //  uint8_t lcd_update_flag;   // LCD更新标志（已注释）

    // 定义各种数据结构
    DINPUT sw_key;                      // 按键输入数据结构
    PulseOut_Msg_TypeDef xPulseOutData; // 脉冲输出数据结构
    PulseIn_Msg_TypeDef xPulseInData;   // 脉冲输入数据结构
    EEprom_Msg_TypeDef xEEpromWriteData;// EEPROM写入数据结构

    // 定义RTC时间和日期结构体并初始化为0
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    bCheckEnable = 0x00;          // 初始化运行时间检查标志为禁用
    device.ucBatteryLow = 0x00;    // 初始化电池低压标志
    
    // 调试模式下输出逻辑任务初始化成功信息
    #if Debug_flag 
    printf("logic task init success");
    #endif
    
    MX_DOG_Init();  // 初始化看门狗

    /* 无限循环 */
    for (;;)
    {
        Dog_ON();  // 喂狗（重置看门狗计数器）
        
        // 系统运行计数
        device.ucAddress = get_addr();  // 获取设备地址
        
        /* 处理按键变化消息队列 */
        if (osMessageQueueGet(xKeyChangeMsg, &sw_key, NULL, NULL) == osOK)
        {
            // 更新设备状态
            device.ucPowerStatus = sw_key.ucPowerStatus;    // 更新电源状态
            device.ucAlarmStatus &= 0x3c;                  // 保留高4位报警状态
            device.ucAlarmStatus |= sw_key.ucSwitchAlarm;   // 更新低2位报警状态
            device.ucKeyValue = sw_key.keyvalue;            // 更新按键值
            
            // 如果没有IR使能信号，清除IR报警状态
            if (device.ucIREn != 0xaa)
            {
                device.ucAlarmStatus &= 0xfd; // 清除bit0（IR报警位）
            }
            
            /* 处理防拆报警 */
            if (device.ucAlarmStatus & 0x01) // 防拆报警检测（位0）
            {
                ucBeepAlarmFlag |= 0x01;  // 设置蜂鸣器报警标志
                ucRelayAlarmFlag |= 0x01; // 设置继电器报警标志
            }
            
            /* 处理红外报警 */
            if (device.ucAlarmStatus & 0x02) // 红外报警检测（位1）
            {
                ucBeepAlarmFlag |= 0x02;  // 设置蜂鸣器报警标志
                ucRelayAlarmFlag |= 0x02; // 设置继电器报警标志
                
                // 检查是否需要记录EEPROM报警信息
                if (!(ucEEAlarmFlag & 0xfd))  // 检查IR报警是否已记录
                {
                    ucEEAlarmFlag |= 0x02;    // 设置IR报警记录标志
                    
                    // 获取当前RTC时间
                    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
                    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

                    // 准备写入EEPROM的数据
                    xEEpromWriteData.ucCmd = 0x01;                // EEPROM命令：报警记录
                    xEEpromWriteData.ucData[0] = 0x00;            // 报警类型：0x00 = 红外
                    xEEpromWriteData.ucData[1] = 0x03;            // 子类型
                    xEEpromWriteData.ucData[2] = 0x00;            // 保留
                    xEEpromWriteData.ucData[3] = sDate.Year;     // 年
                    xEEpromWriteData.ucData[4] = sDate.Month;    // 月
                    xEEpromWriteData.ucData[5] = sDate.Date;     // 日
                    xEEpromWriteData.ucData[6] = sTime.Hours;    // 时
                    xEEpromWriteData.ucData[7] = sTime.Minutes;  // 分
                    
                    // 发送消息到EEPROM写队列
                    osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
                }
            }
            else
            {
                ucEEAlarmFlag &= 0xfd;  // 清除IR报警记录标志
            }
        }
        
        /* 处理脉冲输出队列 */
        if (osMessageQueueGet(xPulseOutQueue, &xPulseOutData, NULL, NULL) == osOK)
        {
            // 更新设备状态
            device.ucAlarmStatus &= 0x03;                   // 保留低2位报警状态
            device.ucAlarmStatus |= ((xPulseOutData.ucAlarmStatus & 0x0f) << 2);  // 更新高4位报警状态
            device.ucAlarmType = xPulseOutData.ucAlarmType;      // 更新报警类型
            device.ucRunningStatus = xPulseOutData.ucRunningStatus;  // 更新运行状态

            // 更新电压电流读数
            device.ucVoltage[0] = xPulseOutData.ucVoltage[0];
            device.ucVoltage[1] = xPulseOutData.ucVoltage[1];
            device.ucCurrent[0] = xPulseOutData.ucCurrent[0];
            device.ucCurrent[1] = xPulseOutData.ucCurrent[1];

            /* 处理防御区1报警 */
            if (xPulseOutData.ucAlarmStatus & 0x01) // 防御区1报警检测（位0）
            {
                ucBeepAlarmFlag |= 0x04;  // 设置蜂鸣器报警标志
                ucRelayAlarmFlag |= 0x04; // 设置继电器报警标志
                
                // 检查是否需要记录EEPROM报警信息
                if (!(ucEEAlarmFlag & 0xfb))  // 检查防御区1报警是否已记录
                {
                    ucEEAlarmFlag |= 0x04;    // 设置防御区1报警记录标志
                    
                    // 获取当前RTC时间
 //                   HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
   //                 HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

                    // 准备写入EEPROM的数据
                    xEEpromWriteData.ucCmd = 0x01;                // EEPROM命令：报警记录
                    xEEpromWriteData.ucData[0] = 0x01;            // 报警区域：0x01 = 防御区1
                    xEEpromWriteData.ucData[1] = device.ucAlarmType & 0x0f;  // 报警类型（低4位）
                    xEEpromWriteData.ucData[2] = 0x00;            // 保留
                    xEEpromWriteData.ucData[3] = sDate.Year;     // 年
                    xEEpromWriteData.ucData[4] = sDate.Month;    // 月
                    xEEpromWriteData.ucData[5] = sDate.Date;     // 日
                    xEEpromWriteData.ucData[6] = sTime.Hours;    // 时
                    xEEpromWriteData.ucData[7] = sTime.Minutes;  // 分
                    
                    // 发送消息到EEPROM写队列
                    osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
                }
            }
            else
            {
                ucEEAlarmFlag &= 0xfb;  // 清除防御区1报警记录标志
            }
            
            /* 处理防御区2报警 */
            if (xPulseOutData.ucAlarmStatus & 0x02) // 防御区2报警检测（位1）
            {
                ucBeepAlarmFlag |= 0x08;  // 设置蜂鸣器报警标志
                ucRelayAlarmFlag |= 0x08; // 设置继电器报警标志
                
                // 检查是否需要记录EEPROM报警信息
                if (!(ucEEAlarmFlag & 0xf7))  // 检查防御区2报警是否已记录
                {
                    ucEEAlarmFlag |= 0x08;    // 设置防御区2报警记录标志
                    
                    // 获取当前RTC时间
                   // HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
                   // HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

                    // 准备写入EEPROM的数据
                    xEEpromWriteData.ucCmd = 0x01;                            // EEPROM命令：报警记录
                    xEEpromWriteData.ucData[0] = 0x02;                         // 报警区域：0x02 = 防御区2
                    xEEpromWriteData.ucData[1] = ((device.ucAlarmType & 0xf0) >> 4);  // 报警类型（高4位）
                    xEEpromWriteData.ucData[2] = 0x00;                         // 保留
                    xEEpromWriteData.ucData[3] = sDate.Year;                  // 年
                    xEEpromWriteData.ucData[4] = sDate.Month;                 // 月
                    xEEpromWriteData.ucData[5] = sDate.Date;                  // 日
                    xEEpromWriteData.ucData[6] = sTime.Hours;                 // 时
                    xEEpromWriteData.ucData[7] = sTime.Minutes;               // 分
                    
                    // 发送消息到EEPROM写队列
                    osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
                }
            }
            else
            {
                ucEEAlarmFlag &= 0xf7;  // 清除防御区2报警记录标志
            }
        }

        /* 检查电源状态 */
        switch (device.ucPowerStatus)
        {
        case 0x00:  // 外部电源供电正常
            device.ucPowerFlag = 0x00;  // 设置电源标志为外部电源正常
            
            // 如果之前是电池低压状态，现在恢复正常，关闭蜂鸣器
            if (device.ucBatteryLow == 0x55)
            {
                device.ucBatteryLow = 0;
                beep_off();
            }
            
            // 根据上次运行状态和设备当前运行状态设置脉冲输出
            if ((device.ucRunningStatus == 0) && (last_status == 0x01))
            {
                device.ucPulseOn = 0x03;  // 同时打开两个区域的脉冲输出
            }
            // 防御区1停止运行
            else if (((device.ucRunningStatus & 0x01) == 0) && (last_status == 0x02))
            {
                device.ucPulseOn = 0x01;  // 只打开区域1的脉冲输出
            }
            // 防御区2停止运行
            else if (((device.ucRunningStatus & 0x02) == 0) && (last_status == 0x03))
            {
                device.ucPulseOn = 0x02;  // 只打开区域2的脉冲输出
            }
            last_status = 0;  // 重置上次状态
            
            // 设置GPIOB PIN4为高电平（可能用于控制外设）
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
            break;

        case 0x01:  // 备用电源供电
            device.ucPowerFlag = 0x01;  // 设置电源标志为备用电源
            // 可能的GPIO控制（已注释）
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
            break;
            
        case 0x02:  // 电池供电
            device.ucPowerFlag = 0x02;  // 设置电源标志为电池供电
            
            // 如果之前是电池低压状态，现在恢复正常，关闭蜂鸣器
            if (device.ucBatteryLow == 0x55)
            {
                device.ucBatteryLow = 0;
                beep_off();
            }
            
            // 设置GPIOB PIN4为低电平（可能用于控制外设）
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
            break;
            
        case 0x03:  // 低压告警状态
            // 只有在运行时间检查使能时才处理
            if (bCheckEnable)
            {
                device.ucPowerFlag = 0x03;  // 设置电源标志为低压告警
                
                // 如果系统正在工作且不是低压状态
                if ((device.ucRunningStatus == 0x03) && (device.ucBatteryLow == 0x00))
                {
                    device.ucPulseOn = 0x00;  // 关闭所有脉冲输出
                    last_status = 0x01;       // 记录上次状态
                }
                else if ((device.ucRunningStatus & 0x01) && (device.ucBatteryLow == 0x00))
                {
                    device.ucPulseOn &= 0x01;  // 只保留区域1（如果正在运行）
                    last_status = 0x02;          // 记录上次状态
                }
                else if ((device.ucRunningStatus & 0x02) && (device.ucBatteryLow == 0x00))
                {
                    device.ucPulseOn &= 0xfd;  // 只保留区域2（如果正在运行）
                    last_status = 0x03;          // 记录上次状态
                }
                
                // 设置低压标志并发出蜂鸣提示（滴-滴声）
                device.ucBatteryLow = 0x55;  // 设置低压标志
                beep_on();       // 开启蜂鸣器
                osDelay(100);    // 持续100ms
                beep_off();      // 关闭蜂鸣器
                osDelay(900);    // 暂停900ms
            }
            break;
        default:
            break;
        }

        /* LED指示灯控制 */
        // 如果任何一个区域有脉冲输出，进行LED呼吸灯控制
        if ((device.ucPulseOn & 0x01) || (device.ucPulseOn & 0x02))
        {
            // LED呼吸灯周期计数
            led_period_cnt[0]++;
            
            // 每1500次重置计数器
            if (led_period_cnt[0] > 1500)
                led_period_cnt[0] = 0;
            // 前500次点亮LED
            else if (led_period_cnt[0] < 500)
                led2_on();   // 开启LED2
            // 后1000次熄灭LED
            else
                led2_off();  // 关闭LED2
        }
        else
        {
            // 没有脉冲输出时处理键盘
            ProcessKeyBord(&device);
        }

        /* 按键事件处理逻辑 */
        // 根据不同的按键事件和设备状态执行不同操作
        
        // 当系统未运行且按键短按
        if ((device.ucKeyValue == KEYENT_SHORT) && (device.ucKeyStatus == 0) && (device.ucRunningStatus == 0x00))
        {
            device.ucKeyValue = 0;  // 清除按键值
            device.ucLcdOnTime = 120;  // 设置LCD背光时间（120个循环）
            lcd_backup_on();        // 打开LCD背光
            device.ucPulseOn = 0x03; // 开启两个区域的脉冲输出（布防）
        }
        // 当系统全速运行且按键短按
        else if ((device.ucKeyValue == KEYENT_SHORT) && (device.ucKeyStatus == 0) && (device.ucRunningStatus == 0x03))
        {
            device.ucKeyValue = 0;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn = 0x00; // 关闭所有脉冲输出（撤防）
        }
        // 当只有区域1运行且按键短按
        else if ((device.ucKeyValue == KEYENT_SHORT) && (device.ucKeyStatus == 0) && (device.ucRunningStatus == 0x01))
        {
            device.ucKeyValue = 0;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn = 0x00; // 关闭所有脉冲输出
        }
        // 当只有区域2运行且按键短按
        else if ((device.ucKeyValue == KEYENT_SHORT) && (device.ucKeyStatus == 0) && (device.ucRunningStatus == 0x02))
        {
            device.ucKeyValue = 0;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn = 0x00; // 关闭所有脉冲输出
        }
        
        /* 特殊布防/撤防指令处理 */
        // 布防区域1
        else if ((device.ucSetAlarm[0] == 0xaa) && (device.ucSetAlarm[1] == 0xaa) && (device.ucKeyStatus == 0) && (device.ucRunningStatus == 0x00))
        {
            device.ucSetAlarm[0] = 0;
            device.ucSetAlarm[1] = 0;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn = 0x03; // 布防两个区域
        }
        // 布防区域1
        else if ((device.ucSetAlarm[0] == 0xaa) && (device.ucKeyStatus == 0) && ((device.ucRunningStatus & 0x01) == 0))
        {
            device.ucSetAlarm[0] = 0;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn |= 0x01; // 布防区域1
        }
        // 布防区域2
        else if ((device.ucSetAlarm[1] == 0xaa) && (device.ucKeyStatus == 0) && ((device.ucRunningStatus & 0x02) == 0))
        {
            device.ucSetAlarm[1] = 0;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn |= 0x02; // 布防区域2
        }
        // 撤防两个区域
        else if ((device.ucSetAlarm[0] == 0x55) && (device.ucSetAlarm[1] == 0x55) && (device.ucKeyStatus == 0) && (device.ucRunningStatus == 0x03))
        {
            device.ucSetAlarm[0] = 0x00;
            device.ucSetAlarm[1] = 0x00;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn = 0x00; // 撤防两个区域
        }
        // 撤防区域1
        else if ((device.ucSetAlarm[0] == 0x55) && (device.ucRunningStatus & 0x01) && (device.ucKeyStatus == 0))
        {
            device.ucSetAlarm[0] = 0x00;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn &= 0xfe; // 撤防区域1
        }
        // 撤防区域2
        else if ((device.ucSetAlarm[1] == 0x55) && (device.ucRunningStatus & 0x02) && (device.ucKeyStatus == 0))
        {
            device.ucSetAlarm[1] = 0x00;
            device.ucLcdOnTime = 120;
            lcd_backup_on();
            device.ucPulseOn &= 0xfd; // 撤防区域2
        }

        /* 报警处理 */
        for (uint8_t i = 0; i < 4; i++)  // 循环处理4种报警类型（防拆、红外、防御区1、防御区2）
        {
            // 蜂鸣器报警处理
            if (device.ucAlarmStatus & (1 << i))  // 检查是否发生了当前报警类型的报警
            {
                device.ucLcdOnTime = 120;  // 设置LCD背光时间
                lcd_backup_on();        // 打开LCD背光
                beep_on();              // 打开蜂鸣器
                beepon_delay_cnt[i] = 0; // 重置蜂鸣器延时计数器
            }
            else if (ucBeepAlarmFlag & (1 << i))  // 如果蜂鸣器报警标志被设置
            {
                beepon_delay_cnt[i]++;  // 增加蜂鸣器延时计数器
                
                // 检查是否达到设定的蜂鸣器开启时间
                if (beepon_delay_cnt[i] >= device.ucBeepOnTime)
                {
                    // 清除蜂鸣器报警标志
                    ucBeepAlarmFlag &= ~(1 << i);
                    beepon_delay_cnt[i] = device.ucBeepOnTime;  // 设置计数器为最大值（防止溢出）
                    
                    // 如果没有其他蜂鸣器报警，关闭蜂鸣器
                    if (ucBeepAlarmFlag == 0x00)
                        beep_off();
                }
            }
            
            // 继电器报警处理
            if (device.ucAlarmStatus & (1 << i))  // 检查是否发生了当前报警类型的报警
            {
                Relay_SetBit(i, GPIO_PIN_SET);  // 设置对应继电器位
                relayon_delay_cnt[i] = 0;        // 重置继电器延时计数器
            }
            else if (ucRelayAlarmFlag & (1 << i))  // 如果继电器报警标志被设置
            {
                relayon_delay_cnt[i]++;  // 增加继电器延时计数器
                
                // 检查是否达到设定的继电器开启时间
                if (relayon_delay_cnt[i] >= device.ucRelayOnTime)
                {
                    relayon_delay_cnt[i] = device.ucRelayOnTime;  // 设置计数器为最大值（防止溢出）
                    
                    // 清除继电器报警标志
                    ucRelayAlarmFlag &= ~(1 << i);
                    
                    // 处理继电器关闭
                    if (i < 2)  // 前两个报警类型（防拆和红外）
                    {
                        // 如果两个都不需要报警了，才关闭继电器
                        if ((ucRelayAlarmFlag & 0x03) == 0x00)
                            Relay_SetBit(i, GPIO_PIN_RESET);
                    }
                    else  // 后两个报警类型（防御区1和2）
                    {
                        Relay_SetBit(i, GPIO_PIN_RESET);
                    }
                }
            }
        }
        
        // 如果不是双防御模式，则只保留区域1的脉冲输出
        if(DUAL_DEFENCE != 1)
            device.ucPulseOn &= 0x01;

        /* 同步脉冲输出状态 */
        if ((device.ucPulseOn ^ device.ucRunningStatus) || device.ucForceSync)
        {
            // 准备脉冲输入数据
            xPulseInData.ucPulseOn = device.ucPulseOn;      // 设置脉冲开启状态
            xPulseInData.ucAlarmCycle = device.ucAlarmCycle; // 设置报警周期
            xPulseInData.ucTouchEN = device.ucTouchEN;      // 设置触摸使能
            
            // 电压范围限制（10-65V）
            if (device.ucSetVoltage[0] < 10)
                device.ucSetVoltage[0] = 9;    // 最小限制
            else if (device.ucSetVoltage[0] > 65)
                device.ucSetVoltage[0] = 65;   // 最大限制
                
            // 同上处理区域2电压
            if (device.ucSetVoltage[1] < 10)
                device.ucSetVoltage[1] = 9;
            else if (device.ucSetVoltage[1] > 65)
                device.ucSetVoltage[1] = 65;
                
            // 设置电压值到数据结构
            xPulseInData.ucSetVoltage[0] = device.ucSetVoltage[0];
            xPulseInData.ucSetVoltage[1] = device.ucSetVoltage[1];
            
            // 发送脉冲输入数据到队列
            osMessageQueuePut(xPulseInQueue, &xPulseInData, 0, NULL);
            
            // 如果状态变化或强制同步
            if ((ucPulseOnLast != device.ucPulseOn) || device.ucForceSync)
            {
                // 准备EEPROM写入数据
                xEEpromWriteData.ucCmd = 0x02;                     // 命令：写入脉冲状态配置
                xEEpromWriteData.ucData[0] = device.ucPulseOn;     // 脉冲开启状态
                xEEpromWriteData.ucData[1] = device.ucSetVoltage[0]; // 区域1电压
                xEEpromWriteData.ucData[2] = device.ucSetVoltage[1]; // 区域2电压
                
                // 发送EEPROM写入请求
                osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
                
                device.ucForceSync = 0x00;             // 清除强制同步标志
                ucPulseOnLast = xPulseInData.ucPulseOn; // 更新上次脉冲状态
            }
        }

        // 系统运行时间计数
        uiRunningTime++;
        
        /* 上电次数统计 */
        if ((uiRunningTime == 1000) && (!bCheckEnable))
        {
            // 读取EEPROM中的上电次数
            device.ucPowerOnTime = AT24CXX_ReadOneByte(POWERON_CNT);
            device.ucPowerOnTime = device.ucPowerOnTime + 1;  // 增加计数
            
            // 准备写入EEPROM的数据
            xEEpromWriteData.ucCmd = 0x05;                    // 命令：写入上电次数
            xEEpromWriteData.ucData[0] = device.ucPowerOnTime; // 新的上电次数
            
            // 发送EEPROM写入请求
            osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
        }
        else if (uiRunningTime >= 5000) // 5秒
        {
            uiRunningTime = 0x00;      // 重置运行时间计数器
            bCheckEnable = 0x01;       // 启用运行时间检查
        }
        
        /* 运行时间分钟中断标志 */
        if(RunTimeStartSaveFlag == 1)
        {
            RunTimeStartSaveFlag = 0;  // 清除保存标志
            
            // 更新运行时间统计
            device.ucRunningMin++;  // 增加分钟计数
            // 如果分钟达到60
            if (device.ucRunningMin >= 60)
            {
                device.ucRunningMin = 0x00; // 重置分钟
                device.ucRunningHour++;     // 增加小时
                // 如果小时达到24
                if (device.ucRunningHour >= 24)
                {
                    device.ucRunningHour = 0x00; // 重置小时
                    device.uiRunningDate++;     // 增加天数
                    // 如果天数达到365
                    if (device.uiRunningDate >= 365)
                    {
                        device.uiRunningDate = 0x00; // 重置天数
                        device.ucRunningYear++;    // 增加年数
                    }
                }
            }
            
            // 准备写入EEPROM的运行时间数据
            xEEpromWriteData.ucCmd = 0x04;                       // 命令：写入运行时间
            xEEpromWriteData.ucData[0] = device.ucRunningMin;     // 分钟
            xEEpromWriteData.ucData[1] = device.ucRunningHour;    // 小时
            xEEpromWriteData.ucData[2] = device.uiRunningDate / 256; // 日期高位
            xEEpromWriteData.ucData[3] = device.uiRunningDate % 256; // 日期低位
            xEEpromWriteData.ucData[4] = device.ucRunningYear;    // 年
            
            // 发送EEPROM写入请求
            osMessageQueuePut(xEEWriteMsg, &xEEpromWriteData, 0, NULL);
        }
        
        // 释放CPU 1ms（FreeRTOS延时）
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
