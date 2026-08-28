/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "fatfs_flash_spi.h"
#include "ff.h"
#include "LCD.H"
#include "sht20.h"
#include "pwr.h"
#include "zlib.h"
#include "calendar.h"
#include "button.h"
#include "uart.h"
#include "stm32l0xx_it.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c2;

LCD_HandleTypeDef hlcd;

//UART_HandleTypeDef hlpuart1;

//RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_I2C2_Init(void);
static void MX_LCD_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI2_Init(void);

FIL fsrc;
FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT res_flash; 
UINT br, bw;            					/* File R/W count */
	


/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define SHT20_Delay			HAL_Delay(800)   

				
#define Period               (uint32_t) 65535
/* Set the Timeout value */
#define Timeout              (uint32_t) (32768 - 1)
#define EN_INT          __enable_irq();     //???????  
#define DIS_INT         __disable_irq();    //???????  

   RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
			RTC_HandleTypeDef RtcHandle;//ÓÃÓÚRTC´¦ÀíµÄ½á¹¹Ìå	
						 RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;


/*************DataEEPROM*****************/
#define PEKEY1  0x89ABCDEF      //FLASH_PEKEYR  
#define PEKEY2  0x02030405      //FLASH_PEKEYR 

#define FLASH_PECR_FTDW                      ((uint32_t)0x00000100)        /*!< Fixed Time Data write for Word/Half Word/Byte programming */

/*************          ****************/

/******************DATAEEPROM Adress assigned*********************/
 //  0x08080000 year month data 
 //  0x08080004 hour minutern second 
 //  0x08080008 number of record
 //  0x0808000C barcode  address  
 /****************´®¿ÚÊäÈëÉè¶¨ÐÅÏ¢µÄÊý¾Ý**************************/
 #define Set_YMD_ADDRESS 						0x08080000   	//ÄêÔÂÈÕÊý¾ÝµØÖ·
 #define Set_HMS_ADDRESS 						0x08080004   	//Ð¡Ê± ·ÖÖÓ Ãë Êý¾ÝµØÖ· 
 #define Set_High_Low_ADDRESS 			0x08080008   	//¸ß±¨ µÍ±¨Êý¾ÝµØÖ·
 #define Set_Inter_Del_ADDRESS 			0x0808000C   	//¼ÇÂ¼¼ä¸ô   ÑÓÊ±Êý¾ÝµØÖ·

 #define Barcord_YM_ADDRESS 			0x08080010   	//ÌõÐÎÂëÉè¶¨Ê±¼äÄêÔÂ Êý¾ÝµØÖ·
 #define Barcord_Num_ADDRESS 			0x08080014   	//ÌõÐÎÂëÉè¶¨ ÊýÁ¿ÐÅÏ¢µØÖ·
 #define Barcord_Life_ADDRESS 		0x08080018   	//ÌõÐÎÂëÉè¶¨ ¼ÇÂ¼ÖÜÆÚ¼ÓÊýÁ¿µÄ×îºóÒ»¸ö×Ö½Ú       ÐÅÏ¢µØÖ·
/*******************************************/

/************************************************/
 #define First_Y_M_D_ADDRESS 				0x0808001C
 #define First_H_M_S_ADDRESS 				0x08080020
 #define Number_Record_ADDRESS 			0x08080024
 #define Data_max_ADDRESS 					0x08080028 
 #define Data_min_ADDRESS 					0x0808002C
 #define T_sum_ADDRESS 					    0x08080030
 
  #define K_start_ADDRESS 					0x08080038
	#define K_stop_ADDRESS 					  0x0808003C
	 #define Data_maxsd_ADDRESS 					0x08080040 
 #define Data_minsd_ADDRESS 					0x08080044
  #define RH_sum_ADDRESS 					    0x08080048
/************************************************/

 /*****************DATAEEPROM   Ð´²Ù×÷º¯Êý******************/
void DataEEPROM_Write(uint32_t Address,uint32_t data)
{	
				/* Set the FTDW bit */
	DIS_INT ;
	FLASH->PEKEYR=PEKEY1;                //unlock  
	FLASH->PEKEYR=PEKEY2;  
	while(FLASH->PECR&FLASH_PECR_PELOCK);  
	FLASH->PECR|=FLASH_PECR_FTDW; 
	FLASH->PECR |= (uint32_t)FLASH_PECR_FTDW;
	*(__IO uint32_t *)Address = data;
	FLASH->PECR|=FLASH_PECR_PELOCK; 
	EN_INT;
}
	/*****************¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªEND¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª******************/	
/****************DATAEEPROM ERASE*******************************/
void DataEEPROM_Erase(uint32_t Page_Address)
{
      /* Unlocking the Data memory and FLASH_PECR register access*/
         FLASH->PEKEYR = FLASH_PEKEY1;
         FLASH->PEKEYR = FLASH_PEKEY2;
	  /* Set the ERASE bit */
    FLASH->PECR |= FLASH_PECR_ERASE;

    /* Set PROG bit */
    FLASH->PECR |= FLASH_PECR_PROG;
  
    /* Write 00000000h to the first word of the program page to erase */
	for(uint8_t i;i<30;i++)
	{
    *(__IO uint32_t *)Page_Address = 0x00000000;
		Page_Address +=4;
		
	}
  /* Set the PRGLOCK Bit to lock the program memory access */
  FLASH->PECR |= FLASH_PECR_PRGLOCK;
}
/**********************END*************************/

/*******************¶ÁÈ¡DATAEEPROM Êý¾Ý*************************/
uint32_t DataEEPROM_Read(uint32_t Address)
{	
				uint32_t	Data_temper;
				DIS_INT ;	
				FLASH->ACR |=(uint32_t)FLASH_ACR_PRE_READ;
				FLASH->ACR |=(uint32_t)FLASH_ACR_PRFTEN;
				Data_temper=*(__IO uint32_t *)Address;
				EN_INT;
				return  Data_temper;
}

/* USER CODE END 0 */
extern PCD_HandleTypeDef hpcd_USB_FS;
//extern LPTIM_HandleTypeDef             LptimHandle;

RTC_HandleTypeDef RTCHandle;
int main(void)
{

  /* USER CODE BEGIN 1 */
		uint16_t i=0,KEY_count=0;
	uint8_t USB_PowerOn_State=0;
	uint8_t Button_One_Flag=0,KEY_State=0,Button_Stop_Flag=0;
	uint8_t	Tempe_Data_wendu[256]={0};
//	uint32_t	Data_wendu_Adress=0x00100000,PC_Set_YMD=0,PC_Set_HMS=0, PC_Set_H_L=0,PC_Set_Inter_Del=0,PC_Set_Barcode_YM=0,PC_Set_Barcode_Num=0,PC_Set_Barcode_Life=0;
	uint32_t	Data_Temperat_Adress=0x00100000;
	uint32_t	PC_Set_YMD=0,PC_Set_HMS=0, PC_Set_H_L=0,PC_Set_Inter_Del=0,PC_Set_Barcode_YM=0,PC_Set_Barcode_Num=0,PC_Set_Barcode_Life=0;
	
//	uint32_t First_Y_M_D=0,First_H_M_S=0;
	uint8_t pdf_creat_flag=0;
	int8_t KEY_shidu=1;
	uint8_t Commn_barcode_over=0,Commn_set_over=0,Set_Over_Flag=0;
	uint8_t Set_Interl_time=1,Set_Start_delay=0;  //¼ÇÂ¼¼ä¸ô¶¨Òå  ¿ª»úÑÓÊ±¶¨Òå
//	uint8_t Stop_time_year=0,Stop_time_month=0,Stop_time_day=0,Stop_time_hour=0,Stop_time_min=0,Stop_time_sec=0;
	uint8_t  Set_Interl=0;
	uint8_t Led_flag=0;
uint16_t T_wendu=0,Data_max_wd=0,Data_min_wd=0,RH_shidu=0,Data_max_sd=0,Data_min_sd=0,RH_average=0,T_average=0;
	uint32_t T_sum=0,RH_sum=0,Number_Record=1;
	//IICµÄÊý¾Ý½ÓÊÜ
	uint16_t I2C_date=1;
//	uint8_t i=0;
	
	uint16_t Bat_Day = 900;
	uint32_t  First_Y_M_D=0,First_H_M_S=0;
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
//SystemPower_Config();     //ÎªµÍ¹¦ºÄ×ö×¼±¸  Ö÷ÒªÅäÖÃIO¼°Ê±Ö	
SHT20_Delay	;
SHT20_Delay	;
SHT20_Delay	;
SHT20_Delay	;
//  /* Initialize all configured peripherals */
//  MX_GPIO_Init();
//  MX_ADC_Init();
//  MX_I2C2_Init();
//  MX_LCD_Init();
////  MX_LPUART1_UART_Init();
////  MX_RTC_Init();
//  MX_SPI2_Init();
////	RTC_DateTypeDef sdatestructureget;
////RTC_TimeTypeDef stimestructureget;
//	 LPUART1_Init();
////	  Button_One_Flag=DataEEPROM_Read(K_start_ADDRESS);              //ÊÇ·ñÒÑ¾­¿ª¹ý¼ÇÂ¼ÒÇ£¿



//#if 1
//	BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_GPIO);
//	USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
//	
//	if(USB_PowerOn_State==1)              															//Èç¹ûµç³ØµçÁ¿²»×ãÒÔÖ§³ÅÏµÍ³¹¤×÷£¬µ¥¶À²åÉÏUSBÊ±¿ÉÒÔ¶Á³öÊý¾Ý
//		{
//			HAL_Delay(20);
//		USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);			
//			if(USB_PowerOn_State==1)         //
//				{

//					if(Button_One_Flag!=0)
//					{
////						PDF_Creat();
//					}

//					

//				}	

//				}
//                                
//	MX_USB_DEVICE_Init();
//	HAL_Delay(500);	
//	BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_GPIO);
//	USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
//	if(USB_PowerOn_State==1)              //Èç¹ûµç³ØµçÁ¿²»×ãÒÔÖ§³ÅÏµÍ³¹¤×÷£¬µ¥¶À²åÉÏUSBÊ±¿ÉÒÔ¶Á³öÊý¾Ý
//	{
//		HAL_Delay(20);          
//		if(USB_PowerOn_State==1)         //
//		{
//			do
//			{	
//				USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
//			}
//			while(USB_PowerOn_State==1) ;
//	
//	 }

//	}
//#endif	
//All_Time_Display();
//	KEY_count=800;
//						while(KEY_count--)
//						{
//						HAL_LCD_UpdateDisplayRequest(&hlcd);
//						}
//Key_Start:	
//		if((Button_One_Flag==0)&&(Set_Over_Flag==0))
//		{		
//			
////			SystemPower_Config();     //ÎªµÍ¹¦ºÄ×ö×¼±¸  Ö÷ÒªÅäÖÃIO¼°Ê±Ö
//			BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
//										 		/**USB²åÈë¼ì²â**/
//			BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_EXTI);
////			/* Enter Stop Mode */
////			HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
//			USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
//			if(USB_PowerOn_State==1)													//ÔÚ²éÑ¯¹ýÒ»´Îºó USB½ÓÉÏPC will restart
//				{
//					USB_PowerOn_State=0;
//					HAL_NVIC_SystemReset();                                          //ÔÚÕý³£ÔËÐÐ×´Ì¬ÏÂ USB½ÓÉÏPC will restart
//				}
//	
//			do
//			{	
//				HAL_Delay(10);
//        KEY_count++;
//				
//				KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
//				if(KEY_count>500)
//					{
//						break;
//					}
//			}while(KEY_State==0);
//				if(KEY_count>500)
//					{
//						 All_Time_Display();
//						while(KEY_count--)
//						{ HAL_LCD_UpdateDisplayRequest(&hlcd);
//						}
//					
//							KEY_count=0;
//							Button_One_Flag=1;				
//					}
//				else
//					{
//						KEY_count=0;
//						goto Key_Start;
//					}

//				LPUART1_Init();	      /* (²âÊÔ×¢ÊÍ)*/
//				
//		if(Set_Over_Flag==0)
//			{


//				do{	
//						BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_GPIO);
//						USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
//						if(USB_PowerOn_State==1)              //Èç¹ûµç³ØµçÁ¿²»×ãÒÔÖ§³ÅÏµÍ³¹¤×÷£¬µ¥¶À²åÉÏUSBÊ±¿ÉÒÔ¶Á³öÊý¾Ý
//						{
//							HAL_Delay(20); 
//							USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
//							if(USB_PowerOn_State==1)         //
//								{
//									HAL_NVIC_SystemReset(); 
//								}
//						}
//					
//						if(Comm_OK_flag==1)      					//½¨Á¢Í¨Ñ¶Á¬½Ó³É¹¦
//						 {
//							 Comm_OK_flag=0;
//							 Rece_count=0;
//						
//							 for(i=0;i<6;i++)
//								 {
//									 LPUART1_SendByte(Send_data[i]);
//								 }
//							for(i=0;i<18;i++)
//								{
//								 Rec_data[i]=0;
//								 Send_data[i]=0;
//								}
//						}
//					 if(Write_set_flag==1)            //Ê±¼äÐÅÏ¢Éè¶¨Ð´Èë   
//						 {
//							 Write_set_flag=0;
//							 Rece_count=0;
//							 
//							 PC_Set_YMD=((uint32_t)Send_data[2]<<16)+((uint32_t)Send_data[3]<<8)+((uint32_t)Send_data[4]<<0);// Äê ÔÂ ÈÕ
//							 PC_Set_HMS=((uint32_t)Send_data[5]<<16)+((uint32_t)Send_data[6]<<8)+((uint32_t)Send_data[7]<<0);// Ð¡Ê± ·ÖÖÓ Ãë
//							 PC_Set_H_L=((uint32_t)Send_data[8]<<24)+((uint32_t)Send_data[9]<<16)+((uint32_t)Send_data[10]<<8)+((uint32_t)Send_data[11]<<0); //¸ß±¨  µÍ±¨ÉèÖÃ
//							 PC_Set_Inter_Del=((uint32_t)Send_data[12]<<8)+((uint32_t)Send_data[13]<<0);// ¼ÇÂ¼¼ä¸ô ¿ª»úÑÓÊ±Êý¾Ý
//							 DataEEPROM_Write(Set_YMD_ADDRESS,PC_Set_YMD);                               //½«»ñµÃµÄÊ±¼ä¡¢¸ß±¨¡¢µÍ±¨µÈÐÅÏ¢Ð´ÈëDATA EEPROM
//							 DataEEPROM_Write(Set_HMS_ADDRESS,PC_Set_HMS);
//							 DataEEPROM_Write(Set_High_Low_ADDRESS,PC_Set_H_L);
//							 DataEEPROM_Write(Set_Inter_Del_ADDRESS,PC_Set_Inter_Del);
//							 /************¶ÁÈ¡Êý¾Ý******************/                   //²âÊÔÓÃ ²âÊÔ¶Á³öµÄÊý¾ÝÓëÐ´ÈëµÄÊý¾ÝÊÇ·ñÒ»ÖÂ£¿²âÊÔOK
////							 PC_Set_YMD=DataEEPROM_Read(Set_YMD_ADDRESS);
////								UART1_SendByte(PC_Set_YMD>>16);
////								UART1_SendByte(PC_Set_YMD>>8);
////							  UART1_SendByte(PC_Set_YMD);
//							 /***************************************/
//							 
//								PC_Set_Inter_Del=DataEEPROM_Read(Set_Inter_Del_ADDRESS); 
//								Set_Interl_time=(PC_Set_Inter_Del>>8);     										//  ¼ÇÂ¼¼ä¸ô 
//								Set_Start_delay=(PC_Set_Inter_Del>>0);												// ¿ª»úÑÓÊ±
//							 

//								YMD_RTC_Y=RTC_ByteToBcd2(Send_data[2]);   

//								
//								YMD_RTC_M=RTC_ByteToBcd2(Send_data[3]);

//								
//								YMD_RTC_D=RTC_ByteToBcd2(Send_data[4]);


//								HMS_RTC_H=RTC_ByteToBcd2(Send_data[5]);
//		

//								HMS_RTC_M=RTC_ByteToBcd2(Send_data[6]);
//					

//								HMS_RTC_S=RTC_ByteToBcd2(Send_data[7]);
//						
//								
//								
//								
//							 for(i=0;i<18;i++)
//								 {
//									 LPUART1_SendByte(Send_data[i]);
//								 }
//				 
//							for(i=0;i<18;i++)
//								{
//									 Rec_data[i]=0;
//									 Send_data[i]=0;
//								}
//				
//							Commn_set_over=1;
//						 }
//					if(Write_barcode_flag==1)         //ÌõÐÎÂëÐÅÏ¢Ð´Èë
//						{
//							Write_barcode_flag=0;
//							Rece_count=0;
//							PC_Set_Barcode_YM=((uint32_t)Send_data[2]<<8)+((uint32_t)Send_data[3]<<0);
//							PC_Set_Barcode_Num=((uint32_t)Send_data[4]<<24)+((uint32_t)Send_data[5]<<16)+((uint32_t)Send_data[6]<<8)+((uint32_t)Send_data[7]<<0);
//							PC_Set_Barcode_Life=((uint32_t)Send_data[8]<<8)+((uint32_t)Send_data[9]<<0);
//							/*******************Ð´ÈëÊý¾Ý******************************/
//								DataEEPROM_Write(Barcord_YM_ADDRESS,PC_Set_Barcode_YM);                               //½«»ñµÃµÄÊ±¼ä¡¢¸ß±¨¡¢µÍ±¨µÈÐÅÏ¢Ð´ÈëDATA EEPROM
//								DataEEPROM_Write(Barcord_Num_ADDRESS,PC_Set_Barcode_Num);
//								DataEEPROM_Write(Barcord_Life_ADDRESS,PC_Set_Barcode_Life);
//							/********************½áÊø******************************/

//							for(i=0;i<14;i++)
//								 {
//									 LPUART1_SendByte(Send_data[i]);
//								 }
//							for(i=0;i<18;i++)
//								 {
//									 Rec_data[i]=0;
//									 Send_data[i]=0;
//								 }
//					
//							Commn_barcode_over=1;
//						}
//				}while((Commn_barcode_over==0)||(Commn_set_over==0));

//				
//				if((Commn_barcode_over==1)&&(Commn_set_over==1))
//					{
//						Set_Over_Flag=1;
//						Button_One_Flag=1;
//						HAL_LCD_Clear(&hlcd);	
//		      	DataEEPROM_Write(K_start_ADDRESS,Set_Over_Flag);											//¼ÇÂ¼´ÎÊýÐ´Èë   ÉèÖÃ¼ÇÂ¼Íê³É±êÖ¾Î»
//					}
					/************  ****RTC ÉèÖÃ**   *************/
						
	

//  /* Ê¹ÄÜLSEÊ±ÖÓ */  
//  LSE_ClockEnable();

//  /* ÅäÖÃRTCÊ±ÖÓÔ´ */  
//  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
//  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;//Ê¹ÓÃLSE×÷ÎªRTCÊ±ÖÓÔ´  
//  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//  { 
//  }
//  
//    /* Ê¹ÄÜRTCÊ±ÖÓ */  
//  __HAL_RCC_RTC_ENABLE(); 
//  
//  /* ÅäÖÃRTC»ù±¾²ÎÊý */  
//  RtcHandle.Instance = RTC; 
//  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;//Ð¡Ê±¸ñÊ½
//  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;//Òì²½Ô¤·ÖÆµ
//  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;//Í¬²½Ô¤·ÖÆµ
//  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;//RTCÊä³ö£º²»Êä³ö
//  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;//Êä³ö¼«ÐÔ
//  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;//Êä³öÄ£Ê½£º¿ªÂ©  
//  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
//  {
//    /* Error */
//  }
//   
//  /* Í¨¹ý±¸·Ý¼Ä´æÆ÷ÀïµÄÓÃ»§×Ô¶¨Òå±ê¼Ç¼ì²âRTCÊÇ·ñÒÑ¾­ÅäÖÃ¹ýÊ±ÖÓ */  
//  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != 0x32F2)//Î´ÅäÖÃÊ±ÖÓ
//  {  
//									/* Configure RTC Calendar */


//								/* Set Date: Monday April 14th 2014 */
//								sdatestructure.Year = 0x16;
//								//sdatestructure.Month = RTC_MONTH_JANUARY;
//								 sdatestructure.Month = 0x07;
//								sdatestructure.Date = 0x09;
//								sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
//								if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,FORMAT_BCD) != HAL_OK)
//								{
//									/* Error */
//								} 
//								
//								/* Set Time: 02:00:00 */
//								stimestructure.Hours = 0x15;
//								stimestructure.Minutes = 0x03;
//								stimestructure.Seconds = 0x50;
//								stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
//								stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
//								stimestructure.StoreOperation = RTC_STOREOPERATION_RESET; 
//								if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,FORMAT_BCD) != HAL_OK)
//								{
//									/* Error */
//								}
//								
//								/* Ð´ÅäÖÃ±ê¼Çµ½RTC±¸·Ý¼Ä´æÆ÷0 */
//								HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2); 
//  }
									
								/* Configure RTC Calendar */
							
							
							
							

//								RTC_DateTypeDef sdatestructure;
//								RTC_TimeTypeDef stimestructure;

//////								/*##-1- Configure the Date #################################################*/
//							
////							First_Y_M_D=DataEEPROM_Read(First_Y_M_D_ADDRESS);				//¶ÁÈ¡ÄêÔÂÈÕÐÅÏ¢  µÚÒ»ÌõÊý¾Ý
////			
////				sdatestructure.Year=(uint8_t)(First_Y_M_D>>16);															//Ç¿ÖÆ×ª»¯

////				
////				sdatestructure.Month=(uint8_t)(First_Y_M_D>>8);

////				
////				sdatestructure.Date=(uint8_t)First_Y_M_D;

//				
////				First_H_M_S=DataEEPROM_Read(First_H_M_S_ADDRESS);				//¶ÁÈ¡Ð¡Ê± ·ÖÖÓ ÃëÐÅÏ¢  µÚÒ»ÌõÊý¾Ý
////				
////				stimestructure.Hours=(uint8_t)(First_H_M_S>>16);
////	
////				stimestructure.Minutes=(uint8_t)(First_H_M_S>>8);

////				stimestructure.Seconds=(uint8_t)First_H_M_S;
//							

////								sdatestructure.Year = YMD_RTC_Y;
////								sdatestructure.Month = YMD_RTC_M;
////								sdatestructure.Date = YMD_RTC_D;

//											sdatestructure.Year = 0x16;
//								sdatestructure.Month = 0x09;
//								sdatestructure.Date = 0x18;
//	
//							
//								if(HAL_RTC_SetDate(&RTCHandle,&sdatestructure,FORMAT_BCD) != HAL_OK)
//								{
//									/* Error */
//								//  Error_Handler(); 
//								} 
//  
////								stimestructure.Hours = HMS_RTC_H;
////								stimestructure.Minutes = HMS_RTC_M;
////								stimestructure.Seconds = HMS_RTC_S;
//											stimestructure.Hours = 0x16;
//								stimestructure.Minutes = 0x58;
//								stimestructure.Seconds = 0x01;
//							
//								stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
//								stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//								stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
//  
//								if(HAL_RTC_SetTime(&RTCHandle,&stimestructure,FORMAT_BCD) != HAL_OK)
//								{
//									/* Error */
//								//  Error_Handler(); 
//								}
//								
////							/*##-3- Writes a data in a RTC Backup data Register0 #######################*/
//							HAL_RTCEx_BKUPWrite(&RTCHandle,RTC_BKP_DR0,0x32F2);  
//													/********************²âÊÔÐ´ÈëµÄÊý¾ÝÊÇ·ñÕýÈ·************************/
//			
//						/************* END *****************/
//			
//	
//		}
//		
//	}

//	Key_Start_1:
//	//if((Button_One_Flag==1)&&(Set_Over_Flag==1))
//	if(Button_One_Flag==1)
//		{
//			
//		Button_Stop_Flag=DataEEPROM_Read(K_stop_ADDRESS);              //ÊÇ·ñ¹Ø¹ý¼ÇÂ¼ÒÇ	

////			SystemPower_Config();     //ÎªµÍ¹¦ºÄ×ö×¼±¸  Ö÷ÒªÅäÖÃIO¼°Ê±ÖÓ

//			if(Button_Stop_Flag!=2)  				//¹Ø»úºó  ²»ÔÙÖ´ÐÐ°´¼ü²Ù×÷
//			{
//				BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
//			}
//	//		BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

//			/**USB²åÈë¼ì²â**/
//			BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_EXTI);

////			/* Enter Stop Mode */
////			HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);       
//USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
//			if(USB_PowerOn_State==1)
//				{
//						
//					if(Button_Stop_Flag==2)
//						HAL_NVIC_SystemReset();                                          //ÔÚÕý³£ÔËÐÐ×´Ì¬ÏÂ USB½ÓÉÏPC will restart
//				}	
//				
//			KEY_count=0;  
//			do
//			{		
//				HAL_Delay(10);
//        KEY_count++;
//				KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
//				if(KEY_count>500)
//				{
//					break;
//				}
//			}while(KEY_State==0);
//				
//				if(KEY_count>500)
//					{
//						
//					
//					}
//				else
//					{
//						KEY_count=0;
//						goto Key_Start_1;
//					}

//					

//	
	
	
		SystemPower_Config();     //ÎªµÍ¹¦ºÄ×ö×¼±¸  Ö÷ÒªÅäÖÃIO¼°Ê±Ö	
		 MX_GPIO_Init();
		MX_I2C2_Init();
		MX_LCD_Init();
//		 MX_SPI2_Init();
 /* Ê¹ÄÜLSEÊ±ÖÓ */  
  LSE_ClockEnable();

  /* ÅäÖÃRTCÊ±ÖÓÔ´ */  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;//Ê¹ÓÃLSE×÷ÎªRTCÊ±ÖÓÔ´  
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
  }
  
    /* Ê¹ÄÜRTCÊ±ÖÓ */  
  __HAL_RCC_RTC_ENABLE(); 
  
  /* ÅäÖÃRTC»ù±¾²ÎÊý */  
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;//Ð¡Ê±¸ñÊ½
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;//Òì²½Ô¤·ÖÆµ
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;//Í¬²½Ô¤·ÖÆµ
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;//RTCÊä³ö£º²»Êä³ö
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;//Êä³ö¼«ÐÔ
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;//Êä³öÄ£Ê½£º¿ªÂ©  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Error */
  }
   
  /* Í¨¹ý±¸·Ý¼Ä´æÆ÷ÀïµÄÓÃ»§×Ô¶¨Òå±ê¼Ç¼ì²âRTCÊÇ·ñÒÑ¾­ÅäÖÃ¹ýÊ±ÖÓ */  
  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != 0x32F2)//Î´ÅäÖÃÊ±ÖÓ
  {  
									/* Configure RTC Calendar */


								/* Set Date: Monday April 14th 2014 */
								sdatestructure.Year = 0x16;
								//sdatestructure.Month = RTC_MONTH_JANUARY;
								 sdatestructure.Month = 0x07;
								sdatestructure.Date = 0x09;
								sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
								if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,FORMAT_BCD) != HAL_OK)
								{
									/* Error */
								} 
								
								/* Set Time: 02:00:00 */
								stimestructure.Hours = 0x15;
								stimestructure.Minutes = 0x03;
								stimestructure.Seconds = 0x50;
								stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
								stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
								stimestructure.StoreOperation = RTC_STOREOPERATION_RESET; 
								if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,FORMAT_BCD) != HAL_OK)
								{
									/* Error */
								}
								
								/* Ð´ÅäÖÃ±ê¼Çµ½RTC±¸·Ý¼Ä´æÆ÷0 */
								HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2); 
  }												//RTC³õÊ¼»¯
////	 LPUART1_Init();
		HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);		//ÏÈ¹Ø±Õ»½ÐÑ	
//		HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x2616, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
		HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x9CE4, RTC_WAKEUPCLOCK_RTCCLK_DIV8);
  HAL_NVIC_SetPriority(RTC_IRQn, 0x0, 0);//ÉèÖÃÖÐ¶ÏÓÅÏÈ¼¶
  HAL_NVIC_EnableIRQ(RTC_IRQn);//Ê¹ÄÜRTCÖÐ¶Ï

//  MX_LCD_Init();
	//SHT-20Èí¼þ¸´Î»¡£ÊµÑéÖ¤Ã÷£¬µÚÒ»¶ÁÈ¡Êý¾Ý²»×¼¡£´Ë´¦ÏÈ¶ÁÈ¡Ò»´Î
	SHT2x_SoftReset();
	SHT20_Delay;								//Á½´Î¶ÁÈ¡Êý¾ÝÖ®¼ä²»ÄÜÌ«½ü¡£´Ë´¦ÒªÓÐÑÓÊ±¡£
	SHT2x_MeasureTempHM();
	SHT20_Delay;
	SHT2x_MeasureHumiHM();	
	SHT20_Delay;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


			I2C_date = (uint16_t)( 10*SHT2x_MeasureTempHM() );
			SHT20_Delay;
			I2C_date = (uint16_t)( 10*SHT2x_MeasureTempHM() );			
			SHT20_Delay;
			Data_Display(I2C_date,Celsius,Normal_display);		//ÏÔÊ¾Êý¾Ý£¬ÉãÊÏ¶È±êÖ¾£¬²»ÏÔÊ¾MACXµÈ·ûºÅ
Data_max_wd=I2C_date;
Data_min_wd=I2C_date;
T_sum=T_sum+I2C_date;
	I2C_date =(uint16_t)( 10*SHT2x_MeasureHumiHM() );
			SHT20_Delay;
			I2C_date =(uint16_t)( 10*SHT2x_MeasureHumiHM() );			
			SHT20_Delay;
			Data_Display(I2C_date,Humidity,Normal_display);
Data_max_sd=I2C_date;
Data_min_sd=I2C_date;
RH_sum=RH_sum+I2C_date;
Data_Temperat_Adress=0x00100000;
  while (1)
  {	
////		HAL_RTC_GetDate(&RtcHandle, &sdatestructure, FORMAT_BIN);
//		HAL_RTC_GetTime(&RtcHandle, &stimestructure, FORMAT_BIN);
//			SystemPower_Config();     //ÎªµÍ¹¦ºÄ×ö×¼±¸  Ö÷ÒªÅäÖÃIO¼°Ê±Ö

//		HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);
//		HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 0x9CE4, RTC_WAKEUPCLOCK_RTCCLK_DIV8);//8/£¨~32.768KHz£©=~0.244ms  WakeUpCounter = ~(10s-200ms)/0,244ms = 40164 = 0x9CE4         (10S-200ms)¶¨Ê±
////							  Set_Interl=1*6;    //¼ÇÂ¼¼ä¸ôÎª Set_Interl=3*2*30/60=3  ±	
//		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_EXTI);
			if(Button_Stop_Flag!=2)  				//¹Ø»úºó  ²»ÔÙÖ´ÐÐ°´¼ü²Ù×÷
			{
					BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
				BSP_STOP_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
			}
		KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
if(Button_Stop_Flag!=2)//Î´¹Ø»úÖ´ÐÐÈçÏÂ³ÌÐò
{
			
//	MX_I2C2_Init();
//		 MX_LCD_Init();
//LPUART1_Init();
//		LPUART1_SendByte(stimestructure.Seconds);			
	

		
									Number_Record++;
									//			//ÊµÑé¼ì²â£º»ñÈ¡Êª¶È»áÓ°Ïì»ñÈ¡ÎÂ¶È¡£¹Ê»ñÈ¡Á½´Î¡£
									I2C_date = (uint16_t)( 10*SHT2x_MeasureTempHM() );
									SHT20_Delay;
									I2C_date = (uint16_t)( 10*SHT2x_MeasureTempHM() );			
									SHT20_Delay;
									T_wendu=I2C_date;
									if(T_wendu>Data_max_wd)
									Data_max_wd=T_wendu;
									if(T_wendu<Data_min_wd)
									Data_min_wd=T_wendu;
									T_sum=T_sum+I2C_date;
									T_average=T_sum/Number_Record;
									//Îª·ÀÖ¹ÎÂ¶È»ñÈ¡Ó°ÏìÊª¶ÈµÄÖµ£¬´Ë´¦Ò²»ñÈ¡Á½´Î¡£
									I2C_date =(uint16_t)( 10*SHT2x_MeasureHumiHM() );
									SHT20_Delay;
									I2C_date =(uint16_t)( 10*SHT2x_MeasureHumiHM() );			
									SHT20_Delay;

									RH_shidu=I2C_date;
									if(RH_shidu>Data_max_sd)
									Data_max_sd=RH_shidu;
									if(RH_shidu<Data_min_sd)
									Data_min_sd=RH_shidu;

									RH_sum=RH_sum+I2C_date;
									RH_average=RH_sum/Number_Record;


									Tempe_Data_wendu[0]=T_wendu; 
									Tempe_Data_wendu[1]=(T_wendu>>8);
									Tempe_Data_wendu[2]=RH_shidu; 
									Tempe_Data_wendu[3]=(RH_shidu>>8);

									MX_GPIO_Init();
									MX_SPI2_Init();
									SPI_FLASH_PageWrite(Tempe_Data_wendu,Data_Temperat_Adress,4);

									Data_Temperat_Adress+=4;

	if(i==0)
		{
				i = 1;
				All_Time_Display();
				Bat_Display(Bat_Day);
				Data_Display(T_wendu,Celsius,Normal_display);		//ÏÔÊ¾Êý¾Ý£¬ÉãÊÏ¶È±êÖ¾£¬²»ÏÔÊ¾MACXµÈ·ûºÅ
				HAL_LCD_UpdateDisplayRequest(&hlcd);
			}

		else
		{
			i = 0;
			All_Time_Display();
			Bat_Display(Bat_Day);
			Data_Display(RH_shidu,Humidity,Normal_display);
			HAL_LCD_UpdateDisplayRequest(&hlcd);

		}


 
	
			if(KEY_State==0)
			{
				KEY_shidu=-1*KEY_shidu;
				if(KEY_shidu>0)
				{
				All_Time_Display();	Bat_Display(Bat_Day);
				Data_Display(Data_max_wd,Celsius, MAX_display);
				HAL_LCD_UpdateDisplayRequest(&hlcd);
				HAL_Delay(1500);
				HAL_LCD_Clear(&hlcd);	
				All_Time_Display();	Bat_Display(Bat_Day);
				Data_Display(Data_min_wd,Celsius,MIN_display);
				HAL_LCD_UpdateDisplayRequest(&hlcd);
				HAL_Delay(1500);
				HAL_LCD_Clear(&hlcd);	
				All_Time_Display();	Bat_Display(Bat_Day);
				Data_Display(T_average,Celsius,AVG_display);
				HAL_LCD_UpdateDisplayRequest(&hlcd);			
				HAL_Delay(1500);
				HAL_LCD_Clear(&hlcd);	
				}else
				{
				All_Time_Display();	Bat_Display(Bat_Day);
				Data_Display(Data_max_sd,Humidity,MAX_display);
				HAL_LCD_UpdateDisplayRequest(&hlcd);
				HAL_Delay(1500);
				HAL_LCD_Clear(&hlcd);	
				All_Time_Display();	Bat_Display(Bat_Day);
				Data_Display(Data_min_sd,Humidity,MIN_display);
				HAL_LCD_UpdateDisplayRequest(&hlcd);		
				HAL_Delay(1500);
				HAL_LCD_Clear(&hlcd);	
				All_Time_Display();	Bat_Display(Bat_Day);
				Data_Display(RH_average,Humidity,AVG_display);
				HAL_LCD_UpdateDisplayRequest(&hlcd);
				HAL_Delay(1500);
				HAL_LCD_Clear(&hlcd);
				}
			}




		KEY_count=0;
			KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
			if((Button_One_Flag==1)&&(KEY_State==0))
			{
				do
					{	
						HAL_Delay(10);
						KEY_count++;
						KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
						if(KEY_count>500)
						{
							break;
						}
					}while(KEY_State==0);
			
				if(KEY_count>500)
					{
					
						KEY_count=0;
						Button_One_Flag=2;
						HAL_LCD_Clear(&hlcd);	
						DataEEPROM_Write(K_stop_ADDRESS,Button_One_Flag);	
						
					}
				else
					{
						KEY_count=0;
					}
		}
		



				if((Button_One_Flag==2)&&(pdf_creat_flag==0))                  //Èç¹û¹Ø»ú°´¼ü°´ÏÂ  ¼´½«Êý¾ÝºÍ¼ÇÂ¼´ÎÊý¼ÇÂ¼ÏÂÀ´
					{
						pdf_creat_flag=1;
						DataEEPROM_Write(Number_Record_ADDRESS,Number_Record);       //¼ÇÂ¼ÌõÊý
						DataEEPROM_Write(Data_max_ADDRESS,Data_max_wd);
						DataEEPROM_Write(Data_min_ADDRESS,Data_min_wd);
						DataEEPROM_Write(T_sum_ADDRESS,T_sum);                    //¼ÇÂ¼ºÍÖµ
				
						DataEEPROM_Write(Data_maxsd_ADDRESS,Data_max_sd);
						DataEEPROM_Write(Data_minsd_ADDRESS,Data_min_sd);
						DataEEPROM_Write(RH_sum_ADDRESS,RH_sum);                    //¼ÇÂ¼ºÍÖµ

					}

						if(Usb_Power_On_Flag==1)
									{
										Usb_Power_On_Flag=0;
										if(Button_One_Flag==2)
											HAL_NVIC_SystemReset();                                          //ÔÚÕý³£ÔËÐÐ×´Ì¬ÏÂ USB½ÓÉÏPC will restart
								 }
							
							}
  /* USER CODE END 3 */

}
	
}


/** System Clock Configuration
*/
void SystemClock_Config(void)
{

 RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC init function */
static void MX_ADC_Init(void)
{

//	ADC_HandleTypeDef hadc;
	
  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_12;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/* I2C2 init function */
static void MX_I2C2_Init(void)
{
	
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00303D5B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

}

/* LCD init function */
static void MX_LCD_Init(void)
{

  hlcd.Instance = LCD;
  hlcd.Init.Prescaler = LCD_PRESCALER_2;
  hlcd.Init.Divider = LCD_DIVIDER_19;
  hlcd.Init.Duty = LCD_DUTY_1_8;
  hlcd.Init.Bias = LCD_BIAS_1_4;
  hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
  hlcd.Init.Contrast = LCD_CONTRASTLEVEL_3;
  hlcd.Init.DeadTime = LCD_DEADTIME_0;
  hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_1;
  hlcd.Init.HighDrive = LCD_HIGHDRIVE_1;
  hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
  hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV8;
  hlcd.Init.MuxSegment = LCD_MUXSEGMENT_DISABLE;
  if (HAL_LCD_Init(&hlcd) != HAL_OK)
  {
    Error_Handler();
  }

    /**Enable the High Driver 
    */
  __HAL_LCD_HIGHDRIVER_ENABLE(&hlcd);

}

///* LPUART1 init function */
//static void MX_LPUART1_UART_Init(void)
//{
//UART_HandleTypeDef hlpuart1;
//  hlpuart1.Instance = LPUART1;
//  hlpuart1.Init.BaudRate = 9600;
//  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
//  hlpuart1.Init.StopBits = UART_STOPBITS_1;
//  hlpuart1.Init.Parity = UART_PARITY_NONE;
//  hlpuart1.Init.Mode = UART_MODE_TX_RX;
//  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
//  {
//    Error_Handler();
//  }

//}

///* RTC init function */
////static void MX_RTC_Init(void)
////{
////RTC_HandleTypeDef hrtc;
////  RTC_TimeTypeDef sTime;
////  RTC_DateTypeDef sDate;

////    /**Initialize RTC and set the Time and Date 
////    */
////  hrtc.Instance = RTC;
////  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
////  hrtc.Init.AsynchPrediv = 127;
////  hrtc.Init.SynchPrediv = 255;
////  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
////  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
////  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
////  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
////  if (HAL_RTC_Init(&hrtc) != HAL_OK)
////  {
////    Error_Handler();
////  }

////  sTime.Hours = 0x0;
////  sTime.Minutes = 0x0;
////  sTime.Seconds = 0x0;
////  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
////  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
////  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
////  {
////    Error_Handler();
////  }

////  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
////  sDate.Month = RTC_MONTH_JANUARY;
////  sDate.Date = 0x1;
////  sDate.Year = 0x0;

////  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
////  {
////    Error_Handler();
////  }

////    /**Enable the WakeUp 
////    */
////  if (HAL_RTCEx_SetWakeUpTimer(&hrtc, 0, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
////  {
////    Error_Handler();
////  }

////}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{
 SPI_HandleTypeDef SPI2Handle;//ÓÃÓÚSPI1´¦ÀíµÄ½á¹¹Ìå 
	
	GPIO_InitTypeDef  GPIO_InitStruct;
    

    
    /* Ê¹ÄÜSPIÊ±ÖÓ */
    __SPI2_CLK_ENABLE();
    
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_13;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;   
    HAL_GPIO_Init( GPIOB, &GPIO_InitStruct);
      
    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;   
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;     
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);    

    /* ÅäÖÃSPI²ÎÊý */    
    SPI2Handle.Instance               = SPI2;
    SPI2Handle.Init.Mode              = SPI_MODE_MASTER;//SPIÄ£Ê½£ºÖ÷»úÄ£Ê½    
    SPI2Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;//SPIÊ±ÖÓ·ÖÆµÏµÊý
    SPI2Handle.Init.Direction         = SPI_DIRECTION_2LINES;//Á½ÏßÈ«Ë«¹¤
    SPI2Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;//Ê±ÖÓÏàÎ»£ºµÚ2¸ö±ßÑØ²ÉÑù
    SPI2Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;//Ê±ÖÓ¼«ÐÔ£º¸ß
    SPI2Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;//CRCÐ£Ñé£º²»Ê¹ÄÜ
    SPI2Handle.Init.CRCPolynomial     = 7;//CRC¶àÏîÊ½
    SPI2Handle.Init.DataSize          = SPI_DATASIZE_8BIT;//Êý¾Ý¿í¶È£º8Î»
    SPI2Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;//´«ÊäµÄµÚÒ»Î»£ºMSB
    SPI2Handle.Init.NSS               = SPI_NSS_SOFT;//NSSÒý½Å£ºÈí¼þ¹ÜÀí
    SPI2Handle.Init.TIMode            = SPI_TIMODE_DISABLED;//Ìû¸ñÊ½£ºÄ¦ÍÐÂÞÀ­Ä£Ê½
    if(HAL_SPI_Init(&SPI2Handle) != HAL_OK)
    {
        /* Initialization Error */
    }
    
    /* Ê¹ÄÜSPI */        
    __HAL_SPI_ENABLE(&SPI2Handle);  

//  hspi2.Instance = SPI2;
//  hspi2.Init.Mode = SPI_MODE_MASTER;
//  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
//  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
//  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
//  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
//  hspi2.Init.NSS = SPI_NSS_SOFT;
//  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
//  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
//  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  hspi2.Init.CRCPolynomial = 7;
//  if (HAL_SPI_Init(&hspi2) != HAL_OK)
//  {
//    Error_Handler();
//  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

	  GPIO_InitTypeDef GPIO_InitStruct;

//  /* GPIO Ports Clock Enable */
//  __GPIOB_CLK_ENABLE();

//		
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
//	
//  /*Configure GPIO pin : Pb12 */
//  GPIO_InitStruct.Pin = GPIO_PIN_12;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//  GPIO_InitTypeDef GPIO_InitStruct;

//  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, PC1_Pin|PC3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC1_Pin PC3_Pin */
//  GPIO_InitStruct.Pin = PC1_Pin|PC3_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_CS_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : VIEW_KEY_Pin START_KEY_Pin USB_detect_Pin */
//  GPIO_InitStruct.Pin = VIEW_KEY_Pin|START_KEY_Pin|USB_detect_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}
void LSE_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
