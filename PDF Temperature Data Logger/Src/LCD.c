
	
	/*/-------------------------------------------------------------------
	
	????
				a
		   ----
		 f|	   | b
		g  ----    
		 e| 	 |c
			 ----			
				d
An LCD character coding is based on the following matrix:
      { A, B, F, G, C, E, D   }
     
 The character '0' for example is:
  -------------------------------
      { 1, 1, 1, 0, 1, 1, 1   }

      -------------------
  '0' = 1  1  1  0  1  1  1 
'0'=1110111,'1'=0100100,'2'=1101011,'3'=1101101,'4'=0111100,'5'=1011101,'6'=1011111,'7'=1100100,'8'=1111111,'9'=1111101

			
//-------------------------------------------------------------------*/
#include "LCD.H"

//extern LCD_HandleTypeDef hlcd;
//LCD_HandleTypeDef hlcd;
/* Constant table for number '0' --> '9' */		//0-9?????10?????,??LCD????
/*????16?????????????????,???????LCD?????????,???????LCD_COM
???????????????????????*/
//const uint32_t NumberMap[11]=
//{
//  /* 0       			1     	  2      			 3       4    		   5      		 6   		    7   	    8    	 		  9  			10*/
//  0x1110111, 0x0100100, 0x1101011, 0x1101101, 0x0111100, 0x1011101, 0x1011111, 0x1100100, 0x1111111, 0x1111101, 0x0000
//};


/* LCD init function */
//void MX_LCD_Init(void)
//{

//  hlcd.Instance = LCD;
//  hlcd.Init.Prescaler = LCD_PRESCALER_64;
//  hlcd.Init.Divider = LCD_DIVIDER_19;
//  hlcd.Init.Duty = LCD_DUTY_1_8;
//  hlcd.Init.Bias = LCD_BIAS_1_4;
//  hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
//  hlcd.Init.Contrast = LCD_CONTRASTLEVEL_4;
//  hlcd.Init.DeadTime = LCD_DEADTIME_0;
//  hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_0;
//  hlcd.Init.HighDrive = LCD_HIGHDRIVE_0;
//  hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
//  hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV8;
//  hlcd.Init.MuxSegment = LCD_MUXSEGMENT_DISABLE;
//  if (HAL_LCD_Init(&hlcd) != HAL_OK)
//  {
////    Error_Handler();
//  }

//}
/**
  * @brief  Converts an ascii char to the a LCD digit.
  * @param  c: a char to display.
  * @param  Point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  Column : flag indicating if a column has to be add in front
  *         of displayed character.
  *         This parameter can be: COLUMN_OFF or COLUMN_ON.
  * @param  digit: array with segment 
  * @retval None
  */
void LCD_Convert_CharToSeg(uint8_t c, uint8_t* digit)
{
  uint32_t ch = 0 ;
  uint8_t i, j;
uint32_t NumberMap[11]=
{
  /* 0       			1     	  2      			 3       4    		   5      		 6   		    7   	    8    	 		  9  			10*/
  0x1110111, 0x0100100, 0x1101011, 0x1101101, 0x0111100, 0x1011101, 0x1011111, 0x1100100, 0x1111111, 0x1111101, 0x0000
};
  switch (c)
  {
	case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
	case 10:	
    ch = NumberMap[c];		
    break;
	default:
				break;
	}
	for(i = 24, j = 0; j < 7; i-=4, j++)
  {
    digit[j] = (ch >> i) & 0x0F; //To isolate the less signifiant digit
  }
}
/**
  * @brief  This function writes a char in the LCD frame buffer.
  * @param  ch: the character to display.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  column: flag indicating if a column has to be add in front
  *         of displayed character.
  *         This parameter can be: COLUMN_OFF or COLUMN_ON.           
  * @param  position: position in the LCD of the caracter to write [0:7]
  * @retval None
  * @par    Required preconditions: The LCD should be cleared before to start the
  *         write operation.  
  */


/**
  * @brief  This function writes a char in the LCD frame buffer.
  * @param  ch: the character to display.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  column: flag indicating if a column has to be add in front
  *         of displayed character.
  *         This parameter can be: COLUMN_OFF or COLUMN_ON.           
  * @param  position: position in the LCD of the caracter to write [0:7]
  * @retval None
  * @par    Required preconditions: The LCD should be cleared before to start the
  *         write operation.  
  */
void BSP_LCD_GLASS_WriteChar(LCD_HandleTypeDef hlcd,uint8_t Position,uint8_t* digit)
{    
	int i=0;
  switch(Position)
  {
		case 1:
			for(i=0;i<7;i++)
			{
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0+(14-i*2), Clear_Position1, ( digit[i]<<5 ) );/* 1A */
			}
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14, Clear_Position1, ( digit[0]<<5 ) );/* 1A */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, Clear_Position1, ( digit[1]<<5 ) );/* 1B */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER10, Clear_Position1, ( digit[2]<<5 ) );/* 1F */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, Clear_Position1, ( digit[3]<<5 ) );/* 1G */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, Clear_Position1, ( digit[4]<<5 ) );/* 1C */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, Clear_Position1, ( digit[5]<<5 ) );/* 1E */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, Clear_Position1, ( digit[6]<<5 ) );/* 1D */ 		
		break;
		case 2:
			for(i=0;i<7;i++)
			{
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0+(14-i*2), Clear_Position2, ( digit[i]<<4 ) );/* 2A */
			}
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14, Clear_Position2, ( digit[0]<<4 ) );/* 2A */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, Clear_Position2, ( digit[1]<<4 ) );/* 2B */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER10, Clear_Position2, ( digit[2]<<4 ) );/* 2F */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, Clear_Position2, ( digit[3]<<4 ) );/* 2G */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, Clear_Position2, ( digit[4]<<4 ) );/* 2C */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, Clear_Position2, ( digit[5]<<4 ) );/* 2E */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, Clear_Position2, ( digit[6]<<4 ) );/* 2D */ 
		break;
		case 3:
			for(i=0;i<7;i++)
			{
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0+(14-i*2), Clear_Position3, ( digit[i]<<3 ) );/* 3A */
			}
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14, Clear_Position3, ( digit[0]<<3 ) );/* 3A *///LCD_RAM_REGISTER14 com 8 
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, Clear_Position3, ( digit[1]<<3 ) );/* 3B *///Clear_Position3 --pin4 /
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER10, Clear_Position3, ( digit[2]<<3 ) );/* 3F *///digit[1]<<3--?????1?0
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, Clear_Position3, ( digit[3]<<3 ) );/* 3G */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, Clear_Position3, ( digit[4]<<3 ) );/* 3C */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, Clear_Position3, ( digit[5]<<3 ) );/* 3E */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, Clear_Position3, ( digit[6]<<3 ) );/* 3D */ 
		break;
		case 4:
			for(i=0;i<7;i++)
			{
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0+(14-i*2), Clear_Position4, ( digit[i]<<2 ) );/* 4A */
			}
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14, Clear_Position4, ( digit[0]<<2 ) );/* 4A */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, Clear_Position4, ( digit[1]<<2 ) );/* 4B */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER10, Clear_Position4, ( digit[2]<<2 ) );/* 4F */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, Clear_Position4, ( digit[3]<<2 ) );/* 4G */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, Clear_Position4, ( digit[4]<<2 ) );/* 4C */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, Clear_Position4, ( digit[5]<<2 ) );/* 4E */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, Clear_Position4, ( digit[6]<<2 ) );/* 4D */  
		break;
		case 5:
			for(i=0;i<7;i++)
			{
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0+(14-i*2), Clear_Position5, ( digit[i]<<6 ) );/* 5A */
			}
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14, Clear_Position5, ( digit[0]<<6 ) );/* 5A */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, Clear_Position5, ( digit[1]<<6 ) );/* 5B */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER10, Clear_Position5, ( digit[2]<<6 ) );/* 5F */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, Clear_Position5, ( digit[3]<<6 ) );/* 5G */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, Clear_Position5, ( digit[4]<<6 ) );/* 5C */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, Clear_Position5, ( digit[5]<<6 ) );/* 5E */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, Clear_Position5, ( digit[6]<<6 ) );/* 5D */ 
		break;		
		case 6:	
			for(i=0;i<7;i++)
			{
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0+(14-i*2), Clear_Position6, ( digit[i]<<7 ) );/* 6A */
			}
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14, Clear_Position6, ( digit[0]<<7 ) );/* 6A */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, Clear_Position6, ( digit[1]<<7 ) );/* 6B */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER10, Clear_Position6, ( digit[2]<<7 ) );/* 6F */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, Clear_Position6, ( digit[3]<<7 ) );/* 6G */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, Clear_Position6, ( digit[4]<<7 ) );/* 6C */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, Clear_Position6, ( digit[5]<<7 ) );/* 6E */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, Clear_Position6, ( digit[6]<<7 ) );/* 6D */ 
		break;		
		case 7:	
			for(i=0;i<7;i++)
			{
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0+(14-i*2), Clear_Position7, ( digit[i]<<8 ) );/* 7A */
			}
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14, Clear_Position7, ( digit[0]<<8 ) );/* 7A */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, Clear_Position7, ( digit[1]<<8 ) );/* 7B */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER10, Clear_Position7, ( digit[2]<<8 ) );/* 7F */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, Clear_Position7, ( digit[3]<<8 ) );/* 7G */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, Clear_Position7, ( digit[4]<<8 ) );/* 7C */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, Clear_Position7, ( digit[5]<<8 ) );/* 7E */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, Clear_Position7, ( digit[6]<<8 ) );/* 7D */ 
		break;		
		case 8:	
			for(i=0;i<7;i++)
			{
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0+(14-i*2), Clear_Position8, ( digit[i]<<8 ) );/* 8A */
			}
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14, Clear_Position8, ( digit[0]<<8 ) );/* 8A */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, Clear_Position8, ( digit[1]<<8 ) );/* 8B */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER10, Clear_Position8, ( digit[2]<<8 ) );/* 8F */
//    HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, Clear_Position8, ( digit[3]<<8 ) );/* 8G */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, Clear_Position8, ( digit[4]<<8 ) );/* 8C */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, Clear_Position8, ( digit[5]<<8 ) );/* 8E */ 
//		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, Clear_Position8, ( digit[6]<<8 ) );/* 8D */  
		break;
		default:
			break;
	}
  /* Update the LCD display */
//  HAL_LCD_UpdateDisplayRequest(&hlcd);
}
/*********************????????*************************
???:Processing_Data
??:????,????????????????????
??:Data_Process:???????
			Data_results:??????????
??:
*/
void Processing_Data( int16_t Data_Process ,uint8_t *Data_Results)
{
	//????,????????????Data_Process=-1234,?Data?4?={-1,-2,-3,-4}
	int16_t num=0;
	num	= Data_Process;
	if(Data_Process>0)
	{
		Data_Process = Data_Process;
	}
	else
	{
		Data_Process = 0-Data_Process;
	}
	Data_Results[0] = Data_Process/1000;
	Data_Results[1] = Data_Process%1000/100;
	Data_Results[2] = Data_Process%100/10;
	Data_Results[3] = Data_Process%10;
	Data_Process = num;
}

/*********************????????*************************
???:Data_Display
??:data:??,????????????
			DanWei:??,???Celsius(???),Fahrenheit(???),Humidity(??)
			type:????,???MAX_display?MIN_display?AVG_display?Normal_display
??:!!!data???????????10??????
			?????????Normal,??????????????MAX?????,MIN?????,AVG?????
*/
void Data_Display(LCD_HandleTypeDef hlcd,int16_t Data,uint8_t DanWei,uint8_t type )
{
uint8_t digit[7]={1,1,1,0,1,1,1};							//??????,??????LCD
	uint8_t Data_Resultsl[4]={0};		//???????????????Data_Results?0?????

	/***************??????********************/
	if( DanWei == Celsius )		//???
	{
		HAL_LCD_Write(&hlcd, LCD_Celsius_REG, ~LCD_Celsius, LCD_Celsius );/* ??????? */
		HAL_LCD_Write(&hlcd, LCD_Fahrenheit_REG, ~LCD_Fahrenheit, 0 );				/* ???????? */
		HAL_LCD_Write(&hlcd, LCD_Humidity_REG, ~LCD_Humidity, 0 );				/* ??????? */
	}	
	else	if( DanWei == Fahrenheit )			//???
	{
		HAL_LCD_Write(&hlcd, LCD_Celsius_REG, ~LCD_Celsius, 0 );/* ???????? */
		HAL_LCD_Write(&hlcd, LCD_Fahrenheit_REG, ~LCD_Fahrenheit, LCD_Fahrenheit );				/* ??????? */
		HAL_LCD_Write(&hlcd, LCD_Humidity_REG, ~LCD_Humidity, 0 );				/* ??????? */
	}
//	else	if( DanWei ==2)	//??%RH
//	{
////		HAL_LCD_Write(&hlcd, LCD_Celsius_REG, ~LCD_Celsius, 0 );/* ???????? */
////		HAL_LCD_Write(&hlcd, LCD_Fahrenheit_REG, ~LCD_Fahrenheit, 0 );				/* ???????? */
////		HAL_LCD_Write(&hlcd, LCD_Humidity_REG, ~LCD_Humidity, LCD_Humidity );				/* ?????? */
//	}
//	else	//????????????,??
//	{
////		HAL_LCD_Write(&hlcd, LCD_Celsius_REG, ~LCD_Celsius, 0 );/* ???????? */
////		HAL_LCD_Write(&hlcd, LCD_Fahrenheit_REG, ~LCD_Fahrenheit, 0 );				/* ???????? */
////		HAL_LCD_Write(&hlcd, LCD_Humidity_REG, ~LCD_Humidity, 0 );				/* ??????? */
//	}
	
	/***************??????********************/
	if( type == MAX_display )		
	{
		HAL_LCD_Write(&hlcd, LCD_MAX_REG, ~LCD_MAX, LCD_MAX );				/* MAX???? */
		HAL_LCD_Write(&hlcd, LCD_No_MIN_REG, ~LCD_No_MIN, LCD_No_MIN );				/* MIN????? */
		HAL_LCD_Write(&hlcd, LCD_No_AVG_REG, ~LCD_No_AVG, LCD_No_AVG );				/* AVG????? */
	}	
	else	if( type == MIN_display )		
	{
		HAL_LCD_Write(&hlcd, LCD_No_MAX_REG, ~LCD_No_MAX, LCD_No_MAX );				/* MAX????? */
		HAL_LCD_Write(&hlcd, LCD_MIN_REG, ~LCD_MIN, LCD_MIN );				/* MIN???? */
		HAL_LCD_Write(&hlcd, LCD_No_AVG_REG, ~LCD_No_AVG, LCD_No_AVG );				/* AVG????? */
	}
	else	if( type == AVG_display )		
	{
		HAL_LCD_Write(&hlcd, LCD_No_MAX_REG, ~LCD_No_MAX, LCD_No_MAX );				/* MAX????? */
		HAL_LCD_Write(&hlcd, LCD_No_MIN_REG, ~LCD_No_MIN, LCD_No_MIN );				/* MIN????? */
		HAL_LCD_Write(&hlcd, LCD_AVG_REG, ~LCD_AVG, LCD_AVG );				/* AVG???? */
	}
	else	if( type == Normal_display )	/* ????? */
	{
		HAL_LCD_Write(&hlcd, LCD_No_MAX_REG, ~LCD_No_MAX, LCD_No_MAX );				/* MAX????? */	
		HAL_LCD_Write(&hlcd, LCD_No_MIN_REG, ~LCD_No_MIN, LCD_No_MIN );				/* MIN????? */	
		HAL_LCD_Write(&hlcd, LCD_No_AVG_REG, ~LCD_No_AVG, LCD_No_AVG );				/* AVG????? */		
	}
//	else	//??????,????,????
//	{
////		HAL_LCD_Write(&hlcd, LCD_MAX_REG, ~LCD_MAX, LCD_MAX );				/* MAX???? */
////		HAL_LCD_Write(&hlcd, LCD_MIN_REG, ~LCD_MIN, LCD_MIN );				/* MIN???? */
////		HAL_LCD_Write(&hlcd, LCD_AVG_REG, ~LCD_AVG, LCD_AVG );				/* AVG???? */
//	}
	/**************????**********************/
	Processing_Data(Data,Data_Resultsl);		//????????????????????
	
	/***************????***********************/
	//??????????,??????
	uint8_t ZeroNum;
	uint8_t i;
	for( ZeroNum=0;(ZeroNum<2)&&(0==Data_Resultsl[ZeroNum]);ZeroNum++)	//???????2??
	{	
		for(i=0;i<7;i++)
		{
			digit[i] = 0;
		}
		BSP_LCD_GLASS_WriteChar(hlcd,ZeroNum+1,digit);
	}
	//?????????
	for(i=ZeroNum;i<4;i++)
	{
		LCD_Convert_CharToSeg(Data_Resultsl[i],digit);
		BSP_LCD_GLASS_WriteChar(hlcd,i+1,digit);
	}
/***************¸ºÎÂ¶ÈÖµ´¦Àí********************/			//´ýÓÅ»¯
	//´Ë´¦¸ù¾ÝÊýÖµ´óÐ¡ÅÐ¶¨¸ººÅÎ»ÖÃ¡£Ò²¿ÉÒÔ¸ú¾ÝÇ°ÃæÎÞÐ§Ç°µ¼ÁãÊýÁ¿£¬È·¶¨¸ººÅµÄÎ»ÖÃ
	if( Data<0 )	//ÊýÖµÊÇ¸ºÊý
	{
		Data = 0-Data;
		if( Data>999 )		//ËÄÎ»ÊýÖµ
		{
			HAL_LCD_Write(&hlcd, LCD_Negative_1_REG, ~LCD_Negative_1, LCD_Negative_1 );	/* ÎÂ¶È¸ºÖµ·ûºÅÏÔÊ¾ */
		}
		else if( Data>99 )		//ÈýÎ»ÊýÖµ
		{
//			HAL_LCD_Write(&hlcd, LCD_Negative_1_REG, ~LCD_Negative_1, LCD_Positive_1 );			/* ÎÂ¶È¸ºÖµ·ûºÅ²»ÏÔÊ¾ */
			HAL_LCD_Write(&hlcd, LCD_Negative_2_REG, ~LCD_Negative_2, LCD_Negative_2 );	/* ÎÂ¶È¸ºÖµ·ûºÅÏÔÊ¾ */
		}
		else if( Data>0 )		//Á½Î»ÊýÖµ
		{
//			HAL_LCD_Write(&hlcd, LCD_Negative_1_REG, ~LCD_Negative_1, LCD_Positive_1 );			/* ÎÂ¶È¸ºÖµ·ûºÅ²»ÏÔÊ¾ */
//			HAL_LCD_Write(&hlcd, LCD_Negative_2_REG, ~LCD_Negative_2, LCD_Positive_2 );		/* ÎÂ¶È¸ºÖµ·ûºÅ²»ÏÔÊ¾ */
			HAL_LCD_Write(&hlcd, LCD_Negative_3_REG, ~LCD_Negative_3, LCD_Negative_3 );	/* ÎÂ¶È¸ºÖµ·ûºÅÏÔÊ¾ */
		}

	}
	else					//ÊýÖµÊÇÕýÊý
	{
		//´Ë´¦ÒªÓÃ1<<5,ÎªÁË´¦Àí²»ÏÔÊ¾µÄÇé¿ö£¬Ê¹ÓÃMASK¶¨ÒåÐèÒªµÄÎ»
		HAL_LCD_Write(&hlcd, LCD_Negative_1_REG, ~LCD_Negative_1, LCD_Positive_1 );			/* ÎÂ¶È¸ºÖµ·ûºÅ²»ÏÔÊ¾ */
	}

	

}
/*********************????????*************************
???:Data_Display_Num
*/
void Data_Display_Num(LCD_HandleTypeDef hlcd,int16_t Data)
{
uint8_t digit[7]={1,1,1,0,1,1,1};							//??????,??????LCD
	uint8_t Data_Resultsl[4]={0};		//???????????????Data_Results?0?????
	/**************????**********************/
	Processing_Data(Data,Data_Resultsl);		//????????????????????
	
	/***************????***********************/
	//??????????,??????
	uint8_t ZeroNum;
	uint8_t i;
	for( ZeroNum=0;(ZeroNum<3)&&(0==Data_Resultsl[ZeroNum]);ZeroNum++)	//???????2??
	{	
		for(i=0;i<7;i++)
		{
			digit[i] = 0;
		}
		BSP_LCD_GLASS_WriteChar(hlcd,ZeroNum+1,digit);
	}
	//?????????
	for(i=ZeroNum;i<4;i++)
	{
		LCD_Convert_CharToSeg(Data_Resultsl[i],digit);
		BSP_LCD_GLASS_WriteChar(hlcd,i+1,digit);
	}

}
/*********************??????????**************************/
/*???:All_Time_Display
??:???????????????
??:???REC?Bat.lev?d?P3
*/
void All_Time_Display(LCD_HandleTypeDef hlcd)
{
	int i=0;
	HAL_LCD_Write(&hlcd, LCD_REC_REG, ~LCD_REC, LCD_REC );			//REC??
	HAL_LCD_Write(&hlcd, LCD_Batlev_REG, ~LCD_Batlev, LCD_Batlev );	//????
	HAL_LCD_Write(&hlcd, LCD_Point_REG, ~LCD_Point, LCD_Point );	//???????
	//???????day??“d”
	HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, ~LCD_day, LCD_day );
				for(i=0;i<4;i++)
			{
				
					HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8-i*2, ~LCD_day, LCD_day );
			}
//	HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER12, ~LCD_day, LCD_day );
//	HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER8, ~LCD_day, LCD_day );
//	HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, ~LCD_day, LCD_day );
//	HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, ~LCD_day, LCD_day );
//	HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, ~LCD_day, LCD_day );
}
/*********************????????**************************/
/*???:State_Display
??:??????
??:state:???????
			????:LCD_Good:??????
								LCD_Error_High:?????
								LCD_Error_Low: ?????
								LCD_Error_High_Low:????????
								LCD_NoState:?????
??:?????????,???????
			???????,????????,???????????:
			State_Display( LCD_State_Clear );  //???????
			State_Display( LCD_Good );		//????????
*/
void State_Display(LCD_HandleTypeDef hlcd, uint8_t state)
{
	int i=0;
	switch(state)
	{
		case LCD_Good:
			HAL_LCD_Write(&hlcd, LCD_Good_1_REG, ~LCD_Good_1, LCD_Good_1 );
			HAL_LCD_Write(&hlcd, LCD_Good_2_REG, ~LCD_Good_2, LCD_Good_2 );
			break;
//		case LCD_Error_High:
//			for(i=0;i<2;i++)
//			{
//				HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14-i*2, ~LCD_Error_High_1, LCD_Error_High_1 );
//					
//			}
////			HAL_LCD_Write(&hlcd, LCD_Error_1_REG, ~LCD_Error_1, LCD_Error_1 );
////			HAL_LCD_Write(&hlcd, LCD_Error_2_REG, ~LCD_Error_2, LCD_Error_2 );
//			HAL_LCD_Write(&hlcd, LCD_Error_High_1_REG, ~LCD_Error_High_1, LCD_Error_High_1 );
//			HAL_LCD_Write(&hlcd, LCD_Error_High_2_REG, ~LCD_Error_High_2, LCD_Error_High_2 );
//			break;
//		case LCD_Error_Low:
//			for(i=0;i<2;i++)
//			{
//				HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14-i*2, ~LCD_Error_High_1, LCD_Error_High_1 );
//					
//			}
//			for(i=0;i<2;i++)
//			{
//				HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2-i*2, ~LCD_Error_High_1, LCD_Error_High_1 );
//					
//			}
////			HAL_LCD_Write(&hlcd, LCD_Error_1_REG, ~LCD_Error_1, LCD_Error_1 );
////			HAL_LCD_Write(&hlcd, LCD_Error_2_REG, ~LCD_Error_2, LCD_Error_2 );
////			HAL_LCD_Write(&hlcd, LCD_Error_Low_1_REG, ~LCD_Error_Low_1, LCD_Error_Low_1 );
////			HAL_LCD_Write(&hlcd, LCD_Error_Low_2_REG, ~LCD_Error_Low_2, LCD_Error_Low_2 );
//			break;
		case LCD_Error_High_Low:
				for(i=0;i<4;i++)
			{
				HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6-i*2, ~LCD_Error_High_1, LCD_Error_High_1 );
					
			}
			for(i=0;i<2;i++)
			{
				HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER14-i*2, ~LCD_Error_High_1, LCD_Error_High_1 );
					
			}
//			HAL_LCD_Write(&hlcd, LCD_Error_1_REG, ~LCD_Error_1, LCD_Error_1 );
//			HAL_LCD_Write(&hlcd, LCD_Error_2_REG, ~LCD_Error_2, LCD_Error_2 );
//			HAL_LCD_Write(&hlcd, LCD_Error_High_1_REG, ~LCD_Error_High_1, LCD_Error_High_1 );
//			HAL_LCD_Write(&hlcd, LCD_Error_High_2_REG, ~LCD_Error_High_2, LCD_Error_High_2 );
//			HAL_LCD_Write(&hlcd, LCD_Error_Low_1_REG, ~LCD_Error_Low_1, LCD_Error_Low_1 );
//			HAL_LCD_Write(&hlcd, LCD_Error_Low_2_REG, ~LCD_Error_Low_2, LCD_Error_Low_2 );
			break;
//		case LCD_NoState:		//LCD_NoState = LCD_State_Clear
////			HAL_LCD_Write(&hlcd, LCD_T1_REG, ~LCD_T1_No, LCD_T1_No );
////			HAL_LCD_Write(&hlcd, LCD_T2_REG, ~LCD_T2_No, LCD_T2_No );
////			HAL_LCD_Write(&hlcd, LCD_T3_REG, ~LCD_T3_No, LCD_T3_No );
////			HAL_LCD_Write(&hlcd, LCD_T7_REG, ~LCD_T7_No, LCD_T7_No );
////			HAL_LCD_Write(&hlcd, LCD_T8_REG, ~LCD_T8_No, LCD_T8_No );
////			HAL_LCD_Write(&hlcd, LCD_T10_REG, ~LCD_T10_No, LCD_T10_No );
////			HAL_LCD_Write(&hlcd, LCD_T11_REG, ~LCD_T11_No, LCD_T11_No );
//			break;
		default:
			break;
	}
}

/*********************Mark??????**************************/
/*???:Mark_Display
??:??????MARK??
??:MarkState:?????
			????:MarkDis,????
								MarkNoDis,?????
*/
void Mark_Display(LCD_HandleTypeDef hlcd, uint8_t MarkState )
{
//	if( MarkState==MarkDis )
//	{
		HAL_LCD_Write(&hlcd, LCD_Mark_REG, ~LCD_MarkDis, LCD_MarkDis);	
//	}
//	else if( MarkState==MarkNoDis )
//	{
//		HAL_LCD_Write(&hlcd, LCD_Mark_REG, ~LCD_MarkNoDis, LCD_MarkNoDis);
//	}
}
/*********************??????????**************************/
/*???:Bat_Display
??:??????????
??:Bat_Day:?????			
*/
void Bat_Display(LCD_HandleTypeDef hlcd,uint16_t Bat_Day)
{
uint8_t digit[7]={1,1,1,0,1,1,1};							//??????,??????LCD
	uint8_t Data_Results[3]={0};		//???????????????Data_Results?0?????
	/**************????**********************/
	Data_Results[0] = Bat_Day%1000/100;
	Data_Results[1] = Bat_Day%100/10;
	Data_Results[2] = Bat_Day%10;		//????????????????????
	/***************????***********************/
	//??????????,??????
	uint8_t ZeroNum;
	uint8_t i;
	for( ZeroNum=0;(ZeroNum<3)&&(0==Data_Results[ZeroNum]);ZeroNum++)	//???????2??
	{	
		for(i=0;i<7;i++)
		{
			digit[i] = 0;
		}
		BSP_LCD_GLASS_WriteChar(hlcd,ZeroNum+5,digit);
	}
	//?????????
	for(i=ZeroNum;i<3;i++)
	{
		LCD_Convert_CharToSeg(Data_Results[i],digit);
		BSP_LCD_GLASS_WriteChar(hlcd,i+5,digit);
	}
}


