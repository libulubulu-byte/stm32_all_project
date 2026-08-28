#include "powerpulse.h"
#include "adc.h"
#include "dma.h"
#include "stdlib.h"
#include "var.h"
#include <string.h>

#define PULSE_PERIOD 1000
#define PWM_P1_PIN GPIO_PIN_11
#define PWM_T1_PIN GPIO_PIN_12 // 11

#define PWM_P2_PIN GPIO_PIN_9 // 10
#define PWM_T2_PIN GPIO_PIN_10

osThreadId_t pulseTaskHandle;
const osThreadAttr_t pulseTask_attributes =
{
    .name = "pulseTask",
    .priority = (osPriority_t)osPriorityAboveNormal,
    .stack_size = 128 * 16
};

// const uint8_t voltage_to_cnt[] =    //.0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
//{
//     0, 5,  5,  5,  5,  5,  5,  5,  5,
//     //Output KV   0.9  1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0 2.1 2.2 2.3 2.4
//                    9,   9,   9,   10,  10,  11,  12,  14,  15,  16,  17, 18, 20,  22, 24, 26,
//     //OutPut KV  2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0
//                   28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58,
//     //Output KV  4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0 5.1 5.2 5.3 5.4 5.5 5.6
//                   60, 62, 64, 66, 68, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
//     //Output KV   5.7 5.8 5.9 6.0 6.1 6.2 6.3 6.4  6.5  6.6  6.7  6.8  6.9  7.0  7.1 7.2   7.3  7.4  7.5
//                   95, 96, 97, 98, 98, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
// } ;//voltage range from 1.2~5.5KV

const uint8_t voltage_to_cnt[] = //.0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
{
    0, 5, 5, 5, 5, 5, 5, 5, 5,
// Output KV  0.9  1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    20, 20,  20,  20,  20,  20,  29,  30,  32,  33,  35,  36,  38,  39,  41,  42,
// OutPut KV  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    44, 45,  47,  48,  50,  51,  53,  54,  56,  57,  59,  60,  62,  63,  65,  66,
// Output KV  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    68, 70,  71,  73,  75,  78,  80,  83,  86,  89,  92,  94,  96,  98,  100, 102,
// Output KV  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0  7.1 7.2   7.3  7.4  7.5
    104, 106, 108, 110, 112, 114, 116, 118, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120
}; // voltage range from 1.2~5.5KV

// const uint8_t voltage_to_cnt[] =    //.0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
//{
//     0, 7,  7,  7,  7,  7,  7,  7,  7,
//     //Output KV   .9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4
//     12, 12,  12,  12, 12,  12,  13,  14,  16,  18,  20,  22, 24,  26,  28, 30,
//     //OutPut KV  2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0
//     32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62,
//     //Output KV  4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0 5.1 5.2 5.3 5.4 5.5 5.6
//     64, 66, 68, 70, 72, 74, 76, 79, 82, 85, 88, 91, 94, 97, 100, 103,
//     //Output KV   5.7 5.8 5.9 6.0 6.1 6.2 6.3 6.4 6.5 6.6 6.7 6.8 6.9 7.0 7.1 7.2 7.3 7.4 7.5
//     106, 109, 112, 115, 118, 121, 124, 127, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130
// } ;

const uint8_t voltage_to_current[] = {0, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                      21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
                                      37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, // 2.5~45
                                      53, 54, 55, 56, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
                                      72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
                                      88, 89, 90, 91, 92, 93, 94, 95
                                     };
// const uint8_t voltage_to_alarm[] =     //.0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
//{
//     0, 18,  18,  18,  18,  18,  18,  18,  18,
//     //Output KV   .9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4
//     20, 20,  20,  20,  20,  24,  28,  34,  38,  42,  48,  54, 60,  66,  74, 80,
//     //OutPut KV  2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0
//     86, 90, 95, 100, 105, 110, 112, 115, 118, 121, 124, 127, 130, 133, 135, 138,
//     //Output KV  4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0 5.1 5.2 5.3 5.4 5.5 5.6
//     140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 161, 162, 163, 164, 165,
//     //Output KV   5.7 5.8 5.9 6.0 6.1 6.2 6.3 6.4 6.5 6.6 6.7 6.8 6.9 7.0 7.1 7.2 7.3 7.4 7.5
//     167, 168, 170, 172, 173, 175, 176, 177, 178, 179, 180, 180, 180, 180, 180, 180, 180, 180, 180
// } ;
const uint16_t voltage_to_alarm[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 30,
// .9     1.0   1.1   1.2   1.3   1.4   1.5   1.6   1.7   1.8   1.9   2.0   2.1   2.2   2.3   2.4
    120,  125,  130,  135,  140,  145,  150,  160,  170,  180,  190,  200,  230,  250,  260,  270,
//  2.5  2.6   2.7   2.8   2.9   3.0   3.1   3.2   3.3   3.4   3.5   3.6   3.7   3.8   3.9   4.0
    300, 305,  310,  315,  320,  330,  340,  350,  355,  360,  370,  375,  380,  385,  390,  400,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 435, 435, 435, 435, 435, 435,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0  7.1  7.2
    435, 435, 435, 435, 435, 435, 445, 445, 445, 445, 445, 445, 445, 445, 445, 445,
//  7.3  7.4  7.5  7.6  7.7  7.8  7.9  8.0
    445, 445, 445, 445, 445, 445, 445, 445
} ;  // for 400K resistor

uint8_t  voltage_to_alarm1[71], voltage_to_alarm2[71];
//6双
const uint8_t voltage_to_alarm26_1[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 130,
// .9    1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    220, 220, 220, 220, 220, 220, 179, 179, 179, 180, 180, 180, 180, 180, 166, 166,
//  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    166, 166, 166, 151, 151, 151, 151, 151, 141, 141, 141, 141, 141, 145, 145, 145,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    145, 145, 103, 103, 103, 103, 103, 110, 110, 110, 110, 110, 107, 107, 107, 107,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0
    107, 104, 104, 104, 104, 104, 116, 116, 116, 116, 116, 116, 116, 116
} ;
const uint8_t voltage_to_alarm26_2[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 120,
// .9    1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    208, 208, 208, 208, 208, 208, 179, 179, 179, 180, 180, 180, 180, 180, 166, 166,
//  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    166, 166, 166, 151, 151, 151, 151, 151, 141, 141, 141, 141, 141, 145, 145, 145,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    145, 145, 150, 150, 150, 150, 150, 125, 125, 125, 125, 125, 115, 115, 115, 115,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0
    115, 111, 111, 111, 111, 111, 116, 116, 116, 116, 116, 116, 116, 116
} ;
//6单
const uint8_t voltage_to_alarm16_1[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 130,
// .9    1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    220, 220, 220, 220, 220, 220, 179, 179, 179, 180, 180, 180, 180, 180, 166, 166,
//  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    166, 166, 166, 151, 151, 151, 151, 151, 141, 141, 141, 141, 141, 145, 145, 145,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    145, 145, 103, 103, 103, 103, 103, 110, 110, 110, 110, 110, 107, 107, 107, 107,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0
    107, 104, 104, 104, 104, 104, 116, 116, 116, 116, 116, 116, 116, 116
} ;

const uint8_t voltage_to_alarm16_2[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 120,
// .9    1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    208, 208, 208, 208, 208, 208, 179, 179, 179, 180, 180, 180, 180, 180, 166, 166,
//  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    166, 166, 166, 151, 151, 151, 151, 151, 141, 141, 141, 141, 141, 145, 145, 145,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    145, 145, 150, 150, 150, 150, 150, 125, 125, 125, 125, 125, 115, 115, 115, 115,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0
    115, 111, 111, 111, 111, 111, 116, 116, 116, 116, 116, 116, 116, 116
} ;
//4双
const uint8_t voltage_to_alarm24_1[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 130,
// .9    1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    220, 220, 220, 220, 220, 220, 179, 179, 179, 180, 180, 180, 180, 180, 166, 166,
//  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    166, 166, 166, 151, 151, 151, 151, 151, 141, 141, 141, 141, 141, 145, 145, 145,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    145, 145, 103, 103, 103, 103, 103, 110, 110, 110, 110, 110, 107, 107, 107, 107,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0
    107, 104, 104, 104, 104, 104, 116, 116, 116, 116, 116, 116, 116, 116
} ;
const uint8_t voltage_to_alarm24_2[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 120,
// .9    1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    208, 208, 208, 208, 208, 208, 179, 179, 179, 180, 180, 180, 180, 180, 166, 166,
//  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    166, 166, 166, 151, 151, 151, 151, 151, 141, 141, 141, 141, 141, 145, 145, 145,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    145, 145, 150, 150, 150, 150, 150, 125, 125, 125, 125, 125, 115, 115, 115, 115,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0
    115, 111, 111, 111, 111, 111, 116, 116, 116, 116, 116, 116, 116, 116
} ;
//4单
const uint8_t voltage_to_alarm14_1[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 130,
// .9    1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    220, 220, 220, 220, 220, 220, 179, 179, 179, 180, 180, 180, 180, 180, 166, 166,
//  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    166, 166, 166, 151, 151, 151, 151, 151, 141, 141, 141, 141, 141, 145, 145, 145,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    145, 145, 103, 103, 103, 103, 103, 110, 110, 110, 110, 110, 107, 107, 107, 107,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0
    107, 104, 104, 104, 104, 104, 116, 116, 116, 116, 116, 116, 116, 116
} ;

const uint8_t voltage_to_alarm14_2[] =
{
// .0  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
    0, 15, 15, 15, 20, 25, 25, 25, 120,
// .9    1.0  1.1  1.2  1.3  1.4  1.5  1.6  1.7  1.8  1.9  2.0  2.1  2.2  2.3  2.4
    208, 208, 208, 208, 208, 208, 179, 179, 179, 180, 180, 180, 180, 180, 166, 166,
//  2.5  2.6  2.7  2.8  2.9  3.0  3.1  3.2  3.3  3.4  3.5  3.6  3.7  3.8  3.9  4.0
    166, 166, 166, 151, 151, 151, 151, 151, 141, 141, 141, 141, 141, 145, 145, 145,
//  4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8  4.9  5.0  5.1  5.2  5.3  5.4  5.5  5.6
    145, 145, 150, 150, 150, 150, 150, 125, 125, 125, 125, 125, 115, 115, 115, 115,
//  5.7  5.8  5.9  6.0  6.1  6.2  6.3  6.4  6.5  6.6  6.7  6.8  6.9  7.0
    115, 111, 111, 111, 111, 111, 116, 116, 116, 116, 116, 116, 116, 116
} ;

osMessageQueueId_t xPulseInQueue;
osMessageQueueId_t xPulseOutQueue;
// osEventFlagsId_t xSampleFlag;
static EFENCEDATA DefArea1, DefArea2;
static uint8_t random_data;
// static uint16_t temperature_data;
// static signed int temperature;
// uint16_t test_num,test_num1;
static uint32_t ADC1_DAT[4] = {0};
static uint32_t ADC2_DAT[4] = {0};

// PE9  defence area 1 Pulse on
// PE10 defence area 2 Pulse on
// PE11  defence area 1 Triger on
// PE12  defence area 1 Triger on
uint8_t DefArea1SelfStudyFlag = 0, DefArea2SelfStudyFlag = 0;
uint16_t DefArea1_AdcValue[4], DefArea2_AdcValue[4];
uint8_t DUAL_DEFENCE = 0, SIX_LINE_BOARD = 0;
uint16_t DefArea1_09KV_Offset = 0, DefArea2_09KV_Offset = 0, DefArea1_09KV_Linelen = 0, DefArea2_09KV_Linelen = 0;

void AlarmVoltageValueSelect(void)
{
    if(SIX_LINE_BOARD == 1)
    {
        if(DUAL_DEFENCE == 1)    //6双
        {
            memcpy(voltage_to_alarm1, voltage_to_alarm26_1, 71);
            memcpy(voltage_to_alarm2, voltage_to_alarm26_2, 71);
            DefArea1_09KV_Linelen = 340;
            DefArea2_09KV_Linelen = 340;
            DefArea1_09KV_Offset = 277;
            DefArea2_09KV_Offset = 273;
        }
        else                    //6单
        {
            memcpy(voltage_to_alarm1, voltage_to_alarm16_1, 71);
            memcpy(voltage_to_alarm2, voltage_to_alarm16_2, 71);
            DefArea1_09KV_Linelen = 340;
            DefArea2_09KV_Linelen = 340;
            DefArea1_09KV_Offset = 277;
            DefArea2_09KV_Offset = 273;

        }
    }
    else
    {
        if(DUAL_DEFENCE == 1)    //4双
        {
            memcpy(voltage_to_alarm1, voltage_to_alarm24_1, 71);
            memcpy(voltage_to_alarm2, voltage_to_alarm24_2, 71);
            DefArea1_09KV_Linelen = 340;
            DefArea2_09KV_Linelen = 340;
            DefArea1_09KV_Offset = 277;
            DefArea2_09KV_Offset = 273;
        }
        else                     //4单
        {
            memcpy(voltage_to_alarm1, voltage_to_alarm14_1, 71);
            memcpy(voltage_to_alarm2, voltage_to_alarm14_2, 71);
            DefArea1_09KV_Linelen = 340;
            DefArea2_09KV_Linelen = 340;
            DefArea1_09KV_Offset = 277;
            DefArea2_09KV_Offset = 273;
        }
    }
}

uint16_t AdcMin(uint16_t* sp)
{
    uint8_t i = 0;
    uint16_t min = sp[0];
    if(SIX_LINE_BOARD == 1) //6 line
    {
        for(i = 1; i < 4; i++)
        {
            if(min >= sp[i])
                min = sp[i];
        }
    }
    else
    {
        //4 line
        for(i = 1; i < 2; i++)
        {
            if(min >= sp[i])
                min = sp[i];
        }
    }
    return min;
}

uint8_t DEFENCE1_SHORT_OPEN(void)
{
    if(SIX_LINE_BOARD == 0) //4 line
    {
        if((DefArea1.AdcValue[0] < DefArea1.AlarmVoltage)  || (DefArea1.AdcValue[1] < DefArea1.AlarmVoltage))
            return 1;
        else
            return 0;
    }
    else                    //6 line
    {
        if((DefArea1.AdcValue[0] < DefArea1.AlarmVoltage) || (DefArea1.AdcValue[1] < DefArea1.AlarmVoltage) || (DefArea1.AdcValue[2] < DefArea1.AlarmVoltage) || (DefArea1.AdcValue[3] < DefArea1.AlarmVoltage))
            return 1;
        else
            return 0;
    }
}

uint8_t DEFENCE1_SHORT(void)
{
    if(SIX_LINE_BOARD == 0) //4 line
    {
        if((DefArea1.AdcValue[0] < DefArea1.AlarmVoltage)  && (DefArea1.AdcValue[1] <  DefArea1.AlarmVoltage))
            return 1;
        else
            return 0;
    }
    else                    //6 line
    {
        if(((DefArea1.AdcValue[0] < DefArea1.AlarmVoltage) && (DefArea1.AdcValue[1] < DefArea1.AlarmVoltage)) && ((DefArea1.AdcValue[2] <  DefArea1.AlarmVoltage) && (DefArea1.AdcValue[3] < DefArea1.AlarmVoltage)))
            return 1;
        else
            return 0;
    }
}

uint8_t DEFENCE1_TOUCH_ALARM(void)
{
    if(SIX_LINE_BOARD == 0) //4 line
    {
        if((DefArea1.AdcValue[0] < (DefArea1.AlarmVoltage << 1)) || (DefArea1.AdcValue[1] < (DefArea1.AlarmVoltage << 1)))
            return 1;
        else
            return 0;
    }
    else                    //6 line
    {
        if(((DefArea1.AdcValue[0] < (DefArea1.AlarmVoltage << 1)) && (DefArea1.AdcValue[1] < (DefArea1.AlarmVoltage << 1))) || ((DefArea1.AdcValue[2] < (DefArea1.AlarmVoltage << 1)) && (DefArea1.AdcValue[3] < (DefArea1.AlarmVoltage << 1))))
            return 1;
        else
            return 0;
    }
}

uint8_t DEFENCE2_SHORT_OPEN(void)
{
    if(SIX_LINE_BOARD == 0) //4 line
    {
        if((DefArea2.AdcValue[0] < DefArea2.AlarmVoltage)  || (DefArea2.AdcValue[1] < DefArea2.AlarmVoltage))
            return 1;
        else
            return 0;
    }
    else                    //6 line
    {
        if((DefArea2.AdcValue[0] < DefArea2.AlarmVoltage) || (DefArea2.AdcValue[1] < DefArea2.AlarmVoltage) || (DefArea2.AdcValue[2] < DefArea2.AlarmVoltage) || (DefArea2.AdcValue[3] < DefArea2.AlarmVoltage))
            return 1;
        else
            return 0;
    }
}

uint8_t DEFENCE2_SHORT(void)
{
    if(SIX_LINE_BOARD == 0) //4 line
    {
        if((DefArea2.AdcValue[0] < DefArea2.AlarmVoltage) && (DefArea2.AdcValue[1] < DefArea2.AlarmVoltage))
            return 1;
        else
            return 0;
    }
    else                    //6 line
    {
        if(((DefArea2.AdcValue[0] < DefArea2.AlarmVoltage) && (DefArea2.AdcValue[1] < DefArea2.AlarmVoltage)) && ((DefArea2.AdcValue[2] < DefArea2.AlarmVoltage) && (DefArea2.AdcValue[3] < DefArea2.AlarmVoltage)))
            return 1;
        else
            return 0;
    }
}

uint8_t DEFENCE2_TOUCH_ALARM(void)
{
    if(SIX_LINE_BOARD == 0) //4 line
    {
        if((DefArea2.AdcValue[0] < (DefArea2.AlarmVoltage << 1)) || (DefArea2.AdcValue[1] < (DefArea2.AlarmVoltage << 1)))
            return 1;
        else
            return 0;
    }
    else                    //6 line
    {
        if(((DefArea2.AdcValue[0] < (DefArea2.AlarmVoltage << 1)) && (DefArea2.AdcValue[1] < (DefArea2.AlarmVoltage << 1))) || ((DefArea2.AdcValue[2] < (DefArea2.AlarmVoltage << 1)) && (DefArea2.AdcValue[3] < (DefArea2.AlarmVoltage << 1))))
            return 1;
        else
            return 0;
    }
}

void HAL_Pulse_init(void)
{
    /*Configure GPIO pins : PE9 PE10 PE11 PE12 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_12 | PWM_T2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    MX_DMA_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();
}

// 定义脉冲任务入口函数
void xStartPulseTask(void* argument)
{
    // 定义局部变量：脉冲输入/输出消息结构体，使能检查计数器，更新标志
    PulseIn_Msg_TypeDef xPulseInData;    // 脉冲输入消息结构
    PulseOut_Msg_TypeDef xPulseOutData;  // 脉冲输出消息结构
    uint16_t uiCheckEnableCnt = 0;      // 使能检查计数器（防误触发）
    uint8_t ucUpdateFlag;               // 状态更新标志位
    // 创建消息队列（容量为2个消息）
    xPulseInQueue = osMessageQueueNew(2, sizeof(PulseIn_Msg_TypeDef), NULL);   // 脉冲输入队列
    xPulseOutQueue = osMessageQueueNew(2, sizeof(PulseOut_Msg_TypeDef), NULL); // 脉冲输出队列
    ucUpdateFlag = 0x00;  // 初始化更新标志

    // 启动ADC的DMA采集（使用两个ADC，各采集4个通道）
    HAL_ADC_Start_DMA(&hadc1, ADC1_DAT, 4);  // 启动ADC1的DMA传输
    HAL_ADC_Start_DMA(&hadc2, ADC2_DAT, 4);  // 启动ADC2的DMA传输

    // 主任务循环
    for(;;)
    {
        uiCheckEnableCnt++;  // 使能计数器递增
        // 计数器限幅（5000次后重置为4500）
        if(uiCheckEnableCnt > 5000)
            uiCheckEnableCnt = 4500;

        // 从脉冲输入队列获取消息
        if(osMessageQueueGet(xPulseInQueue, &xPulseInData, NULL, NULL) == osOK)
        {
            // 区域1使能处理（第0位）
            if(xPulseInData.ucPulseOn & 0x01)  // 检查区域1使能位
            {
                // 设置区域1参数
                DefArea1.bCtrlStatus = 0x01;  // 控制状态设为使能
                DefArea1.ucSetVoltage = xPulseInData.ucSetVoltage[0]; // 设置目标电压
                // 从映射表获取脉宽和报警电压
                DefArea1.PulseWidth = voltage_to_cnt[xPulseInData.ucSetVoltage[0]]; 
                DefArea1.AlarmVoltage = voltage_to_alarm[xPulseInData.ucSetVoltage[0]]; 
                DefArea1.ucVoltage = DefArea1.ucSetVoltage; // 当前电压=设定电压
                DefArea1.ucCurrent = voltage_to_current[DefArea1.ucSetVoltage]; // 获取对应电流
                // 首次运行时重置计数器
                if(!DefArea1.bRunningStatus)
                    uiCheckEnableCnt = 2000;
                DefArea1.bRunningStatus = 0x01;  // 运行状态置位
                ucUpdateFlag = 0x01;          // 设置更新标志
                DefArea1SelfStudyFlag = 1;     // 开启区域1自学习
            }
            else  // 区域1未使能
            {
                DefArea1.bCtrlStatus = 0x00;  // 控制状态禁用
                DefArea1.ucVoltage = 0x00;     // 电压归零
                // 获取0电压对应的电流值
                DefArea1.ucCurrent = voltage_to_current[0]; 
                ucUpdateFlag = 0x01;          // 设置更新标志
            }

            // 区域2使能处理（第1位）
            if(xPulseInData.ucPulseOn & 0x02)  // 检查区域2使能位
            {
                // 设置区域2参数（类似区域1）
                DefArea2.bCtrlStatus = 0x01;
                DefArea2.ucSetVoltage = xPulseInData.ucSetVoltage[1];
                DefArea2.PulseWidth = voltage_to_cnt[xPulseInData.ucSetVoltage[1]];
                DefArea2.AlarmVoltage = voltage_to_alarm[xPulseInData.ucSetVoltage[1]];
                DefArea2.ucVoltage = DefArea2.ucSetVoltage;
                DefArea2.ucCurrent = voltage_to_current[DefArea2.ucSetVoltage];
                ucUpdateFlag = 0x01;
                // 首次运行时重置计数器
                if(!DefArea2.bRunningStatus)
                    uiCheckEnableCnt = 2000;
                DefArea2.bRunningStatus = 0x01;
                DefArea2SelfStudyFlag = 1;     // 开启区域2自学习
                // 特殊处理：区域1脉宽使用区域2的值
                DefArea1.PulseWidth = DefArea2.PulseWidth;
            }
            else  // 区域2未使能
            {
                DefArea2.ucVoltage = 0x00;
                DefArea2.ucCurrent = voltage_to_current[0];
                DefArea2.bCtrlStatus = 0x00;
                ucUpdateFlag = 0x01;
            }

            // 设置公共参数
            DefArea1.ucAlarmCycle = xPulseInData.ucAlarmCycle;  // 报警周期
            DefArea2.ucAlarmCycle = xPulseInData.ucAlarmCycle;
            DefArea1.ucTouchEN = xPulseInData.ucTouchEN;        // 触摸使能
            DefArea2.ucTouchEN = xPulseInData.ucTouchEN;
        }

        // 任一区域运行时处理脉冲周期
        if((DefArea1.bRunningStatus) || (DefArea2.bRunningStatus))
        {
            // 脉宽计数器不同阶段处理
            if(DefArea1.uiPulseCnt == 1)  // 脉冲起始点
            {
                // 设置采样状态
                if(DefArea1.bRunningStatus)
                {
                    DefArea1.ucSampleState = 0x01;  // 区域1开始采样
                }
                if(DefArea2.bRunningStatus)
                {
                    DefArea2.ucSampleState = 0x01;  // 区域2开始采样
                }
                HAL_GPIO_WritePin(GPIOE, PWM_P1_PIN, GPIO_PIN_SET);
            }
            else if(DefArea1.uiPulseCnt == DefArea1.PulseWidth)  // 脉宽结束点
            {
                // 重置ADC计数器和采样状态
                if(DefArea1.bRunningStatus)
                {
                    DefArea1.ucSampleState = 0x02;  // 区域1结束采样
                    DefArea1.ucAdcCnt = 0x00;
                }
                if(DefArea2.bRunningStatus)
                {
                    DefArea2.ucAdcCnt = 0x00;
                    DefArea2.ucSampleState = 0x02;  // 区域2结束采样
                }
                // 切换GPIO状态
                HAL_GPIO_WritePin(GPIOE, PWM_P1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, PWM_T1_PIN, GPIO_PIN_SET);
            }
            else if(DefArea1.uiPulseCnt == DefArea1.PulseWidth + 10)  // 结束点后10个计数
            {
                // 更新采样状态
                if(DefArea1.bRunningStatus)
                {
                    DefArea1.ucSampleState = 0x04;
                }
                if(DefArea2.bRunningStatus)
                {
                    DefArea2.ucSampleState = 0x04;
                }
                // 关闭GPIO
                HAL_GPIO_WritePin(GPIOE, PWM_T1_PIN, GPIO_PIN_RESET);
            }
            // 完整周期结束（PULSE_PERIOD）且使能计数器>4500
            else if((DefArea1.uiPulseCnt == PULSE_PERIOD) && (uiCheckEnableCnt >= 4500))
            {
                // 区域1处理
                if(DefArea1.bRunningStatus)
                {
                    // 模拟电压波动（0-5随机波动）
                    random_data = rand() % 6;
                    if(random_data == 0)
                        DefArea1.ucVoltage = DefArea1.ucSetVoltage;
                    else if(random_data == 1)
                        DefArea1.ucVoltage = DefArea1.ucSetVoltage + 1;
                    else if(random_data == 2)
                        DefArea1.ucVoltage = DefArea1.ucSetVoltage + 2;
                    else if(random_data == 3)
                        DefArea1.ucVoltage = DefArea1.ucSetVoltage - 1;
                    else if(random_data == 4)
                        DefArea1.ucVoltage = DefArea1.ucSetVoltage;
                    else if(random_data == 5)
                        DefArea1.ucVoltage = DefArea1.ucSetVoltage - 2;
                    DefArea1.ucCurrent = voltage_to_current[DefArea1.ucSetVoltage];
                    // 禁用状态时重置电压/电流
                    if(DefArea1.bCtrlStatus == 0x00)
                    {
                        DefArea1.ucVoltage = 0x00;
                        DefArea1.ucCurrent = voltage_to_current[0];
                    }
                    // ADC数据后处理（右移3位相当于除以8）
                    DefArea1.AdcValue[0] = DefArea1.AdcValue[0] >> 3;
                    DefArea1.AdcValue[1] = DefArea1.AdcValue[1] >> 3;
                    DefArea1.AdcValue[2] = DefArea1.AdcValue[2] >> 3;
                    DefArea1.AdcValue[3] = DefArea1.AdcValue[3] >> 3;
                    if(DefArea1SelfStudyFlag > 0)
                    {
                        DefArea1SelfStudyFlag++;
                    }
                    if((DefArea1SelfStudyFlag == 2) || (DefArea1SelfStudyFlag == 3) || (DefArea1SelfStudyFlag == 4))
                    {
                        if(DefArea1SelfStudyFlag == 4)
                            DefArea1SelfStudyFlag = 0;  // 完成自学习
                        // 计算报警阈值（根据最小ADC值和电压）
                        DefArea1.AlarmVoltage = AdcMin(DefArea1.AdcValue) - voltage_to_alarm1[DefArea1.ucSetVoltage];
                        // 低电压特殊处理
                        if((DefArea1.ucSetVoltage <= 14) && (AdcMin(DefArea1.AdcValue) > DefArea1_09KV_Linelen))
                        {
                            DefArea1.AlarmVoltage = AdcMin(DefArea1.AdcValue) - DefArea1_09KV_Offset;
                        }
                        // 保存自学习数据
                        DefArea1_AdcValue[0] = DefArea1.AdcValue[0];
                        DefArea1_AdcValue[1] = DefArea1.AdcValue[1];
                        DefArea1_AdcValue[2] = DefArea1.AdcValue[2];
                        DefArea1_AdcValue[3] = DefArea1.AdcValue[3];
#if Debug_flag
                        printf("DefArea1SelfStudyFlag is %d\n", DefArea1SelfStudyFlag);
                        #endif
                    }
#if (SCAN_EN == 1)
                    if(ucPulseNum1 == 2)
                        printf("Ch1,%d,%d,%d,%d,%d ", DefArea1.ucSetVoltage, DefArea1.AdcValue[0], DefArea1.AdcValue[1], DefArea1.AdcValue[2], DefArea1.AdcValue[3]);
#endif
#if Debug_flag 
                    //                printf("defence area 1 voltage value max is %d\n", DefArea1.uiVoltage);
                    printf(": %d %d %d %d\n", DefArea1_AdcValue[0], DefArea1_AdcValue[1], DefArea1_AdcValue[2], DefArea1_AdcValue[3]);
                    printf("         R2 R1 T2 T1: %d %d %d %d\n", DefArea1.AdcValue[0], DefArea1.AdcValue[1], DefArea1.AdcValue[2], DefArea1.AdcValue[3]);
                    printf("//DefArea1.AlarmVoltage = %d\n", DefArea1.AlarmVoltage);
                    #endif
                    // 报警检测（当使能计数器>=4500）
                    if(uiCheckEnableCnt >= 4500)
                    {
                        // 短路/断路检测
                        if(DEFENCE1_SHORT_OPEN())
                        {
                            if(DEFENCE1_SHORT())  // 短路报警
                            {
                                DefArea1.ucShortAlarmCnt++;  // 短路计数增加
                                #if Debug_flag
                                printf("defence area 1 alarm time %d", DefArea1.ucShortAlarmCnt);
                                #endif
                                if(DefArea1.ucShortAlarmCnt >= DefArea1.ucAlarmCycle)  // 达到报警周期
                                {
                                    DefArea1.ucAlarmStatus = 0x01;  // 报警状态
                                    DefArea1.ucAlarmType = 0x02;    // 短路类型
                                    DefArea1.ucShortAlarmCnt = DefArea1.ucAlarmCycle; // 饱和计数
                                    ucUpdateFlag |= 0x01;           // 更新标志
                                    // 设置报警电流（82表示特殊报警状态）
                                    DefArea1.ucCurrent = 82;
                                    DefArea1.ucVoltage = 0x00;
                                }
                            }
                            else  // 断路报警
                            {
                                DefArea1.ucOpenAlarmCnt++;
                                if(DefArea1.ucOpenAlarmCnt >= DefArea1.ucAlarmCycle)
                                {
                                    DefArea1.ucAlarmStatus = 0x01;  // 报警状态
                                    DefArea1.ucAlarmType = 0x01;    // 断路类型
                                    DefArea1.ucOpenAlarmCnt = DefArea1.ucAlarmCycle;
                                    // 设置0电压状态电流
                                    DefArea1.ucCurrent = voltage_to_current[0];
                                    DefArea1.ucVoltage = 0x00;
                                    ucUpdateFlag |= 0x02;
                                }
                            }
                        }
                        // 触摸报警检测（需使能）
                        else if((DefArea1.ucTouchEN == 0xAA) && (DEFENCE1_TOUCH_ALARM()))
                        {
                            // 触摸报警处理（代码被注释）
                        }
                        else  // 无报警状态
                        {
                            if(DefArea1.ucAlarmStatus == 0x01)
                                DefArea1.ucAlarmStatus = DefArea1.ucAlarmStatus | 0x80;
                            if(DefArea1.ucAlarmStatus & 0x80)
                            {
                                DefArea1.ucAlarmReleaseCnt++;
                                if(DefArea1.ucAlarmReleaseCnt >= 2)  // 连续2周期无报警
                                {
                                    // 清除报警状态
                                    DefArea1.ucAlarmReleaseCnt = 0;
                                    DefArea1.ucAlarmStatus = 0;
                                    DefArea1.ucAlarmType = 0;
                                    ucUpdateFlag |= 0x04;
                                }
                            }
                            else
                            {
                                ucUpdateFlag |= 0x08;  // 正常状态更新
                            }
                            // 重置报警计数器
                            DefArea1.ucShortAlarmCnt = 0;
                            DefArea1.ucOpenAlarmCnt = 0;
                        }
                    }
                    DefArea1.AdcValue[0] = DefArea1.AdcValue[1] = DefArea1.AdcValue[2] = DefArea1.AdcValue[3] = 0;
                    DefArea1.uiVoltage = 0;
                }
                // 区域2处理（逻辑与区域1类似）
								if(DefArea2.bRunningStatus)  // 检查区域2是否正在运行
								{
										// 生成一个0-5的随机数，用于模拟电压波动
										random_data = rand() % 6;
										// 根据随机数调整显示的电压值，模拟实际波动（用于测试或显示）
										if(random_data == 0)
												DefArea2.ucVoltage = DefArea2.ucSetVoltage;       // 显示电压等于设定电压
										else if(random_data == 1)
												DefArea2.ucVoltage = DefArea2.ucSetVoltage + 1;   // 显示电压比设定高1档
										else if(random_data == 2)
												DefArea2.ucVoltage = DefArea2.ucSetVoltage + 2;   // 显示电压比设定高2档
										else if(random_data == 3)
												DefArea2.ucVoltage = DefArea2.ucSetVoltage - 1;   // 显示电压比设定低1档
										else if(random_data == 4)
												DefArea2.ucVoltage = DefArea2.ucSetVoltage;        // 显示电压等于设定电压
										else if(random_data == 5)
												DefArea2.ucVoltage = DefArea2.ucSetVoltage - 2;   // 显示电压比设定低2档

										// 根据设定电压从预设数组中获取对应的电流值（该电流值可能是预设的固定映射）
										DefArea2.ucCurrent = voltage_to_current[DefArea2.ucSetVoltage];
										
										// 如果区域2的控制状态为禁用（0x00），则将显示的电压和电流清零（但实际输出可能已停止）
										if(DefArea2.bCtrlStatus == 0x00)
										{
												DefArea2.ucVoltage = 0x00;      // 显示电压为0
												// 电流设置为0电压对应的电流值（可能是待机电流或最小值）
												DefArea2.ucCurrent = voltage_to_current[0];
										}
										
										// 对ADC采集的4个通道原始数据进行后处理（右移3位，相当于除以8，可能是滤波或单位转换）
										DefArea2.AdcValue[0] = DefArea2.AdcValue[0] >> 3;
										DefArea2.AdcValue[1] = DefArea2.AdcValue[1] >> 3;
										DefArea2.AdcValue[2] = DefArea2.AdcValue[2] >> 3;
										DefArea2.AdcValue[3] = DefArea2.AdcValue[3] >> 3;
										
										// 自学习标志处理：如果自学习标志大于0，则增加自学习阶段
										if(DefArea2SelfStudyFlag > 0)
										{
												DefArea2SelfStudyFlag ++;  // 自学习阶段递增
										}
										
										// 自学习阶段2、3、4的处理逻辑（用于动态调整报警阈值）
										if((DefArea2SelfStudyFlag == 2) || (DefArea2SelfStudyFlag == 3) || (DefArea2SelfStudyFlag == 4))
										{
												// 当自学习阶段为4时，重置为0（完成自学习）
												if(DefArea2SelfStudyFlag == 4)
														DefArea2SelfStudyFlag = 0;
												
												// 计算报警电压阈值：取4个通道ADC最小值，减去该设定电压对应的预设报警偏移量
												DefArea2.AlarmVoltage = AdcMin(DefArea2.AdcValue) - voltage_to_alarm2[DefArea2.ucSetVoltage];
												
												// 针对低电压（设定电压<=14）的特殊处理：如果最小值大于09KV线性段阈值
												if((DefArea2.ucSetVoltage <= 14) && (AdcMin(DefArea2.AdcValue) > DefArea2_09KV_Linelen))
												{
														// 则报警阈值使用最小值减去09KV的预设偏移量（可能是一种校准）
														DefArea2.AlarmVoltage = AdcMin(DefArea2.AdcValue) - DefArea2_09KV_Offset;
												}
												
												// 保存当前自学习阶段的ADC值（用于调试或记录）
												DefArea2_AdcValue[0] = DefArea2.AdcValue[0];
												DefArea2_AdcValue[1] = DefArea2.AdcValue[1];
												DefArea2_AdcValue[2] = DefArea2.AdcValue[2];
												DefArea2_AdcValue[3] = DefArea2.AdcValue[3];
												
												// 调试输出：打印当前自学习阶段
								#if Debug_flag
												printf("DefArea2SelfStudyFlag is %d\n", DefArea2SelfStudyFlag);
								#endif
										}
										
										// 如果启用了扫描模式（SCAN_EN=1），并且在第2个脉冲（ucPulseNum2==2）时，打印调试信息
								#if (SCAN_EN == 1)
										if(ucPulseNum2 == 2)
										{
												// 输出通道2的信息：设定电压和4个通道的ADC值
												printf("Ch2,%d,%d,%d,%d,%d,", DefArea2.ucSetVoltage, DefArea2.AdcValue[0], DefArea2.AdcValue[1], DefArea2.AdcValue[2], DefArea2.AdcValue[3]);
												
												// 温度数据处理：从全局变量temperature_data获取原始温度ADC值
												temperature = temperature_data;
												// 将ADC值转换为电压值（假设参考电压3300mV，12位ADC）
												temperature = temperature * 3300 / 4096;
												// 根据电压计算温度（假设760对应25°C，每25mV变化10°C）
												if(temperature > 760)
														temperature = (temperature - 760) * 10 / 25 + 25; // 高于760mV，每增加25mV增加10°C
												else
														temperature = 25 - (760 - temperature) * 10 / 25; // 低于760mV，每减少25mV减少10°C
												
												// 打印当前温度
												printf("temperature,%d ", temperature);
										}
								#endif

										// 调试输出：打印自学习保存的ADC值、当前ADC值和报警阈值
								#if Debug_flag
										printf("           : %d %d %d %d\n", DefArea2_AdcValue[0], DefArea2_AdcValue[1], DefArea2_AdcValue[2], DefArea2_AdcValue[3]);
										printf("R4 R3 T4 T3: %d %d %d %d\n", DefArea2.AdcValue[0], DefArea2.AdcValue[1], DefArea2.AdcValue[2], DefArea2.AdcValue[3]);
										printf("//DefArea2.AlarmVoltage = %d\n", DefArea2.AlarmVoltage);
								#endif

										// 当使能计数器达到4500（防误触发稳定期）时，进行报警检测
										if(uiCheckEnableCnt >= 4500)
										{
												// 短路/断路检测：调用宏判断是否出现短路或断路
												if(DEFENCE2_SHORT_OPEN())
												{
														// 短路检测：调用宏判断是否为短路
														if(DEFENCE2_SHORT())
														{
																// 短路计数递增
																DefArea2.ucShortAlarmCnt++;
																
																// 调试输出：打印短路报警次数
																#if Debug_flag 
																printf("defence area 2 alarm time %d", DefArea2.ucShortAlarmCnt);
																#endif
																
																// 如果短路报警次数达到设定的报警周期数
																if(DefArea2.ucShortAlarmCnt >= DefArea2.ucAlarmCycle)
																{
																		// 设置报警状态：报警标志置为0x01（报警），类型为0x02（短路）
																		DefArea2.ucAlarmStatus = 0x01;
																		DefArea2.ucAlarmType = 0x02;
																		// 报警计数设置为最大值（避免重复触发）
																		DefArea2.ucShortAlarmCnt = DefArea2.ucAlarmCycle;
																		
																		// 设置报警状态下的显示值：电流为82（特殊代码），电压为0
																		DefArea2.ucCurrent = 82;
																		DefArea2.ucVoltage = 0x00;
																		
																		// 设置更新标志位（0x10表示区域2有更新需要发送）
																		ucUpdateFlag |= 0x10;
																}
														}
														else // 如果不是短路，则判断为断路
														{
																// 断路报警计数递增
																DefArea2.ucOpenAlarmCnt++;
																// 如果断路报警次数达到设定周期
																if(DefArea2.ucOpenAlarmCnt >= DefArea2.ucAlarmCycle)
																{
																		// 设置报警状态：报警标志置为0x01，类型为0x01（断路）
																		DefArea2.ucAlarmStatus = 0x01;
																		DefArea2.ucAlarmType = 0x01;
																		// 报警计数设置为最大值
																		DefArea2.ucOpenAlarmCnt = DefArea2.ucAlarmCycle;
																		
																		// 显示电流设为0电压对应的电流，电压显示为0
																		DefArea2.ucCurrent = voltage_to_current[0];
																		DefArea2.ucVoltage = 0x00;
																		
																		// 设置更新标志位（0x20表示区域2断路报警更新）
																		ucUpdateFlag |= 0x20;
																}
														}
														// 注：此处有注释掉的代码，是短路/断路未发生时的处理（重置计数和更新标志）
												}
												// 触摸报警检测：如果触摸使能（0xAA），并且调用宏判断是否触摸报警
												else if((DefArea2.ucTouchEN == 0xAA) && (DEFENCE2_TOUCH_ALARM()))
												{
														// 此处是触摸报警处理代码，被注释掉了，原因可能是功能未启用或测试阶段
														// 逻辑与短路/断路类似：计数、达到周期后设置报警状态，并更新显示
														// 更新标志会设置为0x30（0x10|0x20）
												}
												else  // 没有短路、断路、触摸报警（正常状态）
												{
														// 如果当前报警状态为0x01（报警中），则标记为需要恢复（添加0x80标记）
														if(DefArea2.ucAlarmStatus == 0x01)
																DefArea2.ucAlarmStatus = DefArea2.ucAlarmStatus | 0x80;
														
														// 如果当前状态为报警恢复中（有0x80标记）
														if(DefArea2.ucAlarmStatus & 0x80)
														{
																// 恢复计数器递增
																DefArea2.ucAlarmReleaseCnt++;
																// 如果连续2个周期都没有报警（说明恢复稳定）
																if(DefArea2.ucAlarmReleaseCnt >= 2)
																{
																		// 清除恢复计数，重置报警状态和类型
																		DefArea2.ucAlarmReleaseCnt = 0;
																		DefArea2.ucAlarmStatus = 0;
																		DefArea2.ucAlarmType = 0;
																		// 设置更新标志0x40（区域2报警解除）
																		ucUpdateFlag |= 0x40;
																}
														}
														else  // 正常无报警状态
														{
																// 设置更新标志0x80（区域2状态正常）
																ucUpdateFlag |= 0x80;
														}
														
														// 重置短路和断路计数
														DefArea2.ucShortAlarmCnt = 0;
														DefArea2.ucOpenAlarmCnt = 0;
												}
										}
										
										// 清除本次周期采集的ADC数据（准备下一个周期）
										DefArea2.AdcValue[0] = DefArea2.AdcValue[1] = DefArea2.AdcValue[2] = DefArea2.AdcValue[3] = 0;
										// 清除电压累积值
										DefArea2.uiVoltage = 0;
								}
							}
            // 脉宽计数器递增
            DefArea1.uiPulseCnt++;
            // 周期复位处理（达到PULSE_PERIOD）
            if(DefArea1.uiPulseCnt > PULSE_PERIOD)
            {
                DefArea1.uiPulseCnt = 1;  // 重置计数器
                // 区域1周期结束处理
                if(DefArea1.bRunningStatus)
                {
                    if(DefArea1.bCtrlStatus == 0x00)  // 控制禁用时
                    {
                        // 清除运行状态和报警
                        DefArea1.bRunningStatus = 0x00;
                        DefArea1.ucAlarmStatus = 0x00;
                        DefArea1.ucAlarmType = 0x00;
                    }
                    #if (SCAN_EN == 1)  // 扫描模式使能
                    ucPulseNum1++;
                    if(ucPulseNum1 > 2)
                    {
                        ucPulseNum1 = 0x00;
                        // 自动循环扫描不同电压
                        DefArea1.ucSetVoltage = ucScanVol1;
                        DefArea1.PulseWidth = voltage_to_cnt[ucScanVol1];
                        DefArea1.AlarmVoltage = voltage_to_alarm[ucScanVol1];
                        ucScanVol1++;
                        if(ucScanVol1 > 66)  // 电压上限
                            ucScanVol1 = 9;  // 复位扫描电压
                    }
                    #endif
                }
                // 区域2周期结束处理（逻辑同区域1）
                if(DefArea2.bRunningStatus)
                {
                    if(DefArea2.bCtrlStatus == 0x00)
                    {
                        DefArea2.bRunningStatus = 0x00;
                        DefArea2.ucAlarmStatus = 0x00;
                        DefArea2.ucAlarmType = 0x00;
                    }
#if (SCAN_EN == 1)
                    ucPulseNum2++;
                    if(ucPulseNum2 > 2)
                    {
                        ucPulseNum2 = 0x00;
                        DefArea2.ucSetVoltage = ucScanVol2;
                        DefArea2.PulseWidth = voltage_to_cnt[ucScanVol2];
                        DefArea2.AlarmVoltage = voltage_to_alarm[ucScanVol2];

                        ucScanVol2++;
                        if(ucScanVol2 > 66)
                            ucScanVol2 = 9;
                    }
#endif
                }
            }
        }
        else  // 无区域运行
        {
            DefArea1.uiPulseCnt = 0x0001;  // 重置脉宽计数器
            // 关闭所有GPIO输出
            HAL_GPIO_WritePin(GPIOE, PWM_P1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOE, PWM_T1_PIN, GPIO_PIN_RESET);
            // 清除电压电流显示
            if(DefArea1.bRunningStatus)
            {
                DefArea1.ucVoltage = 0x00;
                DefArea1.ucCurrent = 0x00;
            }
            if(DefArea2.bRunningStatus)
            {
                DefArea2.ucVoltage = 0x00;
                DefArea2.ucCurrent = 0x00;
            }
        }

        // 状态更新处理
        if(ucUpdateFlag)
        {
            // 双防区模式
            if(DUAL_DEFENCE == 1)
            {
                // 组合运行状态（bit0:区域1，bit1:区域2）
                xPulseOutData.ucRunningStatus = (DefArea2.bRunningStatus << 1) | DefArea1.bRunningStatus;
                // 组合报警状态
                xPulseOutData.ucAlarmStatus = (DefArea2.ucAlarmStatus << 1) | DefArea1.ucAlarmStatus;
                // 组合报警类型（低4位:区域1，高4位:区域2）
                xPulseOutData.ucAlarmType = (DefArea2.ucAlarmType << 4) | (DefArea1.ucAlarmType & 0x0f);
                // 设置电压电流值
                xPulseOutData.ucVoltage[0] = DefArea1.ucVoltage;
                xPulseOutData.ucVoltage[1] = DefArea2.ucVoltage;
                xPulseOutData.ucCurrent[0] = DefArea1.ucCurrent;
                xPulseOutData.ucCurrent[1] = DefArea2.ucCurrent;
            }
            else  // 单防区模式
            {
                xPulseOutData.ucRunningStatus = DefArea1.bRunningStatus;
                xPulseOutData.ucAlarmStatus = DefArea1.ucAlarmStatus;
                xPulseOutData.ucAlarmType = (DefArea1.ucAlarmType & 0x0f);
                xPulseOutData.ucVoltage[0] = DefArea1.ucVoltage;
                xPulseOutData.ucCurrent[0] = DefArea1.ucCurrent;
            }
            // 发送更新消息
            osMessageQueuePut(xPulseOutQueue, &xPulseOutData, 0, NULL);
            ucUpdateFlag = 0x00;  // 清除更新标志
        }
        // 任务延时1ms（保持1kHz运行频率）
        osDelay(1);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        if(DefArea1.ucSampleState == 0x02)
        {
            //		test_num++;
            for(int i = 0; i < 4; i++)
            {
                if(ADC1_DAT[i] > DefArea1.AdcValue[i])
                    DefArea1.AdcValue[i] = ADC1_DAT[i];
            }
            //        DefArea1.ucAdcCnt++;
            //        if (DefArea1.ucAdcCnt>2)
            //        {
            //            DefArea1.ucAdcCnt =0x00;
            //        }
            //        HAL_ADC_Start_DMA(&hadc2,ADC2_DAT,4);
        }
    }
    else if(hadc->Instance == ADC2)
    {
        if(DefArea2.ucSampleState == 0x02)
        {

            for(int i = 0; i < 4; i++)
            {
                if(ADC2_DAT[i] > DefArea2.AdcValue[i])
                    DefArea2.AdcValue[i] = ADC2_DAT[i];
            }
            //       HAL_ADC_Start_DMA(&hadc1,ADC1_DAT,4);
            //        DefArea2.ucAdcCnt++;
            //        if (DefArea2.ucAdcCnt>2)
            //        {
            //            DefArea2.ucAdcCnt =0x00;
            //        }
        }
    }
}

void MX_Pulse_init(void)
{
    HAL_Pulse_init();
    pulseTaskHandle = osThreadNew(xStartPulseTask, NULL, &pulseTask_attributes);
}
