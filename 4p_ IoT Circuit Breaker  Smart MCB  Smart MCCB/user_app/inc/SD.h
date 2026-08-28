/***
 * File    : SD_interface.h
 * By      : Jonas Song
 * Version : V1.0
 * Date    : 2012.7.8
 * Copy    :
 * Brief   : 存储器接口头文件。
 *******************************************************************************
 */

#ifndef SD_H
#define SD_H
/* Includes ------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/
#define MSD_BLOCKSIZE       512             /*SD卡扇区大小*/

/*SD卡类型*/
#define CARDTYPE_MMC        0x00
#define CARDTYPE_SDV1       0x01
#define CARDTYPE_SDV2SC     0x02
#define CARDTYPE_SDV2HC     0x04

/*SD指令*/
#define CMD0    0       // Reset
#define CMD1    1       // Send Operator Condition - SEND_OP_COND
#define CMD8    8       // Send Interface Condition - SEND_IF_COND
#define CMD9    9       // Read CSD
#define CMD10   10      // Read CID
#define CMD12   12      // Stop data transmit
#define CMD16   16      // Set block size, should return 0x00
#define CMD17   17      // Read single block
#define CMD18   18      // Read multi block
#define ACMD23  23      // Prepare erase N-blokcs before multi block write
#define CMD24   24      // Write single block
#define CMD25   25      // Write multi block
#define ACMD41  41      // should return 0x00
#define CMD55   55      // should return 0x01
#define CMD58   58      // Read OCR
#define CMD59   59      // CRC disable/enbale, should return 0x00
#define DUMMY_BYTE      0xFF


/* Macro ---------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/
/* Card Specific Data */
typedef struct
{
  uint8_t  CSDStruct;            /* CSD structure */
  uint8_t  SysSpecVersion;       /* System specification version */
  uint8_t  Reserved1;            /* Reserved */
  uint8_t  TAAC;                 /* Data read access-time 1 */
  uint8_t  NSAC;                 /* Data read access-time 2 in CLK cycles */
  uint8_t  MaxBusClkFrec;        /* Max. bus clock frequency */
  uint16_t CardComdClasses;      /* Card command classes */
  uint8_t  RdBlockLen;           /* Max. read data block length */
  uint8_t  PartBlockRead;        /* Partial blocks for read allowed */
  uint8_t  WrBlockMisalign;      /* Write block misalignment */
  uint8_t  RdBlockMisalign;      /* Read block misalignment */
  uint8_t  DSRImpl;              /* DSR implemented */
  uint8_t  Reserved2;            /* Reserved */
  uint32_t DeviceSize;           /* Device Size */
  uint8_t  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
  uint8_t  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
  uint8_t  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
  uint8_t  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
  uint8_t  DeviceSizeMul;        /* Device size multiplier */
  uint8_t  EraseGrSize;          /* Erase group size */
  uint8_t  EraseGrMul;           /* Erase group size multiplier */
  uint8_t  WrProtectGrSize;      /* Write protect group size */
  uint8_t  WrProtectGrEnable;    /* Write protect group enable */
  uint8_t  ManDeflECC;           /* Manufacturer default ECC */
  uint8_t  WrSpeedFact;          /* Write speed factor */
  uint8_t  MaxWrBlockLen;        /* Max. write data block length */
  uint8_t  WriteBlockPaPartial;  /* Partial blocks for write allowed */
  uint8_t  Reserved3;            /* Reserded */
  uint8_t  ContentProtectAppli;  /* Content protection application */
  uint8_t  FileFormatGrouop;     /* File format group */
  uint8_t  CopyFlag;             /* Copy flag (OTP) */
  uint8_t  PermWrProtect;        /* Permanent write protection */
  uint8_t  TempWrProtect;        /* Temporary write protection */
  uint8_t  FileFormat;           /* File Format */
  uint8_t  ECC;                  /* ECC code */
  uint8_t  CSD_CRC;              /* CSD CRC */
  uint8_t  Reserved4;            /* always 1*/
} stMSD_CSD;

/*Card Identification Data*/
typedef struct
{
  uint8_t  ManufacturerID;       /* ManufacturerID */
  uint16_t OEM_AppliID;          /* OEM/Application ID */
  uint32_t ProdName1;            /* Product Name part1 */
  uint8_t  ProdName2;            /* Product Name part2*/
  uint8_t  ProdRev;              /* Product Revision */
  uint32_t ProdSN;               /* Product Serial Number */
  uint8_t  Reserved1;            /* Reserved1 */
  uint16_t ManufactDate;         /* Manufacturing Date */
  uint8_t  CID_CRC;              /* CID CRC */
  uint8_t  Reserved2;            /* always 1 */
} stMSD_CID;

/*Card Information Data*/
typedef struct
{
  stMSD_CSD CSD;
  stMSD_CID CID;
  uint64_t    Capacity;
  /* Card Capacity */   //BUG
  uint32_t    BlockSize;       /* Card Block Size */
  uint16_t    RCA;
  uint8_t     CardType;
  uint32_t    SpaceTotal;      /* Total space size in file system */
  uint32_t    SpaceFree;       /* Free space size in file system */
} stMSD_CARDINFO;

/* Variable declarations -----------------------------------------------------*/
extern stMSD_CARDINFO   CardInfo;

/* Function prototypes -------------------------------------------------------*/
void    MSD_Init(void);
BOOL    MSD_Configuration(void);

BOOL    MSD_ReadBlocks(uint32_t sector, uint8_t* buffer, uint32_t NbrOfSector);
BOOL    MSD_WriteBlocks(uint32_t sector, const uint8_t* buffer, uint32_t NbrOfSector);

#endif
