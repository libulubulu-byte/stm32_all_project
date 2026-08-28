/*
本代码修改于201610906 1058
仅仅可以使用void SHT2x_SoftReset(void)、float SHT2x_MeasureTempHM(void)、float SHT2x_MeasureHumiHM(void) 
（其他代码没有测试，不能保证能正确运行
分别是：软件复位。获取主机模式下的温度，获取主机模式下的湿度
*/

//#include "LCD_Display.h"
#include "stm32l0xx_hal.h"
#include "sht20.h"
#include "stdint.h"

 extern I2C_HandleTypeDef hi2c2;
// SHT2x_PARAM g_sht2x_param;

/* Buffer used for transmission */
 uint8_t aTxBuffer[1]={0};
/* Buffer used for reception */
 uint8_t aRxBuffer[2]={0,0};

 
/******主机模式测温度*******
 功能：获取主机模式下的温度
 参数：无
 返回值：Float类型的温度值。
 */
float SHT2x_MeasureTempHM(void)
{
	/* Buffer used for transmission */
// uint8_t aTxBuffer[1]={0};
///* Buffer used for reception */
// uint8_t aRxBuffer[2]={0,0};
	uint16_t shift=0;
	float Temperature=0;
	aTxBuffer[0]=I2C_ADR_W;
  while(HAL_I2C_Master_Transmit_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
  {

  }
    //发送读取温度命令
	  aTxBuffer[0]=TRIG_TEMP_MEASUREMENT_HM;
		while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	   while(HAL_I2C_Master_Transmit_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
		{

		}
	aTxBuffer[0]=I2C_ADR_R;
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	 while(HAL_I2C_Master_Transmit_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
		{

		}
		 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	 while(HAL_I2C_Master_Receive_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aRxBuffer, 2) != HAL_OK)
		{
		
		} 
		while(HAL_I2C_Master_Transmit_IT2(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
		{
  
		}
    shift = ( aRxBuffer[0]<< 8) | (aRxBuffer[1] << 0);
    shift &= ~0x0003;
    Temperature = ((float)shift * 0.00268127) - 46.85;

    return (Temperature);
}
/******主机模式测湿度*******
 功能：获取主机模式下的湿度
 参数：无
 返回值：Float类型湿度值。
*/
float SHT2x_MeasureHumiHM(void)
{ 
/* Buffer used for transmission */
// uint8_t aTxBuffer[1]={0};
///* Buffer used for reception */
// uint8_t aRxBuffer[2]={0,0};
	uint16_t shift=0;
	float Humidityl=0;
	aTxBuffer[0]=I2C_ADR_W;
  while(HAL_I2C_Master_Transmit_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
  {

  }
    //发送读取温度命令
	  aTxBuffer[0]=TRIG_HUMI_MEASUREMENT_HM;
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	   while(HAL_I2C_Master_Transmit_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
  {

  }
	aTxBuffer[0]=I2C_ADR_R;
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	 while(HAL_I2C_Master_Transmit_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
		{

		}
		 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	 while(HAL_I2C_Master_Receive_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aRxBuffer, 2) != HAL_OK)
		{
			
		} 
		while(HAL_I2C_Master_Transmit_IT2(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
		{
  
		}	
    shift = (aRxBuffer[0] << 8) | (aRxBuffer[1] << 0);
    shift &= ~0x0003;
    Humidityl = ((float)shift * 0.00190735) - 6;
		if(Humidityl>100)
		{
			Humidityl=100;
		}
    return (Humidityl);
}

float SHT2x_MeasureTempPoll(void)
{
	SHT2x_SoftReset();   
	float TEMP;
    uint8_t tmp1[2];
    uint16_t ST;
    
		  uint8_t bTransferRequest = 0;
    bTransferRequest=I2C_ADR_W;
	//传输地址和写命令
  while(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)&bTransferRequest, 1,1000)!= HAL_OK)
  {

    /* Error_Handler() function is called when Timout error occurs.
       When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
    {
//      Error_Handler();
    }
  }
    //发送读取温度命令
	  bTransferRequest=TRIG_TEMP_MEASUREMENT_POLL;
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	  while(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)&bTransferRequest, 1,1000)!= HAL_OK)
  {

    /* Error_Handler() function is called when Timout error occurs.
       When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
    {
//      Error_Handler();
    }
  }

  //发送地址和读命令
	
	
		  bTransferRequest=I2C_ADR_R;
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	  while(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)&bTransferRequest, 1,1000)!= HAL_OK)
  {

    /* Error_Handler() function is called when Timout error occurs.
       When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
    {
//      Error_Handler();
    }
  }
	 
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }

    /*##-7- Master receives aRxBuffer from slave #############################*/
    while(HAL_I2C_Master_Receive_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)tmp1, 2)!= HAL_OK)
    {
      /* Error_Handler() function is called when Timout error occurs.
         When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
         Master restarts communication */
      if(HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
      {
//        Error_Handler();
      }
    }

	
    ST = (tmp1[0] << 8) | (tmp1[1] << 0);
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;

    return (TEMP);	  
}

float SHT2x_MeasureHumiPoll(void)
{
	SHT2x_SoftReset();   
	 float HUMI;
    uint8_t  tmp1[2];    
    uint16_t SRH;
   
		  uint8_t bTransferRequest = 0;
    bTransferRequest=I2C_ADR_W;
	//传输地址和写命令
  while(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)&bTransferRequest, 1,1000)!= HAL_OK)
  {

    /* Error_Handler() function is called when Timout error occurs.
       When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
    {
//      Error_Handler();
    }
  }
    //发送读取温度命令
	  bTransferRequest=TRIG_HUMI_MEASUREMENT_POLL;
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	  while(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)&bTransferRequest, 1,1000)!= HAL_OK)
  {

    /* Error_Handler() function is called when Timout error occurs.
       When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
    {
//      Error_Handler();
    }
  }

  //发送地址和读命令
	
	
		  bTransferRequest=I2C_ADR_R;
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	  while(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)&bTransferRequest, 1,1000)!= HAL_OK)
  {

    /* Error_Handler() function is called when Timout error occurs.
       When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
    {
//      Error_Handler();
    }
  }
	 
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }

    /*##-7- Master receives aRxBuffer from slave #############################*/
    while(HAL_I2C_Master_Receive_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)tmp1, 2)!= HAL_OK)
    {
      /* Error_Handler() function is called when Timout error occurs.
         When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
         Master restarts communication */
      if(HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
      {
//        Error_Handler();
      }
    }
   
    SRH = (tmp1[0] << 8) | (tmp1[1] << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    return (HUMI);
}

//uint8_t SHT2x_ReadUserReg(void)    
//{
//    uint8_t reg;

//    
//    SHT2x_I2cStartCondition();                 
//    SHT2x_I2cWriteByte(I2C_ADR_W);
//    SHT2x_I2cWriteByte(USER_REG_R);
//    SHT2x_I2cStartCondition(); 
//    SHT2x_I2cWriteByte(I2C_ADR_R);
//    reg = SHT2x_I2cReadByte();
//    SHT2x_I2cNoAcknowledge();
//    SHT2x_I2cStopCondition();
//    
//    return (reg); 
//}

// 
void SHT2x_SoftReset(void)
{
/* Buffer used for transmission */
// uint8_t aTxBuffer[1]={0};
///* Buffer used for reception */
// uint8_t aRxBuffer[2]={0,0};
	
 HAL_Delay(5);             //  必须延时

	aTxBuffer[0]=I2C_ADR_W;
  while(HAL_I2C_Master_Transmit_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
    {
//      Error_Handler();
    }   
  }
	  aTxBuffer[0]=SOFT_RESET;
	 while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
    }
	   while(HAL_I2C_Master_Transmit_IT(&hi2c2, (uint16_t)I2C_ADR, (uint8_t*)aTxBuffer, 1) != HAL_OK)
  {
    if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
    {
//      Error_Handler();
    }   
  }
	 HAL_Delay(5);             //  必须延时
}

//void SHT2x_GetSerialNumber(uint8_t *buf)
//{
//    /* Read from memory location 1 */
//    SHT2x_I2cStartCondition();
//    SHT2x_I2cWriteByte(I2C_ADR_W); //I2C address
//    SHT2x_I2cWriteByte(0xFA); //Command for readout on-chip memory
//    SHT2x_I2cWriteByte(0x0F); //on-chip memory address
//    SHT2x_I2cStartCondition();
//    SHT2x_I2cWriteByte(I2C_ADR_R); //I2C address
//    buf[5] = SHT2x_I2cReadByte(); //Read SNB_3
//    SHT2x_I2cAcknowledge();
//    SHT2x_I2cReadByte(); //Read CRC SNB_3 (CRC is not analyzed)
//    SHT2x_I2cAcknowledge();
//    buf[4] = SHT2x_I2cReadByte(); //Read SNB_2
//    SHT2x_I2cAcknowledge();
//    SHT2x_I2cReadByte(); //Read CRC SNB_2 (CRC is not analyzed)
//    SHT2x_I2cAcknowledge();
//    buf[3] = SHT2x_I2cReadByte(); //Read SNB_1
//    SHT2x_I2cAcknowledge();
//    SHT2x_I2cReadByte(); //Read CRC SNB_1 (CRC is not analyzed)
//    SHT2x_I2cAcknowledge();
//    buf[2] = SHT2x_I2cReadByte(); //Read SNB_0
//    SHT2x_I2cAcknowledge();
//    SHT2x_I2cReadByte(); //Read CRC SNB_0 (CRC is not analyzed)
//    SHT2x_I2cNoAcknowledge();
//    SHT2x_I2cStopCondition();

//    /* Read from memory location 2 */
//    SHT2x_I2cStartCondition();
//    SHT2x_I2cWriteByte(I2C_ADR_W); //I2C address
//    SHT2x_I2cWriteByte(0xFC); //Command for readout on-chip memory
//    SHT2x_I2cWriteByte(0xC9); //on-chip memory address
//    SHT2x_I2cStartCondition();
//    SHT2x_I2cWriteByte(I2C_ADR_R); //I2C address
//    buf[1] = SHT2x_I2cReadByte(); //Read SNC_1
//    SHT2x_I2cAcknowledge();
//    buf[0] = SHT2x_I2cReadByte(); //Read SNC_0
//    SHT2x_I2cAcknowledge();
//    SHT2x_I2cReadByte(); //Read CRC SNC0/1 (CRC is not analyzed)
//    SHT2x_I2cAcknowledge();
//    buf[7] = SHT2x_I2cReadByte(); //Read SNA_1
//    SHT2x_I2cAcknowledge();
//    buf[6] = SHT2x_I2cReadByte(); //Read SNA_0
//    SHT2x_I2cAcknowledge();
//    SHT2x_I2cReadByte(); //Read CRC SNA0/1 (CRC is not analyzed)
//    SHT2x_I2cNoAcknowledge();
//    SHT2x_I2cStopCondition();
//}

//void SHT2x_Test(void)
//{   
//    g_sht2x_param.TEMP_HM = SHT2x_MeasureTempHM();
//    g_sht2x_param.HUMI_HM = SHT2x_MeasureHumiHM();

//    g_sht2x_param.TEMP_POLL = SHT2x_MeasureTempPoll();
//    g_sht2x_param.HUMI_POLL = SHT2x_MeasureHumiPoll();
//    
//    //SHT2x_GetSerialNumber(g_sht2x_param.SerialNumber);
//}

