#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

// user include files
#include "stdint.h"
#include "cmsis_os.h"
#include "var.h"

#define KEY_SERIES_FLAG 150
#define KEY_SERIES_DELAY 3

#define KEY_DOWN 0xA0
#define KEY_LONG 0xB0
#define KEY_LIAN 0xC0
#define KEY_UP 0xD0

#define NO_KEY 0x00

#define KEYMODE_SHORT 0xa1
#define KEYUP_SHORT 0xa3
#define KEYDOWN_SHORT 0xa2
#define KEYENT_SHORT 0xa4

#define KEYMODE_LIAN 0xc1
#define KEYUP_LIAN 0xc3
#define KEYDOWN_LIAN 0xc2
#define KEYENT_LIAN 0xc4
#define KEYMODE_LONG 0xb1
#define KEYUP_LONG 0xb2
#define KEYDOWN_LONG 0xb3

#define KEY_PORT GPIOC
#define KEY_MODE_PIN GPIO_PIN_0 // ent
#define KEY_ENT_PIN GPIO_PIN_5	// up
#define KEY_UP_PIN GPIO_PIN_4	// down
#define KEY_DOWN_PIN GPIO_PIN_1 // mode
extern DEVICEDATA device;
extern uint8_t KeySetIpFlag;
typedef struct
{
	uint8_t ucPowerStatus;
	uint8_t ucSwitchAlarm;
	uint8_t keyvalue;
} DINPUT;

extern osMessageQueueId_t xKeyChangeMsg;
void vDinputTask(void *argument);

#endif
