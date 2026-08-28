#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Define for DanWei*/
#define Celsius 		0
#define Fahrenheit	1
#define Humidity		2

#define LCD_Celsius					(1<<1)								//COM5
#define LCD_Celsius_REG			LCD_RAM_REGISTER6			
#define LCD_Fahrenheit			(1<<1)								//COM6
#define LCD_Fahrenheit_REG	LCD_RAM_REGISTER4
#define LCD_Humidity				(1<<1)								//COM7
#define LCD_Humidity_REG		LCD_RAM_REGISTER2

#define LCD_Negative_1			(1<<5)								//COM7
#define LCD_Negative_1_REG	LCD_RAM_REGISTER0
#define LCD_Positive_1			(0<<5)								//COM7

#define LCD_Negative_2			(1<<5)								//COM4
#define LCD_Negative_2_REG	LCD_RAM_REGISTER8
#define LCD_Positive_2			(0<<5)								//COM4

#define LCD_Negative_3			(1<<4)								//COM4
#define LCD_Negative_3_REG	LCD_RAM_REGISTER8
#define LCD_Positive_3			(0<<4)								//COM4


/* Define for type*/
#define MAX_display 			0
#define MIN_display 			1
#define AVG_display				2
#define Normal_display  	3

#define LCD_MAX							(1<<1)								//COM2
#define LCD_MAX_REG					LCD_RAM_REGISTER12	
#define LCD_No_MAX					(0<<1)								//COM2
#define LCD_No_MAX_REG			LCD_RAM_REGISTER12	
#define LCD_MIN							(1<<1)								//COM3
#define LCD_MIN_REG					LCD_RAM_REGISTER10
#define LCD_No_MIN					(0<<1)								//COM3
#define LCD_No_MIN_REG			LCD_RAM_REGISTER10
#define LCD_AVG							(1<<1)								//COM4
#define LCD_AVG_REG					LCD_RAM_REGISTER8
#define LCD_No_AVG					(0<<1)								//COM4
#define LCD_No_AVG_REG			LCD_RAM_REGISTER8

/* Define for All_Time_Display*/
#define LCD_REC							(1<<0)								//COM3
#define LCD_REC_REG					LCD_RAM_REGISTER10
#define LCD_Batlev					(1<<6)								//COM8
#define LCD_Batlev_REG			LCD_RAM_REGISTER0
#define LCD_Point						(1<<2)								//COM8
#define LCD_Point_REG				LCD_RAM_REGISTER0

/* Define for Bat.lev day*/
#define LCD_day							(1<<9)								//COM2/4/5/6/7

/* Define for State*/
#define LCD_Good						0
#define LCD_Error_High			1
#define LCD_Error_Low				2
#define LCD_Error_High_Low	3
#define LCD_NoState					4
#define LCD_State_Clear			4

#define LCD_Good_1							(1<<1)								//COM1
#define LCD_Good_1_REG					LCD_RAM_REGISTER14
#define LCD_Good_2							(1<<0)								//COM1
#define LCD_Good_2_REG					LCD_RAM_REGISTER14
#define LCD_Error_1							(1<<0)								//COM2
#define LCD_Error_1_REG					LCD_RAM_REGISTER12
#define LCD_Error_2							(1<<0)								//COM1
#define LCD_Error_2_REG					LCD_RAM_REGISTER14
#define LCD_Error_High_1				(1<<0)								//COM6
#define LCD_Error_High_1_REG		LCD_RAM_REGISTER4
#define LCD_Error_High_2				(1<<0)								//COM5
#define LCD_Error_High_2_REG		LCD_RAM_REGISTER6
#define LCD_Error_Low_1					(1<<0)								//COM8
#define LCD_Error_Low_1_REG			LCD_RAM_REGISTER0
#define LCD_Error_Low_2					(1<<0)								//COM7
#define LCD_Error_Low_2_REG			LCD_RAM_REGISTER2

#define	LCD_T1_No								(0<<0)								//COM2		?????,???
#define	LCD_T1_REG							LCD_RAM_REGISTER12
#define	LCD_T2_No								(0<<0)								//COM1
#define	LCD_T2_REG							LCD_RAM_REGISTER14
#define	LCD_T3_No								(0<<1)								//COM1
#define	LCD_T3_REG							LCD_RAM_REGISTER14
#define	LCD_T7_No								(0<<0)								//COM5
#define	LCD_T7_REG							LCD_RAM_REGISTER6
#define	LCD_T8_No								(0<<0)								//COM7
#define	LCD_T8_REG							LCD_RAM_REGISTER2
#define	LCD_T10_No							(0<<0)								//COM6
#define	LCD_T10_REG							LCD_RAM_REGISTER4
#define	LCD_T11_No							(0<<0)								//COM8
#define	LCD_T11_REG							LCD_RAM_REGISTER0

/* Define for Mark*/
#define MarkDis									0
#define MarkNoDis								1

#define	LCD_MarkDis							(1<<0)								//COM4
#define	LCD_Mark_REG						LCD_RAM_REGISTER8
#define	LCD_MarkNoDis						(0<<0)								//COM4

/* Define for WriteChar*/
#define Clear_Position1					~(1<<5)
#define Clear_Position2					~(1<<4)
#define Clear_Position3					~(1<<3)
#define Clear_Position4					~(1<<2)
#define Clear_Position5					~(1<<6)
#define Clear_Position6					~(1<<7)
#define Clear_Position7					~(1<<8)
#define Clear_Position8					~(1<<9)			//?????‘d’

/* Define for scrolling sentences*/
#define SCROLL_SPEED  	200
#define SCROLL_SPEED_L  400
#define SCROLL_NUM    	10

/* Define for character '.' */
#define POINT_OFF      0
#define POINT_ON       1

/* Define for caracter ":" */
#define COLUMN_OFF     0
#define COLUMN_ON      1

/* Element values correspond to LCD Glass BAR Level. */
#define BARLEVEL_OFF    0
#define BARLEVEL_1_4    1
#define BARLEVEL_1_2    2
#define BARLEVEL_3_4    3
#define BARLEVEL_FULL   4

#define DOT             0x8000 /* for add decimal point in string */
#define DOUBLE_DOT      0x4000 /* for add decimal point in string */

/* Code for '? character */
#define C_UMAP          0x6084

/* Code for 'm' character */
#define C_MMAP          0xB210

/* Code for 'n' character */
#define C_NMAP          0x2210

/* Constant code for '*' character */
#define STAR            0xA0DD

/* Constant code for '-' character */
#define C_MINUS         0xA000

/* Constant code for '+' character */
#define C_PLUS          0xA014

/* Constant code for '/' */
#define C_SLATCH        0x00C0

/* Constant code for ?*/
#define C_PERCENT_1     0xEC00

/* Constant code  for small o */
#define C_PERCENT_2     0xB300

#define C_FULL          0xFFDD

void BSP_LCD_GLASS_Init(void);
//void BSP_LCD_GLASS_WriteChar(uint8_t Position);
void BSP_LCD_GLASS_Clear(void);
void BSP_LCD_GLASS_DisplayString(uint8_t* ptr);
void BSP_LCD_GLASS_DisplayStrDeci(uint16_t* ptr);
void BSP_LCD_GLASS_BarLevelConfig(uint8_t BarLevel);
void BSP_LCD_GLASS_ScrollSentence(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);
void Data_Display_Num(LCD_HandleTypeDef hlcd,int16_t Data);

#define LCD_RH       		  (1<<22)    //COM0
#define LCD_RH_REG  		  LCD_RAM_REGISTER14    //COM0
#define LCD_C       		  (1<<22)    //COM1
#define LCD_C_REG    		  LCD_RAM_REGISTER12    //COM1
#define LCD_CHAR     		  (1<<23)    //COM1
#define LCD_CHAR_REG  		LCD_RAM_REGISTER12    //COM1
#define LCD_POINT    			(1<<19)    //COM0
#define LCD_POINT_REG    	LCD_RAM_REGISTER14   //COM0
#define LCD_H1   		 			(1<<0)     //COM3
#define LCD_H1_REG   		  LCD_RAM_REGISTER8    //COM3
#define LCD_H2       			(1<<0)     //COM2
#define LCD_H2_REG        LCD_RAM_REGISTER10     //COM2
#define LCD_H3       			(1<<0)     //COM0
#define LCD_H3_REG        LCD_RAM_REGISTER14    //COM1
#define LCD_H4       			(1<<0)     //COM1
#define LCD_H4_REG        LCD_RAM_REGISTER12     //COM0

//void MX_LCD_Init(void);
void LCD_Convert_CharToSeg(uint8_t c, uint8_t* digit);
//void BSP_LCD_GLASS_WriteChar(uint8_t Position);
void BSP_LCD_GLASS_WriteChar(LCD_HandleTypeDef hlcd,uint8_t Position,uint8_t* digit);
void Processing_Data(int16_t Data_Process ,uint8_t *Data_Results);

/*         T2?T3????              */
void Data_Display(LCD_HandleTypeDef hlcd, int16_t Data,uint8_t DanWei,uint8_t type );
void All_Time_Display(LCD_HandleTypeDef hlcd);
void State_Display( LCD_HandleTypeDef hlcd,uint8_t state);
void Mark_Display(LCD_HandleTypeDef hlcd, uint8_t MarkState );
void Bat_Display(LCD_HandleTypeDef hlcd,uint16_t Bat_Day);


#endif /* __LCD_H */

/* end of __LCD_H */
