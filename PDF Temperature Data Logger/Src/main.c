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
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage_if.h"
/* USER CODE BEGIN Includes */
#include "fatfs_flash_spi.h"
#include "ff.h"
#include "LCD.H"
#include "usbd_storage_if.h"
#include "pwr.h"
#include "zlib.h"
#include "calendar.h"
#include "button.h"
#include "uart.h"
#include "stm32l0xx_it.h"
#include "malloc.h"
#include "stream1.h"
#include "adc.h"
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
//static void MX_I2C2_Init(void);
HAL_StatusTypeDef MX_LCD_Voltage_change(LCD_HandleTypeDef hlcd);
static void MX_SPI2_Init(void);
//static void ADC1_ChannelConfig(void);
LCD_HandleTypeDef  DISplay_WD_RH(LCD_HandleTypeDef hlcd,int16_t T_wendu2,uint16_t Normal,uint16_t canshu,uint16_t Bat_Day);
LCD_HandleTypeDef MX_LCD_Init(LCD_HandleTypeDef hlcd);
//uint8_t DISplay_RH_High_Low(LCD_HandleTypeDef hlcd,int16_t RH_shidu,uint8_t LCD_Error_flag_RH);
uint8_t DISplay_WD_High_Low(LCD_HandleTypeDef hlcd,int16_t T_wendu,uint8_t LCD_Error_flag_WD);
LCD_HandleTypeDef DISplay_MAX_MIN_AVG(LCD_HandleTypeDef hlcd,int16_t T_wendu,int16_t RH_shidu,uint8_t LCD_Error_flag_WD,uint8_t KEY_State,uint8_t KEY_shidu,uint8_t LCD_Error_flag_RH,int16_t Data_max_wd,int16_t Data_min_wd,int16_t T_average,uint16_t Bat_Day,int16_t Data_max_sd,int16_t Data_min_sd,int16_t RH_average);
uint16_t RTC_TIME_record(RTC_DateTypeDef sdatestructure,RTC_TimeTypeDef stimestructure,uint32_t End_Y_M_D,uint32_t End_H_M_S);
static void MX_RTC_Init(RTC_DateTypeDef sdatestructure,RTC_TimeTypeDef stimestructure,RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct);
LCD_HandleTypeDef MX_LCD_SPI_I2C_Init(LCD_HandleTypeDef hlcd);
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//#define SHT20_Delay			HAL_Delay(50)   




 #define Set_YMD_ADDRESS 						0x08080000   	//???????
 #define Set_HMS_ADDRESS 						0x08080004   	//?? ?? ? ???? 
 #define Set_High_Low_ADDRESS 			0x08080008   	//?? ??????
 #define Set_Inter_Del_ADDRESS 			0x0808000C   	//????   ??????

 #define Barcord_YM_ADDRESS 			0x08080010   	//????????? ????
 #define Barcord_Num_ADDRESS 			0x08080014   	//????? ??????
 #define Barcord_Life_ADDRESS 		0x08080018   	//????? ??????????????       ????
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
 #define End_Y_M_D_ADDRESS 				0x08080050
 #define End_H_M_S_ADDRESS 				0x08080054

 #define TIME_FLAG_ADDRESS 				0x0808016C

 
  #define USE_COUNT_ADDRESS 		0x08080188  
	 #define KAIJI_Set_YMD_ADDRESS 	0x0808018C   	//???????
 #define KAIJI_Set_HMS_ADDRESS 		0x08080190   	//?? ?? ? ???? 
	#define MAK_NUM_ADDRESS 		0x0808020C
	#define MAK_ONE_ADDRESS 		0x08080210
/* USER CODE END 0 */
//extern PCD_HandleTypeDef hpcd_USB_FS;
//extern LPTIM_HandleTypeDef             LptimHandle;
/* Private variables ---------------------------------------------------------*/



//I2C_HandleTypeDef hi2c2;


 
			RTC_HandleTypeDef RtcHandle;//??RTC??????			
//UART_HandleTypeDef hlpuart1;

//RTC_HandleTypeDef hrtc;

uint8_t YMD_RTC_Y=0,YMD_RTC_M=0,YMD_RTC_D=0,HMS_RTC_H=0,HMS_RTC_M=0,HMS_RTC_S=0;

int main(void)
{

  /* USER CODE BEGIN 1 */
		uint16_t i=0,j=0,KEY_count=0,Alarm_times_wd=0,Alarm_times_sd=0;
	uint8_t USB_PowerOn_State=0;
	uint8_t Button_One_Flag=0,KEY_State=0,Button_Stop_Flag=0;

//	uint32_t	Data_wendu_Adress=0x00100000,PC_Set_YMD=0,PC_Set_HMS=0, PC_Set_H_L=0,PC_Set_Inter_Del=0,PC_Set_Barcode_YM=0,PC_Set_Barcode_Num=0,PC_Set_Barcode_Life=0;
	uint32_t	Data_Temperat_Adress=0x00100000;
	uint32_t	PC_Set_YMD=0,PC_Set_HMS=0, PC_Set_H_L=0,PC_Set_Inter_Del=0,PC_Set_Barcode_YM=0,PC_Set_Barcode_Num=0,PC_Set_Barcode_Life=0;
	
//	uint32_t First_Y_M_D=0,First_H_M_S=0;
	uint8_t pdf_creat_flag=0,pdf_end_flag=0,KEY_State_8=1;
	int8_t KEY_shidu=1;
	uint8_t Commn_barcode_over=0,Commn_set_over=0,Set_Over_Flag=0;
	uint8_t Set_Interl_time=1,Set_Start_delay=0;  //??????  ??????
//	uint8_t Stop_time_year=0,Stop_time_month=0,Stop_time_day=0,Stop_time_hour=0,Stop_time_min=0,Stop_time_sec=0;
	uint8_t  Set_Interl=1;
	int16_t Data_max_wd=0,Data_min_wd=0,T_average=0;
	uint16_t RH_shidu=0,Data_max_sd=0,Data_min_sd=0,RH_average=0;
	int32_t T_sum=0;
	uint32_t RH_sum=0;
	uint32_t Number_Record=1;
	//IIC?????
	int16_t I2C_date=1;
//	uint8_t i=0;
	
	uint16_t Bat_Day=400,k=0;
	uint32_t  First_Y_M_D=0,First_H_M_S=0,End_Y_M_D=0,End_H_M_S=0;
	  RTC_DateTypeDef sdatestructure;	
  RTC_TimeTypeDef stimestructure;		 RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  /* USER CODE END 1 */
		ADC_HandleTypeDef hadc;LCD_HandleTypeDef hlcd;LCD_HandleTypeDef hlcdd;	
  /* MCU Configuration----------------------------------------------------------*/
	UART_HandleTypeDef LpUart1Handle;
	uint8_t LCD_Error_flag_WD=0;
	uint8_t LCD_Error_flag_RH=0;
	uint8_t MAK_FLAG=0;
	uint8_t Collect_FLAG=0;
	uint32_t Accumulative_min=0;
uint16_t month=2,day=27,year=10,hour=17,min=18,sec=16;
uint16_t Wake_up_delay=0;
			uint8_t point_show=0;
			uint8_t LED_ALARM_FLAG=0;
			uint8_t	below_above_flag[8];
					int16_t Alarm_above[8];
uint32_t	Alarm_Accumulative[8];
	uint16_t Alarm_stop_flag[8];
	uint16_t Alarm_above_flag[8];
				uint16_t	Alarm_Accumulative_num=0;
				uint16_t kaiji_delay=0,t=0;
				int16_t  D_wendu_value=0;
				uint8_t dayu_xiaoyu_flag=0;

					uint8_t Work_day=90;	uint8_t USB_Flag=0;uint32_t Work_day_flag=0;uint8_t guanji_Flag=0;
//							char *url="url:";
//							char *usb_name;
//							char *Device_ID;
//							char *Trip_description; 
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
//HAL_Delay(800);
//HAL_Delay(800);
//HAL_Delay(800);
//HAL_Delay(800);
  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */

 End_Y_M_D=DataEEPROM_Read(End_Y_M_D_ADDRESS);				//???????  ?????
		YMD_RTC_Y=(uint8_t)(End_Y_M_D>>16);															//????
		YMD_RTC_M=(uint8_t)(End_Y_M_D>>8);
		YMD_RTC_D=(uint8_t)End_Y_M_D;
		End_H_M_S=DataEEPROM_Read(End_H_M_S_ADDRESS);				//???? ?? ???  ?????
		HMS_RTC_H=(uint8_t)(End_H_M_S>>16);
		HMS_RTC_M=(uint8_t)(End_H_M_S>>8);
		HMS_RTC_S=(uint8_t)End_H_M_S;
	MX_RTC_Init(sdatestructure,stimestructure,PeriphClkInitStruct);
//	pdf_end_flag=DataEEPROM_Read(0x0808004c);
	Button_One_Flag=DataEEPROM_Read(K_stop_ADDRESS);              //??????????
	BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_GPIO);
	USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
	if(USB_PowerOn_State==1)              															//如果电池电量不足以支撑系统工作，单独插上USB时可以读出数据
		{
			HAL_Delay(20);
		USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);			
			if(USB_PowerOn_State==1)         //
				{
					MX_GPIO_Init();
					MX_SPI2_Init();	
				
					if(DataEEPROM_Read(0x08080058)==0)
					{	
						CSV_Creat();
					}else
					{
							
							CSV_Creat();PDF_Creat();
					}

				}	
					

}	
		USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
			if(USB_PowerOn_State==1)              
			{
				HAL_Delay(20);          
				if(USB_PowerOn_State==1)         
				{
						if(DataEEPROM_Read(0x08080200)==1)
						{																	
											BYTE *Tempe_Data_wendul=mymalloc(512);																		
													TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendul,64,1);												
													TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendul, 1663, 1);
													TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendul,70,1);											
													TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendul, 1664, 1);
													TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendul,76,1);								
													TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendul, 1665, 1);
													myfree(Tempe_Data_wendul);DataEEPROM_Write(0x08080200,2);
											HAL_NVIC_SystemReset(); 
						}

									USBD_HandleTypeDef *hUsbDeviceFS;
									if(DataEEPROM_Read(0x08080044)==1)
									{
											hUsbDeviceFS=mymalloc(124);
									}else
									{
										hUsbDeviceFS=mymalloc(824);
									
									}
									USBD_Init(hUsbDeviceFS, &FS_Desc, DEVICE_FS);

									USBD_RegisterClass(hUsbDeviceFS, &USBD_MSC);

									USBD_MSC_RegisterStorage(hUsbDeviceFS, &USBD_Storage_Interface_fops_FS);

									USBD_Start(hUsbDeviceFS);										
					do
					{
						HAL_Delay(100);
						USB_PowerOn_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
						if(usb_write_flag==1)
						{
							for(i=0;i<9;i++)
							{
									DataEEPROM_Write(0x08080160+4*i,0);
							}
							for(i=0;i<20;i++)
							{
									DataEEPROM_Write(0x08080058+4*i,0);
							}
								usb_write_flag=0;		
									uint8_t	*Tempe_Data_wendu;
								char *battery;
								char *stop_mode;
								char *language;
								char *unit;
//								char *probe_type;
								char *Times_use;
								char *time_zonew;
								char *time_time;
								char *Recording_day;
								char *output_format;

//							char *Firmware;
							char *Time_format;
						char *usb_name;	char usb_name2[15];
						usb_name=mymalloc(9);		
							Tempe_Data_wendu=mymalloc(1536);
							 battery=mymalloc(13);
							stop_mode=mymalloc(6);
							 language=mymalloc(7);
							 unit=mymalloc(3);
//							 probe_type=mymalloc(14);
							 Times_use=mymalloc(6);				
							 time_time=mymalloc(5);
							 Recording_day=mymalloc(15);
							output_format=mymalloc(14);
//							PDF_custom=mymalloc(11);
							Time_format=mymalloc(12);
							time_zonew=mymalloc(12);
							battery="Don't replace";
							 stop_mode="manual";
							 language="Russian";
							 unit[0]=0xA1,unit[1]=0xE3,unit[2]=0x46;//huashidu
//							 probe_type="type:in";
							 Times_use="single";
							 time_time="time:";
							 Recording_day="Recording days:";
							output_format="format:PDF";
//							PDF_custom="custom:yes";
							Time_format="MM/DD/YY";
							usb_name="usb name:";
							TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendu, 108,3);
							Collect_FLAG=0;
							Alarm_times_sd=0;
							Alarm_times_wd=0;
							dayu_xiaoyu_flag=0;USB_Flag=0;
							for(i=0;i<1536;i++)
							{
								
												if(Tempe_Data_wendu[i]==battery[0])
												{
																			for(j=0;j<13;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==battery[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==13)
																			{
																					DataEEPROM_Write(0x08080170 ,	1);//8080170 不换电池，标志位为零
																				
																			}
												
												}
												if(Tempe_Data_wendu[i]==stop_mode[0])
												{
																			for(j=0;j<6;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==stop_mode[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==6)
																			{
																					DataEEPROM_Write(0x08080174 ,	1);//手动置1
																			}
												
												}
												if(Tempe_Data_wendu[i]==language[0])
												{
																			for(j=0;j<7;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==language[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==7)
																			{
																					DataEEPROM_Write(0x08080178 ,	1);//中文置1
																			}
												
												}
												if(Tempe_Data_wendu[i]==unit[0])
												{
																			for(j=0;j<3;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==unit[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==3)
																			{
																					DataEEPROM_Write(0x0808017c ,	1);//温度huashidu 置1
																			}
												
												}
//												if(Tempe_Data_wendu[i]==probe_type[0])
//												{
//																			for(j=0;j<7;j++)
//																			{
//																				if(Tempe_Data_wendu[i+j]==probe_type[j])
//																				{
//																				}else
//																				{
//																					break;
//																				}
//																			}
//																			if(j==7)
//																			{
//																					DataEEPROM_Write(0x08080180 ,	1);//内置探头
//																			}
//												
//												}
												if(Tempe_Data_wendu[i]==Times_use[0])
												{
																			for(j=0;j<6;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==Times_use[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==6)
																			{
																					DataEEPROM_Write(0x08080184 ,	1);//使用次数单次置1
																			}
												
												}
												if(Tempe_Data_wendu[i]==usb_name[0])
												{
																			for(j=0;j<9;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==usb_name[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==9)
																			{
																					for(j=0;j<15;j++)
																					{
																						if(Tempe_Data_wendu[i+9+j]==0x2E)
																						{
																									break;
																						}
																						if(Tempe_Data_wendu[i+9+j]==0x3E&&Tempe_Data_wendu[i+10+j]==0x3E)
																						{
																									break;
																						}
																						usb_name2[j]=Tempe_Data_wendu[i+9+j];
																					}
																					usb_name2[j]='\0';

																							USB_Flag=1;
																			}
												
												}
												if(Tempe_Data_wendu[i]==time_time[0])
												{
																			for(j=0;j<5;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==time_time[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==5)
																			{
																					year=(Tempe_Data_wendu[i+5]-48)*10+(Tempe_Data_wendu[i+6]-48);
																					month=(Tempe_Data_wendu[i+8]-48)*10+(Tempe_Data_wendu[i+9]-48);
																					day=(Tempe_Data_wendu[i+11]-48)*10+(Tempe_Data_wendu[i+12]-48);
																					hour=(Tempe_Data_wendu[i+14]-48)*10+(Tempe_Data_wendu[i+15]-48);
																					min=(Tempe_Data_wendu[i+17]-48)*10+(Tempe_Data_wendu[i+18]-48);
																					sec=(Tempe_Data_wendu[i+20]-48)*10+(Tempe_Data_wendu[i+21]-48);
																					PC_Set_YMD=((uint32_t)year<<16)+((uint32_t)month<<8)+((uint32_t)day<<0);// 年 月 日
																					PC_Set_HMS=((uint32_t)hour<<16)+((uint32_t)min<<8)+((uint32_t)sec<<0);// 小时 分钟 秒
																					DataEEPROM_Write(End_Y_M_D_ADDRESS,	PC_Set_YMD);
																					DataEEPROM_Write(End_H_M_S_ADDRESS ,PC_Set_HMS);
																					YMD_RTC_Y=RTC_ByteToBcd2(year);															//????
																					YMD_RTC_M=RTC_ByteToBcd2(month);
																					YMD_RTC_D=RTC_ByteToBcd2(day);
																					HMS_RTC_H=RTC_ByteToBcd2(hour);
																					HMS_RTC_M=RTC_ByteToBcd2(min);
																					HMS_RTC_S=RTC_ByteToBcd2(sec);
																					MX_RTC_Init(sdatestructure,stimestructure,PeriphClkInitStruct);
																				DataEEPROM_Write(TIME_FLAG_ADDRESS,1);
																			}
												
												}
												
												if(Tempe_Data_wendu[i]==Recording_day[0])
												{
																			for(j=0;j<15;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==Recording_day[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==15)
																			{
																				k=0;
																					for(j=0;j<75;j++)
																					{
																							if(Tempe_Data_wendu[i+15+j]>47&&Tempe_Data_wendu[i+15+j]<58)
																							{
																								time_zonew[k]=Tempe_Data_wendu[i+15+j];k++;
																							}
																						
																					}													
																						sec=(time_zonew[0]-48)*100+(time_zonew[1]-48)*10+(time_zonew[2]-48);//days
																						DataEEPROM_Write(0x08080064,sec);
																						sec=(time_zonew[3]-48)*10+(time_zonew[4]-48);//Set_Interl_time
																						DataEEPROM_Write(0x08080068,sec);
																						sec=(time_zonew[5]-48)*100+(time_zonew[6]-48)*10+(time_zonew[7]-48);//kaiji_delay
																						DataEEPROM_Write(0x0808006c,sec);
																					Collect_FLAG=1;
																			}
												
												}
												if(Collect_FLAG==1)
												{
													
													if(Tempe_Data_wendu[i]==0x61&&Tempe_Data_wendu[i+1]==0x62&&Tempe_Data_wendu[i+2]==0x6F&&Tempe_Data_wendu[i+3]==0x76&&Tempe_Data_wendu[i+4]==0x65)//above
													{

																			DataEEPROM_Write(0x08080120+dayu_xiaoyu_flag*4 ,	1);
																			dayu_xiaoyu_flag++;
														
													
													}
													if(Tempe_Data_wendu[i]==0x62&&Tempe_Data_wendu[i+1]==0x65&&Tempe_Data_wendu[i+2]==0x6c&&Tempe_Data_wendu[i+3]==0x6F&&Tempe_Data_wendu[i+4]==0x77)//below
													{
													

																			DataEEPROM_Write(0x08080120+dayu_xiaoyu_flag*4 ,	0);
																			dayu_xiaoyu_flag++;
																	
														
													
													}
													
													if(Tempe_Data_wendu[i+11]>47&&Tempe_Data_wendu[i+11]<58)
													{
														
														if(Tempe_Data_wendu[i+12]>47&&Tempe_Data_wendu[i+12]<58)
														{
																if(Tempe_Data_wendu[i+13]==0x2e)
																{				
																	
																			if(Tempe_Data_wendu[i+16]==0x43||Tempe_Data_wendu[i+16]==0x46)
																			{				
																								if(Tempe_Data_wendu[i+10]>47&&Tempe_Data_wendu[i+10]<58)
																								{
																													
																									
																								}else
																								{
																												Alarm_times_wd++;

																															if(Tempe_Data_wendu[i+10]==0x2d)
																															{
																																						if(Tempe_Data_wendu[i+14]>47&&Tempe_Data_wendu[i+14]<58)
																																						{
																																								I2C_date=(Tempe_Data_wendu[i+11]-48)*100+(Tempe_Data_wendu[i+12]-48)*10+(Tempe_Data_wendu[i+14]-48);
																																							I2C_date=-I2C_date;
																																						}
																															
																															}else
																															{
																																					if(Tempe_Data_wendu[i+14]>47&&Tempe_Data_wendu[i+14]<58)
																																						{
																																							I2C_date=(Tempe_Data_wendu[i+11]-48)*100+(Tempe_Data_wendu[i+12]-48)*10+(Tempe_Data_wendu[i+14]-48);
																																						}
																															
																															}
																															DataEEPROM_Writeint32(0x0808006C+Alarm_times_wd*4,I2C_date);
																														
																													}
																								
																								
																								
																								}
																						
																
																}else
																{
																			if(Tempe_Data_wendu[i+13]>47&&Tempe_Data_wendu[i+13]<58)
																			{
																													if(Tempe_Data_wendu[i+14]==0x2e)
																													{	
																																		if(Tempe_Data_wendu[i+17]==0x46||Tempe_Data_wendu[i+17]==0x43)
																																			{				

																																							Alarm_times_wd++;

																																						if(Tempe_Data_wendu[i+10]==0x2d)
																																						{
																																													if(Tempe_Data_wendu[i+15]>47&&Tempe_Data_wendu[i+15]<58)
																																													{
																																															I2C_date=(Tempe_Data_wendu[i+11]-48)*1000+(Tempe_Data_wendu[i+12]-48)*100+(Tempe_Data_wendu[i+13]-48)*10+(Tempe_Data_wendu[i+15]-48);
																																														I2C_date=-I2C_date;
																																													}
																																						
																																						}else
																																						{
																																												if(Tempe_Data_wendu[i+15]>47&&Tempe_Data_wendu[i+15]<58)
																																													{
																																															I2C_date=(Tempe_Data_wendu[i+11]-48)*1000+(Tempe_Data_wendu[i+12]-48)*100+(Tempe_Data_wendu[i+13]-48)*10+(Tempe_Data_wendu[i+15]-48);
																																													}
																																						
																																						}
																																						DataEEPROM_Writeint32(0x0808006C+Alarm_times_wd*4,I2C_date);
																																					
																																				}
																														
																														
																													}
																				
																				
																							if(Tempe_Data_wendu[i+14]>47&&Tempe_Data_wendu[i+14]<58)
																							{
																										if(Tempe_Data_wendu[i+15]>47&&Tempe_Data_wendu[i+15]<58)
																										{
																												
																											Accumulative_min=(Tempe_Data_wendu[i+11]-48)*10000+(Tempe_Data_wendu[i+12]-48)*1000+(Tempe_Data_wendu[i+13]-48)*100+(Tempe_Data_wendu[i+14]-48)*10+(Tempe_Data_wendu[i+15]-48);
																											DataEEPROM_Write(0x080800AC+Alarm_times_wd*4+Alarm_times_sd*4,Accumulative_min);
																										}
																									
																							}
																					
																					
																			}
															}
													
															
														}
												
												
													}
											}
										if(Tempe_Data_wendu[i]==output_format[0])
												{
																			for(j=0;j<7;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==output_format[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==7)
																			{
																				if(Tempe_Data_wendu[i+7]=='C'&&Tempe_Data_wendu[i+8]=='S'&&Tempe_Data_wendu[i+9]=='V')
																				{
																					DataEEPROM_Write(0x08080058,0);
																				}else if(Tempe_Data_wendu[i+7]=='P'&&Tempe_Data_wendu[i+8]=='D'&&Tempe_Data_wendu[i+9]=='F')
																				{
																					DataEEPROM_Write(0x08080058,1);
																				}
																				if(Tempe_Data_wendu[i+11]=='a'&&Tempe_Data_wendu[i+12]=='n'&&Tempe_Data_wendu[i+13]=='d')
																				{
																					DataEEPROM_Write(0x08080058,2);
																				}
																				
																				
																			}
												
												}
												if(Tempe_Data_wendu[i]==Time_format[0])
												{
																			for(j=0;j<8;j++)
																			{
																				if(Tempe_Data_wendu[i+j]==Time_format[j])
																				{
																				}else
																				{
																					break;
																				}
																			}
																			if(j==8)
																			{
																				DataEEPROM_Write(0x08080060,1);
																			}
												
												}

												
												

							}
							DataEEPROM_Write(0x08080160 ,	Alarm_times_wd);
							DataEEPROM_Write(0x08080164 ,	Alarm_times_sd);
							DataEEPROM_Write(0x08080198,1);
						 myfree(Tempe_Data_wendu);
						 myfree(battery);
						 myfree(stop_mode);
						 myfree(language);
						 myfree(unit);
//						 myfree(probe_type);
						 myfree(Times_use);
						 myfree(time_time);
						 myfree(Recording_day);
						myfree(output_format);
							myfree(time_zonew);
							myfree(usb_name);
						
									if(USB_Flag==1)
									{
												uint8_t	*Tempe_Data_wendu;
												Tempe_Data_wendu=mymalloc(512);	
												TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendu, 76,1);
												j=0;t=0;
												for(i=0;i<11;i++)
												{
													Tempe_Data_wendu[i+j]=0x20;
													if(usb_name2[i]=='\0'||t==1)
													{
														Tempe_Data_wendu[i+j]=0x20;t=1;
													}else
													{
														Tempe_Data_wendu[i+j]=usb_name2[i];		
													}
												}
//													Tempe_Data_wendu[11+j]=0x08;
//													Tempe_Data_wendu[22+j]=0xF2;Tempe_Data_wendu[23+j]=0x78;Tempe_Data_wendu[24+j]=0x9B;Tempe_Data_wendu[25+j]=0x4C;
													TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendu, 76, 1);
													TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendu, 1665, 1);
													myfree(Tempe_Data_wendu);Usb_Power_On_Flag=0;
													RTC_TIME_record(sdatestructure,stimestructure,End_Y_M_D,End_H_M_S);
													HAL_NVIC_SystemReset(); //???????,??usb??????
												
										}
							
						}
						if(DataEEPROM_Read(0x08080198)>0&&DataEEPROM_Read(0x08080064)>0)
						{
								hlcd=MX_LCD_Init(hlcd);	
								Data_Display_Num(hlcd,DataEEPROM_Read(USE_COUNT_ADDRESS));		
								HAL_LCD_UpdateDisplayRequest(&hlcd);
								HAL_Delay(300);
								HAL_LCD_Clear(&hlcd);
						
						}
						if(DataEEPROM_Read(0x08080198)==0&&DataEEPROM_Read(0x08080044)==1&&Button_One_Flag==0)
						{
								hlcd=MX_LCD_Init(hlcd);	
								Data_Display_Num(hlcd,666);		
								HAL_LCD_UpdateDisplayRequest(&hlcd);
								HAL_Delay(300);
								HAL_LCD_Clear(&hlcd);
						
						}
						
					}
					while(USB_PowerOn_State==1) ;
					myfree(hUsbDeviceFS);
					Usb_Power_On_Flag=0;
					if(Button_One_Flag>0)
					{
						RTC_TIME_record(sdatestructure,stimestructure,End_Y_M_D,End_H_M_S);
					}
					HAL_NVIC_SystemReset(); //???????,??usb??????
				}
			}


Key_Start:	
		if((Button_One_Flag==0)&&(Set_Over_Flag==0)&&DataEEPROM_Read(0x08080044)==1)
		{		
			
		SystemPower_Config();     //???????  ????IO???	
			BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
										 		/**USB插入检测**/
			BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_EXTI);
			anjian_0:
//			/* Enter Stop Mode */
			HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);   
			if(Usb_Power_On_Flag==1)
			{
				Usb_Power_On_Flag=0;
				HAL_NVIC_SystemReset(); 
			}		
			KEY_count=0;
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
						hlcdd=MX_LCD_SPI_I2C_Init(hlcdd);
						KEY_count=0;
						Button_One_Flag=1;				
					}
				else
					{
						KEY_count=0;

						goto anjian_0;
					}
	LPUART1_Init();	      /* (测试注释)*/
 __HAL_UART_DISABLE(&LpUart1Handle);//失能USART
  LpUart1Handle.Instance        = LPUART1;
  LpUart1Handle.Init.BaudRate   = 9600;
  LpUart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
  LpUart1Handle.Init.StopBits   = UART_STOPBITS_1;
  LpUart1Handle.Init.Parity     = UART_PARITY_NONE;
  LpUart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  LpUart1Handle.Init.Mode       = UART_MODE_TX_RX;
  if(HAL_UART_Init(&LpUart1Handle) != HAL_OK)
  {
      
  }
  __HAL_UART_DISABLE(&LpUart1Handle);
  LPUART1->RQR = 1<<3;//clears the RXNE flag
  LPUART1->CR1 |= 1<<5;//enable the RXNE interrupt
  HAL_NVIC_SetPriority(LPUART1_IRQn, 3, 0);//设置USART中断优先级:抢占优先级3；子优先级0（M0+内核无子优先级）
  HAL_NVIC_EnableIRQ(LPUART1_IRQn);//使能USART中断
  __HAL_UART_ENABLE(&LpUart1Handle);//使能USART	
tiaoma_mak:
		HAL_LCD_Clear(&hlcdd);				
	All_Time_Display(hlcdd);
	HAL_LCD_UpdateDisplayRequest(&hlcdd);					
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);		
	Data_Display_Num(hlcdd,8888);		
	HAL_LCD_UpdateDisplayRequest(&hlcdd);	
	HAL_Delay(200);					
	
		if(Set_Over_Flag==0)
			{


				do{	
					
						KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
						if(Comm_OK_flag!=1&&Write_set_flag!=1&&Write_barcode_flag!=1&&KEY_State==0)
						{
							HAL_Delay(200);
							KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
							if(KEY_State==0)
								{
									goto tiaoma_mak;
								}
								
						}
						if(Comm_OK_flag==1)      					//建立通讯连接成功
						 {
							 Comm_OK_flag=0;
							 Rece_count=0;
						
							 for(i=0;i<6;i++)
								 {
									 LPUART1_SendByte(Send_data[i]);
								 }
							for(i=0;i<18;i++)
								{
								 Rec_data[i]=0;
								 Send_data[i]=0;
						
								}
								HAL_Delay(200);
						}
					 if(Write_set_flag==1)            //时间信息设定写入   
						 {
							 Write_set_flag=0;
							 Rece_count=0;
								DataEEPROM_Write(TIME_FLAG_ADDRESS,1);
								YMD_RTC_Y=RTC_ByteToBcd2(Send_data[2]);   
								YMD_RTC_M=RTC_ByteToBcd2(Send_data[3]);
								YMD_RTC_D=RTC_ByteToBcd2(Send_data[4]);
								HMS_RTC_H=RTC_ByteToBcd2(Send_data[5]);
								HMS_RTC_M=RTC_ByteToBcd2(Send_data[6]);
								HMS_RTC_S=RTC_ByteToBcd2(Send_data[7]);
							 for(i=0;i<18;i++)
								 {
									 LPUART1_SendByte(Send_data[i]);
								 }
				 
							for(i=0;i<18;i++)
								{
									 Rec_data[i]=0;
									 Send_data[i]=0;
								}
							HAL_Delay(200);
							Commn_set_over=1;
						 }
					if(Write_barcode_flag==1)         //条形码信息写入
						{
							Write_barcode_flag=0;
							Rece_count=0;
							PC_Set_Barcode_YM=((uint32_t)Send_data[2]<<8)+((uint32_t)Send_data[3]<<0);
							PC_Set_Barcode_Num=((uint32_t)Send_data[4]<<24)+((uint32_t)Send_data[5]<<16)+((uint32_t)Send_data[6]<<8)+((uint32_t)Send_data[7]<<0);
							PC_Set_Barcode_Life=((uint32_t)Send_data[8]<<8)+((uint32_t)Send_data[9]<<0);
							/*******************写入数据******************************/
								DataEEPROM_Write(Barcord_YM_ADDRESS,PC_Set_Barcode_YM);                               //将获得的时间、高报、低报等信息写入DATA EEPROM
								DataEEPROM_Write(Barcord_Num_ADDRESS,PC_Set_Barcode_Num);
								DataEEPROM_Write(Barcord_Life_ADDRESS,PC_Set_Barcode_Life);
							/********************结束******************************/

							for(i=0;i<14;i++)
								 {
									 LPUART1_SendByte(Send_data[i]);
								 }
							for(i=0;i<18;i++)
								 {
									 Rec_data[i]=0;
									 Send_data[i]=0;
								 }
							HAL_Delay(200);
							Commn_barcode_over=1;
						}
				}while((Commn_barcode_over==0)||(Commn_set_over==0));

				
				if((Commn_barcode_over==1)&&(Commn_set_over==1))
					{
						Set_Over_Flag=1;
						Button_One_Flag=1;
		      	DataEEPROM_Write(K_start_ADDRESS,Set_Over_Flag);											//记录次数写入   设置记录完成标志位
						DataEEPROM_Write(0x08080200,1);
					}
		/************  ****RTC 设置**   *************/
					
					/* ??RTC??? */  
						MX_RTC_Init(sdatestructure,stimestructure,PeriphClkInitStruct);
						HAL_LCD_Clear(&hlcdd);		
						DataEEPROM_Write(K_stop_ADDRESS,1);//K_start_ADDRESS 
						HAL_NVIC_SystemReset(); 
}
}
		uint8_t	*Tempe_Data_wendu;	
	Key_Start_1:
	if(Button_One_Flag==1)
		{
			SystemPower_Config();     //???????  ????IO???	
			Button_Stop_Flag=DataEEPROM_Read(K_stop_ADDRESS);            
			if(Button_Stop_Flag!=2&&DataEEPROM_Read(0x08080064)>0)  				//???  ????????
			{
				if(DataEEPROM_Read(0x08080184)==0||DataEEPROM_Read(0x08080198)>0)
				{
						BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
				}
				
			}

			BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_EXTI);
								anjian_1:		
								HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);        
									if(Usb_Power_On_Flag==1)
									{
										Usb_Power_On_Flag=0;
											RTC_TIME_record(sdatestructure,stimestructure,End_Y_M_D,End_H_M_S);
											HAL_NVIC_SystemReset();
									}		
	
								KEY_count=0;
							
								LSE_ClockEnable();
								HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);		//?????	
								HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x0029, RTC_WAKEUPCLOCK_RTCCLK_DIV8);//8/（~32.768KHz）=~0.244ms  WakeUpCounter = ~(10s-200ms)/0,244ms = 40164 = 0x9CE4         (10S-200ms)定时
								HAL_NVIC_SetPriority(RTC_IRQn, 0x0, 0);//???????
								HAL_NVIC_EnableIRQ(RTC_IRQn);//??RTC??
								do
								{		
									HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
									KEY_count++;
									KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
									if(KEY_count>500)
									{
										break;
									}
								}while(KEY_State==0);
									
									if(KEY_count>500)
										{
													hlcd=MX_LCD_SPI_I2C_Init(hlcd);
												BSP_PB_DISable(BUTTON_KEY, BUTTON_MODE_EXTI);HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x0400, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
											for(i=0;i<5;i++)
											{
												Data_Display_Num(hlcd,DataEEPROM_Read(USE_COUNT_ADDRESS));		
												HAL_LCD_UpdateDisplayRequest(&hlcd);
												HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
												HAL_LCD_Clear(&hlcd);
												HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
											}
											
										}
									else
										{
											KEY_count=0;
										__HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle);
										/* Disable the Wakeup Timer */
										__HAL_RTC_WAKEUPTIMER_DISABLE(&RtcHandle);
							
										goto anjian_1;
										}
										

				point_show=DataEEPROM_Read(0x08080160);
			if(point_show>8)
			{
					point_show=8;
			}
					j=0;
				for(i=0;i<point_show;i++)
					{
					
							j=j+4;
							Alarm_above[i]=DataEEPROM_Read(0x0808006C+j);	
							Alarm_Accumulative[i]=DataEEPROM_Read(0x080800AC+j);
					
					}


					for(i=0;i<point_show;i++)
					{
						below_above_flag[i]=DataEEPROM_Read(0x08080120+i*4);
						
					}	
			
				kaiji_delay=DataEEPROM_Read(0x0808006c)	;	
			
			Wake_up_delay=0;	

					
		if(kaiji_delay>0)
		{	

							while(Wake_up_delay<(kaiji_delay*2))
							{

												HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0xEE8A, RTC_WAKEUPCLOCK_RTCCLK_DIV16);//16/（~32.768KHz）=~0.488ms  WakeUpCounter = ~(30s-200ms)/0,488ms = 61066 = 0xEE8A   30S-200ms定?
												HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
												Wake_up_delay++;

										
							}
						
		}
	Wake_up_10s=0;
	Set_Interl_time=DataEEPROM_Read(0x08080068); 
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
	LCD_Error_flag_WD=0;	
	DataEEPROM_Write(0x0808003C,2);//电池没电后,促使生成PDF	
	BSP_USB_POWERON_DISable(USB_PWON, USBPWON_MODE_EXTI);
	Set_Interl=Set_Interl_time*6;		//  记录间隔 		
	BSP_STOP_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
	Tempe_Data_wendu=mymalloc(256);
	ADC_Trave();	
	ADC_Trave();
	I2C_date=T_wendu;			
	Data_max_wd=I2C_date;
	Data_min_wd=I2C_date;
	T_sum=T_sum+I2C_date;
	T_average=T_sum/Number_Record;
	Data_Temperat_Adress=0x00100000;
	k=0;
	Tempe_Data_wendu[k]=T_wendu; 
	Tempe_Data_wendu[k+1]=(T_wendu>>8);
	k++;
	Work_day=DataEEPROM_Read(0x08080064);
	HAL_RTC_GetTime(&RtcHandle, &stimestructure, FORMAT_BIN);													/* Get the RTC current Date */
	HAL_RTC_GetDate(&RtcHandle, &sdatestructure, FORMAT_BIN);
	First_Y_M_D=((uint32_t)sdatestructure.Year<<16)+((uint32_t)sdatestructure.Month<<8)+(uint32_t)sdatestructure.Date;
	First_H_M_S=((uint32_t)stimestructure.Hours<<16)+((uint32_t)stimestructure.Minutes<<8)+stimestructure.Seconds;
	DataEEPROM_Write(First_Y_M_D_ADDRESS,First_Y_M_D);
	DataEEPROM_Write(First_H_M_S_ADDRESS,First_H_M_S);
	if(DataEEPROM_Read(TIME_FLAG_ADDRESS)==1||DataEEPROM_Read(0x08080170)==0)
	{
		DataEEPROM_Write(KAIJI_Set_YMD_ADDRESS,First_Y_M_D);
		DataEEPROM_Write(KAIJI_Set_HMS_ADDRESS,First_H_M_S);
		DataEEPROM_Write(TIME_FLAG_ADDRESS,0);
	}
	PC_Set_YMD=DataEEPROM_Read(KAIJI_Set_YMD_ADDRESS);				//???????  ?????
	Bat_Day=(Work_day-(((sdatestructure.Year-(uint8_t)(PC_Set_YMD>>16))*12+sdatestructure.Month-(uint8_t)(PC_Set_YMD>>8))*31+sdatestructure.Date-(uint8_t)PC_Set_YMD));
	HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x9F23, RTC_WAKEUPCLOCK_RTCCLK_DIV8);
	Wake_up_10s=0;
	__HAL_LCD_HIGHDRIVER_DISABLE(&hlcd);
	guanji_Flag=DataEEPROM_Read(0x08080174);
	if(guanji_Flag==0)
	{
		Work_day_flag=Work_day*1440/Set_Interl_time+2;

	}
	D_wendu_value=T_wendu;	
}
while (1)
  {		
		if(Button_Stop_Flag!=2&&Button_One_Flag!=2)//?????????
			{	
		for(i=0;i<point_show;i++)
		{
							if(below_above_flag[i]==0&&D_wendu_value<Alarm_above[i])
							{
																				
													Alarm_above_flag[i]++;
																					
									
							}else if(below_above_flag[i]==1&&D_wendu_value>Alarm_above[i])
							{
																						
													Alarm_above_flag[i]++;
																							
											
							}
							if(Alarm_Accumulative[i]%Set_Interl_time!=0&&Alarm_Accumulative[i]>Set_Interl_time)
							{
									Alarm_Accumulative_num=Alarm_Accumulative[i]/Set_Interl_time+1;
							}else
							{
									Alarm_Accumulative_num=Alarm_Accumulative[i]/Set_Interl_time;
							}
							if(Alarm_above_flag[i]>=(Alarm_Accumulative_num))
							{
									if(Alarm_above_flag[i]!=0)
									{
											Alarm_stop_flag[i]=1;
									}
							}
			}
				
				LED_ALARM_FLAG=0;
				for(i=0;i<8;i++)
				{
						LED_ALARM_FLAG+=Alarm_stop_flag[i];
				}
										if(LED_ALARM_FLAG>0)
										{
											LCD_Error_flag_WD=1;
										}
								DISplay_MAX_MIN_AVG(hlcd,T_wendu,RH_shidu,LCD_Error_flag_WD,KEY_State,KEY_shidu,LCD_Error_flag_RH,Data_max_wd,Data_min_wd,T_average,Bat_Day,Data_max_sd,Data_min_sd,RH_average);
								KEY_count=0;
								KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
									if(guanji_Flag==0)
									{
											if(Number_Record>=Work_day_flag)
											{
												Button_One_Flag=2;KEY_State=0;
											}
									
									}		
								if(KEY_State==0)
								{
									HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x0029, RTC_WAKEUPCLOCK_RTCCLK_DIV8);	
										do
										{		
										HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
										Wake_up_10s--;	
										KEY_count++;
										KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
											if(KEY_count==100)
											{
												DataEEPROM_Write(MAK_ONE_ADDRESS+MAK_FLAG*4,Number_Record);MAK_FLAG++;
												LCD_Error_flag_WD=DISplay_WD_High_Low(hlcd,T_wendu,LCD_Error_flag_WD);
												Mark_Display(hlcd,MarkDis);
												hlcd=DISplay_WD_RH(hlcd,T_wendu,Normal_display,Celsius,Bat_Day);											
											}
											if(KEY_count>500)
											{
											break;
											}
										}while(KEY_State==0);
 
										if(KEY_count>500)
										{
											KEY_count=0;
											Button_One_Flag=2;
											DataEEPROM_Write(K_stop_ADDRESS,Button_One_Flag);	
											DataEEPROM_Write(MAK_NUM_ADDRESS,MAK_FLAG-1);
										}
										else
										{
											HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle,0x9FE6, RTC_WAKEUPCLOCK_RTCCLK_DIV8);
											KEY_count=0;
										}
										if((Button_One_Flag==2)&&(pdf_creat_flag==0))                  //????????  ?????????????
										{
											pdf_creat_flag=1;
											DataEEPROM_Write(Number_Record_ADDRESS,Number_Record);       //????
											DataEEPROM_Write(Data_max_ADDRESS,Data_max_wd);
											DataEEPROM_Write(Data_min_ADDRESS,Data_min_wd);
											DataEEPROM_Write(T_sum_ADDRESS,T_sum);                    //????
											SPI_FLASH_BufferWritel(Tempe_Data_wendu,Data_Temperat_Adress,256);
											myfree(Tempe_Data_wendu);
											RTC_TIME_record(sdatestructure,stimestructure,End_Y_M_D,End_H_M_S);
											HAL_NVIC_SystemReset(); 
										}						
								}
							
							if(Wake_up_10s%6==0||Wake_up_10s>=Set_Interl)
							{
													ADC_Trave();
													I2C_date=T_wendu;
											if(Wake_up_10s>=Set_Interl)
											{
													Wake_up_10s=0;
													Number_Record++;
													if(T_wendu>Data_max_wd)
													Data_max_wd=T_wendu;
													if(T_wendu<Data_min_wd)
													Data_min_wd=T_wendu;
													T_sum=T_sum+I2C_date;
													T_average=T_sum/Number_Record;
													Tempe_Data_wendu[k*2]=T_wendu; 
													Tempe_Data_wendu[k*2+1]=(T_wendu>>8);
													k++;
													if(k==128)        																	 //???????256 ???1page 
														{
															SPI_FLASH_BufferWritel(Tempe_Data_wendu,Data_Temperat_Adress,256);
															Data_Temperat_Adress+=256;
														}
													if(k==128)        																	 //???????256 ???1page 
													{
														k=0;
														HAL_RTC_GetDate(&RtcHandle, &sdatestructure, FORMAT_BIN);
														PC_Set_YMD=DataEEPROM_Read(KAIJI_Set_YMD_ADDRESS);				//???????  ?????
														Bat_Day=(Work_day-(((sdatestructure.Year-(uint8_t)(PC_Set_YMD>>16))*12+sdatestructure.Month-(uint8_t)(PC_Set_YMD>>8))*31+sdatestructure.Date-(uint8_t)PC_Set_YMD));															//????
													}

											}
												LCD_Error_flag_WD=DISplay_WD_High_Low(hlcd,T_wendu,LCD_Error_flag_WD);
													hlcd=DISplay_WD_RH(hlcd,T_wendu,Normal_display,Celsius,Bat_Day);
													HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x9FE6, RTC_WAKEUPCLOCK_RTCCLK_DIV8);//0x9fe2
							}
							D_wendu_value=T_wendu;
							



			HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);			
			}else
			{										
													SystemPower_Config(); 
													BSP_USB_POWERON_Init(USB_PWON, USBPWON_MODE_EXTI);
													if( Usb_Power_On_Flag==1)
													{
														 Usb_Power_On_Flag=0;
															RTC_TIME_record(sdatestructure,stimestructure,End_Y_M_D,End_H_M_S);
															HAL_NVIC_SystemReset(); 
													}	
													BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);													
													HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
																		KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
																
																		while(KEY_State==0)
																		{
																						LSE_ClockEnable();
																						HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);		//?????	
																						HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x0029, RTC_WAKEUPCLOCK_RTCCLK_DIV8);//8/（~32.768KHz）=~0.244ms  WakeUpCounter = ~(10s-200ms)/0,244ms = 40164 = 0x9CE4         (10S-200ms)定时
																						HAL_NVIC_SetPriority(RTC_IRQn, 0x0, 0);//???????
																						HAL_NVIC_EnableIRQ(RTC_IRQn);//??RTC??

																					do
																						{	
																						HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
																						Wake_up_10s--;		
																						KEY_count++;
																						KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
																							if(KEY_count>500)
																							{
																								break;
																							}
																						}while(KEY_State==0);
																						KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);

																						if(KEY_State==0&&KEY_count>500)
																						{	
																							KEY_count=0;
																							SystemPower_Config();
																								hlcd=MX_LCD_SPI_I2C_Init(hlcd);	
																								Data_Display_Num(hlcd,1111);		
																								HAL_LCD_UpdateDisplayRequest(&hlcd);
																								t=0;
																								t=DataEEPROM_Read(USE_COUNT_ADDRESS)+1;
																							
//																							//擦除flash0x08080068
																								DataEEPROM_Write(USE_COUNT_ADDRESS,t);
																								t=0;
																								DataEEPROM_Write(0x08080038,1);//K_start_ADDRESS 
																								DataEEPROM_Write(0x0808003C,1);//K_stop_ADDRESS
																								DataEEPROM_Write(0x08080200,2);
																								DataEEPROM_Write(0x08080198,0);
																								DataEEPROM_Write(0x0808019C,0);
																								DataEEPROM_Write(MAK_NUM_ADDRESS,0);
																								BYTE *Tempe_Data_wendul=mymalloc(512);	
																								TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendul,1663,1);								
																								TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendul, 64, 1);
																								TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendul,1664,1);								
																								TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendul, 70, 1);
																								TM_FATFS_FLASH_SPI_disk_read(Tempe_Data_wendul,1665,1);								
																								TM_FATFS_FLASH_SPI_disk_write(Tempe_Data_wendul, 76, 1);
																								myfree(Tempe_Data_wendul);
																								for(i=0;i<140;i++)
																								{
																									SPI_FLASH_SectorErase(247296+i*4096);
																								}
																								uint32_t Data_wendu_Adress=0x00100000;
																								for(i=0;i<256;i++)
																								{
																									SPI_FLASH_SectorErase(Data_wendu_Adress+i*4096);
																								}
																								HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x1002, RTC_WAKEUPCLOCK_RTCCLK_DIV8);
																								HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
																								Button_One_Flag=1;
																								pdf_end_flag=0;
																								RTC_TIME_record(sdatestructure,stimestructure,End_Y_M_D,End_H_M_S);
																								HAL_NVIC_SystemReset();  
																						}
																						else
																						{
																									__HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle);
																									/* Disable the Wakeup Timer */
																									__HAL_RTC_WAKEUPTIMER_DISABLE(&RtcHandle);
																								KEY_count=0;
																						}
																						
																		}
		
										
			}
}

}
	
	
//}

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
//    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
//    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
//    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

///* ADC init function */
//HAL_StatusTypeDef  MX_ADC_Init(LCD_HandleTypeDef hlcd)
//{

//	
//}
HAL_StatusTypeDef MX_LCD_Voltage_change(LCD_HandleTypeDef hlcd)
{
  uint32_t tickstart = 0x00U;
							MODIFY_REG(hlcd.Instance->FCR, \
             (LCD_FCR_PS | LCD_FCR_DIV | LCD_FCR_BLINK| LCD_FCR_BLINKF | \
             LCD_FCR_DEAD | LCD_FCR_PON | LCD_FCR_CC), \
             (hlcd.Init.Prescaler | hlcd.Init.Divider | hlcd.Init.BlinkMode | hlcd.Init.BlinkFrequency | \
             hlcd.Init.DeadTime | hlcd.Init.PulseOnDuration | hlcd.Init.Contrast | hlcd.Init.HighDrive));
	  tickstart = HAL_GetTick();
  
  /*!< Wait Until the LCD Booster is ready */
  while((((hlcd.Instance->SR & (LCD_FLAG_RDY)) == (LCD_FLAG_RDY)) ) == RESET)
  {
    if((HAL_GetTick() - tickstart ) > 1000U)
    {   
      hlcd.ErrorCode = HAL_LCD_ERROR_RDY;
      return HAL_TIMEOUT;
    } 
  }
	return HAL_OK;
	

}
///* I2C2 init function */
//static void MX_I2C2_Init(void)
//{
//	
//  hi2c2.Instance = I2C2;
//  hi2c2.Init.Timing = 0x00303D5B;
//  hi2c2.Init.OwnAddress1 = 0;
//  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c2.Init.OwnAddress2 = 0;
//  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
//  {
////    Error_Handler();
//  }

//    /**Configure Analogue filter 
//    */
//  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//  {
////    Error_Handler();
//  }

//}

/* LCD init function */
LCD_HandleTypeDef MX_LCD_Init(LCD_HandleTypeDef hlcd)
{
						MX_GPIO_Init();
					//‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘’‘					
						hlcd.Instance = LCD;
						hlcd.Init.Prescaler = LCD_PRESCALER_2;
						hlcd.Init.Divider = LCD_DIVIDER_19;
						hlcd.Init.Duty = LCD_DUTY_1_8;
						hlcd.Init.Bias = LCD_BIAS_1_4;
						hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
						hlcd.Init.Contrast =LCD_CONTRASTLEVEL_0;
						hlcd.Init.DeadTime = LCD_DEADTIME_0;
						hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_1;
						hlcd.Init.HighDrive = LCD_HIGHDRIVE_1;
						hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
						hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV8;
						hlcd.Init.MuxSegment = LCD_MUXSEGMENT_DISABLE;
					
						if (HAL_LCD_Init(&hlcd) != HAL_OK)
						{	
							hlcd.Init.Contrast =LCD_CONTRASTLEVEL_2;
								if (MX_LCD_Voltage_change(hlcd)!= HAL_OK)
								{																													
													hlcd.Init.Contrast =LCD_CONTRASTLEVEL_3;
													if (MX_LCD_Voltage_change(hlcd)!= HAL_OK)
													{																													
																			hlcd.Init.Contrast =LCD_CONTRASTLEVEL_4;
																		if (MX_LCD_Voltage_change(hlcd)!= HAL_OK)
																		{																													
																											
																					CLEAR_BIT(hlcd.Instance->CR, LCD_CR_LCDEN);//lcd 寄存器 清空

																					

																		}


													}

									}
	

						}

							/**Enable the High Driver 
							*/
						__HAL_LCD_HIGHDRIVER_ENABLE(&hlcd);	
						__HAL_LCD_HIGHDRIVER_DISABLE(&hlcd);
						return hlcd;
  
}

uint8_t DISplay_WD_High_Low(LCD_HandleTypeDef hlcd,int16_t T_wenduk,uint8_t LCD_Error_flag_WD)
{
													HAL_LCD_Clear(&hlcd);
													if(LCD_Error_flag_WD==0)
													{

															
																			State_Display(hlcd, LCD_Good); 
																
														}
													if(LCD_Error_flag_WD==1)
													{							

																			State_Display(hlcd, LCD_Error_High_Low);  

													}
											
													return LCD_Error_flag_WD;
}
LCD_HandleTypeDef DISplay_WD_RH(LCD_HandleTypeDef hlcd,int16_t T_wendu2,uint16_t Normal,uint16_t canshu,uint16_t Bat_Day)
{
			uint8_t	C_F_FLAG1=DataEEPROM_Read(0x0808017c);
		if(C_F_FLAG1==1)
		{
			T_wendu2=	T_wendu2*1.8+320;canshu=1;
		}
		All_Time_Display(hlcd);	Bat_Display(hlcd,Bat_Day);
		Data_Display(hlcd,T_wendu2,canshu,Normal);		//????,?????,???MACX???
		if(HAL_LCD_UpdateDisplayRequest(&hlcd)!= HAL_OK)
		{
//				hlcd.Init.Contrast =LCD_CONTRASTLEVEL_2;
//								if (MX_LCD_Voltage_change(hlcd)!= HAL_OK)
//								{
//									hlcd.Init.Contrast =LCD_CONTRASTLEVEL_0;
//															if (MX_LCD_Voltage_change(hlcd)!= HAL_OK)
//															{
//																CLEAR_BIT(hlcd.Instance->CR, LCD_CR_LCDEN);//lcd 寄存器 清空
//															}
//									}
//								
//						__HAL_LCD_HIGHDRIVER_ENABLE(&hlcd);	
//						__HAL_LCD_HIGHDRIVER_DISABLE(&hlcd);						
		}
			
	return hlcd;
}
LCD_HandleTypeDef DISplay_MAX_MIN_AVG(LCD_HandleTypeDef hlcd,int16_t T_wendul,int16_t RH_shidu,uint8_t LCD_Error_flag_WD,uint8_t KEY_State,uint8_t KEY_shidu,uint8_t LCD_Error_flag_RH,int16_t Data_max_wd,int16_t Data_min_wd,int16_t T_average,uint16_t Bat_Day,int16_t Data_max_sd,int16_t Data_min_sd,int16_t RH_average)
{
		KEY_State=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
		uint8_t	C_F_FLAG1=DataEEPROM_Read(0x0808017c);
	uint8_t	canshu=0;uint8_t i=0;	int16_t Data_wd[4]; 
	if(C_F_FLAG1==1)
	{
		canshu=1;

	}
	Data_wd[0]=Data_max_wd;
		Data_wd[1]=Data_min_wd;
		Data_wd[2]=T_average;	
		Data_wd[3]=	T_wendul;
			if(KEY_State==0)
								{
										BSP_STOP_DISable(BUTTON_KEY, BUTTON_MODE_EXTI);
										BSP_PB_DISable(BUTTON_KEY, BUTTON_MODE_EXTI);
									for(i=0;i<4;i++)
									{
											HAL_LCD_Clear(&hlcd);
											HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x335D, RTC_WAKEUPCLOCK_RTCCLK_DIV8);
										if(i==3)
										{
											LCD_Error_flag_WD=DISplay_WD_High_Low(hlcd,T_wendu,LCD_Error_flag_WD);
										}
											hlcd=DISplay_WD_RH(hlcd,Data_wd[i],MAX_display+i,canshu,Bat_Day);
											HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
									}
										BSP_STOP_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
										BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
										Wake_up_10s=Wake_up_10s-2;
										HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x9FE6, RTC_WAKEUPCLOCK_RTCCLK_DIV8);//8/（~32.768KHz）=~0.244ms  WakeUpCounter = ~(10s-200ms)/0,244ms = 40164 = 0x9CE4         (10S-200ms)定时
								}
	return hlcd;
}
uint16_t RTC_TIME_record(RTC_DateTypeDef sdatestructure,RTC_TimeTypeDef stimestructure,uint32_t End_Y_M_D,uint32_t End_H_M_S)
{

						HAL_RTC_GetTime(&RtcHandle, &stimestructure, FORMAT_BCD);
																/* Get the RTC current Date */
						HAL_RTC_GetDate(&RtcHandle, &sdatestructure, FORMAT_BCD);
						End_Y_M_D=((uint32_t)sdatestructure.Year<<16)+((uint32_t)sdatestructure.Month<<8)+(uint32_t)sdatestructure.Date;
						End_H_M_S=((uint32_t)stimestructure.Hours<<16)+((uint32_t)stimestructure.Minutes<<8)+stimestructure.Seconds;
						DataEEPROM_Write(End_Y_M_D_ADDRESS,End_Y_M_D);
						DataEEPROM_Write(End_H_M_S_ADDRESS,End_H_M_S);
						return 0;
}

LCD_HandleTypeDef MX_LCD_SPI_I2C_Init(LCD_HandleTypeDef hlcd)
{
						hlcd=MX_LCD_Init(hlcd);
						MX_SPI2_Init();	
						SPI_FLASH_CS_HIGH();
//						MX_I2C2_Init();	
	return hlcd;
}
/* RTC init function */
static void MX_RTC_Init(RTC_DateTypeDef sdatestructure,RTC_TimeTypeDef stimestructure,RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct)
{

PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;//??LSE??RTC???  
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
  }
  
    /* ??RTC?? */  
  __HAL_RCC_RTC_ENABLE(); 
  
  /* ??RTC???? */  
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;//????
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;//?????
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;//?????
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;//RTC??:???
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;//????
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;//????:??  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Error */
  }
   	/* Configure RTC Calendar */
							sdatestructure.Year = YMD_RTC_Y;
								sdatestructure.Month = YMD_RTC_M;
								sdatestructure.Date = YMD_RTC_D;
								if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,FORMAT_BCD) != HAL_OK)
								{
									/* Error */
								} 

								stimestructure.Hours = HMS_RTC_H;
								stimestructure.Minutes = HMS_RTC_M;
								stimestructure.Seconds = HMS_RTC_S;
								stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
								stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
								stimestructure.StoreOperation = RTC_STOREOPERATION_RESET; 
								if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,FORMAT_BCD) != HAL_OK)
								{
									/* Error */
								}
								
								/* ??????RTC?????0 */
//								HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2); 
  /* ??????????????????RTC????????? */  


}


/* SPI2 init function */
static void MX_SPI2_Init(void)
{
 
SPI_HandleTypeDef SPI2Handle;//??SPI1?????? 
	GPIO_InitTypeDef  GPIO_InitStruct;

    /* ??SPI?? */
    __SPI2_CLK_ENABLE();
    
//    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_13;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;   
    HAL_GPIO_Init( GPIOB, &GPIO_InitStruct);
      
    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;   
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
//    /* SPI MOSI GPIO pin configuration  */
//    GPIO_InitStruct.Pin = GPIO_PIN_15;
//    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;     
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  

    SPI2Handle.Instance               = SPI2;
    SPI2Handle.Init.Mode              = SPI_MODE_MASTER;//SPI??:????    
    SPI2Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;//SPI??????
    SPI2Handle.Init.Direction         = SPI_DIRECTION_2LINES;//?????
    SPI2Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;//????:?2?????
    SPI2Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;//????:?
    SPI2Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;//CRC??:???
    SPI2Handle.Init.CRCPolynomial     = 7;//CRC???
    SPI2Handle.Init.DataSize          = SPI_DATASIZE_8BIT;//????:8?
    SPI2Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;//??????:MSB
    SPI2Handle.Init.NSS               = SPI_NSS_SOFT;//NSS??:????
    SPI2Handle.Init.TIMode            = SPI_TIMODE_DISABLED;//???:??????
    if(HAL_SPI_Init(&SPI2Handle) != HAL_OK)
    {
        /* Initialization Error */
    }
    
    /* ??SPI */        
    __HAL_SPI_ENABLE(&SPI2Handle);  



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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin : FLASH_CS_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);



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
