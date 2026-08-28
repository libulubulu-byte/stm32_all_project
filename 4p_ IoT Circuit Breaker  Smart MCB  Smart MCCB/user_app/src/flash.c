#include "main.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_hal_flash_ex.h"
#include "bord.h"
#include "proc.h"
#include "prjlib.h"
#include "user_comm.h"
#include "uart.h"
#include "flash.h"
#include <stdlib.h>
#include <stdio.h>


void Flash_PageErase_1(uint32_t Pageaddr)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t  PAGEError = 0;
  
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = Pageaddr;
  EraseInitStruct.NbPages     = 1;

  HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
}

int8_t read_flash(uint32_t adr, uint8_t *wr_buf, uint16_t len){
  uint32_t ADR;
  uint8_t *Buff;
  uint16_t LEN=len;
  ADR=adr;
  Buff=wr_buf;
  while(len<=LEN){
	*Buff = *(uint32_t*)(ADR);
        ADR+=4;
        Buff+=4;
	len-=4;
  }
    return 0;
}

uint16_t read_uint16_t_flash(uint32_t adr){
  uint16_t data;
  FlashRead(adr,(uint8_t*)&data,2);
//  read_flash(adr, (uint8_t*)&data, 2);
  return data;
}

int8_t write_flash(uint32_t adr, uint8_t *wr_buf, uint16_t len){
  uint32_t ADR;
  uint8_t *Buff;
  uint16_t LEN=len;
  ADR=adr;
  Buff=wr_buf;
  HAL_FLASH_Unlock();
  //__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR|FLASH_FLAG_PGERR);
  Flash_PageErase_1(adr); 
  while(len<=LEN)
  {
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,ADR,(uint64_t)Buff);
    ADR+=4;
    Buff+=4;
    len-=4;
  }
  HAL_FLASH_Lock();
  return 0;
}

void Flash_PageErase(uint32_t Pageaddr)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t  PAGEError = 0;
  
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = Pageaddr;
  EraseInitStruct.NbPages     = 1;

  HAL_FLASH_Unlock();

  HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);

  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) { 
  }

   HAL_FLASH_Lock();
}
/**
 * @brief
 * @note  字节转换为U32数据，按小端格式处理
 * @retval u32
 */
uint32_t BytetoUint32(uint8_t* buffer)
{
    uint16_t Result1=0,Result2=0;
    uint32_t Result=0;

    Result1 =( (uint16_t)buffer[3])<<8 |buffer[2];
    Result2 = ((uint16_t)buffer[1])<<8|buffer[0];
    Result =((uint32_t)Result1)<<16|Result2;

    return Result;
}
/**
 * @brief
 * @note  U32数据转换为字节，按小端格式处理
 * @retval void
 */
void Uint32toByte(uint32_t data,uint8_t* buffer)
{
    uint8_t i;

    for(i=0; i<4; i++)
    {
        buffer[i]=(uint8_t)(data>>(8*i));
    }
}
/**
 * @brief
 * @note  内部FLASH写数据，按小端格式处理
 * @retval void
 */
void FlashWrite(uint32_t pageaddr,uint8_t *databuffer,uint16_t len){
  uint16_t cnt =0,addroffset = 0,i;
  uint8_t leftbytes =0,buffer[4];
  uint32_t data =0;

   cnt = len /4;
   leftbytes = len % 4;

   /* Unlock the Flash to enable the flash control register access */
   HAL_FLASH_Unlock();

   for( i =0;i < cnt; i++){

     data =BytetoUint32(databuffer+addroffset);
     HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, pageaddr+addroffset, data);
     addroffset +=4;
   }
   
   if(leftbytes > 0){ //写剩余字节
    for(i =0;i <4;i++){
       if(i < leftbytes){
	     buffer[i] = databuffer[addroffset +i];
	   }else {
         buffer[i] = 0xFF;
	   }
	}
    data =BytetoUint32(buffer);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, pageaddr+addroffset, data);
   }
   	
   HAL_FLASH_Lock();
}
/**
 * @brief
 * @note  内部FLASH读数据，按小端格式处理
 * @retval void
 */
void FlashRead(uint32_t pageaddr,uint8_t *databuffer,uint16_t len){
	uint16_t cnt =0,addroffset = 0,i;
	uint8_t leftbytes =0;
	uint32_t data =0;
	
	 cnt = len /4;
	 leftbytes = len % 4;
	
	 /* Unlock the Flash to enable the flash control register access */
	 HAL_FLASH_Unlock();

	 for( i =0;i < cnt; i++){
	   data =*(__IO uint32_t*)(pageaddr+addroffset); 
	   Uint32toByte(data,databuffer +addroffset);
	   addroffset +=4;
	 }

	 if(leftbytes > 0){ //读剩余字节
	   data =*(__IO uint32_t*)(pageaddr+addroffset); 

	   for(i =0;i <leftbytes;i++){
	    databuffer[addroffset+i]=(uint8_t)(data>>(8*i));
	   }
	 }

	 HAL_FLASH_Lock();
}
