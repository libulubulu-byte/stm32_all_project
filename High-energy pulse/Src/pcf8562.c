/**************************************************************************************
 * FileName       : PCF8562.c
 * Version        : V1.0
 * Programmer(s)  :
 * Parameters     : ARM STM32F407
 * DebugTools     : JLINK V8.0 And Realview MDK V5
 * Description    :
 **************************************************************************************/
#include "main.h"
#include "pcf8562.h"

DTA_BUF DataBuf;
//#define LCD output
uint8_t const Lcd_Seg[10] = {0xfd, 0x0d, 0xdb, 0x9f, 0x2f, 0xb7, 0xf7, 0x8d, 0xff, 0xbf};
uint8_t const Lcd_Seg1[10] = {0xfb, 0x0b, 0xbd, 0x9f, 0x4f, 0xde, 0xfe, 0x1b, 0xff, 0xdf};
uint8_t const Lcd_Seg2[10] = {0xf3, 0x03, 0xb5, 0x97, 0x47, 0xd6, 0xf6, 0x13, 0xf7, 0xd7};
uint8_t g_dis_abuf[11];
uint8_t g_dis_bbuf[11];

uint8_t g_disa_dat[11];
uint8_t g_disb_dat[11];
uint8_t lcd_flag = 0;

uint8_t ack;
uint8_t HT16C22_ack;

static void I2C_SomeNOP(uint16_t t)
{
  while (t--)
  {
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
  }
}

void Start_I2c()
{
  PCF8562_SDA_HIGH;
  I2C_SomeNOP(5);
  PCF8562_SCL_HIGH;
  I2C_SomeNOP(10);
  PCF8562_SDA_LOW;
  I2C_SomeNOP(10);
  PCF8562_SCL_LOW;
  I2C_SomeNOP(5);
}

void Stop_I2c()
{
  PCF8562_SDA_LOW;
  I2C_SomeNOP(5);
  PCF8562_SCL_HIGH;
  I2C_SomeNOP(10);
  PCF8562_SDA_HIGH;
  I2C_SomeNOP(10);
}

void SendByte(uint8_t c)
{
  uint8_t BitCnt;
  for (BitCnt = 0; BitCnt < 8; BitCnt++)
  {
    if ((c << BitCnt) & 0x80)
      PCF8562_SDA_HIGH;
    else
      PCF8562_SDA_LOW;
    I2C_SomeNOP(5);

    PCF8562_SCL_HIGH;
    I2C_SomeNOP(10);
    PCF8562_SCL_LOW;
  }

  I2C_SomeNOP(5);
  PCF8562_SDA_HIGH;
  PCF8562_SDA_IN();
  //	FIO1DIR &= ~PCF85162_SDA;
  I2C_SomeNOP(5);
  PCF8562_SCL_HIGH;
  I2C_SomeNOP(10);
  if (PCF8562_SDA_READ)
    ack = 0;
  else
    ack = 1;
  PCF8562_SCL_LOW;
  I2C_SomeNOP(5);
  PCF8562_SDA_OUT();
  //	FIO1DIR |= PCF85162_SDA;
}

uint8_t RcvByte()
{
  uint8_t retc;
  uint8_t BitCnt;

  retc = 0;
  //	FIO1DIR &= ~PCF85162_SDA;
  PCF8562_SDA_IN();
  for (BitCnt = 0; BitCnt < 8; BitCnt++)
  {
    I2C_SomeNOP(5);
    PCF8562_SCL_LOW;
    I2C_SomeNOP(10);
    PCF8562_SCL_HIGH;
    I2C_SomeNOP(5);
    retc = retc << 1;
    if (PCF8562_SDA_READ)
      retc = retc + 1;
    I2C_SomeNOP(5);
  }
  PCF8562_SCL_LOW;
  I2C_SomeNOP(5);
  PCF8562_SDA_OUT();
  //	FIO1DIR |= PCF85162_SDA ;
  return (retc);
}

void Ack_I2c(uint8_t a)
{

  if (a == 0)
    PCF8562_SDA_LOW;
  else
    PCF8562_SDA_HIGH;
  I2C_SomeNOP(7);
  PCF8562_SCL_HIGH;
  I2C_SomeNOP(10);
  PCF8562_SCL_LOW;
  I2C_SomeNOP(5);
}

uint8_t ISendByte(uint8_t sla, uint8_t c)
{
  Start_I2c();
  SendByte(sla);
  if (ack == 0)
    return (0);
  SendByte(c);
  if (ack == 0)
    return (0);
  Stop_I2c();
  return (1);
}

uint8_t ISendStr(uint8_t sla, uint8_t suba, uint8_t *s, uint8_t no)
{
  uint8_t i;

  Start_I2c();
  SendByte(sla);
  if (ack == 0)
    return (0);
  SendByte(suba);
  if (ack == 0)
    return (0);

  for (i = 0; i < no; i++)
  {
    SendByte(*s);
    if (ack == 0)
      return (0);
    s++;
  }
  Stop_I2c();
  return (1);
}

uint8_t IRcvByte(uint8_t sla, uint8_t *c)
{
  Start_I2c();
  SendByte(sla + 1);
  if (ack == 0)
    return (0);
  *c = RcvByte();
  Ack_I2c(1);
  Stop_I2c();
  return (1);
}

uint8_t IRcvStr(uint8_t sla, uint8_t suba, uint8_t *s, uint8_t no)
{
  uint8_t i;

  Start_I2c();
  SendByte(sla);
  if (ack == 0)
    return (0);
  SendByte(suba);
  if (ack == 0)
    return (0);

  Start_I2c();
  SendByte(sla + 1);
  if (ack == 0)
    return (0);

  for (i = 0; i < no - 1; i++)
  {
    *s = RcvByte();
    Ack_I2c(0);
    s++;
  }
  *s = RcvByte();
  Ack_I2c(1);
  Stop_I2c();
  return (1);
}

uint8_t PCF8562_Init(uint8_t addr, uint8_t mode, uint8_t glitter)
{
  mode = (mode & 0x1f) | 0x40;
  if (ISendByte(addr, mode) == 0)
  {
    return (0);
  }
  glitter = (glitter & 0x07) | 0x07;
  ISendByte(addr, glitter);
  return (1);
}

void PCF8562_Display(uint8_t addr, uint8_t drive, uint8_t adr, uint8_t *buf, uint8_t no)
{
  drive = (drive & 0x07) | 0x60;
  ISendByte(addr, drive);

  adr = adr & 0x1f;
  ISendStr(addr, adr, buf, no);
}

// 2022-07-18 BT TERRY
void HT16C22_IIC_Start(void)
{
  HT16C22_SDA_H();
  HT16C22_SCL_H();
  I2C_SomeNOP(10);

  HT16C22_SDA_L();
  I2C_SomeNOP(10);
  HT16C22_SCL_L();
  I2C_SomeNOP(5);
}
void HT16C22_IIC_Stop(void)
{
  HT16C22_SCL_L(); // 1
  I2C_SomeNOP(5);
  HT16C22_SDA_L();
  I2C_SomeNOP(5);
  HT16C22_SCL_H();
  I2C_SomeNOP(10);
  HT16C22_SDA_H();
  I2C_SomeNOP(5);
}

#if 0
void HT16C22_IIC_WaitAck(void)
{
    HT16C22_SCL_L();
    HT16C22_SDA_H();
    I2C_SomeNOP(5);
    HT16C22_SCL_H();
    I2C_SomeNOP(10);
    HT16C22_SCL_L();
    I2C_SomeNOP(5);
}
#endif

void HT16C22_IIC_WriteByte(u8 byte)
{
  u8 i = 0;
  for (i = 0; i < 8; i++)
  {
    if (byte & 0x80)
    {
      HT16C22_SDA_H();
    }
    else
    {
      HT16C22_SDA_L();
    }
    I2C_SomeNOP(5);

    HT16C22_SCL_H();
    I2C_SomeNOP(10);

    HT16C22_SCL_L();
    byte <<= 1;
    I2C_SomeNOP(5);
  }

  HT16C22_SDA_H();
  HT16C22_SDA_IN();
  I2C_SomeNOP(5);
  HT16C22_SCL_H();
  I2C_SomeNOP(5);

  if (HT16C22_SDA_READ())
  {
    HT16C22_ack = 0;
  }
  else
  {
    HT16C22_ack = 1;
  }

  HT16C22_SCL_L();
  I2C_SomeNOP(5);
  HT16C22_SDA_OUT();
}

void HT16C22_Write_RAM(u8 *pBuffer, u8 RamAdd, u8 NumByte)
{
  u8 i, temp;
  temp = RamAdd & 0x1f;
  HT16C22_IIC_Start();
  HT16C22_IIC_WriteByte(HT16C22_I2C_ADDW);
  HT16C22_IIC_WriteByte(temp); // addr:0~0X1F
  for (i = 0; i < NumByte; i++)
  {
    HT16C22_IIC_WriteByte(pBuffer[i]);
  }
  HT16C22_IIC_Stop();
}

void HT16C22_Init(void)
{
  I2C_SomeNOP(5000); // UserDelay(50000);     //�ȴ�оƬ�ȶ�
  HT16C22_IIC_Start();
  HT16C22_IIC_WriteByte(HT16C22_I2C_ADDW);

  //ģʽ����  1/3 Bais 1/4 Duty֡Ƶ��80Hz,�ڲ�ϵͳ��������lcd����ʾ
  HT16C22_IIC_WriteByte(0X8C);
  HT16C22_IIC_Stop();
}

#if 1
void HT16C22_disD_chg(void)
{
  unsigned char i, temp;
  for (i = 0; i < 11; i++)
  {
    g_disa_dat[i] = 0;
    temp = DataBuf.DisBuf1[i];
    if (temp & 0x01)
    {
      g_disa_dat[i] |= 0x80;
    }
    if (temp & 0x02)
    {
      g_disa_dat[i] |= 0x40;
    }
    if (temp & 0x04)
    {
      g_disa_dat[i] |= 0x20;
    }
    if (temp & 0x08)
    {
      g_disa_dat[i] |= 0x10;
    }
    if (temp & 0x10)
    {
      g_disa_dat[i] |= 0x08;
    }
    if (temp & 0x20)
    {
      g_disa_dat[i] |= 0x04;
    }
    if (temp & 0x40)
    {
      g_disa_dat[i] |= 0x02;
    }
    if (temp & 0x80)
    {
      g_disa_dat[i] |= 0x01;
    }

    g_disb_dat[i] = 0;
    temp = DataBuf.DisBuf2[i];
    if (temp & 0x01)
    {
      g_disb_dat[i] |= 0x80;
    }
    if (temp & 0x02)
    {
      g_disb_dat[i] |= 0x40;
    }
    if (temp & 0x04)
    {
      g_disb_dat[i] |= 0x20;
    }
    if (temp & 0x08)
    {
      g_disb_dat[i] |= 0x10;
    }
    if (temp & 0x10)
    {
      g_disb_dat[i] |= 0x08;
    }
    if (temp & 0x20)
    {
      g_disb_dat[i] |= 0x04;
    }
    if (temp & 0x40)
    {
      g_disb_dat[i] |= 0x02;
    }
    if (temp & 0x80)
    {
      g_disb_dat[i] |= 0x01;
    }
  }
}
#endif

void HT16C22_DisAll(unsigned char dat)
{
  unsigned char segi;
  HT16C22_IIC_Start();
  HT16C22_IIC_WriteByte(HT16C22_I2C_ADDW);
  HT16C22_IIC_WriteByte(0x00);
  for (segi = 0; segi < 32; segi++)
  {
    HT16C22_IIC_WriteByte(dat);
  }
  HT16C22_IIC_Stop();
}

void TEST_DisAll(unsigned char dat)
{
  unsigned char i;
  g_dis_abuf[0] = 0xff;
  for (i = 0; i < 11; i++)
  {
    g_dis_abuf[i] = dat;
  }
}

void TEST_Dis2All(unsigned char dat)
{
  unsigned char i;
  g_dis_bbuf[0] = 0xff;
  for (i = 0; i < 11; i++)
  {
    g_dis_bbuf[i] = dat;
  }
}
// END HT16C22A

void LCD_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_2; // lcd i2c gpio init
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_3; // lcd i2c gpio init
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);

  // HT16C22A
  GPIO_InitStruct.Pin = GPIO_PIN_14; // LCD_I2C_SCL
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13; // LCD_I2C_SDA
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

#if 0
    GPIO_InitStruct.Pin = GPIO_PIN_12;    //NA
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;    //NA
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
#endif

  //    PCF8562_Init(PCF8562_ADDR, 0x08, 0x00);
  //    PCF8562_Init(PCF8562_ADDR, 0x08, 0x00);
  HT16C22_Init();
  HT16C22_Init();
  HT16C22_DisAll(0x00);
//  TEST_DisAll(0x01);
//  TEST_Dis2All(0x80);
//  HT16C22_DisAll(0x80);
}

void Disp(void)
{
  HT16C22_disD_chg();
  PCF8562_Display(PCF8562_ADDR, 0x01, 0x00, DataBuf.DisBuf2, 10);
  PCF8562_Display(PCF8562_ADDR, 0x00, 0x00, DataBuf.DisBuf1, 11);
  HT16C22_Write_RAM(g_disb_dat, 11, 10);
  HT16C22_Write_RAM(g_disa_dat, 0, 11);
}

// end of file
