/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; Yu may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "TaskMetering.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "console.h"
#include "uart.h"
#include "drv_flash.h"
#include "user_comm.h"
#include "proc.h"
#include "bord.h"
#include "adc.h" 
#include "pwm.h"
#include "HT7038bu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define WORK_PARA_SECTOR_ADDR 0x0800FC00

#define WORK_PARA2_SECTOR_ADDR 0x0800F800

__IO ITStatus UartReady = RESET;    
/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT**** ";
/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

uint8_t asdf[3]={0};
    
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc,hadc_1;
DMA_HandleTypeDef hdma_adc;

CRC_HandleTypeDef hcrc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;

TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart1;

IWDG_HandleTypeDef hiwdg;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM6_Init(void);
static void MX_IWDG_Init(void);

double baifen_he=0.50;
double baifen_fen=0.30;
double baifen2_he=0.355;
double baifen2_fen=0.106;
uint32_t timeout=1;
/* USER CODE BEGIN PFP */
uint8_t key_scan(uint16_t delay)
{
	uint16_t  i=0;uint8_t flag=0;
	while(1)
	{
		HAL_Delay(1);
		i++;
		if(IS_SW_TEST_STATUS()==1)
		{
			flag=0;
			break;
		}
		if(IS_SW_TEST_STATUS()==0&&i==delay)
		{
			flag=1;
			break;
			
		}
		
		
	}
	return flag;

}
uint32_t  key_scann()
{
	uint32_t  i=0;
	LED_YELLOW_OFF();
	LED_GREEN_OFF();
	LED_RED_OFF();
	while(1)
	{
		

		if(IS_SW_TEST_STATUS()==1)
		{
			break;
		}
		if(IS_SW_TEST_STATUS()==0)
		{
			i++;timeout=0;

		}
		HAL_Delay(1);
		if(i==1000)
		{
			LED_YELLOW_ON();
		}
		if(i==2500)
		{
			LED_YELLOW_ON();
			LED_GREEN_ON();
			LED_RED_ON();
		}
		if(i>5000)
		{
			LED_YELLOW_OFF();
			LED_GREEN_OFF();
			LED_RED_OFF();
			HAL_Delay(500);
			
			break;
		}
	
		
	}
				
	LED_YELLOW_ON();
	return i;

}
uint8_t moter_openfen1=100;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * ??PWM??   ??
 */
void SW_break_fen(void)
{
  Duty_ratio=moter_openfen1;
  Pwm_Cycle=666;
  MX_TIM15_Init();
  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
static FLASH_EraseInitTypeDef EraseInitStruct;
#define FLASH_USER_START_ADDR       ADDR_FLASH_PAGE_60  /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR         ADDR_FLASH_PAGE_63+ FLASH_PAGE_SIZE   /* End @ of user Flash area */
uint16_t adcvaluep1,adcvaluep1_W,adcvaluep1_m,adcvaluep2,adcvaluep2_W,adcvaluep2_m,adcvaluep3,adcvaluep3_W,adcvaluep3_m,adcvaluep4,adcvaluep4_W,adcvaluep4_m;
uint32_t datap=0;
uint16_t adcvaluep1_avr=0,adcvaluep2_avr=0,adcvaluep3_avr=0,adcvaluep4_avr=0;
void huoqumax_min()
{
			adcvaluep1= ADC_Getvalue(8);//fen
			adcvaluep2= ADC_Getvalue(9);//he
			adcvaluep3=ADC_Getvalue(2);//he deng
			adcvaluep4=ADC_Getvalue(3);//fen deng
			if(adcvaluep1<1900)
			{
				adcvaluep1=adcvaluep1_m;
			}
			if(adcvaluep2<1900)
			{
				adcvaluep2=adcvaluep2_m;
			}
			if(adcvaluep3<1900)
			{
				adcvaluep3=adcvaluep3_m;
			}
			if(adcvaluep4<1900)
			{
				adcvaluep4=adcvaluep4_m;
			}
			if(adcvaluep1>adcvaluep1_W)
			{
				adcvaluep1_W=adcvaluep1;
			}
			if(adcvaluep1<adcvaluep1_m)
			{
				adcvaluep1_m=adcvaluep1;
			}
			if(adcvaluep2>adcvaluep2_W)
			{
				adcvaluep2_W=adcvaluep2;
			}
			if(adcvaluep2<adcvaluep2_m)
			{
				adcvaluep2_m=adcvaluep2;
			}
			if(adcvaluep3>adcvaluep3_W)
			{
				adcvaluep3_W=adcvaluep3;
			}
			if(adcvaluep3<adcvaluep3_m)
			{
				adcvaluep3_m=adcvaluep3;
			}
			if(adcvaluep4>adcvaluep4_W)
			{
				adcvaluep4_W=adcvaluep4;
			}
			if(adcvaluep4<adcvaluep4_m)
			{
				adcvaluep4_m=adcvaluep4;
			}

}
void zidonghuoqu()
{
	uint16_t i=0,k=0,la_flag=0;
				adcvaluep1= ADC_Getvalue(8);//fen
			adcvaluep2= ADC_Getvalue(9);//he
			adcvaluep3=ADC_Getvalue(2);//he deng
			adcvaluep4=ADC_Getvalue(3);//fen deng
			adcvaluep1_W=adcvaluep1;
			adcvaluep2_W=adcvaluep2;
			adcvaluep3_W=adcvaluep3;
			adcvaluep4_W=adcvaluep4;
			adcvaluep1_m=adcvaluep1;
			adcvaluep2_m=adcvaluep2;
			adcvaluep3_m=adcvaluep3;
			adcvaluep4_m=adcvaluep4;
			LED_YELLOW_OFF();LED_RED_OFF();LED_GREEN_OFF();
			if(adcvaluep1<=1900)
			{
				while(1)
				{
					LED_RED_ON();
					HAL_Delay(500);
					LED_RED_OFF();
					HAL_Delay(500);
				}
			}
			if(adcvaluep2<=1900)
			{
				while(1)
				{
					LED_GREEN_ON();
					HAL_Delay(500);
					LED_GREEN_OFF();
					HAL_Delay(500);
				}
			}
			if(adcvaluep3<=1900)
			{
				while(1)
				{
					LED_YELLOW_ON();
					HAL_Delay(500);
					LED_YELLOW_OFF();
					HAL_Delay(500);
				}
			}
			if(adcvaluep4<=1900)
			{
				while(1)
				{
					LED_YELLOW_ON();
				}
			}
			while(1)
			{
					SW_break();
					HAL_Delay(1);
					HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
					HAL_Delay(4);
					adcvaluep3=ADC_Getvalue(2);//he deng
					adcvaluep4=ADC_Getvalue(3);//fen deng
					if(adcvaluep4>adcvaluep3)
					{
						
						huoqumax_min();
						break;

					}
			}
			while(1)
			{
					SW_break();
					HAL_Delay(1);
					HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
					HAL_Delay(4);
					adcvaluep3=ADC_Getvalue(2);//he deng
					adcvaluep4=ADC_Getvalue(3);//fen deng
					if(adcvaluep3>adcvaluep4)
					{
						
						huoqumax_min();
						break;

					}
			}
		
		while(1)
		{
			SW_break();
			HAL_Delay(1);
			HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
			HAL_Delay(4);
			adcvaluep1= ADC_Getvalue(8);//fen
			adcvaluep2= ADC_Getvalue(9);//he
			adcvaluep3=ADC_Getvalue(2);//he deng
			adcvaluep4=ADC_Getvalue(3);//fen deng
			huoqumax_min();
			if(adcvaluep3-adcvaluep3_m<adcvaluep4-adcvaluep4_m&&i==0)
			{
				i=1;
			}
			if(adcvaluep1-adcvaluep1_m>adcvaluep3-adcvaluep3_m&&i==1)
			{
				i=2;
			}
			if(adcvaluep2-adcvaluep2_m>adcvaluep4-adcvaluep4_m&&i==2)
			{
				i=3;
			}

			if(adcvaluep3-adcvaluep3_m>adcvaluep1-adcvaluep1_m&&i==3)
			{

					k++;
					if(k<2)
					{
					i=0;
					}else
					{
						i=4;
					
					}
				
				
			}
			if(k==2)
			{
				adcvaluep1_avr=adcvaluep1_W-adcvaluep1_m;
				adcvaluep2_avr=adcvaluep2_W-adcvaluep2_m;
				adcvaluep3_avr=adcvaluep3_W-adcvaluep3_m;
				adcvaluep4_avr=adcvaluep4_W-adcvaluep4_m;
				break;
			}
		}

		
			while(la_flag==0)
			{
						SW_break();
						HAL_Delay(1);
						HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
						HAL_Delay(4);
						adcvaluep2= ADC_Getvalue(9);//he
						if(adcvaluep2_avr*baifen2_he<adcvaluep2-adcvaluep2_m&&SW_ON)
						{
							if(SW_ON)
							{
									LED_RED_ON();
							}
								
							la_flag=1;
							last_switchstate=1;
						}
				}	
		

}
void flash_beifen()
{
	uint32_t Address=0;
			HAL_FLASH_Unlock();
		Address = FLASH_USER_START_ADDR;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, 1) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, adcvaluep1_avr) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, adcvaluep2_avr) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, adcvaluep3_avr) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, adcvaluep4_avr) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, adcvaluep1_m) ;
		Address = Address + 4;	
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, adcvaluep2_m) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, adcvaluep3_m) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, adcvaluep4_m) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, (uint32_t)(baifen2_he*10000)) ;
		Address = Address + 4;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, (uint32_t)(baifen2_fen*10000)) ;
		HAL_FLASH_Lock();

}
void flash_read()
{
			uint32_t data32=0;
			uint32_t Address=0;
			Address = FLASH_USER_START_ADDR;
			Address = Address + 4;
			adcvaluep1_avr=*(__IO uint32_t *)Address;
			Address = Address + 4;
			adcvaluep2_avr=*(__IO uint32_t *)Address;
			Address = Address + 4;
			adcvaluep3_avr=*(__IO uint32_t *)Address;
			Address = Address + 4;
			adcvaluep4_avr=*(__IO uint32_t *)Address;
			Address = Address + 4;
			adcvaluep1_m=*(__IO uint32_t *)Address;
			Address = Address + 4;
			adcvaluep2_m=*(__IO uint32_t *)Address;
			Address = Address + 4;
			adcvaluep3_m=*(__IO uint32_t *)Address;
			Address = Address + 4;
			adcvaluep4_m=*(__IO uint32_t *)Address;
			Address = Address + 4;
			data32=*(__IO uint32_t *)Address;
			baifen2_he=((double)data32)/10000;
			Address = Address + 4;
			data32=*(__IO uint32_t *)Address;
			baifen2_fen=((double)data32)/10000;



}
void flash_ERASH()
{
					uint32_t PageError = 0;
					HAL_FLASH_Unlock();
					/* Fill EraseInit structure*/
					EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
					EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
					EraseInitStruct.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

					if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
					{
				
					}
					HAL_FLASH_Lock();

}
void dengdai_queren()
{
	static	uint32_t clktick=0;
	uint8_t open_flag=0,jiaozhun_flag=0;
	if(SW_OFF)
	{
		last_switchstate=0;open_flag=0;
	}
	if(!SW_ON)
	{
		
	last_switchstate=swich_prama(last_switchstate);
	}
	last_switchstate=1;open_flag=1;
	TMS_INIT(clktick);
	
	while(1)
	{
						if (SW_OFF) {
						LED_GREEN_ON();
						LED_RED_OFF();
						} else if (SW_ON){
							LED_RED_ON();
							LED_GREEN_OFF();
						}else{
							LED_RED_OFF();
							LED_GREEN_OFF();
						}
						if(SW_OFF)
						{
							if(last_switchstate!=0&&jiaozhun_flag==0)
							{
								jiaozhun_flag++;
							}
							last_switchstate=0;
						}
						if(SW_ON)
						{
							if(last_switchstate!=1&&jiaozhun_flag==1)
							{
								jiaozhun_flag++;
							}
							last_switchstate=1;
						}
						if(jiaozhun_flag==1)
						{
							open_flag=0;
						}
						if(jiaozhun_flag>=2)
						{
								break;
						}
						
		 if (TMS_IS_OVER(clktick, 3000)) 
			 {
						TMS_INIT(clktick);
						if(open_flag==0)
						{
								baifen2_fen-=0.02;
								if(baifen2_fen<=0.03)
								{
									break;
								}
								last_switchstate=swich_prama(last_switchstate);HAL_Delay(134);
								last_switchstate=swich_prama(last_switchstate);
								
						}
						if(open_flag==1)
						{
								
								baifen2_he+=0.02;
								if(baifen2_he>=1)
								{
									baifen2_he=1;break;
								}
								last_switchstate=swich_prama(last_switchstate);HAL_Delay(134);
								last_switchstate=swich_prama(last_switchstate);
						}
				 
			 }
	}
	if(SW_OFF)
	{
		last_switchstate=1;
	}
	if(SW_ON)
	{
		
	last_switchstate=0;
	}
	
	

}	uint32_t adcvaluew=0,timeouy=0;
int main(void)
{
	uint16_t i=0,k=0;uint32_t time=0;uint8_t za_flag=0,tflag=0,hf_flag=0,ts_flag=0;;
	uint16_t data32=0;uint32_t Address=0;uint32_t V_A=0,V_C=0,V_B=0,I_C=0;int16_t P_C=0;

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
 /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
   HAL_Init();
last_switchstate=0;
  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_DMA_Init();
  MX_ADC_Init();//»ô¶ûÍ¨µÀ1
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_CRC_Init();
 MX_FATFS_Init();
  MX_TIM6_Init();
  HAL_TIM_Base_Start_IT(&htim6); 
	Address = FLASH_USER_START_ADDR;
	datap=*(__IO uint32_t *)Address;
	datap=*(__IO uint32_t *)Address;
	if(datap==1)
	{
		flash_read();
	}
	if(datap>0xFFFF)
	{
		zidonghuoqu();
		dengdai_queren();
		if(!(baifen2_he==1||baifen2_fen<0.02))
		{
			flash_beifen();
		}
	}
			
	if(key_scan(1500)==1)
	{

			zidonghuoqu();
			dengdai_queren();

		
		while(1)
		{


			time=key_scann();
			if(IS_SW_TEST_STATUS()==1)
			{
				if(timeout!=0)
				{
					HAL_Delay(1);
					timeout++;
						
				}
				if(timeout>=10000)
				{
					break;
				}
				
			}
			if(SW_OFF)
			{
					za_flag=0;
					LED_GREEN_ON();
					LED_RED_OFF();
			
			}else if(SW_ON)
			{
					za_flag=1;
					LED_GREEN_OFF();
					LED_RED_ON();
					

			}else
			{
					za_flag=2;
					LED_GREEN_OFF();
					LED_RED_OFF();
			}
			
				if(5000<time)//10s
				{
					flash_ERASH();
					flash_beifen();
						break;
//					HAL_NVIC_SystemReset();
				
				}
				if(1000<time&&time<2000)//2s
				{
					if(ts_flag==0)
					{
						baifen2_he+=0.02;
					}else if(ts_flag==1)
					{
						baifen2_fen-=0.005;
//						moter_openfen1-=5;
//						if(moter_openfen1==0)
//						{
//							moter_openfen1=100;
//						}
					}
				
//					HAL_NVIC_SystemReset();
				}
				if(2000<time&&time<4000)//5s
				{
					ts_flag++;
				}
				if(time<1000&&time>10)
				{
						
							last_switchstate=swich_prama(last_switchstate);
						
				}
		
		}
		
		
	}

////////	
  MX_IWDG_Init();

  /* USER CODE BEGIN 2 */
  __HAL_UART_ENABLE_IT(&huart1, USART1_IRQn);
  uart_fifoinit(); 
  workparam_init();
  init_HT7038bu();
//  adjust_cali_para_all();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);
  /*##-4- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }
  LED_YELLOW_OFF();
//  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
//  {
//    Error_Handler();
//  }
  //ADC_start();

   while (1){
    /* USER CODE END WHILE */
 
//    /* USER CODE BEGIN 3 */
    static uint32_t getadctick = 0;  
    if(TMS_IS_OVER(getadctick, 100))
    {
      TMS_INIT(getadctick);
			adcvalue=ADC_Getvalue(4);
    }
    static uint32_t HT7038currenttick = 0;
    static uint32_t alarm_tick = 0;    
    if (TMS_IS_OVER(HT7038currenttick, 200)) {
      TMS_INIT(HT7038currenttick);
				HT7038pro();
				adcpro();
    }   
     swtich_pro();
    
     led_pro();
     
     if (TMS_IS_OVER(alarm_tick, 1000)) {
      TMS_INIT(alarm_tick);
      alarm_pro();
     } 

     uartdataproc();   
     HAL_IWDG_Refresh(&hiwdg);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

 /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{
uint8_t rdl_buff[3],wrl_buf[4];
  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
		wrl_buf[0]=0x00;
	SPI_CS_CLEAR();
    TS_DELAY(5);
    HAL_SPI_Transmit(&hspi1,wrl_buf,1,10);
    HAL_SPI_Receive(&hspi1, rdl_buff, 3,20);
	SPI_CS_SET();
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 47;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}



/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI1_CS_Pin, GPIO_PIN_SET);


  /*Configure GPIO pin Output Level */
  
  HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OFF_LED_Pin|ON_LED_Pin|DEBUG_TX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : RS485_EN_Pin */
  GPIO_InitStruct.Pin = RS485_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS485_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SW_TEST_Pin */
  GPIO_InitStruct.Pin = SW_TEST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW_TEST_GPIO_Port, &GPIO_InitStruct);

//  /*Configure GPIO pins : SET_NUMBER_IN_Pin OV_DETE_Pin UV_DETE_Pin HT7038_IRQ_Pin */
//  GPIO_InitStruct.Pin = OV_DETE_Pin|UV_DETE_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = HT7038_RET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
 /*Configure GPIO pin : SET_NUMBER_OUT_Pin */
  GPIO_InitStruct.Pin = SET_NUMBER_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SET_NUMBER_IN_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = SET_NUMBER_IN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SET_NUMBER_IN2_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = SET_NUMBER_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SET_NUMBER_OUT_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = SET_NUMBER_OUT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SET_NUMBER_OUT2_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SET_NUMBER_OUT_GPIO_Port, SET_NUMBER_OUT_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SET_NUMBER_OUT2_GPIO_Port, SET_NUMBER_OUT2_Pin, GPIO_PIN_RESET);
  
  /*Configure GPIO pins : TRIP_DETE1_Pin TRIP_DETE2_Pin SW_ON_DETE_Pin SW_OFF_DETE_Pin */
  GPIO_InitStruct.Pin = SW_ON_DETE_Pin|SW_OFF_DETE_Pin;//TRIP_DETE1_Pin|TRIP_DETE2_Pin|
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MOTOR_DRIVE_Pin OFF_LED_Pin ON_LED_Pin */
  GPIO_InitStruct.Pin = FAULT_LED_Pin|OFF_LED_Pin|ON_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DEBUG_TX_Pin */
  GPIO_InitStruct.Pin = DEBUG_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DEBUG_TX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DEBUG_RX_Pin HT7038_PF_Pin HT7038_QF_Pin */
  GPIO_InitStruct.Pin = DEBUG_RX_Pin|HT7038_PF_Pin|HT7038_QF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LEAKAGE_STATUS_DETE_Pin */
  GPIO_InitStruct.Pin = LEAKAGE_STATUS_DETE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(LEAKAGE_STATUS_DETE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LEAKAGE_SW_DETE_Pin */
  GPIO_InitStruct.Pin = LEAKAGE_SW_DETE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(LEAKAGE_SW_DETE_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Window = 3125;
  hiwdg.Init.Reload = 3125;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
