/***
 * File    : SD_interface.c
 * By      : Jonas Song
 * Version : V1.0
 * Date    : 2012.7.8
 * Copy    :
 * Brief   : SD接口函数文件, 可以在任务与中断中共同使用，支持MMC、SDV1.0、SD V.20、SDHC。
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "user_comm.h"
#include "stm32f0xx_ll_spi.h"

/* Private define ------------------------------------------------------------*/
#if SD_SPI_USED_NUM == 1
#define SD_SPI_X           SPI1
#define SD_SPI_PORT        GPIOA
#define SD_SPI_SS_PIN      GPIO_Pin_4
#define SD_SPI_SCK_PIN     GPIO_Pin_5
#define SD_SPI_MISO_PIN    GPIO_Pin_6
#define SD_SPI_MOSI_PIN    GPIO_Pin_7
#define SD_SPI_DMA_TX      DMA1_Channel3
#define SD_SPI_DMA_RX      DMA1_Channel2
#define SD_SPI_DMA_TX_TC   DMA1_IT_TC3
#define SD_SPI_DMA_RX_TC   DMA1_IT_TC2
#define SD_SPI_IRQ         SPI1_IRQn
#define SD_SPI_DMA_TX_IRQ  DMA1_Channel3_IRQn
#define SD_SPI_DMA_RX_IRQ  DMA1_Channel2_IRQn
#define SD_SPI_RCC         RCC_APB2Periph_SPI1


#elif SD_SPI_USED_NUM == 2
#define SD_SPI_X           SPI2
#define SD_SPI_PORT        GPIOB
#define SD_SPI_SS_PIN      GPIO_Pin_12
#define SD_SPI_SCK_PIN     GPIO_Pin_13
#define SD_SPI_MISO_PIN    GPIO_Pin_14
#define SD_SPI_MOSI_PIN    GPIO_Pin_15
#define SD_SPI_DMA_TX      DMA1_Channel5
#define SD_SPI_DMA_RX      DMA1_Channel4
#define SD_SPI_DMA_TX_TC   DMA1_IT_TC5
#define SD_SPI_DMA_RX_TC   DMA1_IT_TC4
#define SD_SPI_IRQ         SPI2_IRQn
#define SD_SPI_DMA_TX_IRQ  DMA1_Channel5_IRQn
#define SD_SPI_DMA_RX_IRQ  DMA1_Channel4_IRQn
#define SD_SPI_RCC         RCC_APB1Periph_SPI2
#endif

/* Private macro -------------------------------------------------------------*/
#define SD_CS_H()               IO_H(SD_CS)
#define SD_CS_L()               IO_L(SD_CS)

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;

/* Public variables ----------------------------------------------------------*/
stMSD_CARDINFO   CardInfo;

/* Private function prototypes -----------------------------------------------*/

static void     MEM_SPI_SpeedHigh(BOOL highEn);
static uint8_t    MEM_SPI_WriteRead(uint8_t c);
static void     MEM_SPI_SendBuffer(const uint8_t* buff, uint16_t len);
static void     MEM_SPI_ReadBuffer(uint8_t* buff, uint16_t len);

static BOOL     MSD_GetCardInfo(stMSD_CARDINFO* cardinfo);
static uint8_t    MSD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc);
static BOOL     MSD_MMC_Init(void);
static BOOL     MSD_SDV1_Init(void);
static BOOL     MSD_SDV2_Init(void);
static BOOL     MSD_Wait(uint8_t waitByte, uint32_t ms);
static uint8_t MEM_SPI_CMD_Wait(uint8_t cmd, uint32_t tick);

static void SD_Console(int argc, char* argv[]);

/* function definition -------------------------------------------------------*/

/**
 * SD模块初始化
 */
void MSD_Init(void) {
  __HAL_SPI_ENABLE(&hspi1);

  CMD_ENT_DEF(sd, SD_Console);
  Cmd_AddEntrance(CMD_ENT(sd));

  SD_CS_H();

  DBG_LOG("SD Init.");
}

/***
 * Description : MSD read multi blocks.
 * Arguments   : sector         -MSD read first sector.
 *               buffer         -Read buffer point.
 *               NbrOfSector    -Number of read sector.
 ********************************************************************************
 */
BOOL MSD_ReadBlocks(uint32_t sector, uint8_t* buffer, uint32_t NbrOfSector) {
  uint32_t i = 0;
  BOOL r = FALSE;
  uint8_t cmd = 0;

  MEM_SPI_WriteRead(DUMMY_BYTE);
  SD_CS_L();
  cmd = (NbrOfSector == 1) ? CMD17 : CMD18;
  /*if ver = SD2.0 HC, sector need <<9*/
  if (CardInfo.CardType != CARDTYPE_SDV2HC)
    sector = sector << 9;

  /*发送读数据指令*/
  if (MSD_SendCommand(cmd, sector, 0) == 0x00) {
    /*Start read*/
    for (i = 0; i < NbrOfSector; i++) {
      /*等待开始标志, 超时300ms*/
      if (MEM_SPI_CMD_Wait(0xFE, 300) == 0xFE) {
        /*接收数据*/
        MEM_SPI_ReadBuffer(buffer, MSD_BLOCKSIZE);
        /*2bytes dummy CRC*/
        MEM_SPI_WriteRead(DUMMY_BYTE);
        MEM_SPI_WriteRead(DUMMY_BYTE);
        r = TRUE;
      } else {
        r = FALSE;
        DBG_LOG("MSD Read 0xFE fail.");
      }
      buffer += MSD_BLOCKSIZE;
    }
  }
  MSD_SendCommand(CMD12, 0, 0);
  SD_CS_H();
  MEM_SPI_WriteRead(DUMMY_BYTE);


  if (r == FALSE) /*非中断*/
    DBG_LOG("MSD Read block fail.");
  return r;
}

/***
 * Description : MSD write multi blocks.
 * Arguments   : sector         -MSD write first sector.
 *               buffer         -Write buffer point.
 *               NbrOfSector    -Number of write sector.
 ********************************************************************************
 */
BOOL MSD_WriteBlocks(uint32_t sector, const uint8_t* buffer, uint32_t NbrOfSector) {
  BOOL r = FALSE;
  uint8_t i, r1 = 0, cmd = 0, token;

  MEM_SPI_WriteRead(DUMMY_BYTE);
  SD_CS_L();
  cmd = (NbrOfSector == 1) ? CMD24 : CMD25;
  token = (NbrOfSector == 1) ? 0xFE : 0xFC;

  /*if ver = SD2.0 HC, sector need <<9*/
  if (CardInfo.CardType != CARDTYPE_SDV2HC)
    sector = sector << 9;

  /*Send command ACMD23 berfore multi write if is not a MMC card*/
  if (sector > 1 && CardInfo.CardType != CARDTYPE_MMC) {
    MSD_SendCommand(CMD55, NbrOfSector, 0x00);
    MSD_SendCommand(ACMD23, NbrOfSector, 0x00);
  }
  /*Send CMD25 : Write nulti blocks command*/
  if (MSD_SendCommand(cmd, sector, 0) == 0x00) {
    MEM_SPI_WriteRead(DUMMY_BYTE);
    for (i = 0; i < NbrOfSector; i++) {
      /*发送写起始标志*/
      MEM_SPI_WriteRead(token);
      /*发送数据*/
      MEM_SPI_SendBuffer(buffer, MSD_BLOCKSIZE);
      /*2bytes dummy CRC*/
      MEM_SPI_WriteRead(DUMMY_BYTE);
      MEM_SPI_WriteRead(DUMMY_BYTE);
      /*卡接收数据成功？*/
      r1 = MEM_SPI_WriteRead(DUMMY_BYTE);
      if ((r1 & 0x1F) == 0x05) {
        /*等待数据写完成，超时时间500ms*/
        r = MSD_Wait(0xFF, 500);
      } else {
        r = FALSE;
        DBG_LOG("MSD not accept write data.");
      }
      buffer += MSD_BLOCKSIZE;
    }
    if (NbrOfSector > 1) {
      /*发送多块写结束标志*/
      MEM_SPI_WriteRead(0xFD);
      /*等待数据写完成，超时时间500ms*/
      r = MSD_Wait(0xFF, 500);
    }
  }
  SD_CS_H();
  MEM_SPI_WriteRead(DUMMY_BYTE);

  if (r == FALSE) /*非中断*/
    DBG_LOG("MSD write block fail.");
  return r;
}


/***
 * Description : Configuration MMC/SD card.
 * Caller      : Application.
 ********************************************************************************
 */
BOOL MSD_Configuration(void) {
  uint8_t r1 = 0;
  uint16_t retry = 0;
  BOOL r = FALSE;

  MEM_SPI_SpeedHigh(FALSE);    /*低速模式*/

  SD_CS_L();
  /*Satrt send 74 clocks at least*/
  for (retry = 0; retry < 100; retry++)
    MEM_SPI_WriteRead(DUMMY_BYTE);

  /*发送CMD0使卡进入IDLE状态*/
  retry = 0;
  do {
    r1 = MSD_SendCommand(CMD0, 0, 0x95);
  } while (r1 != 0x01 && retry++ < 100);

  if (r1 == 0x01) {
    r1 = MSD_SendCommand(CMD8, 0x1AA, 0x87);
    if (r1 == 0x05) {  /*SD v1.0 or MMC*/
      r  = MSD_SDV1_Init();
      if (r == FALSE)
        r = MSD_MMC_Init();
    } else if (r1 == 0x01) { /*SD v2.0 or late*/
      r = MSD_SDV2_Init();
    } else {
      DBG_LOG("Card type not support!!!");
    }
  } else {
    DBG_LOG("MSD Card idle state enter failed!");
  }
  if (r != TRUE) {
    DBG_LOG("MSD card Init Failed!");
  }
  /*Get card information*/
  if (r == TRUE) {
    if (MSD_GetCardInfo(&CardInfo) == FALSE) {
      r = FALSE;
      DBG_LOG("MSD card get information failed.");
    } else {
      DBG_LOG("MSD Init OK.");
    }
  }
  SD_CS_H();
  MEM_SPI_SpeedHigh(TRUE);    /*高速模式*/

  return r;
}

/***
 * Description : Get MMC/SD Card infomation.
 * Arguments   : cardinfo   -infomation data point.
 ********************************************************************************
 */
static BOOL MSD_GetCardInfo(stMSD_CARDINFO* cardinfo) {
  uint8_t CSD_Tab[16];
  uint8_t CID_Tab[16];

  /*Send CMD9, Read CSD*/
  if (MSD_SendCommand(CMD9, 0, 0xFF) != 0x00)
    return FALSE;
  if (MEM_SPI_CMD_Wait(0xFE, 100) == 0xFE) {
    /*接收数据*/
    MEM_SPI_ReadBuffer(CSD_Tab, 16);
    /*2bytes dummy CRC*/
    MEM_SPI_WriteRead(DUMMY_BYTE);
    MEM_SPI_WriteRead(DUMMY_BYTE);
  }

  /*Send CMD10, Read CID*/
  if (MSD_SendCommand(CMD10, 0, 0xFF) != 0x00)
    return FALSE;
  if (MEM_SPI_CMD_Wait(0xFE, 100) == 0xFE) {
    /*接收数据*/
    MEM_SPI_ReadBuffer(CID_Tab, 16);
    /*2bytes dummy CRC*/
    MEM_SPI_WriteRead(DUMMY_BYTE);
    MEM_SPI_WriteRead(DUMMY_BYTE);
  }

  /* Byte 0 */
  cardinfo->CSD.CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
  cardinfo->CSD.SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
  cardinfo->CSD.Reserved1 = CSD_Tab[0] & 0x03;
  /* Byte 1 */
  cardinfo->CSD.TAAC = CSD_Tab[1];
  /* Byte 2 */
  cardinfo->CSD.NSAC = CSD_Tab[2];
  /* Byte 3 */
  cardinfo->CSD.MaxBusClkFrec = CSD_Tab[3];
  /* Byte 4 */
  cardinfo->CSD.CardComdClasses = CSD_Tab[4] << 4;
  /* Byte 5 */
  cardinfo->CSD.CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
  cardinfo->CSD.RdBlockLen = CSD_Tab[5] & 0x0F;
  /* Byte 6 */
  cardinfo->CSD.PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
  cardinfo->CSD.WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
  cardinfo->CSD.RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
  cardinfo->CSD.DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
  cardinfo->CSD.Reserved2 = 0; /* Reserved */
  cardinfo->CSD.DeviceSize = (CSD_Tab[6] & 0x03) << 10;
  /* Byte 7 */
  cardinfo->CSD.DeviceSize |= (CSD_Tab[7]) << 2;
  /* Byte 8 */
  cardinfo->CSD.DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;
  cardinfo->CSD.MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
  cardinfo->CSD.MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);
  /* Byte 9 */
  cardinfo->CSD.MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
  cardinfo->CSD.MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
  cardinfo->CSD.DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;
  /* Byte 10 */
  cardinfo->CSD.DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
  cardinfo->CSD.EraseGrSize = (CSD_Tab[10] & 0x7C) >> 2;
  cardinfo->CSD.EraseGrMul = (CSD_Tab[10] & 0x03) << 3;
  /* Byte 11 */
  cardinfo->CSD.EraseGrMul |= (CSD_Tab[11] & 0xE0) >> 5;
  cardinfo->CSD.WrProtectGrSize = (CSD_Tab[11] & 0x1F);
  /* Byte 12 */
  cardinfo->CSD.WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
  cardinfo->CSD.ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
  cardinfo->CSD.WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
  cardinfo->CSD.MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;
  /* Byte 13 */
  cardinfo->CSD.MaxWrBlockLen |= (CSD_Tab[13] & 0xc0) >> 6;
  cardinfo->CSD.WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
  cardinfo->CSD.Reserved3 = 0;
  cardinfo->CSD.ContentProtectAppli = (CSD_Tab[13] & 0x01);
  /* Byte 14 */
  cardinfo->CSD.FileFormatGrouop = (CSD_Tab[14] & 0x80) >> 7;
  cardinfo->CSD.CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
  cardinfo->CSD.PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
  cardinfo->CSD.TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
  cardinfo->CSD.FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
  cardinfo->CSD.ECC = (CSD_Tab[14] & 0x03);
  /* Byte 15 */
  cardinfo->CSD.CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
  cardinfo->CSD.Reserved4 = 1;

  if (cardinfo->CardType == CARDTYPE_SDV2HC) {
    /* Byte 7 */
    cardinfo->CSD.DeviceSize = (uint16_t)(CSD_Tab[8]) * 256;
    /* Byte 8 */
    cardinfo->CSD.DeviceSize += CSD_Tab[9];
  }
  cardinfo->Capacity = cardinfo->CSD.DeviceSize * MSD_BLOCKSIZE * 1024;
  cardinfo->BlockSize = MSD_BLOCKSIZE;

  /* Byte 0 */
  cardinfo->CID.ManufacturerID = CID_Tab[0];
  /* Byte 1 */
  cardinfo->CID.OEM_AppliID = CID_Tab[1] << 8;
  /* Byte 2 */
  cardinfo->CID.OEM_AppliID |= CID_Tab[2];
  /* Byte 3 */
  cardinfo->CID.ProdName1 = CID_Tab[3] << 24;
  /* Byte 4 */
  cardinfo->CID.ProdName1 |= CID_Tab[4] << 16;
  /* Byte 5 */
  cardinfo->CID.ProdName1 |= CID_Tab[5] << 8;
  /* Byte 6 */
  cardinfo->CID.ProdName1 |= CID_Tab[6];
  /* Byte 7 */
  cardinfo->CID.ProdName2 = CID_Tab[7];
  /* Byte 8 */
  cardinfo->CID.ProdRev = CID_Tab[8];
  /* Byte 9 */
  cardinfo->CID.ProdSN = CID_Tab[9] << 24;
  /* Byte 10 */
  cardinfo->CID.ProdSN |= CID_Tab[10] << 16;
  /* Byte 11 */
  cardinfo->CID.ProdSN |= CID_Tab[11] << 8;
  /* Byte 12 */
  cardinfo->CID.ProdSN |= CID_Tab[12];
  /* Byte 13 */
  cardinfo->CID.Reserved1 |= (CID_Tab[13] & 0xF0) >> 4;
  /* Byte 14 */
  cardinfo->CID.ManufactDate = (CID_Tab[13] & 0x0F) << 8;
  /* Byte 15 */
  cardinfo->CID.ManufactDate |= CID_Tab[14];
  /* Byte 16 */
  cardinfo->CID.CID_CRC = (CID_Tab[15] & 0xFE) >> 1;
  cardinfo->CID.Reserved2 = 1;

  return TRUE;
}

/***
 * Description : SPI写入数据.
 ********************************************************************************
 */
static void MEM_SPI_SendBuffer(const uint8_t* buff, uint16_t len) {
  uint16_t retry = 0;

  for (retry = 0; retry < len; retry++) {
    MEM_SPI_WriteRead(*(buff + retry));
  }
}

/***
 * Description : SPI读取数据.
 ********************************************************************************
 */
static void MEM_SPI_ReadBuffer(uint8_t* buff, uint16_t len) {
  uint16_t retry = 0;

  for (retry = 0; retry < len; retry++) {
    *(buff + retry) = MEM_SPI_WriteRead(DUMMY_BYTE);
  }
}

/***
 * Description : SD card SPI interface speed control.
 * Arguments   : highEn       -TRUE then SD clock running high.
 ********************************************************************************
 */
static void MEM_SPI_SpeedHigh(BOOL highEn) {
  SPI_TypeDef* spi = hspi1.Instance;

  if (highEn) {
    LL_SPI_SetBaudRatePrescaler(spi, LL_SPI_BAUDRATEPRESCALER_DIV4);
  } else {
    LL_SPI_SetBaudRatePrescaler(spi, LL_SPI_BAUDRATEPRESCALER_DIV256);
  }
}

/***
 * Description : SD SPI write and read one.
 ********************************************************************************
 */
static uint8_t MEM_SPI_WriteRead(uint8_t c) {
  uint8_t ret = 0;
  SPI_TypeDef* spi = hspi1.Instance;

  if (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) != RESET) {
    __HAL_SPI_CLEAR_OVRFLAG(&hspi1);
  }

  while ((spi->SR & SPI_FLAG_TXE) == RESET);
  *((__IO uint8_t*)&(spi->DR)) = c;
  while ((spi->SR & SPI_FLAG_RXNE) == RESET);
  ret = *((__IO uint8_t*)&(spi->DR));

  return ret;

}

/***
 * Description  : SD send command.
 * Arguments    : cmd     -SD command.
 *                arg     -Command argument.
 *                crc     -Command crc check data.
 *                hold    -CS hold enable.
 ********************************************************************************
 */
static uint8_t MSD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc) {
  uint8_t r, i = 0;

  MEM_SPI_WriteRead(DUMMY_BYTE);
  /*Command, argument and crc*/
  MEM_SPI_WriteRead(cmd | 0x40);
  MEM_SPI_WriteRead(arg >> 24);
  MEM_SPI_WriteRead(arg >> 16);
  MEM_SPI_WriteRead(arg >> 8);
  MEM_SPI_WriteRead(arg);
  MEM_SPI_WriteRead(crc);

  /*Ncr max 8 byte*/
  for (i = 0; i < 8; i++) {
    r = MEM_SPI_WriteRead(DUMMY_BYTE);
    if (r != 0xFF)
      break;
  }
  return r;
}

/***
 * Description  : MMC卡初始化.
 ********************************************************************************
 */
static BOOL MSD_MMC_Init(void) {
  BOOL r =  FALSE;
  uint8_t r1 = 0;
  int retry = 0;

  do {
    r1 = MSD_SendCommand(CMD1, 0, 0);
  } while (retry++ < 1000 && r1 != 0x00);
  if (r1 == 0x00) {
    /*Disable CRC*/
    MSD_SendCommand(CMD59, 0, 0x01);
    /*Set the block size*/
    MSD_SendCommand(CMD16, MSD_BLOCKSIZE, 0xFF);

    CardInfo.CardType = CARDTYPE_MMC;
    r = TRUE;
    DBG_LOG("MMC Card init ok.");
  }
  return r;
}

/***
 * Description  : V1.1版SD卡初始化.
 ********************************************************************************
 */
static BOOL MSD_SDV1_Init(void) {
  BOOL r =  FALSE;
  uint8_t r1 = 0;
  int retry = 0;

  do {
    if (MSD_SendCommand(CMD55, 0, 0) == 0x01)
      r1 = MSD_SendCommand(ACMD41, 0x40000000, 0);
  } while (retry++ < 10000 && r1 != 0x00);

  if (r1 == 0x00) {
    /*Disable CRC*/
    MSD_SendCommand(CMD59, 0, 0x01);
    /*Set the block size*/
    MSD_SendCommand(CMD16, MSD_BLOCKSIZE, 0xFF);

    CardInfo.CardType = CARDTYPE_SDV1;
    r = TRUE;
    DBG_LOG("SD Card V1.0 init ok.");
  }
  return r;
}

/***
 * Description  : V2.0版SD卡初始化.
 ********************************************************************************
 */
static BOOL MSD_SDV2_Init(void) {
  int retry = 0;
  uint8_t buff[10], r1 = 0;
  BOOL r =  FALSE;

  /*4Bytes returned after CMD8 sent*/
  buff[0] = MEM_SPI_WriteRead(DUMMY_BYTE);
  buff[1] = MEM_SPI_WriteRead(DUMMY_BYTE);
  buff[2] = MEM_SPI_WriteRead(DUMMY_BYTE);
  buff[3] = MEM_SPI_WriteRead(DUMMY_BYTE);


  /*Check voltage range be 2.7-3.6V*/
  if (buff[2] == 0x01 && buff[3] == 0xAA) {
    retry = 0;
    do {
      if (MSD_SendCommand(CMD55, 0, 0) == 0x01)
        r1 = MSD_SendCommand(ACMD41, 0x40000000, 0);
    } while (retry++ < 10000 && r1 != 0x00);
    /*Read OCR by CMD58*/
    if (r1 == 0x00) {
      if (MSD_SendCommand(CMD58, 0, 0) == 0x00) {
        buff[0] = MEM_SPI_WriteRead(DUMMY_BYTE);
        buff[1] = MEM_SPI_WriteRead(DUMMY_BYTE);
        buff[2] = MEM_SPI_WriteRead(DUMMY_BYTE);
        buff[3] = MEM_SPI_WriteRead(DUMMY_BYTE);
        /*OCR -> CCS(bit30)  1: SDV2HC 0: SDV2*/
        if (buff[0] & 0x40) {
          CardInfo.CardType = CARDTYPE_SDV2HC;
          DBG_LOG("SDHC v2.0 Card Configuration ok.");
        } else {
          CardInfo.CardType = CARDTYPE_SDV2SC;
          DBG_LOG("SDSC v2.0 Card Configuration ok.");
        }
        r = TRUE;
      }
    }
  } else {
    DBG_LOG("Card voltage not support!!!");
  }
  return r;
}


/***
 * Description : SD卡 等待写结束.
 *******************************************************************************
 */
static BOOL MSD_Wait(uint8_t waitByte, uint32_t ms) {
  uint8_t res = 0;

  res = MEM_SPI_CMD_Wait(waitByte, ms);

  return ((res != waitByte) ? FALSE : TRUE);
}


/***
 * Description : 等待SPI命令返回结果.
 *******************************************************************************
 */
static uint8_t MEM_SPI_CMD_Wait(uint8_t cmd, uint32_t tick) {
  uint8_t r = cmd;
  uint32_t time = 0;

  do {
    TS_INIT(time);
    while (!TS_IS_OVER(time, 1000)) {
      r = MEM_SPI_WriteRead(DUMMY_BYTE);
      if (r == cmd)
        break;
    }
  } while (r != cmd && tick > 0);
  return r;
}


/**
 * SD卡调试命令
 *
 */
static void SD_Console(int argc, char* argv[]) {
  argv++;
  argc--;

  if (ARGV_EQUAL("spi")) {
    SD_CS_L();
    DBG_LOG("MEM_SPI_WriteRead():%#x.", MEM_SPI_WriteRead(uatoi(argv[1])));
    SD_CS_H();
  } else if (ARGV_EQUAL("readblock")) {}
}
