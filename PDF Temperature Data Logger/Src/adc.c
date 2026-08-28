/*******************************************************************************
** 版权:		    利尔达科技有限公司
** 文件名: 		  uart.c
** 版本：  	    V1.00   
** 工作环境: 	  RealView MDK-ARM V5.11
** 作者: 		    ydc
** 功能:		    UART驱动						
** 日期: 	      2014-08-12
** 修改日志：	  2015-01-19
                  1、整理、添加注释
** 版权所有     (C)2014-2015 利尔达科技有限公司
*******************************************************************************/
//#include "stm32l0xx_hal_adc_ex.h"

#include "adc.h"
//#include "rcc.h"
#include "stm32l0xx_hal.h"

#include "stm32l0xx_hal_adc.h"
//#include "spi.h"
//#include "uart.h"

#include "stm32l0xx_hal_def.h"

//float  T_wendu=0; //温度变量
int16_t  T_wendu=0; //温度变量
uint16_t const Temp_Tab[101]={
17918,16833,15822,14878,13995,13171,12400,11681,11006,10375,//[-30,21]
9784,9230,8711,8224,7768,7340,6938,6560,6206,5873,//[-20,-11]
5560,5264,4987,4726,4481,4049,4030,3825,3631,3448,//[-10,-1]
3276,3112,2959,2813,2676,2546,2423,2307,2197,2093,//[0,9]
1994,1901,1812,1728,1649,1574,1502,1434,1370,1308,//[10,19]
1250,1195,1142,1093,1045,1000,957,916,877,840,//[20,29]
805,771,739,709,680,652,626,600,576,553,//[30,39]
531,510,490,471,453,435,419,403,387,373,//[40,49]
359,345,333,320,308,297,286,276,266,256,//[50,59]
247,238,230,222,214,207,199,192,186,180,//[60,69]
174};//[70]

//uint16_t const Temp_Tab[81]={
//17927,16841,15827,14882,13998,13173,12402,11681,11006,10375,//-30~-21
//9784,																									//-20
//9230,8711,8225,7758,7340,6938,6561,6206,5873,5560,		// -19~-10?
//5265,4987,4726,4481,4249,4031,3825,3631,3448,3275,  // -9?~0?				
//3112,2958,2813,2676,2546,2423,2307,2197,2092,1994,// 1?~10?	 	// 40?~31?
//1900,1812,1728,1649,1573,1502,1434,1369,1308,1250,// 11?~20?	 // 30?~21?
//1195,1142,1093,1045,1000,957,916,877,840,805,	 	// 21?~30?    		// 20?~11?
//771,740,709,680,652,626,601,576,553,531,			// 31?~40?
//510,490,471,453,436,419,403,387,373,359				// 41?~50?
//};

//uint16_t const Temp_Tab[1001]={										//分辨率为0.1°C	

//17918,																														// -30.0°C		
//17805,17695,17585,17476,17366,17258,17151,17044,16939,16833,			// -29.9~-29.0°C		
//16729,16625,16522,16421,16318,16219,16118,16018,15920,15822,			// -28.9~-28.0°C	
//15725,15629,15532,15436,15342,15247,15155,15061,14969,14878,			// -27.9~-27.0°C		079079
//	
//14786,14696,14607,14518,14428,14341,14254,14167,14080,13995,			// -26.9~-26.0°C		
//13910,13826,13743,13659,13577,13494,13412,13332,13251,13171,			// -25.9~-25.0°C	
//13092,13012,12935,12857,12780,12702,12627,12551,12475,12400,			// -24.9~-24.0°C	
//12327,12253,12180,12107,12034,11962,11891,11820,11750,11681,			// -23.9~-23.0°C	
//11611,11542,11474,11405,11338,11271,11203,11138,11072,11006,			// -22.9~-22.0°C	
//10941,10876,10813,10748,10686,10622,10560,10498,10436,10375,			// -21.9~-21.0°C	
//10314,10254,10194,10134,10075,10016,9958,9900,9842,9784,					// -20.9~-20.0°C
//9726,9670,9614,9558,9502,9447,9393,9338,9284,9230,								// -19.9~-19.0°C
//9177,9123,9071,9018,8967,8914,8863,8813,8762,8711,								// -18.9~-18.0°C	
//8661,8611,8562,8513,8464,8416,8367,8320,8272,8224,								// -17.9~-17.0°C		
//8177,8131,8085,8039,7993,7947,7902,7857,7813,7768,								// -16.9~-16.0°C	
//7724,7681,7637,7594,7551,7508,7466,7424,7381,7340,								// -15.9~-15.0°C	
//7299,7258,7217,7176,7136,7096,7056,7017,6977,6938,								// -14.9~-14.0°C	
//6900,6861,6822,6784,6746,6709,6671,6634,6598,6560,								// -13.9~-13.0°C		
//6524,6488,6452,6416,6381,6345,6310,6275,6241,6206,								// -12.9~-12.0°C	
//6172,6138,6104,6070,6037,6004,5971,5938,5905,5873,								// -11.9~-11.0°C	
//5841,5809,5777,5745,5714,5683,5651,5620,5590,5560,								// -10.9~-10.0°C
//5529,5499,5469,5439,5410,5380,5351,5322,5293,5264,								// -9.9°C~-9.0°C	
//5236,5208,5180,5151,5124,5096,5069,5041,5014,4987,								// -8.9°C~-8.0°C
//4961,4934,4907,4881,4855,4829,4803,4777,4752,4726,								// -7.9°C~-7.0°C	
//4701,4676,4651,4626,4602,4577,4553,4528,4504,4481,								// -6.9°C~-6.0°C	
//4457,4433,4410,4386,4363,4340,4317,4294,4271,4049,								// -5.9°C~-5.0°C
//4227,4204,4182,4160,4138,4116,4095,4073,4052,4030,								// -4.9°C~-4.0°C	
//4009,3989,3968,3947,3926,3906,3885,3865,3845,3825,								// -3.9°C~-3.0°C		
//3805,3785,3766,3746,3726,3707,3688,3669,3650,3631,								// -2.9°C~-2.0°C		
//3612,3593,3575,3557,3538,3520,3512,3484,3466,3448,								// -1.9°C~-1.0°C	
//3430,3413,3395,3378,3360,3343,3326,3309,3292,3276,								// -0.9°C~0.0°C		
//3259,3242,3226,3209,3193,3176,3160,3144,3129,3112,								// 0.1°C~1.0°C
//3097,3081,3066,3050,3035,3019,3004,2989,2974,2959,								// 1.1°C~2.0°C 
//2944,2929,2914,2900,2885,2871,2856,2842,2828,2813,								// 2.1°C~3.0°C
//2800,2785,2771,2757,2744,2730,2716,2703,2689,2676,								// 3.1°C~4.0°C
//2663,2649,2636,2623,2610,2597,2584,2572,2559,2546,								// 4.1°C~5.0°C
//2533,2521,2508,2496,2484,2471,2459,2447,2435,2423,								// 5.1°C~6.0°C
//2411,2399,2388,2376,2364,2353,2341,2330,2318,2307,								// 6.1°C~7.0°C
//2296,2284,2273,2262,2251,2240,2229,2219,2208,2197,								// 7.1°C~8.0°C
//2186,2176,2165,2155,2144,2134,2124,2113,2103,2093,								// 8.1°C~9.0°C
//2083,2073,2063,2053,2043,2033,2023,2014,2004,1994,								// 9.1°C~10.0°C
//1985,1975,1966,1956,1947,1938,1928,1919,1910,1901,								// 10.1°C~11.0°C
//1892,1883,1874,1865,1856,1847,1838,1830,1821,1812,								// 11.1°C~12.0°C
//1804,1795,1787,1778,1770,1761,1753,1745,1737,1728,								// 12.1°C~13.0°C
//1720,1712,1704,1696,1688,1680,1672,1665,1657,1649,								// 13.1°C~14.0°C
//1641,1634,1626,1618,1611,1603,1596,1588,1581,1574,								// 14.1°C~15.0°C
//1566,1559,1552,1544,1537,1530,1523,1516,1509,1502,								// 15.1°C~16.0°C
//1495,1488,1481,1474,1468,1461,1454,1447,1441,1434,								// 16.1°C~17.0°C	
//1427,1421,1414,1408,1401,1395,1389,1382,1376,1370,								// 17.1°C~18.0°C	
//1363,1357,1351,1345,1339,1332,1326,1320,1314,1308,								// 18.1°C~19.0°C
//1302,1296,1291,1285,1279,1273,1267,1262,1256,1250,// 19.1°C~20.0°C
//1245,1239,1233,1228,1222,1217,1211,1206,1200,1195,// 20.1°C~21.0°C
//1190,1184,1179,1174,1168,1163,1158,1153,1148,1142,// 21.1°C~22.0°C
//1137,1132,1127,1122,1117,1112,1107,1102,1097,1093,// 22.1°C~23.0°C
//1088,1083,1078,1073,1069,1064,1059,1054,1050,1045,// 23.1°C~24.0°C
//1041,1036,1031,1027,1022,1018,1013,1009,1004,1000,// 24.1°C~25.0°C
//996,991,987,983,978,974,970,965,961,957,					// 25.1°C~26.0°C
//953,949,945,940,936,932,928,924,920,916,					// 26.1°C~27.0°C
//912,908,904,900,897,892,889,885,881,877,					// 27.1°C~28.0°C
//873,870,866,862,859,855,851,848,844,840,					// 28.1°C~29.0°C
//837,833,829,826,822,819,815,812,808,805,					// 29.1°C~30.0°C
//802,798,794,791,788,785,781,778,775,771,					// 30.1°C~31.0°C
//768,765,762,758,755,752,749,746,743,739,					// 31.1°C~32.0°C
//736,733,730,727,724,721,718,715,712,709,					// 32.1°C~33.0°C
//706,703,700,697,694,691,688,686,683,680,					// 33.1°C~34.0°C
//677,674,671,669,666,663,660,658,655,652,					// 34.1°C~35.0°C
//649,647,644,641,639,636,633,631,628,626,					// 35.1°C~36.0°C
//623,620,618,615,613,610,608,605,603,600,					// 36.1°C~37.0°C
//598,595,593,591,588,586,583,581,579,576,					// 37.1°C~38.0°C
//574,572,569,567,565,562,560,558,556,553,					// 38.1°C~39.0°C
//551,549,547,544,542,540,538,536,533,531,					// 39.1°C~40.0°C
//529,527,525,523,521,519,517,514,512,510,					// 40.1°C~41.0°C
//508,506,504,502,500,498,496,494,492,490,					// 41.1°C~42.0°C
//488,486,484,483,481,479,477,475,473,471,					// 42.1°C~43.0°C
//469,467,465,464,462,460,458,457,455,453,					// 43.1°C~44.0°C
//451,449,448,446,444,442,440,439,437,435,					// 44.1°C~45.0°C
//434,432,430,429,427,425,424,422,420,419,					// 45.1°C~46.0°C
//417,415,414,412,411,409,407,406,404,403,					// 46.1°C~47.0°C
//401,400,398,396,395,393,392,390,389,387,					// 47.1°C~48.0°C
//386,384,383,381,380,379,377,376,374,373,					// 48.1°C~49.0°C
//371,370,369,367,366,364,363,362,360,359,					// 49.1°C~50.0°C
//357,356,355,353,352,351,349,348,347,345,					// 50.1°C~51.0°C
//344,343,341,340,339,338,336,335,334,333,					// 51.1°C~52.0°C
//331,330,329,328,326,325,324,323,321,320,					// 52.1°C~53.0°C
//319,318,317,315,314,313,312,311,309,308,					// 53.1°C~54.0°C
//307,306,305,304,303,302,301,299,298,297,					// 54.1°C~55.0°C
//296,295,294,293,292,291,290,289,287,286,					// 55.1°C~56.0°C
//285,284,283,282,281,280,279,278,277,276,					// 56.1°C~57.0°C
//275,274,273,272,271,270,269,268,267,266,					// 57.1°C~58.0°C
//265,264,263,262,261,260,259,258,257,256,					// 58.1°C~59.0°C
//255,254,253,252,251,250,249,248,247,247,					// 59.1°C~60.0°C
//246,245,244,243,243,242,241,240,239,238,					// 60.1°C~61.0°C
//237,237,236,235,234,233,232,231,231,230,					// 61.1°C~62.0°C
//229,228,227,226,225,224,223,223,222,222,					// 62.1°C~63.0°C
//221,220,219,219,218,217,216,215,215,214,					// 63.1°C~64.0°C
//213,212,212,211,210,209,209,208,207,207,					// 64.1°C~65.0°C
//206,205,204,204,203,202,201,201,200,199,					// 65.1°C~66.0°C
//199,198,197,197,196,195,195,194,193,192,					// 66.1°C~67.0°C
//192,191,191,190,189,189,188,187,187,186,					// 67.1°C~68.0°C
//185,185,184,183,183,182,181,181,180,180,					// 68.1°C~69.0°C
//179,178,178,177,177,176,175,175,174,174,					// 69.1°C~70.0°C
//};



//ADC_HandleTypeDef Adc1Handle;//用于ADC1处理的结构体
//ADC_HandleTypeDef Adc2Handle;//用于ADC2处理的结构体

/*******************************************************************************
** 功能	      ADC1初始化
** 参数       无
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/

/**
  * Brief   This function enables the clock in the RCC for the ADC
  *        and for the System configuration (mandatory to enable VREFINT)
  * Param   None
  * Retval  None
  */
__INLINE void SetClockForADC(void)
{
  /* (1) Enable the peripheral clock of the ADC and SYSCFG */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_SYSCFGEN; /* (1) */
}


/**
  * Brief   This function performs a self-calibration of the ADC
  * Param   None
  * Retval  None
  */
__INLINE void  CalibrateADC(void)
{
  /* (1) Ensure that ADEN = 0 */
  /* (2) Clear ADEN */ 
  /* (3) Set ADCAL=1 */
  /* (4) Wait until EOCAL=1 */
  /* (5) Clear EOCAL */
  if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
  {
    ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);  /* (2) */  
  }
  ADC1->CR |= ADC_CR_ADCAL; /* (3) */
  while ((ADC1->ISR & ADC_ISR_EOCAL) == 0) /* (4) */
  {
    /* For robust implementation, add here time-out management */
  }  
  ADC1->ISR |= ADC_ISR_EOCAL; /* (5) */
}

/**
  * Brief   This function configure the ADC to convert the internal reference voltage (VRefInt)
  *         The conversion frequency is 16 MHz 
  * Param   None
  * Retval  None
  */
__INLINE void ConfigureADC(void)
{
  /* (1) Select HSI16 by writing 00 in CKMODE (reset value) */ 
  /* (2) Select the auto off mode */
  /* (3) Select CHSEL17 for VRefInt */
  /* (4) Select a sampling mode of 111 i.e. 239.5 ADC clk to be greater than 17.1us */
  /* (5) Wake-up the VREFINT (only for VLCD, Temp sensor and VRefInt) */
  /* (6) Enable VREFINT buffer  for ADC by setting EN_VREFINT and 
         ENBUF_VREFINT_ADC bit in SYSCFG_CFGR3 */
  /* (7) Wait for VREFINT ADC buffer ready */
  
	ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE; /* (1) */  
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1; /* 10: PCLK/4 (Synchronous clock mode)*/ 
	
  ADC1->CFGR1 |= ADC_CFGR1_AUTOFF; /* (2) */
  ADC1->CHSELR = ADC_CHSELR_CHSEL17; /* (3) */
  ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; /* (4) */
  ADC->CCR |= ADC_CCR_VREFEN; /* (5) */
  SYSCFG->CFGR3 |= SYSCFG_CFGR3_EN_VREFINT 
                 | SYSCFG_CFGR3_ENBUF_VREFINT_ADC; /* (6) */
  while ((SYSCFG->CFGR3 & SYSCFG_CFGR3_VREFINT_ADC_RDYF) == 0) /* (7) */
  {
    /* For robust implementation, add here time-out management */
  }
}


/**
  * Brief   This function enables the ADC
  * Param   None
  * Retval  None
  */
 void EnableADC(void)
{
  /* (1) Enable the ADC */
  /* (2) Wait until ADC ready if AUTOFF is not set */
  ADC1->CR |= ADC_CR_ADEN; /* (1) */
  if ((ADC1->CFGR1 &  ADC_CFGR1_AUTOFF) == 0)
  {
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* (2) */
    {
      /* For robust implementation, add here time-out management */
    }
  }
}


/**
  * Brief   This function disables the ADC
  * Param   None
  * Retval  None
  */
 void DisableADC(void)
{
  /* (1) Ensure that no conversion on going */
  /* (2) Stop any ongoing conversion */
  /* (3) Wait until ADSTP is reset by hardware i.e. conversion is stopped */
  /* (4) Disable the ADC */
  /* (5) Wait until the ADC is fully disabled */
  if ((ADC1->CR & ADC_CR_ADSTART) != 0) /* (1) */
  {
    ADC1->CR |= ADC_CR_ADSTP; /* (2) */
  }
  while ((ADC1->CR & ADC_CR_ADSTP) != 0) /* (3) */
  {
     /* For robust implementation, add here time-out management */
  }
  ADC1->CR |= ADC_CR_ADDIS; /* (4) */
  while ((ADC1->CR & ADC_CR_ADEN) != 0) /* (5) */
  {
    /* For robust implementation, add here time-out management */
  }  
}

void ADC1_Init(void)
{
//  ADC_HandleTypeDef Adc1Handle;  
//	ADC1_BaseConfig();
//  __HAL_ADC_ENABLE(&Adc1Handle);
}
/*******************************************************************************
** 功能	      ADC1初始化  采集内部基准电压初始化函数
** 参数       无
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void ADC1_Verf_Init(void)   //采集内部基准电压初始化函数
{
//	ADC_HandleTypeDef Adc1Handle;
//	
//	PWR_CSR_VREFINTRDYF;  
//	ADC_CCR_VREFEN;          //使能内部参考 
//	ADC1_BaseConfig();
//	__HAL_ADC_ENABLE(&Adc1Handle); //使能ADC
}


/*******************************************************************************
** 功能	     PA0  PA2 的初始化设置
** 参数       无
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void ADC_GPIO_Init(void)   //PA0  PA2 的初始化设置
{
    GPIO_InitTypeDef GPIO_InitStruct;
  
  
    /* 使能通道对应的GPIO时钟 */    
    __GPIOC_CLK_ENABLE();
  
    /* 初始化通道对应的GPIO  PA0 做为输出电源 */    
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//推挽输出

    GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;//输出速度：快速
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
   
  	PC3_ENABLE;                             //初始化阶段先不打开
	
     /* 初始化通道对应的GPIO  PA2 做为输入地 */ 
	
    GPIO_InitStruct.Pin = GPIO_PIN_1;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//推挽输出
	
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		PC1_DISABLE;													//初始化阶段先关闭
}


/*******************************************************************************
** 功能	      ADC1基本配置，各通道适用             
** 参数       无
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void ADC1_BaseConfig(ADC_HandleTypeDef* Adc1Handle)
{   
// ADC_HandleTypeDef Adc1Handle;
	
	//ADC_HandleTypeDef Adc1Handle;
	/* Enable ADC1 clock */
    __ADC1_CLK_ENABLE();
    
    /* 初始化ADC参数 */
    Adc1Handle->Instance = ADC1;
    Adc1Handle->Init.OversamplingMode      = DISABLE;//过采样模式：失能
    Adc1Handle->Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;//ADC时钟预分频ADC_CLOCKPRESCALER_PCLK_DIV2
   // Adc1Handle.Init.LowPowerAutoOff       = DISABLE;//自动关闭模式（转换结束后自动失能ADC直到下次触发）：失能
	  Adc1Handle->Init.LowPowerAutoWait       = ENABLE;//自动关闭模式（转换结束后自动失能ADC直到下次触发）：失能
    Adc1Handle->Init.LowPowerFrequencyMode = DISABLE;//当ADC时钟频率小于2.8MHz时，低频模式必须使能;
    Adc1Handle->Init.LowPowerAutoWait      = ENABLE;//自动等待模式（转换结束后等待读取DR才可以进行下次转换）：使能;
    Adc1Handle->Init.Resolution            = ADC_RESOLUTION12b;//分辨率(6/8/10/12bit)
  //  Adc1Handle.Init.SamplingTime          = ADC_SAMPLETIME_7CYCLES_5;//采样时间，tCONV = Sampling time + 12.5 x ADC clock cycles   ADC_SAMPLETIME_28CYCLES_5
	  Adc1Handle->Init.SamplingTime          = ADC_SAMPLETIME_239CYCLES_5;//采样时间，tCONV = Sampling time + 12.5 x ADC clock cycles   ADC_SAMPLETIME_28CYCLES_5
    Adc1Handle->Init.ScanConvMode         = ADC_SCAN_ENABLE;//通道扫描方向
    Adc1Handle->Init.DataAlign             = ADC_DATAALIGN_RIGHT;//数据对齐方式
    Adc1Handle->Init.ContinuousConvMode    = DISABLE;//连续转换方式（每次转换一个序列（所有通担├后继续下一次转换）：失能
    Adc1Handle->Init.DiscontinuousConvMode = ENABLE;//非连续转换方式（一个序列中，一个触发事件只转换一个通道），与上面的模式不能同时使能
  //  Adc1Handle.Init.ContinuousConvMode    = ENABLE;//连续转换方式（每次转换一个序列（所有通担├后继续下一次转换）：失能
   // Adc1Handle.Init.DiscontinuousConvMode = DISABLE;//非连续转换方式（一个序列中，一个触发事件只转换一个通道），与上面的模式不能同时使能
    Adc1Handle->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIG_EDGE_NONE;//外部触发转换边沿
    Adc1Handle->Init.EOCSelection          = EOC_SINGLE_CONV;//转换结束选择标志，用于库的轮询和中断API
    Adc1Handle->Init.DMAContinuousRequests = DISABLE;//DMA连续模式
    if (HAL_ADC_Init(Adc1Handle) != HAL_OK)
    {
     }  
    /* ADC校准 */
    if (HAL_ADCEx_Calibration_Start(Adc1Handle, ADC_SINGLE_ENDED) != HAL_OK)
    {
     }
	
}

/*******************************************************************************
** 功能	      ADC1通道配置   
** 参数       无
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void ADC1_ChannelConfig(uint16_t GPIO_PIN_N)

{
//  ADC_HandleTypeDef Adc1Handle; 
//	GPIO_InitTypeDef GPIO_InitStruct;
//    ADC_ChannelConfTypeDef sConfig;
  
//    /* 使能通道对应的GPIO时钟 */    
//    __GPIOA_CLK_ENABLE();
//  
//    /* 初始化通道对应的GPIO */    
//    GPIO_InitStruct.Pin = GPIO_PIN_N;
//    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;//模拟模式
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//    

//	 sConfig.Channel = ADC_CHANNEL_1; 
//    if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK)
//    {
//    }    
}

///*******************************************************************************
//** 功能	      ADC1通道配置   
//** 参数       无
//** 返回值	    无
//** 注意       
//** 修改日志
//*******************************************************************************/
//void ADC1_VER_Channel_Config(void)
//{
//ADC_HandleTypeDef Adc1Handle;
//    ADC_ChannelConfTypeDef sConfig;                           
//   sConfig.Channel = ADC_CHANNEL_17;             //内部的参考电压采样
//	
//    if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK)
//    {
//    }    
//}


///*******************************************************************************
//** 功能	      转换一个通道，获得ADC转换值
//** 参数       无
//** 返回值	    ADC转换结果
//** 注意       
//** 修改日志
//*******************************************************************************/
//uint16_t ADC1_ConvertOnDiscMode(void)
//{
// ADC_HandleTypeDef Adc1Handle;   
//	ADC1_STAR();
//    while(__HAL_ADC_GET_FLAG(&Adc1Handle,ADC_FLAG_EOC) == RESET);
//    if(__HAL_ADC_GET_FLAG(&Adc1Handle,ADC_FLAG_EOS) != RESET)
//    {
//        __HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
//    }
//    return HAL_ADC_GetValue(&Adc1Handle);
//}
///*******************************************************************************
//** 功能	      ADC1通道配置   
//** 参数       无
//** 返回值	    无
//** 注意       
//** 修改日志
//*******************************************************************************/
void ADC1_VER_Channel_Config(void)
{
//		ADC_HandleTypeDef Adc1Handle;
//    ADC_ChannelConfTypeDef sConfig;                           
//   sConfig.Channel = ADC_CHANNEL_17;             //内部的参考电压采样
//	
//    if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK)
//    {
//    }    
}


/*******************************************************************************
** 功能	      转换一个通道，获得ADC转换值
** 参数       无
** 返回值	    ADC转换结果
** 注意       
** 修改日志
*******************************************************************************/
uint16_t ADC1_ConvertOnDiscMode(ADC_HandleTypeDef* Adc1Handle)
{
  
	  ADC1_STAR();
    while(__HAL_ADC_GET_FLAG(Adc1Handle,ADC_FLAG_EOC) == RESET);			//////  2015 08 14 修改
    if(__HAL_ADC_GET_FLAG(Adc1Handle,ADC_FLAG_EOS) != RESET)
    {
        __HAL_ADC_CLEAR_FLAG(Adc1Handle,ADC_FLAG_EOS);
    }
    return HAL_ADC_GetValue(Adc1Handle);
}



//void MX_GPIO_Init(void)                   //SPI flash片选信号  需要检测一下功耗的问题  待测
//{

//  GPIO_InitTypeDef GPIO_InitStruct;

//  /* GPIO Ports Clock Enable */
//  __GPIOA_CLK_ENABLE();

//		
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//	
//  /*Configure GPIO pin : PA4 */
//  GPIO_InitStruct.Pin = GPIO_PIN_4;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


//}
//	



void  ADC_Trave(void)
//int8_t ADC_Trave(void)
{
//ADC_HandleTypeDef Adc1Handle; 		
	int8_t T_temp=0;
	uint16_t Test_data=0,Test_data_1=0;		
	uint16_t	Value_Vdda=0,Value_Vdda_1=0,Test_data_2=0,Test_data_3=0,Test_data_4=0,V_temperature=0;
	uint32_t T_Rx=0,Test_data_5=0,i=0;
	
	
#if 1
   
	GPIO_InitTypeDef GPIO_InitStruct;	  


	ADC_GPIO_Init();	//ADC_CFGR1_OVRMOD;
	
	SetClockForADC();
  ConfigureADC(); //默认是内部通道17
  CalibrateADC(); 
  EnableADC();
//  //第一次丢失 
//  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
//	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
//  Test_data = ADC1->DR; //HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);

//	
//  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
//	while((ADC1->ISR & ADC_FLAG_EOC)==RESET );	
//  Test_data =  ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);

	
  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
  Test_data_1 = ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle)>>8;//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);

  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
	while((ADC1->ISR & ADC_FLAG_EOC)==RESET );	
  Test_data_2 = ADC1->DR;// HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
   	for(i=0;i<25;i++)
	{
	Test_data_2 += ADC1->DR;
	}	
	Test_data_2=Test_data_2/26;
	//Value_Vdda=(uint32_t)1392*3600/Test_data_2;          //Vdda测试成功  2015.06.29  在USB 供电情况下
	Value_Vdda=(uint32_t)1224*4096/Test_data_2;          //在3V 情况下  参考电压1.224  电池供电
  DisableADC();
	
		
	/* 使能通道对应的GPIO时钟 */    
   __GPIOC_CLK_ENABLE();
  
    /* 初始化通道对应的GPIO */    
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;//模拟模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  ADC1->CHSELR=0x0000;				//选择通道以前要先清通道
		ADC1->CHSELR |=  ADC_CHANNEL_12;
		EnableADC();
		Test_data_5=0;
	ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
  Test_data_3 =  ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
	
	ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
  Test_data_4 =  ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
		
	ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
  Test_data_5+=ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
	for(i=0;i<25;i++)
	{
	Test_data_5+=ADC1->DR;
	}
//printf("%d %d\n",ADC1->DR,ADC1->DR);
	Test_data_5=(Test_data_3+Test_data_4+Test_data_5)/28;
//		Test_data_3=ADC1_ConvertOnDiscMode(&Adc1Handle);	
//		Test_data_4=ADC1_ConvertOnDiscMode(&Adc1Handle)>>8;
//		Test_data_5=ADC1_ConvertOnDiscMode(&Adc1Handle);
		
		V_temperature=(uint32_t)(Test_data_5*Value_Vdda>>12); 
		T_Rx=(uint32_t)V_temperature*100000/(Value_Vdda-V_temperature);  //负载为100KΩ
	//	T_Rx=179280;
		T_wendu=Temp_Coun(T_Rx);   //对比表中阻值扩大100倍   T_Rx计算时扩大了1000倍 /10即为实际的电阻值
  //  T_wendu=T_wendu/10;
//	printf(" %d ",T_Rx/10);
	DisableADC();
#endif
		DisableADC();
		PC3_DISABLE;PC1_DISABLE;
	ADC->CCR =0; 
			SYSCFG->CFGR3 &= ~SYSCFG_CFGR3_EN_VREFINT ;           //  采样完成后 关闭内部参考电压采样
			SYSCFG->CFGR3 &= ~SYSCFG_CFGR3_ENBUF_VREFINT_ADC; 
}
//void  ADC_Trave(void)
////int8_t ADC_Trave(void)
//{
////ADC_HandleTypeDef Adc1Handle; 		
//	int8_t T_temp=0;
//	uint16_t Test_data=0,Test_data_1=0;		
//	uint16_t	Value_Vdda=0,Value_Vdda_1=0,Test_data_2=0,Test_data_3=0,Test_data_4=0,V_temperature=0;
//	uint32_t T_Rx=0,Test_data_5=0,i=0;
//	
//	
//#if 1
//   
//	GPIO_InitTypeDef GPIO_InitStruct;	  


//	ADC_GPIO_Init();	//ADC_CFGR1_OVRMOD;
//	
//	SetClockForADC();
//  ConfigureADC(); //默认是内部通道17
//  CalibrateADC(); 
////  EnableADC();
////  //第一次丢失 
////  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
////	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
////  Test_data = ADC1->DR; //HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);

////	
////  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
////	while((ADC1->ISR & ADC_FLAG_EOC)==RESET );	
////  Test_data =  ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);

//	
////  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
////	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
////  Test_data_1 = ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle)>>8;//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);

////  ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
////	while((ADC1->ISR & ADC_FLAG_EOC)==RESET );	
////  Test_data_2 = ADC1->DR;// HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
////   	for(i=0;i<25;i++)
////	{
////	Test_data_2 += ADC1->DR;
////	}	
////	Test_data_2=Test_data_2/26;
////	//Value_Vdda=(uint32_t)1392*3600/Test_data_2;          //Vdda测试成功  2015.06.29  在USB 供电情况下
////	Value_Vdda=(uint32_t)1224*4096/Test_data_2;          //在3V 情况下  参考电压1.224  电池供电
////  DisableADC();
//	
//		
//	/* 使能通道对应的GPIO时钟 */    
//   __GPIOC_CLK_ENABLE();
//  
//    /* 初始化通道对应的GPIO */    
//    GPIO_InitStruct.Pin = GPIO_PIN_2;
//    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;//模拟模式
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//	  ADC1->CHSELR=0x0000;				//选择通道以前要先清通道
//		ADC1->CHSELR |=  ADC_CHANNEL_12;
//		EnableADC();
//		Test_data_5=0;
//	ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
//	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
//  Test_data_3 =  ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
//	
//	ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
//	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
//  Test_data_4 =  ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
//		
//	ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
//	while((ADC1->ISR & ADC_FLAG_EOC)==RESET);	
//  Test_data_5+=ADC1->DR;//HAL_ADC_GetValue(&Adc1Handle);//__HAL_ADC_CLEAR_FLAG(&Adc1Handle,ADC_FLAG_EOS);
//	for(i=0;i<25;i++)
//	{
//	Test_data_5+=ADC1->DR;
//	}
////printf("%d %d\n",ADC1->DR,ADC1->DR);
//	Test_data_5=(Test_data_3+Test_data_4+Test_data_5)/28;
////		Test_data_3=ADC1_ConvertOnDiscMode(&Adc1Handle);	
////		Test_data_4=ADC1_ConvertOnDiscMode(&Adc1Handle)>>8;
////		Test_data_5=ADC1_ConvertOnDiscMode(&Adc1Handle);
//		
////		V_temperature=(uint32_t)(Test_data_5*Value_Vdda>>12); 
////		T_Rx=(uint32_t)V_temperature*100000/(Value_Vdda-V_temperature);  //负载为100KΩ
//	//	T_Rx=179280;
//			T_Rx=	Test_data_5*100000/4095;
//		T_wendu=Temp_Coun(T_Rx);   //对比表中阻值扩大100倍   T_Rx计算时扩大了1000倍 /10即为实际的电阻值
//  //  T_wendu=T_wendu/10;
////	printf(" %d ",T_Rx/10);
//	DisableADC();
//#endif
//		DisableADC();
//		PC3_DISABLE;PC1_DISABLE;
//	ADC->CCR =0; 
//			SYSCFG->CFGR3 &= ~SYSCFG_CFGR3_EN_VREFINT ;           //  采样完成后 关闭内部参考电压采样
//			SYSCFG->CFGR3 &= ~SYSCFG_CFGR3_ENBUF_VREFINT_ADC; 
//}

/*******************************************************************************
** 功能	     通过查表获取温度值
** 参数       无
** 返回值	   温度值
** 注意       
** 修改日志
*******************************************************************************/




int16_t Temp_Coun(uint32_t Temp_Data)
{
  int16_t Temp_Buf;
	uint16_t Tab_Add=0,Tab_Add2=0;
  Tab_Add = 0;
	uint16_t y=0;
uint32_t detem=0;
	Tab_Add=0;Tab_Add2=0;
	if(Temp_Data>=Temp_Tab[0]*10)
	{
		Temp_Buf = -300;
	}
	if(Temp_Data<=Temp_Tab[100]*10)
	{
		Temp_Buf = 700;
	}
	if(Temp_Tab[0]*10>Temp_Data&&Temp_Data>Temp_Tab[100]*10)
		{
		detem=(Temp_Tab[Tab_Add]*10-(Temp_Tab[Tab_Add]-Temp_Tab[Tab_Add+1])*Tab_Add2);
			while(Temp_Data<=detem)
			{
				Tab_Add2++;
//				printf("%d %d ",Tab_Add,Tab_Add2);

				detem=(Temp_Tab[Tab_Add]*10-(Temp_Tab[Tab_Add]-Temp_Tab[Tab_Add+1])*Tab_Add2);
//				printf(" %d %d\n",(Temp_Tab[Tab_Add]-Temp_Tab[Tab_Add+1])*Tab_Add2/10,detem);
				if(Tab_Add2%10==0)
				{
						Tab_Add++;Tab_Add2=0;
				}
				
			}
			
			Temp_Buf =-301+Tab_Add*10+Tab_Add2;
		}

  return Temp_Buf;
}