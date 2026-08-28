#include "main.h"

#include "keyboard.h"
#include "pcf8562.h"

uint8_t KeySetIpFlag = 0;
osMessageQueueId_t xKeyChangeMsg;

static uint8_t jitter_cnt[5];

void HAL_Key_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void HAL_DigitalIn_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9; // vdc power and back power
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13; // Tamper
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1; // IR EN and IR GATE
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

static uint8_t get_ir_pwr(void)
{
  uint8_t tmp = 0;
  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)) // vbak
    tmp |= 0x01;
  if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9)) // vdc
    tmp |= 0x02;
//  if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) // tamper
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) // tamper
    tmp |= 0x04;
  if (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1)) // IR_EN
    tmp |= 0x08;
  if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0)) // IR_IN
    tmp |= 0x10;
  return tmp;
}

uint8_t alarm_io_scan(void)
{
  static uint8_t ir_pwr_pre;
  uint8_t ir_pwr_return = 0x00;
  uint8_t ir_chg, ir_pwr;
  ir_pwr = get_ir_pwr();

  ir_chg = ir_pwr ^ ir_pwr_pre;
  for (int8_t i = 0; i < 5; i++)
  {
    if (ir_chg & (1 << i))
    {
      jitter_cnt[i]++;
      if (jitter_cnt[i] >= 10)
      {
        ir_pwr_return |= (1 << i);
        jitter_cnt[i] = 10;
      }
    }
    else
    {
      jitter_cnt[i] = 0;
    }
  }
  ir_pwr_return = ir_pwr_pre ^ ir_pwr_return;
  ir_pwr_pre = ir_pwr_return;
  return ir_pwr_return;
}

/***************************************************************
KEY1 P1.04
KEY2 P1.08
KEY3 P1.09
KEY4 P1.10
****************************************************************/
static uint8_t Get_Key(void)
{

  if ((HAL_GPIO_ReadPin(KEY_PORT, KEY_MODE_PIN) == 0) && (HAL_GPIO_ReadPin(KEY_PORT, KEY_UP_PIN) == 0))
    return 0x05;
  if ((HAL_GPIO_ReadPin(KEY_PORT, KEY_DOWN_PIN) == 0) && (HAL_GPIO_ReadPin(KEY_PORT, KEY_MODE_PIN) == 0))
    return 0x06;
  if (HAL_GPIO_ReadPin(KEY_PORT, KEY_MODE_PIN) == 0)
    return 0x01;
  if (HAL_GPIO_ReadPin(KEY_PORT, KEY_DOWN_PIN) == 0)
    return 0x02;
  if (HAL_GPIO_ReadPin(KEY_PORT, KEY_UP_PIN) == 0)
    return 0x03;
  if (HAL_GPIO_ReadPin(KEY_PORT, KEY_ENT_PIN) == 0)
    return 0x04;
  return NO_KEY;
}
/***************************************************************
 *
 ****************************************************************/
uint8_t key_scan(void)
{
  static uint8_t Key_State = 0;
  static uint8_t Key_Prev = 0;
  static uint8_t Key_Delay = 0;
  static uint8_t Key_Series = 0;

  uint8_t Key_Press = NO_KEY;
  uint8_t Key_Return = NO_KEY;

  Key_Press = Get_Key();
  switch (Key_State)
  {
  case 0:
    if (Key_Press != NO_KEY)
    {
      Key_State = 1;
      Key_Prev = Key_Press;
    }
    break;
  case 1:
    if (Key_Press == Key_Prev)
    {
      Key_State = 2;
      Key_Return = KEY_DOWN | Key_Prev;
    }
    else
    {
      Key_State = 0;
    }
    break;
  case 2:
    if (Key_Press == NO_KEY)
    {
      Key_State = 0;
      Key_Delay = 0;
      Key_Series = 0;
      Key_Return = KEY_UP | Key_Prev;
      break;
    }
    if (Key_Press == Key_Prev)
    {
      Key_Delay++;
      if ((Key_Series == 1) && (Key_Delay > KEY_SERIES_DELAY))
      {
        Key_Delay = 0;
        Key_Return = KEY_LIAN | Key_Press;
        Key_Prev = Key_Press;
        break;
      }
      if (Key_Delay > KEY_SERIES_FLAG)
      {
        Key_Series = 1;
        Key_Delay = 0;
        Key_Return = KEY_LONG | Key_Prev;
        break;
      }
    }
  default:
    break;
  }
  if (Key_Return != NO_KEY)
  {
    device.ucLcdOnTime = 120;
    lcd_backup_on();
  }
  return Key_Return;
}

void vDinputTask(void *argument)
{
  DINPUT SW_KEY = {0};
  uint8_t ir_pwr_status;
  //  uint8_t ir_change;
  uint8_t alarm_flag;

  //  ir_pwr_status = 0x00;   //default value is 1
  //  sw_alarm_flag = 0x00;
  alarm_flag = 0x00;
  xKeyChangeMsg = osMessageQueueNew(5, sizeof(DINPUT), NULL);
  if (xKeyChangeMsg != 0)
  {
    HAL_Key_MspInit();
    HAL_DigitalIn_MspInit();
  }
  else
  {
    // creat queue error;
  }
  for (;;)
  {
    ir_pwr_status = alarm_io_scan();
    //    ir_change = ir_pwr_status^ir_pwr_pre;
    //    ir_pwr_pre = ir_pwr_status;
    // // if(ir_change)
    // // {
    //   if(ir_change&0x03)     // power status
    //   {
    //     SW_KEY.ucPowerStatus = ir_pwr_status&0x03;
    //     sw_alarm_flag = sw_alarm_flag + 1;
    //   }
    //   if (ir_change&0x04)   //Tamper bit change
    //   {
    //     if((ir_pwr_status&0x04) ==0x00)
    //     {
    //       alarm_flag = alarm_flag | 0x01;
    //       sw_alarm_flag = sw_alarm_flag + 1;
    //     }
    //     else
    //     {
    //       //tamper alarm is removed
    //     }
    //   }
    //   else
    //   {
    //     alarm_flag = alarm_flag & 0xfe;
    //     /* code */
    //   }

    //   if (ir_change&0x08)   //IR bit change
    //   {
    //     if((ir_pwr_pre&0x04) == 0)
    //     {
    //       alarm_flag = alarm_flag | 0x02;
    //       sw_alarm_flag = sw_alarm_flag + 1;
    //     }
    //     else
    //     {
    //       alarm_flag = alarm_flag & 0xfd;
    //       //tamper alarm is removed
    //     }
    //   }
    // }
    SW_KEY.ucPowerStatus = ir_pwr_status & 0x03;
    alarm_flag = 0x00;
    if (ir_pwr_status & 0x04)
      alarm_flag |= 0x01;
    if ((ir_pwr_status & 0x08) || (ir_pwr_status & 0x10))
      alarm_flag |= 0x02;
    SW_KEY.ucSwitchAlarm = alarm_flag;
    SW_KEY.keyvalue = key_scan();
    osMessageQueuePut(xKeyChangeMsg, &SW_KEY, 0, NULL);
    osDelay(20);
  }
}
