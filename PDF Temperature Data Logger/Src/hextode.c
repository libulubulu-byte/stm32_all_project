
#include "stdint.h"
#include "hextode.h"
/**
  * @brief  Converts a 2 digit decimal to BCD format.
  * @param  Value: Byte to be converted
  * @retval Converted byte
  */
//uint8_t RTC_ByteToBcd2(uint8_t Value)
//{
//  uint32_t bcdhigh = 0;

//  while(Value >= 10)
//  {
//    bcdhigh++;
//    Value -= 10;
//  }

//  return  ((uint8_t)(bcdhigh << 4) | Value);
//}
uint8_t powe(uint8_t n)  //????16?n?????
{
 uint8_t i;
 uint8_t s=1;
 for(i=1;i<=n;i++)
 {
  s=s*16;
 }
 return s;
}
uint8_t HextoDE(uint8_t data)  //十六进制转十进制
{
    uint8_t n=0;  ////????n??10???
    uint8_t i=0;  //????i????
    uint8_t r;   //????r??16????????

    while(data)
		{
			r=data%10;
			n=n+r*powe(i);
			data=data/10;
			i++;
		}
	return n;

}
uint8_t AscDE(uint8_t tempe_data)
{

return 	 tempe_data=tempe_data-0x30;

}
char ASCfromHex(uint8_t tem_data)
{
		char char_data;
	 switch (tem_data)
				 {
					case 0x41:
									char_data= 'A';              //7day
									break;
					case 0x42:
									char_data= 'B';							//15
									break;
					case 0x43:
									char_data= 'C';							//30
									break;
					case 0x44:
									char_data= 'D';							//60
									break;
					case 0x45:
									char_data= 'E';							//90
									break;
					case 0x46:
									char_data= 'F';							//120
									break;
					default: 
									break;
				 }
 return char_data;
}
