#ifndef __FLASH__H__
#define __FLASH__H__
#include "prjlib.h"
#ifdef __cplusplus
extern "C"
{
#endif
/*---------------------C O M P L I E R  C O M M A N D----------------*/
/*---------------------I N C L U D  F I L E S------------------------*/

void Flash_PageErase_1(uint32_t Pageaddr);
int8_t read_flash(uint32_t adr, uint8_t *wr_buf, uint16_t len);
uint16_t read_uint16_t_flash(uint32_t adr);
int8_t write_flash(uint32_t adr, uint8_t *wr_buf, uint16_t len);
void FlashRead(uint32_t pageaddr,uint8_t *databuffer,uint16_t len);
void FlashWrite(uint32_t pageaddr,uint8_t *databuffer,uint16_t len);
void Uint32toByte(uint32_t data,uint8_t* buffer);
uint32_t BytetoUint32(uint8_t* buffer);
void Flash_PageErase(uint32_t Pageaddr);
#ifdef __cplusplus
}
#endif

#endif