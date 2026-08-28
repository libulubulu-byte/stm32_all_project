/***************************************************************************
* NAME:         eeprom.c
*----------------------------------------------------------------------------
* Copyright (c)
*----------------------------------------------------------------------------
* DATE:         2019-07-12
* REVISION:         V1.0
*----------------------------------------------------------------------------
* NOTE:
*
* Modified:
*****************************************************************************/

/*---------------------C O M P L I E R  C O M M A N D----------------*/

/*---------------------I N C L U D  F I L E S------------------------*/
#include "proc.h"
#include "pwm.h"
#include "adc.h" 
#include "ntc.h"
#include "user_comm.h"
#include "HT7038bu.h"
#include "bord.h"
#include "flash.h"
#include "uart.h"
#include "prjlib.h"
#include "stm32f0xx_hal_flash.h"
#ifdef DEBUG_AFU
#include "fsl_debug_console.h"
#endif

#define WORK_PARA_SECTOR_ADDR    0x0800FC00

#define CMD_CLEAR_ONLINE              0X00
#define CMD_CHECK_ONLINE              0X01
#define CMD_INQUIRE_BASIC_INFO        0X02
#define CMD_INQUIRE_CURRENT_INFO      0X03
#define CMD_READ_WORK_PARA            0X04
#define CMD_WRITE_WORK_PARA           0X05

#define CMD_SET_TEMPE                 0X06 

#define CMD_READ_ATT_PARA             0X08
#define CMD_SET_ATT_PARA              0X09
#define CMD_CLEAR_ATT_PARA            0X0A

#define CMD_SET_LIMITVOLAT_H          0X10
#define CMD_SET_LIMITVOLAT_L          0X11
#define CMD_SET_SWITCH                0X12
#define CMD_SET_LIMITAMP_PARA         0X13
#define CMD_SET_LIMITPOWER_PARA       0X14  
#define CMD_SORT_ORDER                0X15
#define CMD_SORT_LEVE                 0X16
#define CMD_AUTO_ALLOW                0X17


#define CMD_READ_ALL_WORK_PARA        0X20
#define CMD_WRITE_ALL_WORK_PARA       0X21

#define CMD_READ_ALL_ATT_PARA         0X22
#define CMD_WRITE_ALL_ATT_PARA        0X23

#define CMD_NOT_MANUAL                0X30  

#define STATUS_SWITCH                 0X01 
#define STATUS_SWITCH_MANUAL          0X02 

#define ALARM_LEAKAGE               (uint8_t)0X01 //漏电保护事件
#define ALARM_TEMPERATURE           (uint8_t)0X02 //过温提醒事件
#define ALARM_TEMPERATURE_OVER      (uint8_t)0X04 //过温保护事件
#define ALARM_SHORT                 (uint8_t)0X10 //短路保护事件


#define ALARM_POWER                 (uint16_t)0X08u//过载保护事件
#define ALARM_VOLTAGE_HIGH          (uint16_t)0X20u//过压保护事件
#define ALARM_VOLTAGE_LOW           (uint16_t)0X40u//欠压保护事件

#define ALARM_CLOSE                 (uint16_t)0X80u //合闸事件
#define ALARM_MANUAL_OPEN           (uint16_t)0X100u//手动分闸事件
#define ALARM_SHORTANTICIPATION     (uint16_t)0X200u//短路预判事件
#define ALARM_AMPOVER               (uint16_t)0X400u//过流保护事件
#define ALARM_LACKOFPHASE           (uint16_t)0X1000u//缺相保护事件
#define ALARM_BRAKEFAILURE          (uint16_t)0X8000u//故障跳闸事件


#define DESIGN_TEMPERATURE          100

#define NUM_leakageamp  3
#define NUM_tempmax  4
#define NUM_currmax  5
#define NUM_short_circuit 6
#define NUM_power_smax 7
#define NUM_voltmin  8
#define NUM_voltmax  9
#define NUM_elecmax  11
#define REFERENCE_LEAKGEAMP  23
uint32_t dianya=0;
/*---------------------M A C R O  D E F I N I T I O N S--------------*/
/*---------------------V A R  D E F I N I T I O N S------------------*/
uint16_t adcvalue=0;

extern TIM_HandleTypeDef htim15;
struct workpara_t workpara = {
  0x00,
  0x02,   //版本
  0x02,   //设备类型
  0x04,
  63,
  0, //地址
  23,     //最大漏电电流23ma
  80,  //温度

  54,  //NTC常数
  0x0000, //事件码
  2200,  //最大视在功率2200w

  0.081396046, //电压系数
  10, //ͨ通道1电流系数 在 MV MA的情况下扩大100倍，用于抵消算法误差
  0.001432766, //通道2电流系数
  3233, //通道1功率系数
  0.000990873, //通道2功率系数
  3200, //脉冲常数

  0x64,  //hf_const
  0x0000,
  0xFF92,
  0xF33A,
  200,
  200,

  460, //最大电压460000mv
  300, //最低电压300000mv

  63000,  //最大电流63000ma
  181002001, // 设备序列号
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  20,
  20000,
  0,
  0
};
struct workdata_t  workdata;

static uint8_t  Flag_leakageamp=0;
static uint8_t  Flag_tempmax=0;
uint8_t  Flag_cross_limitamp=0;
uint8_t         Flag_w_currmax=0;
static uint8_t  Flag_currmax=0;
static uint8_t  Flag_short_circuit=0;
static uint8_t  Flag_power_smax=0;
static uint8_t  Flag_voltmin=0;
static uint8_t  Flag_voltmax=0;
static uint8_t  Flag_elecmax=0;
static uint8_t  Flag_RepeatSwitch = 0;//重合闸标志 1为允许重合闸
static uint8_t  Flag_LeakageTestRepeatSwitch = 0;
static uint8_t  Flag_V_RepeatSwitch = 0;
static uint8_t  Flag_LEAKAGE=0;
 uint8_t led_flag=0;
 uint8_t led_flag2=0;
uint8_t  Flag_leakagetest=0;
//static uint8_t  Flag_FAULT=0;

/*---------------------E X T E R N  V A R  D E C L A R E S-----------*/
static uint32_t tickms = 0;
static uint32_t tick_10ms = 0;
static uint8_t hand_switch_state = 1;//手动分合闸状态  0为分闸
static uint8_t swremote_control = 1;//1为允许远程控制
static uint8_t SW_RepeatStatus = 0;//1为允许远程控制
static uint8_t Led_SerialNUM=0;
static uint8_t Led_shiny=0;
static uint8_t Brakefailure=1;
uint8_t Brakefailure2=0;
uint8_t last_switchstatus;
uint8_t last_switchstate;
uint8_t Duty_ratio=50;
uint16_t Pwm_Cycle=666;
uint8_t SW_Repeatcnt=0;
uint8_t SW_V_Repeatcnt=0;

uint16_t moter_closedelay=170;//144;
uint16_t moter_openzkb=100;
extern uint16_t moter_openfen;
uint16_t moter_openfen=100;
extern uint16_t adcvaluep,adcvaluep2;


uint8_t swich_prama(uint8_t	hf_flag)
{
						uint8_t za_flag=0,tflag=0,ts_flag=0,flag_state=0;
							adcvaluep1= ADC_Getvalue(8);//fen
							adcvaluep2= ADC_Getvalue(9);//he
							adcvaluep3=ADC_Getvalue(2);//he deng
							adcvaluep4=ADC_Getvalue(3);//fen deng
							if(SW_OFF)
							{
								za_flag=0;
							}else if(SW_ON)
							{
								za_flag=1;
							}else
							{
								za_flag=2;
							}
							if(za_flag==0)
							{
								
										if(hf_flag==1&&za_flag==0)
											{
															while(hf_flag==1)	
															{
																SW_break();
																HAL_Delay(1);
																HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
																HAL_Delay(4);
																adcvaluep1= ADC_Getvalue(8);//fen
																if(adcvaluep1_avr*baifen2_fen<adcvaluep1-adcvaluep1_m&&SW_OFF)//fenza
																{
																			
																			
																			hf_flag=0;

																	}	
															}
													
											}
										
										
										while(za_flag==0)
										{
													SW_break();
													HAL_Delay(1);
													HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
													HAL_Delay(4);
													adcvaluep2= ADC_Getvalue(9);//he
													if(adcvaluep2_avr*baifen2_he<adcvaluep2-adcvaluep2_m&&SW_ON)
													{
															
															
															if(!SW_ON)
															{
																za_flag=0;hf_flag=0;
															}else
															{
																za_flag=1;hf_flag=1;
															}
															
													}
													
											}	

	
											
									}else if(za_flag==1)
									{
											flag_state=0;
											while(za_flag==1)
											{
													
													if(hf_flag==0&&za_flag==1)
													{
															while(hf_flag==0)	
															{
																SW_break();
																HAL_Delay(1);
																HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
																HAL_Delay(30);
																adcvaluep2= ADC_Getvalue(9);//he
																if(adcvaluep2_avr*baifen2_he<adcvaluep2-adcvaluep2_m&&SW_ON)
																{
																		hf_flag=1;flag_state=1;
																}
																
															}
													
													}else
													{
//														if(tflag>0)
//														{
															SW_break();
//														}else{
//															SW_break();
//														}
														HAL_Delay(1);
														HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
														HAL_Delay(28);
														if(flag_state==1)
														{
															HAL_Delay(25);
														}
													}
													
													adcvaluep1= ADC_Getvalue(8);//fen
													adcvaluep3=ADC_Getvalue(2);//fen deng
													adcvaluep2= ADC_Getvalue(9);//heza
													if(adcvaluep2_avr*0.10>adcvaluep2-adcvaluep2_m)
													{
														adcvaluep2= ADC_Getvalue(9);//heza
														if(adcvaluep2_avr*0.10>adcvaluep2-adcvaluep2_m)
														{
															tflag=1;
														}
													}
													if(adcvaluep3_avr*0.90>adcvaluep3-adcvaluep3_m)
													{
															tflag=2;
													
													}else
													{
															tflag=0;
													
													}
													if(tflag>0)
													{

																HAL_Delay(15);
															if(adcvaluep1_avr*baifen2_fen<adcvaluep1-adcvaluep1_m&&SW_OFF)//fenza
															{
																	if(tflag==2)
																	{
																		tflag=0;
																		if(!SW_ON)
																		{
																			za_flag=0;hf_flag=0;
																		}else
																		{
																			za_flag=1;hf_flag=1;
																		}
																	}
																		
															}
													}
											}			
									
									}else if(za_flag==2)
									{
										
											while(za_flag==2)
											{
													SW_break();
													HAL_Delay(1);
													HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
													HAL_Delay(20);
													adcvaluep1= ADC_Getvalue(8);//fen
													adcvaluep3=ADC_Getvalue(2);//fen deng
													adcvaluep2= ADC_Getvalue(9);//heza
													if(adcvaluep2_avr*0.10>adcvaluep2-adcvaluep2_m)
													{
														adcvaluep2= ADC_Getvalue(9);//heza
														if(adcvaluep2_avr*0.10>adcvaluep2-adcvaluep2_m)
														{
															tflag=1;
														}
													}
													if(adcvaluep3_avr*0.90>adcvaluep3-adcvaluep3_m)
													{
															tflag=2;
													
													}else
													{
															tflag=0;
													
													}
													if(tflag>0)
													{
														
															HAL_Delay(15);
															if(adcvaluep1_avr*baifen2_fen<adcvaluep1-adcvaluep1_m&&SW_OFF)//fenza
															{
																	tflag=0;
																		HAL_Delay(4);
																		if(!SW_ON)
																		{
																			za_flag=0;hf_flag=0;
																		}else
																		{
																			za_flag=2;hf_flag=1;
																		}
																		
															}
													}
											}	
//										SW_break();
//										
//										while(za_flag==2)
//										{
//													adcvaluep2= ADC_Getvalue(9);//heza
//													if(adcvaluep2_avr*baifen2_he<adcvaluep2-adcvaluep2_m&&SW_ON)
//													{
//															
//															HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
//															za_flag=1;hf_flag=1;
//													}
//										}	
//										
//									
									}
									return hf_flag;


}
uint32_t sumpulse_mantissa_p=0;uint16_t abc_vi=0;
void workparam_init(void) {
  uint16_t crc = 0; 
  struct workpara_t  pworkpara ;
  FlashRead(WORK_PARA_SECTOR_ADDR,(uint8_t *)&pworkpara.crc,sizeof(workpara));
  crc = CRC_16(0xFFFF, (uint8_t*)&pworkpara + 2, OBJ_LEN(workpara)-2);
  if ((crc ==pworkpara.crc) && (workpara.version ==pworkpara.version)) {
    FlashRead(WORK_PARA_SECTOR_ADDR, (uint8_t*)&workpara, sizeof(workpara));
    workdata.attdata.total_enery_p = workpara.total_enery_p;
    workdata.attdata.total_enery_q = workpara.total_enery_q;
    workdata.attdata.total_enery_s = workpara.total_enery_s;
    return;
  } else {
    workpara.crc = CRC_16(0xFFFF, (uint8_t*)&workpara + 2, OBJ_LEN(workpara) - 2);
    Flash_PageErase(WORK_PARA_SECTOR_ADDR);
    FlashWrite(WORK_PARA_SECTOR_ADDR, (uint8_t*)&workpara,  sizeof(workpara));
  }
	sumpulse_mantissa_p=workpara.total_enery_p;
}

uint32_t gettickms(void) {
  return tickms;
}

void inctickms(void) {
  tickms++;
  if(tickms%10==0){
      tick_10ms++;
    }
}int16_t HT7038_POW_AS=0,HT7038_POW_AQ=0;int16_t power_ABCssum=0,power_ABCQsum=0,power_ABCpsum=0,HT7038_POW_BS=0,HT7038_POW_BQ=0;int16_t HT7038_POW_CS=0,HT7038_POW_CQ=0;

uint16_t power_ABCp=0,a_Fact=0,b_Fact=0,c_Fact=0,sum_Fact=0,sum_cos=0,ab_cos=0,ab_cos_A=0,bc_cos_A=0,ac_cos_A=0;	double p_yinshu=1.0,p_yinshu2=1.0,p_yinshu3=1.0,p_yinshusum=1.0;
static void attreadiup(void) { 
  static uint32_t HT7038currenttick = 0;

  uint8_t temp[4];//		workdata.attdata.leakageamp=adc_Jiaoliu(0);
//  read_rms(HT7038_REG_I1_RMS, &workdata.attdata.leakageamp,  0.000662148);
//  read_rms(HT7038_REG_IA_RMS, &workdata.attdata.amp,  0.023144826);
//	read_rms(HT7038_REG_IB_RMS, &workdata.attdata.amp,  0.023144826);
		read_rms(HT7038_REG_IA_RMS, &workdata.attdata.Aamp,  1.00);
		read_rms(HT7038_REG_IB_RMS, &workdata.attdata.Bamp,  1.00);
		read_rms(HT7038_REG_IC_RMS, &workdata.attdata.Camp,  1.00);
		//read_rms(HT7038_REG_IABC_RMS,&workdata.attdata.sumamp,  1.05);
//				read_rms(HT7038_REGC_V_RMS, &V_C, 0.01582943);
//			  read_rms(HT7038_REG_IC_RMS	, &I_C,  0.023144826);
			 
////			read_power(HT7038_REG_POW_CP	, &P_C,  0.01582943);
  if (TMS_IS_OVER(HT7038currenttick, 10)) {
    TMS_INIT(HT7038currenttick);
    if(HT7038currenttick>(HT7038currenttick+10)){
      HT7038currenttick=0;
    }
		
		if(abc_vi==0)
		{
			read_powerfact(r_YUaUb,&ab_cos_A,1);
			read_powerfact(r_YUaUc,&ac_cos_A,1);
			read_powerfact(r_YUbUc,&bc_cos_A,1);
			
		}
		if(ab_cos_A>1000&&ac_cos_A>1000&&bc_cos_A>1000)
		{
			abc_vi=1;
		
		}
		if(abc_vi==0)
		{
			ab_cos_A=1000;ac_cos_A=1000;bc_cos_A=1000;
		}
		//ab_cos_A=1762;ac_cos_A=1762;bc_cos_A=1762;
//		ab_cos_A=1000;ac_cos_A=1000;bc_cos_A=1000;
//    read_HT7038bu(HT7038_REG_PF_CNT, temp);
    read_rms(HT7038_REGA_V_RMS, &workdata.attdata.Avolt, (double)(ab_cos_A)/1000);
		read_rms(HT7038_REGB_V_RMS, &workdata.attdata.Bvolt, (double)(ac_cos_A)/1000);
		read_rms(HT7038_REGC_V_RMS, &workdata.attdata.Cvolt, (double)(bc_cos_A)/1000);
//		    read_rms(HT7038_REGA_V_RMS, &workdata.attdata.Avolt, 1);
//		read_rms(HT7038_REGB_V_RMS, &workdata.attdata.Bvolt, 1);
//		read_rms(HT7038_REGC_V_RMS, &workdata.attdata.Cvolt, 1);
		//read_rms(HT7038_REGsum_V_RMS, &workdata.attdata.sumvolt, 1);
//    read_freq(&workdata.attdata.freq);
//		workdata.attdata.power_Ap=0;
//		workdata.attdata.power_Bp=0;
//		workdata.attdata.power_Cp=0;
//		read_powerfact(r_Pfa,&a_Fact,1);
//		read_powerfact(r_Pfb,&b_Fact,1);
//		read_powerfact(r_Pfc,&c_Fact,1);
//		sum_cos=0;
//		read_powerfact(r_Pga,&ab_cos,1);
//		sum_cos+=ab_cos;
//		read_powerfact(r_Pgb,&ab_cos,1);
//		sum_cos+=ab_cos;
//		read_powerfact(r_Pgc,&ab_cos,1);
//		sum_cos+=ab_cos;
//		ab_cos=sum_cos/3;

//		workdata.attdata.Avolt*=(((double)(ab_cos_A))/1000);
//		workdata.attdata.Bvolt*=(((double)(ac_cos_A))/1000);
//		workdata.attdata.Cvolt*=(((double)(bc_cos_A))/1000);
		
		read_power(HT7038_REG_POW_AP	, &workdata.attdata.power_Ap,  0.01582943);
		read_power(HT7038_REG_POW_AQ	, &HT7038_POW_AQ,  0.01582943);
		read_power(HT7038_REG_POW_AS	, &HT7038_POW_AS,  0.01582943);
//			if(HT7038_POW_AS!=0)
//		{
				p_yinshu=(double)(workdata.attdata.power_Ap)/HT7038_POW_AS;
//				//p_yinshu=(double)(a_Fact)/1000;
		//workdata.attdata.power_Ap=(workdata.attdata.Avolt*workdata.attdata.Aamp/1000)*p_yinshu;
//		}
//		if(workdata.attdata.power_Ap<0)
//		{
//			workdata.attdata.power_Ap=-workdata.attdata.power_Ap;
//		}
		read_power(HT7038_REG_POW_BP	, &workdata.attdata.power_Bp,  0.01582943);
		read_power(HT7038_REG_POW_BQ	, &HT7038_POW_BQ,  0.01582943);
		read_power(HT7038_REG_POW_BS	, &HT7038_POW_BS,  0.01582943);
//		if(HT7038_POW_BS!=0)
//		{
			p_yinshu2=(double)(workdata.attdata.power_Bp)/HT7038_POW_BS;
//			//p_yinshu2=(double)(b_Fact)/1000;
		//workdata.attdata.power_Bp=(workdata.attdata.Bvolt*workdata.attdata.Bamp/1000)*p_yinshu2;
//		}
//		if(workdata.attdata.power_Bp<0)
//		{
//			workdata.attdata.power_Bp=-workdata.attdata.power_Bp;
//		}
		read_power(HT7038_REG_POW_CP	, &workdata.attdata.power_Cp,  0.01582943);
		read_power(HT7038_REG_POW_CQ	, &HT7038_POW_CQ,  0.01582943);
		read_power(HT7038_REG_POW_CS	, &HT7038_POW_CS,  0.01582943);
//		if(HT7038_POW_CS!=0)
//		{
			p_yinshu3=(double)(workdata.attdata.power_Cp)/HT7038_POW_CS;
//			//p_yinshu3=(double)(c_Fact)/1000;
		//workdata.attdata.power_Cp=(workdata.attdata.Cvolt*workdata.attdata.Camp/1000)*p_yinshu3;
//		}
//		if(workdata.attdata.power_Cp<0)
//		{
//			workdata.attdata.power_Cp=-workdata.attdata.power_Cp;
//		}
//		power_ABCp=workdata.attdata.power_Ap+workdata.attdata.power_Bp+workdata.attdata.power_Cp;
//		if(ab_cos_A>1.2||bc_cos_A>1.2||ac_cos_A>1.2)
		{
//			power_ABCp
//				workdata.attdata.sumvolt=(1.732*((workdata.attdata.Avolt+workdata.attdata.Bvolt+workdata.attdata.Cvolt)/3-1.5));
				read_powerfact(r_Pft,&sum_Fact,1);
				read_power( HT7038_REG_POW_ABCP	, &power_ABCpsum,  0.01582943);
				read_power( HT7038_REG_POW_ABCS	, &power_ABCssum,  0.01582943);
				read_power( HT7038_REG_POW_ABCQ	, &power_ABCQsum,  0.01582943);
				power_ABCp=(power_ABCpsum);
				workdata.attdata.power_s=(power_ABCpsum);
//			if(workdata.attdata.Aamp>5&&workdata.attdata.Bamp>5&&workdata.attdata.Camp>5&&abc_vi==1)
//			{
//				power_ABCp=(power_ABCpsum*1.732);
//				workdata.attdata.power_s=(power_ABCpsum*1.732);
//			}
 				p_yinshusum=(double)(power_ABCpsum)/power_ABCssum;
//				workdata.attdata.sumamp=(workdata.attdata.Aamp+workdata.attdata.Bamp+workdata.attdata.Camp);
//				power_ABCp=(workdata.attdata.sumvolt*workdata.attdata.sumamp/1000)*ab_cos/1000;
//		
		}
	
//		workdata.attdata.power_p=110;
//    read_power(HT7038_REG_POW_P2, &workdata.attdata.power_p, 0.020542932);
//    read_power(HT7038_REG_POW_Q2, &workdata.attdata.power_q, 0.020542932);
//    read_power(HT7038_REG_POW_AS, &workdata.attdata.power_s, 0.020542932);
//    workdata.attdata.power_ps=workdata.attdata.power_p;
//		workdata.attdata.Aamp=101;
//		workdata.attdata.Bamp=102;
//		workdata.attdata.Camp=103;
//		workdata.attdata.Avolt=111;
//		workdata.attdata.Bvolt=112;
//		workdata.attdata.Cvolt=113;

//		if(workdata.attdata.volt==0)
//		{
//			if(workdata.attdata.amp<11)
//			{
//			
//				workdata.attdata.amp=0;
//			}		
//		}
  }
}

static void energycumulative(uint16_t *energycumul,uint32_t *total_enery){
  if(*energycumul>=32){
    *total_enery+=*energycumul/32;
    *energycumul=*energycumul%32;
  }
}
uint16_t pulse_mantissa_Ap=0,pulse_mantissa_Bp=0,pulse_mantissa_Cp=0,pulse_mantissa_sump=0;
uint32_t total_enery_Ap=0,total_enery_Bp=0,total_enery_Cp=0,total_enery_sump=0,total_enery_psum=0;
static void attreadenergy(void) {
  static uint32_t HT7038energytick = 0;
  uint16_t curenergya = 0;
	 uint16_t curenergyb = 0;
	 uint16_t curenergyc = 0;
	 uint16_t curenergy = 0,curenergysum=0;
	read_energy(HT7038_SUM_ENE_Q, &curenergy, workpara.coefficient.ec);
  workpara.pulse_mantissa_q += curenergy;
  energycumulative(&workpara.pulse_mantissa_q, &workpara.total_enery_q);

  read_energy(HT7038_SUM_ENE_S, &curenergy, workpara.coefficient.ec);
  workpara.pulse_mantissa_s += curenergy;
  energycumulative(&workpara.pulse_mantissa_s, &workpara.total_enery_s);
  	read_energy(HT7038_SUM_ENE_Q, &curenergy, workpara.coefficient.ec);
  workpara.pulse_mantissa_q += curenergy;
  energycumulative(&workpara.pulse_mantissa_q, &workpara.total_enery_q);

  read_energy(HT7038_SUM_ENE_S, &curenergy, workpara.coefficient.ec);
  workpara.pulse_mantissa_s += curenergy;
  energycumulative(&workpara.pulse_mantissa_s, &workpara.total_enery_s);
//  read_energy(r_Epa, &curenergya, workpara.coefficient.ec);
//	pulse_mantissa_Ap += curenergya;
//  energycumulative(&pulse_mantissa_Ap, &total_enery_Ap);
//	read_energy(r_Epb, &curenergyb, workpara.coefficient.ec);
//	pulse_mantissa_Bp += curenergyb;
//  energycumulative(&pulse_mantissa_Bp, &total_enery_Bp);
//  read_energy(r_Epc, &curenergyc, workpara.coefficient.ec);
//	pulse_mantissa_Cp += curenergyc;
//  energycumulative(&pulse_mantissa_Cp, &total_enery_Cp);  
//	workpara.total_enery_p=total_enery_Ap+total_enery_Bp+total_enery_Cp;
	read_energy(r_Ept, &curenergysum, workpara.coefficient.ec);
	pulse_mantissa_sump += curenergysum;
  energycumulative(&pulse_mantissa_sump, &total_enery_sump);  
	workpara.total_enery_p=total_enery_sump;
	total_enery_psum=total_enery_sump;
  if (TMS_IS_OVER(HT7038energytick, 3600000)) 
	{ 
    TMS_INIT(HT7038energytick);
		if(sumpulse_mantissa_p!=workpara.total_enery_p)
		{
				saveparam();
		}
		sumpulse_mantissa_p=workpara.total_enery_p;
  }

}

void HT7038pro(void) {
  attreadiup();
  attreadenergy(); 
}

void  adcpro(void){	
	
  workdata.ntc_temperature=GetTemperature(adcvalue);
	
}
uint8_t Warning_led_SLOW(uint8_t flag_led){

static uint32_t voltmin_tick=0;
  if(TMS_IS_OVER(voltmin_tick,500)){
    TMS_INIT(voltmin_tick);
    if(flag_led==0){
      LED_YELLOW_OFF();
      flag_led=1;
    }else{
      LED_YELLOW_ON();
      flag_led=0;
    } 
  } 
	return flag_led;

}
void Warning_led(uint8_t cnt){
  static uint8_t cnt_led=0;
  static uint8_t flag_led=0;
  static uint32_t voltmin_tick=0;

  Led_shiny=1;
  if(TMS_IS_OVER(voltmin_tick, 200)){
    TMS_INIT(voltmin_tick);
    if(flag_led==0){
      LED_YELLOW_OFF();
      flag_led=1;
    }else{
      LED_YELLOW_ON();
      cnt_led++;
      flag_led=0;
    } 
  } 
  if(cnt_led>cnt){
    Led_shiny=0;
    LED_YELLOW_OFF();
    if(Led_SerialNUM>7){
      Led_SerialNUM=0;
    }else{
      Led_SerialNUM++;
    }
    cnt_led=0;
  }
}

void led_pro(void) {//IS_SWITCH_LOCK()||IS_SWITCH_FREE()


	
  if(Led_shiny==0){
    Led_SerialNUM++;
  }
  if(Flag_leakageamp==1&&Led_SerialNUM==1){
    Warning_led(NUM_leakageamp);
  }
  if(Flag_tempmax==1&&Led_SerialNUM==2){
    Warning_led(NUM_tempmax);
  }
  if(Flag_currmax==1&&Led_SerialNUM==3){
    Warning_led(NUM_currmax);
  }
  if(Flag_short_circuit==1&&Led_SerialNUM==4){
    Warning_led(NUM_short_circuit);
  }
  if(Flag_power_smax==1&&Led_SerialNUM==5){
    Warning_led(NUM_power_smax);
  }
  if(Flag_voltmin==1&&Led_SerialNUM==6){
    Warning_led(NUM_voltmin);
  }
  if(Flag_voltmax==1&&Led_SerialNUM==7){
    Warning_led(NUM_voltmax);
  }
  if(Flag_elecmax==1&&Led_SerialNUM==8){
    Warning_led(NUM_elecmax);
  }
	if(Flag_cross_limitamp==1)
	{
		led_flag2=1;
		led_flag=Warning_led_SLOW(led_flag);
	}else
	{
				led_flag2=0;
	}
				
		
	if(Flag_leakageamp||Flag_tempmax||Flag_currmax||Flag_short_circuit||Flag_power_smax||Flag_voltmin||Flag_voltmax||Flag_elecmax)
	{
			LED_RED_OFF();LED_GREEN_OFF();led_flag2=1;
	}else
	{
			led_flag2=0;Led_SerialNUM=0;Led_shiny=0;
			if (SW_OFF) {
				Flag_cross_limitamp=0;
					LED_GREEN_ON();
				LED_RED_OFF();
			} else if (SW_ON){
				LED_RED_ON();
				LED_GREEN_OFF();
			}else{
				LED_RED_OFF();
				LED_GREEN_OFF();
			}
			if(Flag_cross_limitamp==1)
			{

				led_flag2=1;
			
			}
	}
		if(led_flag2==0)
			{
				LED_YELLOW_OFF();
			}
}

void Test_remotestatus(void){
  if((hand_switch_state==1)&&(Flag_voltmax==0)&&(Flag_voltmin==0)&&(Flag_currmax==0)&&(Flag_power_smax==0)&&(Flag_tempmax==0)&&(Flag_LEAKAGE==0)){
    swremote_control = 1;
  }else{
    swremote_control = 0;
  }
}

/**
 * 开关动作处理
 */
uint8_t swtich_pro(void) {
  
  static uint8_t initial_breakbrake=1;
  uint16_t adcvaluep_back,adcvaluep2_back;

  if(initial_breakbrake){//开关状态初始化
    if(SW_ON){
      workdata.switchcurrentstatus=1;
      last_switchstatus=1;last_switchstate=1;
      workdata.switchhopestatus=1;  
    }
    if(SW_OFF){
      workdata.switchcurrentstatus=0;
      last_switchstatus=0;last_switchstate=0;
      workdata.switchhopestatus=0; 
    }
    initial_breakbrake=0;
  }

  if(SW_ON){//检测当前分合闸状态  0为分闸
    workdata.switchcurrentstatus=1;
//		if(Brakefailure2==1)
//		{
//			workdata.switchhopestatus=1;Brakefailure2=0;
//		}
    MASK_SET(workdata.alarmcode, ALARM_CLOSE);
    MASK_CLEAR(workdata.alarmcode, ALARM_MANUAL_OPEN);
    MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
  }
  if(SW_OFF){
    workdata.switchcurrentstatus=0;
    if(!hand_switch_state){
      MASK_SET(workdata.alarmcode, ALARM_MANUAL_OPEN);
    }
    if(SW_Repeatcnt>3 || SW_V_Repeatcnt>3){
      MASK_SET(workdata.alarmcode, ALARM_BRAKEFAILURE);
    }
    MASK_CLEAR(workdata.alarmcode, ALARM_CLOSE);
  }

  if(keyscan()==0){//按键分合闸
    if(SW_OFF)
			{
      //等待分闸霍尔检测
           
        if (SW_OFF) 
					{
						
						
            workdata.switchcurrentstatus =swich_prama(last_switchstate);
						last_switchstate=workdata.switchcurrentstatus;
						workdata.switchhopestatus=workdata.switchcurrentstatus;
            MASK_CLEAR(workdata.alarmcode, ALARM_CLOSE);
            MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
            MASK_SET(workdata.alarmcode, ALARM_MANUAL_OPEN);
          }
          
       
    }else if(SW_ON)
			{
								
                workdata.switchcurrentstatus = swich_prama(last_switchstate);
								last_switchstate=workdata.switchcurrentstatus;
                MASK_CLEAR(workdata.alarmcode, ALARM_MANUAL_OPEN);
                MASK_SET(workdata.alarmcode, ALARM_CLOSE);
								workdata.switchhopestatus=workdata.switchcurrentstatus;
           
      }
			else if(!SW_ON&&!SW_OFF)
					{
											
											if(last_switchstate==1)
											{
												workdata.switchcurrentstatus = swich_prama(last_switchstate);
												last_switchstate=workdata.switchcurrentstatus;
												MASK_CLEAR(workdata.alarmcode, ALARM_MANUAL_OPEN);
												MASK_SET(workdata.alarmcode, ALARM_CLOSE);
											
											}else
											{
											
															workdata.switchcurrentstatus = swich_prama(last_switchstate);
															last_switchstate=workdata.switchcurrentstatus;
															MASK_CLEAR(workdata.alarmcode, ALARM_MANUAL_OPEN);
															MASK_SET(workdata.alarmcode, ALARM_CLOSE);
															workdata.switchhopestatus=workdata.switchcurrentstatus;
											}
					
								 
				}
		}

  if(workdata.switchcurrentstatus==0){//检测是否手动分合闸
    if(workdata.switchcurrentstatus!=last_switchstatus){
      hand_switch_state=0;
      MASK_CLEAR(workdata.alarmcode, ALARM_CLOSE);
      MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
      MASK_SET(workdata.alarmcode, ALARM_MANUAL_OPEN);
			last_switchstatus=workdata.switchcurrentstatus;
    }
  }else if(workdata.switchcurrentstatus==1){
    MASK_CLEAR(workdata.alarmcode, ALARM_MANUAL_OPEN);
    MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
   if(workdata.switchcurrentstatus!=last_switchstatus){
      hand_switch_state=1;
      workdata.switchhopestatus=workdata.switchcurrentstatus;
      last_switchstatus=workdata.switchcurrentstatus;
      MASK_SET(workdata.alarmcode, ALARM_CLOSE);
    }else{
      hand_switch_state=1;

//      MASK_SET(workdata.alarmcode, ALARM_CLOSE);
    }
  }
 
  if(((Flag_RepeatSwitch==1)||(Flag_V_RepeatSwitch==1))&&(SW_RepeatStatus==1)){ //故障重合闸
		
//			if (SW_ON) 
//			{
//				last_switchstate=swich_prama(last_switchstate);
//				workdata.switchcurrentstatus = last_switchstate;
//         
//          MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
//          MASK_SET(workdata.alarmcode, ALARM_CLOSE);
//      }
//			if (SW_OFF) 
//			{
//						last_switchstate=swich_prama(last_switchstate);
//						workdata.switchcurrentstatus = last_switchstate;
//            MASK_CLEAR(workdata.alarmcode, ALARM_CLOSE);
//            MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
//						MASK_SET(workdata.alarmcode, ALARM_MANUAL_OPEN);
//      }
//      SW_RepeatStatus=0;Brakefailure=1;	Flag_RepeatSwitch=0;Flag_V_RepeatSwitch=0;
	
  }
  
  if(!Brakefailure){//故障跳闸
    if (SW_ON)
			{
      
      //等待分闸霍尔检测
						last_switchstate=swich_prama(last_switchstate);
						workdata.switchcurrentstatus = last_switchstate;
						MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
						MASK_SET(workdata.alarmcode, ALARM_CLOSE);
						Brakefailure=1;Brakefailure2=1;
						workdata.switchhopestatus=workdata.switchcurrentstatus;

    }else{
      if(Flag_LEAKAGE && (SW_Repeatcnt>3)){//漏电锁止
        				last_switchstate=swich_prama(last_switchstate);
								workdata.switchcurrentstatus = last_switchstate;



      }
    }
  }  
  
  

  Test_remotestatus();//检测远程控制允许状态
  if(!swremote_control){
    return -1;
  }
  
  if (workdata.switchhopestatus != workdata.switchcurrentstatus) {//
    if (workdata.switchhopestatus == 1) {
       if (SW_OFF)
			{
      
      //等待分闸霍尔检测
						last_switchstate=swich_prama(last_switchstate);
            workdata.switchcurrentstatus = last_switchstate;
            last_switchstatus=workdata.switchcurrentstatus;
            MASK_SET(workdata.alarmcode, ALARM_CLOSE);
            MASK_CLEAR(workdata.alarmcode, ALARM_MANUAL_OPEN);
            MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
						workdata.switchhopestatus=workdata.switchcurrentstatus;
          
      }
    } else if (workdata.switchhopestatus == 0) {

         if(SW_ON) {
							last_switchstate=swich_prama(last_switchstate);
              workdata.switchcurrentstatus = last_switchstate;
              last_switchstatus=workdata.switchcurrentstatus;
              MASK_CLEAR(workdata.alarmcode, ALARM_CLOSE);
              MASK_CLEAR(workdata.alarmcode, ALARM_MANUAL_OPEN);
              MASK_CLEAR(workdata.alarmcode, ALARM_BRAKEFAILURE);
							workdata.switchhopestatus=workdata.switchcurrentstatus;

           
        }
      }
  }
  return 0;
}
/**
 * 电机PWM驱动   合闸快速
 */
void SW_close_speedfast(void)
{
  Duty_ratio=100;
  Pwm_Cycle=20;
  MX_TIM15_Init();
  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
}

/**
 * 电机PWM驱动   合闸低速
 */
void SW_close_speedslow(void)
{
  Duty_ratio=100;
  Pwm_Cycle=10;
  MX_TIM15_Init();
  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
}
/**
 * 电机PWM驱动   分闸
 */
void SW_break(void)
{
  Duty_ratio=moter_openzkb;
  Pwm_Cycle=666;
  MX_TIM15_Init();
  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
}
/**
 * 电机PWM驱动   分闸
 */
void SW_break2(void)
{
  Duty_ratio=20;
  Pwm_Cycle=100;
  MX_TIM15_Init();
  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
}
uint8_t flag_PK=0;
/**
 * 报警事件处理
 */
void alarm_pro(void) {
  static uint32_t volt_tick = 0;
  static uint32_t leakage_test_tick = 0;
  static uint32_t RepeatSwitchtick = 0;
  static uint32_t V_RepeatSwitchtick = 0;
  static uint32_t amp_tick1 = 0;
  static uint32_t amp_tick2 = 0;
  static uint32_t amp_tick3 = 0;
  static uint32_t power_p_tick = 0;
  static uint32_t temperature_tick = 0;
  static uint8_t temperature_overtick=0;
uint8_t flag=0;


  if(SW_ON){
	Flag_leakageamp=0;
  Flag_tempmax=0;
  Flag_currmax=0;
  Flag_short_circuit=0;
  Flag_power_smax=0;
  Flag_voltmin=0;
  Flag_voltmax=0;
  Flag_elecmax=0;
		Brakefailure=1;
    Flag_RepeatSwitch = 0;
    Flag_V_RepeatSwitch = 0;
    Flag_LeakageTestRepeatSwitch=0;
		Led_shiny=0;
		HT7038pro();
			//	workdata.ntc_temperature=91;
//		workdata.attdata.leakageamp=23;
//		workdata.attdata.amp=25600;
//		workdata.attdata.volt=271;
//		workdata.attdata.power_s =2300;
		 if (workdata.ntc_temperature > workpara.limit_temperature) {      
     if (TMS_IS_OVER(temperature_tick, 500)) { //过温提醒时延
       MASK_SET(workdata.alarmcode, ALARM_TEMPERATURE);//过温提醒
     }
			}else{
				MASK_CLEAR(workdata.alarmcode, ALARM_TEMPERATURE);
				TMS_INIT(temperature_tick);
			} 
			if (workdata.ntc_temperature > (workpara.limit_temperature+10)){
				if (TMS_IS_OVER(temperature_overtick, 500)) { //过温保护时延
					MASK_SET(workdata.alarmcode, ALARM_TEMPERATURE_OVER);//过温保护
					Brakefailure=0;
					Flag_tempmax=1;
					Flag_RepeatSwitch = 1; //1019
				 }           
			} else {
				MASK_CLEAR(workdata.alarmcode, ALARM_TEMPERATURE_OVER);
				TMS_INIT(temperature_overtick);
				if(swremote_control){
		/*      TMS_INIT(RepeatSwitchtick);
					SW_Repeatcnt=1;*/
					Flag_RepeatSwitch = 0;
				}
				Flag_tempmax=0;
			}
				
//		if (!IS_LEAKAGE_TEST()) { //漏电检测
//    HAL_Delay(10);
//    if(!IS_LEAKAGE_TEST()){
//			if((SW_OFF)/* && (workdata.attdata.leakageamp>=REFERENCE_LEAKGEAMP)&&(workdata.attdata.leakageamp<=100)*/){
//				Flag_leakagetest=1;
//				last_switchstate=swich_prama(last_switchstate);HAL_Delay(500);
//				
//			}
//       
//      if(((SW_ON) /*&& (workdata.attdata.leakageamp>=REFERENCE_LEAKGEAMP)&&(workdata.attdata.leakageamp<=100)*/)||Flag_leakagetest==1){
//      last_switchstate=swich_prama(last_switchstate);Flag_leakagetest=0;
//      }
////      HAL_Delay(1000);
////        if(Flag_leakagetest==1){
////          if(SW_ON){
////            last_switchstate=swich_prama(last_switchstate);
////						}
////          }
//                    
//    }else{
//      TMS_INIT(leakage_test_tick);
//    }
//  }
		
//		if (!IS_LEAKAGE_TEST()) { //漏电检测
//    HAL_Delay(10);
//    if(!IS_LEAKAGE_TEST()){
//			if((SW_OFF) && (workdata.attdata.leakageamp>=REFERENCE_LEAKGEAMP)&&(workdata.attdata.leakageamp<=100)){
//				Flag_leakagetest=1;
//				last_switchstate=swich_prama(last_switchstate);
//				
//			}
//       HAL_Delay(500);
//      if(((SW_ON) && (workdata.attdata.leakageamp>=REFERENCE_LEAKGEAMP)&&(workdata.attdata.leakageamp<=100))||Flag_leakagetest==1){
//      last_switchstate=swich_prama(last_switchstate);Flag_leakagetest=0;
//      }
////      HAL_Delay(1000);
////        if(Flag_leakagetest==1){
////          if(SW_ON){
////            last_switchstate=swich_prama(last_switchstate);
////						}
////          }
//                    
//    }else{
//      TMS_INIT(leakage_test_tick);
//    }
//  }
  
  if ((IS_LEAKAGE_TEST()) && (workdata.attdata.leakageamp>=REFERENCE_LEAKGEAMP && workdata.attdata.leakageamp<=100)&&(Flag_leakagetest==0)) { //漏电保护
      MASK_SET(workdata.alarmcode, ALARM_LEAKAGE);
      workdata.leakageteststatus=0;
      Brakefailure=0;
      Flag_LEAKAGE=1;Flag_leakageamp=1;
      Flag_RepeatSwitch = 1;  //1019
  }else{
    MASK_CLEAR(workdata.alarmcode, ALARM_LEAKAGE);
    workdata.leakageteststatus=1;
    Flag_LEAKAGE=0;
  }
  
  Flag_leakagetest=0;
    if ((workdata.attdata.Avolt > workpara.limit_volt_high)||(workdata.attdata.Bvolt > workpara.limit_volt_high)||(workdata.attdata.Cvolt > workpara.limit_volt_high)) 
			{
						HAL_Delay(10);
						read_rms(HT7038_REGA_V_RMS, &workdata.attdata.Avolt, (double)(ab_cos_A)/1000);
						read_rms(HT7038_REGB_V_RMS, &workdata.attdata.Bvolt, (double)(ac_cos_A)/1000);
						read_rms(HT7038_REGC_V_RMS, &workdata.attdata.Cvolt, (double)(bc_cos_A)/1000);

				   if ((workdata.attdata.Avolt > workpara.limit_volt_high)||(workdata.attdata.Bvolt > workpara.limit_volt_high)||(workdata.attdata.Cvolt > workpara.limit_volt_high)) 
					 {
								MASK_CLEAR(workdata.alarmcode, ALARM_VOLTAGE_LOW);
								if (TMS_IS_OVER(volt_tick, 1000)) { //过压时延
									MASK_SET(workdata.alarmcode, ALARM_VOLTAGE_HIGH);//过压保护事件
									Brakefailure=0;
									Flag_voltmax=1;
									Flag_V_RepeatSwitch = 1;  //1019
								}
					}
    } else if (((workdata.attdata.Avolt < workpara.limit_volt_low)&&(workdata.attdata.Avolt >100))||((workdata.attdata.Bvolt < workpara.limit_volt_low)&&(workdata.attdata.Bvolt >100))||((workdata.attdata.Cvolt < workpara.limit_volt_low)&&(workdata.attdata.Cvolt >100))) 
		{
//					dianya=workdata.attdata.Avolt;
					HAL_Delay(10);
					read_rms(HT7038_REGA_V_RMS, &workdata.attdata.Avolt, (double)(ab_cos_A)/1000);
					read_rms(HT7038_REGB_V_RMS, &workdata.attdata.Bvolt, (double)(ac_cos_A)/1000);
					read_rms(HT7038_REGC_V_RMS, &workdata.attdata.Cvolt, (double)(bc_cos_A)/1000);
				if (((workdata.attdata.Avolt < workpara.limit_volt_low)&&(workdata.attdata.Avolt >100))||((workdata.attdata.Bvolt < workpara.limit_volt_low)&&(workdata.attdata.Bvolt >100))||((workdata.attdata.Cvolt < workpara.limit_volt_low)&&(workdata.attdata.Cvolt >100))) 
				{
								if (TMS_IS_OVER(volt_tick,1000)) { //欠压时延
									MASK_CLEAR(workdata.alarmcode, ALARM_VOLTAGE_HIGH);
									MASK_SET(workdata.alarmcode, ALARM_VOLTAGE_LOW);//欠压保护事件
									Brakefailure=0;
									Flag_voltmin=1;
									Flag_V_RepeatSwitch = 1;  //1019
								}
					}
    } else {
      MASK_CLEAR(workdata.alarmcode, ALARM_VOLTAGE_HIGH | ALARM_VOLTAGE_LOW);
      TMS_INIT(volt_tick);
      if(swremote_control){
/*        TMS_INIT(V_RepeatSwitchtick);
        SW_Repeatcnt=1;*/
//        Flag_V_RepeatSwitch = 0;  
      }
      Flag_voltmax=0;
      Flag_voltmin=0;
    }
//		workdata.attdata.amp=14500;
			if ((workdata.attdata.Aamp >= (workpara.limit_amp*90/100)||workdata.attdata.Bamp >= (workpara.limit_amp*90/100)||workdata.attdata.Camp >= (workpara.limit_amp*90/100)) && (workdata.attdata.Aamp < (workpara.limit_amp*145/100)&&workdata.attdata.Bamp < (workpara.limit_amp*145/100)&&workdata.attdata.Camp < (workpara.limit_amp*145/100))) { //过流事件 
				Flag_cross_limitamp=1;
      }else
			{
				Flag_cross_limitamp=0;
			}
//    if ((workdata.attdata.Aamp >= (workpara.limit_amp*113/100)) && (workdata.attdata.Aamp < (workpara.limit_amp*145/100))) { //过流事件
////      if (TMS_IS_OVER(amp_tick1, 3600000)) { 
////        Flag_w_currmax=1;
////      }
////      if(Flag_w_currmax){
//////        Brakefailure=0;
////        Flag_currmax=1;    //1019    
////        Flag_RepeatSwitch = 1;
////        MASK_SET(workdata.alarmcode, ALARM_AMPOVER);
////        TMS_INIT(amp_tick2);
////        TMS_INIT(amp_tick3);
////      }
//    }else
	if ((workdata.attdata.Aamp >= (workpara.limit_amp*145/100)||workdata.attdata.Bamp >= (workpara.limit_amp*145/100)||workdata.attdata.Camp >= (workpara.limit_amp*145/100)) && (workdata.attdata.Aamp < (workpara.limit_amp*255/100)&&workdata.attdata.Bamp < (workpara.limit_amp*255/100)&&workdata.attdata.Camp < (workpara.limit_amp*255/100))) { //过流事件 
      if(TMS_IS_OVER(amp_tick2, 5000)){
        Flag_w_currmax=1;
      }
      if(Flag_w_currmax){
        Brakefailure=0;
        Flag_currmax=1; //1019
        Flag_RepeatSwitch = 1;
        MASK_SET(workdata.alarmcode, ALARM_AMPOVER);
        TMS_INIT(amp_tick3);
      }
    }else if ((workdata.attdata.Aamp >= (workpara.limit_amp*255/100)||workdata.attdata.Bamp >= (workpara.limit_amp*255/100)||workdata.attdata.Camp >= (workpara.limit_amp*255/100)) && (workdata.attdata.Aamp < (workpara.limit_amp*5)&&workdata.attdata.Bamp < (workpara.limit_amp*5)&&workdata.attdata.Camp < (workpara.limit_amp*5))) { //过流事件 
      MASK_SET(workdata.alarmcode, ALARM_SHORTANTICIPATION);
      if(TMS_IS_OVER(amp_tick3, 1000)){
        Flag_w_currmax=1;
      }
      if(Flag_w_currmax){
        Brakefailure=0;
        Flag_currmax=1; //1019
        Flag_RepeatSwitch = 1;
        MASK_SET(workdata.alarmcode, ALARM_AMPOVER);
      }
    }else if(workdata.attdata.Aamp >= (workpara.limit_amp*5)||workdata.attdata.Bamp >= (workpara.limit_amp*5)||workdata.attdata.Camp >= (workpara.limit_amp*5)){
      Brakefailure=0;
      Flag_currmax=1; //1019
      Flag_RepeatSwitch = 1;
      MASK_SET(workdata.alarmcode, ALARM_SHORT);
      MASK_SET(workdata.alarmcode, ALARM_AMPOVER);
      workpara.ShortTripp_times++;
      saveparam();
    }else {
      MASK_CLEAR(workdata.alarmcode, ALARM_AMPOVER|ALARM_SHORT|ALARM_SHORTANTICIPATION);
      TMS_INIT(amp_tick1);
      TMS_INIT(amp_tick2);
      TMS_INIT(amp_tick3);
      if(swremote_control){
/*        TMS_INIT(RepeatSwitchtick);        
        SW_Repeatcnt=1;*/
//        Flag_RepeatSwitch = 0;
      }
      Flag_w_currmax=0;
      Flag_currmax=0;
    }

    if (workdata.attdata.power_s > workpara.limit_power_s) {
      if (TMS_IS_OVER(power_p_tick, 500)) { //过载时延
        MASK_SET(workdata.alarmcode, ALARM_POWER);//过载保护事件
        Brakefailure=0;
        Flag_power_smax=1;  //1019
        Flag_RepeatSwitch = 1;
      }
    } else {
      MASK_CLEAR(workdata.alarmcode, ALARM_POWER);
      TMS_INIT(power_p_tick);
      if(swremote_control){
/*      TMS_INIT(RepeatSwitchtick);
        SW_Repeatcnt=1;*/
//        Flag_RepeatSwitch = 0;
      }   
      Flag_power_smax=0;
    }
  }
  
  if(Flag_V_RepeatSwitch==1||Flag_RepeatSwitch==1||Flag_RepeatSwitch==1)
	{
			//重合闸优先级，过欠压重合闸优先级最高
				if(Flag_V_RepeatSwitch)
				{
								if (TMS_IS_OVER(V_RepeatSwitchtick, 1000)){
											SW_RepeatStatus=0;
											if(SW_V_Repeatcnt<3){//记录重合闸次数，3次后不再合闸
												SW_RepeatStatus=1;
												SW_V_Repeatcnt+=1;
												TMS_INIT(V_RepeatSwitchtick);
											}
											if((SW_V_Repeatcnt>=3)&&(SW_OFF)){
												Flag_V_RepeatSwitch=0;
												MASK_SET(workdata.alarmcode, ALARM_BRAKEFAILURE); 
											}      
								}
					}
				else if(Flag_RepeatSwitch)
					{
										if(Flag_LeakageTestRepeatSwitch==1){
											if(SW_Repeatcnt<1){//漏电测试
												SW_RepeatStatus=1;
												SW_Repeatcnt+=1;
												TMS_INIT(RepeatSwitchtick);
											}else
											{
												Flag_RepeatSwitch=0;
											}
										}else if(Flag_RepeatSwitch)
										{      
														if (TMS_IS_OVER(RepeatSwitchtick, 1000))
														{       
																if(SW_Repeatcnt<3)
																{
																	SW_RepeatStatus=1;
																	SW_Repeatcnt+=1;
																	TMS_INIT(RepeatSwitchtick);
																}
																
																if((SW_V_Repeatcnt>3)&&(SW_OFF))
																{
																		Flag_RepeatSwitch=0;
																	MASK_SET(workdata.alarmcode, ALARM_BRAKEFAILURE);  SW_Repeatcnt=0;      
																}      
														}
										}
					}
		}else{
				TMS_INIT(RepeatSwitchtick);
				TMS_INIT(V_RepeatSwitchtick);
				SW_Repeatcnt=0;
				Flag_RepeatSwitch = 0;
				SW_V_Repeatcnt=0;
				Flag_V_RepeatSwitch = 0;
				Flag_LeakageTestRepeatSwitch=0;
			}

//		if(flag_PK==0)
//		{
//										Brakefailure=0;
////			Flag_cross_limitamp=1;
////			Flag_tempmax=1;
////			Flag_currmax=1;
////			Flag_short_circuit=1;
////			Flag_power_smax=1;
//			Flag_voltmin=1;
////			Flag_voltmax=1;
////			Flag_elecmax=1;
////			//Flag_power_smax=1;
////								//	Flag_voltmin=1;
////			 //Flag_LEAKAGE=1;
////			Flag_leakageamp=1;
////									Flag_V_RepeatSwitch = 1;  //1019
//			flag_PK=1;
//		}else if(flag_PK==1)
//		{
//			Flag_voltmax=1;flag_PK=2;Brakefailure=0;
//		
//		}

			
		}


static void onlineset(uint8_t set) {
  workdata.onlinestatus = set;
}

static uint8_t basicinfopack(uint8_t* pdata, uint8_t* plen) {
  uint8_t* p = NULL;
  p = pdata;
  *p++ = workpara.version;
  *p++ = workpara.type;
  *p++ = workpara.np;
  *p++ = (uint8_t)(workpara.mode >> 8);
  *p++ = (uint8_t)(workpara.mode);
  *p++ = workpara.limit_temperature;
  *p++ = workpara.limit_leakage;
  *p++ = (uint8_t)(workpara.limit_volt_high >> 24);
  *p++ = (uint8_t)(workpara.limit_volt_high >> 16);
  *p++ = (uint8_t)(workpara.limit_volt_high >> 8);
  *p++ = (uint8_t)(workpara.limit_volt_high);
  *p++ = (uint8_t)(workpara.limit_volt_low >> 24);
  *p++ = (uint8_t)(workpara.limit_volt_low >> 16);
  *p++ = (uint8_t)(workpara.limit_volt_low >> 8);
  *p++ = (uint8_t)(workpara.limit_volt_low);
  *p++ = (uint8_t)(workpara.limit_amp >> 24);
  *p++ = (uint8_t)(workpara.limit_amp >> 16);
  *p++ = (uint8_t)(workpara.limit_amp >> 8);
  *p++ = (uint8_t)(workpara.limit_amp);
  *p++ = (uint8_t)(workpara.limit_power_s >> 24);
  *p++ = (uint8_t)(workpara.limit_power_s >> 16);
  *p++ = (uint8_t)(workpara.limit_power_s >> 8);
  *p++ = (uint8_t)(workpara.limit_power_s);
  *p++ = (uint8_t)(workpara.limit_enery >> 24);
  *p++ = (uint8_t)(workpara.limit_enery >> 16);
  *p++ = (uint8_t)(workpara.limit_enery >> 8);
  *p++ = (uint8_t)(workpara.limit_enery);
  *p++ = (uint8_t)(workpara.volt_action_time);
  *p++ = (uint8_t)(workpara.volt_recovery_time);
  *p++ = (uint8_t)(workpara.Leakage_detection);
 // *p++ = (uint8_t)(workdata.switchauto);
  *plen = p - pdata;
  return *plen;
}

static  uint8_t readpara(uint8_t* pdata, uint8_t para_num, uint8_t* plen){
  uint8_t* p = NULL;

  p = pdata;

  *p++ =para_num;

  *p++ =0x01;
  *p++ =4;
  *p++ = (uint8_t)(workpara.limit_power_s >> 24);
  *p++ = (uint8_t)(workpara.limit_power_s >> 16);
  *p++ = (uint8_t)(workpara.limit_power_s >> 8);
  *p++ = (uint8_t)(workpara.limit_power_s);

  *p++ =0x02;
  *p++ =4;
  *p++ = (uint8_t)(workpara.limit_enery >> 24);
  *p++ = (uint8_t)(workpara.limit_enery >> 16);
  *p++ = (uint8_t)(workpara.limit_enery >> 8);
  *p++ = (uint8_t)(workpara.limit_enery);

  *p++ =0x03;
  *p++ =1;
  *p++ = workpara.limit_temperature;

  *p++ =0x04;
  *p++ =4;
  *p++ = (uint8_t)(workpara.limit_amp >> 24);
  *p++ = (uint8_t)(workpara.limit_amp >> 16);
  *p++ = (uint8_t)(workpara.limit_amp >> 8);
  *p++ = (uint8_t)(workpara.limit_amp);

  *p++ =0x05;
  *p++ =4;
  *p++ = (uint8_t)(workpara.limit_volt_high >> 24);
  *p++ = (uint8_t)(workpara.limit_volt_high >> 16);
  *p++ = (uint8_t)(workpara.limit_volt_high >> 8);
  *p++ = (uint8_t)(workpara.limit_volt_high);

  *p++ =0x06;
  *p++ =4;
  *p++ = (uint8_t)(workpara.limit_volt_low >> 24);
  *p++ = (uint8_t)(workpara.limit_volt_low >> 16);
  *p++ = (uint8_t)(workpara.limit_volt_low >> 8);
  *p++ = (uint8_t)(workpara.limit_volt_low);

  *p++ =0x07;
  *p++ =1;
  *p++ = workpara.volt_action_time;

  *p++ =0x08;
  *p++ =1;
  *p++ = (uint8_t)(workpara.volt_recovery_time);

  *p++ =0x09;
  *p++ =1;
  *p++ = workpara.Leakage_detection;
  
  *p++ =0x0A;
  *p++ =4;
  *p++ = (uint8_t)(workpara.sn >> 24);
  *p++ = (uint8_t)(workpara.sn >> 16);
  *p++ = (uint8_t)(workpara.sn >> 8);
  *p++ = (uint8_t)(workpara.sn);

  *plen = p - pdata + 1;
  return *plen;
}

static uint8_t writepara(uint8_t* pdata,uint8_t* rec_buf,uint8_t* plen){
  static uint8_t num;
  uint32_t status=0;
  uint8_t  para_cnt=0;
  uint8_t  para_len=0;
  uint8_t  para_id=0;
  uint8_t  index=9;
  uint8_t* p = NULL;
  p = pdata;
  num=0;
  memcpy(&para_cnt, rec_buf + 8, 1);
  for(uint8_t i=0;i<para_cnt;i++){
    memcpy(&para_id, rec_buf + index, 1);
    index++;
    memcpy(&para_len, rec_buf + index, 1);
    index++;
    memcpy(&status, rec_buf + index, para_len);
    index+=para_len;

    if(para_len==4){
      int32tBS(&status);
    }else if(para_len==2){
      int16tBS((uint16_t *)&status);
    }
    
    switch(para_id){
      case 0x01:
       workpara.limit_power_s=status;
       num++; 
      break;

      case 0x02:
      workpara.limit_enery=status;
      num++; 
      break;

      case 0x03:
       workpara.limit_temperature=status;
       num++; 
      break;

      case 0x04:
       workpara.limit_amp=status;
       num++; 
      break;

      case 0x05:
       workpara.limit_volt_high=status;
       num++; 
      break;

      case 0x06:
       workpara.limit_volt_low=status;
       num++; 
      break;

      case 0x07:
       workpara.volt_action_time=status;
       num++; 
      break;

      case 0x08:
       workpara.volt_recovery_time=status;
       num++; 
      break;

      case 0x09:
       workpara.Leakage_detection=status;
       num++; 
      break;
      
      case 0x0A:
       workpara.sn=status;
       num++; 
      break;
    }
  }
  if(num==para_cnt){
    num=0;
    *p++=0x00;
    saveparam();
  }else{
  *p++=0x01;
  num=0;
  }
  *plen = p - pdata + 1;
  return *plen;
}
uint32_t total_enery_p=0;
static  uint8_t currentinfopack(uint8_t* pdata, uint8_t* plen) {
  uint8_t* p = NULL;
	total_enery_p=workpara.total_enery_p;
//	if(workdata.attdata.Aamp>5&&workdata.attdata.Bamp>5&&workdata.attdata.Camp>5&&abc_vi==1)
//	{
//		total_enery_p=workpara.total_enery_p*1.732;
//	}

  p = pdata;
  *p++ = workpara.type;
  *p++ = workpara.np;
  *p++ = (uint8_t)(workpara.mode >> 8);
  *p++ = (uint8_t)(workpara.mode);
  *p++ = workdata.switchcurrentstatus;
  *p++ = workdata.thunderstatus;
  *p++ = workdata.ntc_temperature;
  *p++ = workdata.attdata.leakageamp;
  *p++ = (uint8_t)(workdata.alarmcode >> 8);
  *p++ = (uint8_t)(workdata.alarmcode);
  *p++ = (uint8_t)(sum_Fact >> 8);
  *p++ = (uint8_t)(sum_Fact);
  *p++ = (uint8_t)(workdata.attdata.Avolt >> 24);
  *p++ = (uint8_t)(workdata.attdata.Avolt >> 16);
  *p++ = (uint8_t)(workdata.attdata.Avolt >> 8);
  *p++ = (uint8_t)(workdata.attdata.Avolt);
	  *p++ = (uint8_t)(workdata.attdata.Bvolt >> 24);
  *p++ = (uint8_t)(workdata.attdata.Bvolt >> 16);
  *p++ = (uint8_t)(workdata.attdata.Bvolt >> 8);
  *p++ = (uint8_t)(workdata.attdata.Bvolt);
	  *p++ = (uint8_t)(workdata.attdata.Cvolt >> 24);
  *p++ = (uint8_t)(workdata.attdata.Cvolt >> 16);
  *p++ = (uint8_t)(workdata.attdata.Cvolt >> 8);
  *p++ = (uint8_t)(workdata.attdata.Cvolt);
  *p++ = (uint8_t)(workdata.attdata.Aamp >> 24);
  *p++ = (uint8_t)(workdata.attdata.Aamp >> 16);
  *p++ = (uint8_t)(workdata.attdata.Aamp >> 8);
  *p++ = (uint8_t)(workdata.attdata.Aamp);
	  *p++ = (uint8_t)(workdata.attdata.Bamp >> 24);
  *p++ = (uint8_t)(workdata.attdata.Bamp >> 16);
  *p++ = (uint8_t)(workdata.attdata.Bamp >> 8);
  *p++ = (uint8_t)(workdata.attdata.Bamp);
	  *p++ = (uint8_t)(workdata.attdata.Camp >> 24);
  *p++ = (uint8_t)(workdata.attdata.Camp >> 16);
  *p++ = (uint8_t)(workdata.attdata.Camp >> 8);
  *p++ = (uint8_t)(workdata.attdata.Camp);
  *p++ = (uint8_t)(power_ABCp >> 8);
	*p++ = (uint8_t)(power_ABCp);
//	*p++ = (uint8_t)(workdata.attdata.power_Ap >> 8);
//  *p++ = (uint8_t)(workdata.attdata.power_Ap);
	*p++ = (uint8_t)(power_ABCQsum >> 8);
  *p++ = (uint8_t)(power_ABCQsum);
	*p++ = (uint8_t)(power_ABCssum >> 8);
  *p++ = (uint8_t)(power_ABCssum);
	*p++ = (uint8_t)(0 >> 8);
  *p++ = (uint8_t)(0);
	*p++ = (uint8_t)(0 >> 8);
  *p++ = (uint8_t)(0);
	*p++ = (uint8_t)(0>> 8);
  *p++ = (uint8_t)(0);
	*p++ = (uint8_t)(0 >> 8);
  *p++ = (uint8_t)(0);
	*p++ = (uint8_t)(0>> 8);
  *p++ = (uint8_t)(0);
	*p++ = (uint8_t)(0 >> 8);
  *p++ = (uint8_t)(0);
//	//  *p++ = (uint8_t)(power_ABCp >> 8);
////  *p++ = (uint8_t)(power_ABCp);
//	*p++ = (uint8_t)(workdata.attdata.power_Ap >> 8);
//  *p++ = (uint8_t)(workdata.attdata.power_Ap);
//	*p++ = (uint8_t)(HT7038_POW_AQ >> 8);
//  *p++ = (uint8_t)(HT7038_POW_AQ);
//	*p++ = (uint8_t)(HT7038_POW_AS >> 8);
//  *p++ = (uint8_t)(HT7038_POW_AS);
//	*p++ = (uint8_t)(workdata.attdata.power_Bp >> 8);
//  *p++ = (uint8_t)(workdata.attdata.power_Bp);
//	*p++ = (uint8_t)(HT7038_POW_BQ >> 8);
//  *p++ = (uint8_t)(HT7038_POW_BQ);
//	*p++ = (uint8_t)(HT7038_POW_BS>> 8);
//  *p++ = (uint8_t)(HT7038_POW_BS);
//	*p++ = (uint8_t)(workdata.attdata.power_Cp >> 8);
//  *p++ = (uint8_t)(workdata.attdata.power_Cp);
//	*p++ = (uint8_t)(HT7038_POW_CQ >> 8);
//  *p++ = (uint8_t)(HT7038_POW_CQ);
//	*p++ = (uint8_t)(HT7038_POW_CS >> 8);
//  *p++ = (uint8_t)(HT7038_POW_CS);
	
//	*p++ = (uint8_t)(workdata.attdata.power_Cp >> 8);
//  *p++ = (uint8_t)(workdata.attdata.power_Cp);
  //*p++ = (uint8_t)(workdata.attdata.power_q >> 8);
  //*p++ = (uint8_t)(workdata.attdata.power_q);
  //*p++ = (uint8_t)(workdata.attdata.power_s >> 8);
  //*p++ = (uint8_t)(workdata.attdata.power_s);
  *p++ = (uint8_t)(total_enery_p >> 24);
  *p++ = (uint8_t)(total_enery_p >> 16);
  *p++ = (uint8_t)(total_enery_p >> 8);
  *p++ = (uint8_t)(total_enery_p);
  *p++ = (uint8_t)(workpara.total_enery_q >> 24);
  *p++ = (uint8_t)(workpara.total_enery_q >> 16);
  *p++ = (uint8_t)(workpara.total_enery_q >> 8);
  *p++ = (uint8_t)(workpara.total_enery_q);
  *p++ = (uint8_t)(workpara.total_enery_s >> 24);
  *p++ = (uint8_t)(workpara.total_enery_s >> 16);
  *p++ = (uint8_t)(workpara.total_enery_s >> 8);
  *p++ = (uint8_t)(workpara.total_enery_s);
  *p++ = (uint8_t)(workpara.ShortTripp_times >> 8);
  *p++ = (uint8_t)(workpara.ShortTripp_times);
  
  *plen = p - pdata + 1;
  return *plen;
}

static int8_t  setswitch(uint8_t *swtich ,uint8_t array, uint8_t address, uint32_t status) {
  uint8_t temp = 0;
  temp = array * 32;
  if (address > temp) {
    address -= temp;
  } else {
    return -1;
  }
  if (status ) {
    *swtich = 1;
//    network_switch_state=1;
  } else {
    *swtich = 0;
//    network_switch_state=0;
  }
  return 0;
}

void saveparam(void)
{
  workpara.crc = CRC_16(0xFFFF, (uint8_t*)&workpara + 2, OBJ_LEN(workpara) - 2);
  Flash_PageErase(WORK_PARA_SECTOR_ADDR);
  FlashWrite(WORK_PARA_SECTOR_ADDR,(uint8_t *)&workpara,sizeof(workpara));
	
}


uint8_t cmddatapack(uint8_t cmd, uint8_t addr, uint32_t sn,
                    uint8_t* psdata, uint8_t slen,
                    uint8_t* pddata, uint8_t* pdlen) {
  uint8_t* p = NULL;
  uint8_t len = 0;
  p = pddata;
  *p++ = 0x7E;
  *p++ = addr;
  int32tBS(&sn);
  memcpy(p, &sn, 4);
  p += 4;
  *p++ = cmd|0x80;
  *p++ = slen;
  memcpy(p, psdata, slen);
  p += slen;
  len = slen + 8;
  uint16_t crc = 0;
  crc = CRC_16(0xFFFF, pddata, len);
  int16tBS(&crc);
  memcpy(p, &crc, 2);
  p += 2;
  *p = 0x7E;
  len += 3;
  *pdlen = len;
  return    len;
}

static uint8_t askdata[150] = { 0, 0 };
int8_t cmddataproc(uint8_t* pdata, uint8_t len) {
  static uint16_t crc = 0,crc2 = 0;
  static  uint8_t debugcnt = 0;
  static uint8_t order = 0;static uint8_t sort_flag = 0;
  uint8_t address = 0;
  uint32_t sn = 0;
  if (*(pdata + len - 1) != 0x7E) {
    return -1;
  }  
  crc = (*(pdata + len - 3) << 8) + (*(pdata + len - 2));
  crc2 = CRC_16(0xFFFF, pdata, len - 3);
  if (crc != crc2) {
    return -2;
  } 
  address = *(pdata + 1);
  order = *(pdata + 6);
  memcpy(&sn, pdata + 2, 4);
  int32tBS(&sn);
  debugcnt++;

   if ((address != workpara.address)&&(order!=CMD_SORT_ORDER)) {
    return -2;
  }
  uint8_t asklen = 0;
  uint8_t para_num=9;
  uint32_t status = 0;
  
  switch (order) {
    case CMD_CLEAR_ONLINE:
      onlineset(0);
      asklen = 0;
      break;
    case CMD_CHECK_ONLINE:
      onlineset(1);
      askdata[0] = workpara.type;
      asklen = 1;
      break;

    case CMD_INQUIRE_BASIC_INFO:
      basicinfopack(askdata, &asklen);
      break;

    case CMD_INQUIRE_CURRENT_INFO:
      currentinfopack(askdata, &asklen);
      break;

    case CMD_READ_WORK_PARA:
      readpara(askdata, para_num, &asklen);
      break;

    case CMD_WRITE_WORK_PARA:
      writepara(askdata,pdata,&asklen);
      break;
    case CMD_SET_TEMPE:
        
        memcpy(&status, pdata + 9, 2);
        int16tBS((uint16_t *)&status);
        workpara.limit_temperature = status;
        saveparam();
      break;
    case CMD_READ_ATT_PARA:

      break;
    case CMD_SET_ATT_PARA:

      break;
    case CMD_CLEAR_ATT_PARA:

      break;
    case CMD_SET_LIMITVOLAT_H: {
        uint32_t status = 0;
        memcpy(&status, pdata + 9, 4);
        int32tBS(&status);
        workpara.limit_volt_high = status;
        saveparam();
      }
      break;
    case CMD_SET_LIMITVOLAT_L: {
        uint32_t status = 0;
        memcpy(&status, pdata + 9, 4);
        int32tBS(&status);
        workpara.limit_volt_low = status;
        saveparam();
      }

      break;
    case CMD_SET_SWITCH: {
        uint32_t status = 0;
        memcpy(&status, pdata + 9, 4);
        int32tBS(&status);
        if (hand_switch_state==1) {
          if(status==0){
            workdata.switchhopestatus=0;
          }else{
            workdata.switchhopestatus=1;
          }         
          askdata[0] = 0;
          asklen = 1;
        } else {
          askdata[0] = 1;
          asklen = 1;
        }
      }
      break;
    case CMD_AUTO_ALLOW: {
        uint32_t status = 0;
        uint8_t array = *(pdata + 8);
        memcpy(&status, pdata + 9, 4);
        int32tBS(&status);
        setswitch(&workdata.switchauto, array, workpara.address, status);
      }
      break;
    case CMD_SET_LIMITAMP_PARA: {
        uint32_t status = 0;
        memcpy(&status, pdata + 9, 4);
        int32tBS(&status);
        workpara.limit_amp = status;
        askdata[0] = 0;
        asklen = 1;
        saveparam();
      }
      break;
    case CMD_SET_LIMITPOWER_PARA: {
        uint32_t status = 0;
        memcpy(&status, pdata + 9, 4);
        int32tBS(&status);
        workpara.limit_power_s = status;
        askdata[0] = 0;
        asklen = 1;
        saveparam();
      }
      break;
    case CMD_READ_ALL_WORK_PARA: {
        asklen = sizeof(workpara);
        memcpy(askdata, (uint8_t*)&workpara, asklen);
      }
      break;
    	case CMD_SORT_ORDER:
			/*IO判断，排序时在所有分机地址一致的情况下用于判断那个分机占有总线的通信权*/
//			if (SORT_IN() == 0){ 
//				workpara.address = address;//设置分机地址   
//				askdata[0] = 0;
//				asklen = 1;
//				saveparam();//保存工作参数
//			}
			if (SORT_IN() == 0 || SORT_INs() == 0){ 
				workpara.address = address;//设置分机地址   
				askdata[0] = 0;
				asklen = 1;
				saveparam();//保存工作参数
			}		
			break;
			
		case CMD_NOT_MANUAL: 
		{
			uint32_t status = 0;
			uint8_t array = *(pdata + 8);
			memcpy(&status, pdata + 9, 4);
			int32tBS(&status);
			setswitch(&workdata.switchnotset, array, workpara.address, status);
			workdata.switchhopestatus = 0;
			break;
		}
	
		case CMD_SORT_LEVE:
		{
			uint32_t status = 0;
			if(sn==workpara.sn){       
				memcpy(&status, pdata + 8, 1);
//				if(status==0){
//					SORT_OUT_LOW();
//				}else{
//					SORT_OUT_HIGH();
//				}
				if(status==0){
					if(SORT_IN() == 0){
						SORT_OUT_LOW();
						sort_flag = 1;
					}
					if(SORT_INs() == 0){
						SORT_OUT_LOWs();
						sort_flag = 2;
					}
				}else{
					if(SORT_IN() == 0 || sort_flag == 1){
						SORT_OUT_HIGH();
						sort_flag = 0;
					}
					if(SORT_INs() == 0|| sort_flag == 2){
						SORT_OUT_HIGHs();
						sort_flag = 0;
					}						
				}  					
				askdata[0] = 0;
				asklen = 1;
			}
			break;
		}
		
		default:
			break;

	}
	if (address == 0 || sn == 0) { 
		asklen = 0;
	}
	if (asklen > 0) {
		static uint8_t senddata[150];
		static uint8_t sendlen = 0;
		cmddatapack(order,
					workpara.address, 
					workpara.sn,
					askdata, 
					asklen,
					senddata, 
					&sendlen);
		UART_WriteBlocking_ex(senddata, sendlen);//发送数据
	}
	return 0;
}
/*
void test(void){
    static uint8_t senddata[100]={"asdfaasdf"};
    uint8_t sendlen = 9;
    UART_WriteBlocking_ex(senddata, sendlen);
}
*/
uint8_t keyscan(){
  static uint8_t key_up=1;
  if(key_up==1 && IS_SW_TEST_STATUS()==0){
    TS_DELAY(15);
    key_up=0;
    if(IS_SW_TEST_STATUS()==0){
      return 0;
    }
  }
  if(IS_SW_TEST_STATUS()==1){
    if(key_up==0){
      key_up=1;
      return 1;
    }    
  }
  return 1;
}

void delayms(uint32_t ms) {
  uint32_t tick = 0;
  tick = gettickms();
//  TMS_INIT(tick);
  while (TMS_IS_OVER(tick, ms));
}

void adjust_cali_para_all(void) {
  static uint8_t debug_temp[4];
//  write_HT7038bu(HT7038_REG_WP, 0xA6);
//  adjust_cali_para(HT7038_REG_HFCONST, workpara.calibration.hf_const);
//  read_HT7038bu(HT7038_REG_HFCONST, debug_temp);
//  adjust_cali_para(HT7038_REG_GP1, workpara.calibration.gp1);
//  adjust_cali_para(HT7038_REG_GQ1, workpara.calibration.gp1);
//  adjust_cali_para(HT7038_REG_GS1, workpara.calibration.gp1);
////  adjust_cali_para(HT7038_REG_GPHASE1, workpara.calibration.gphase1);
////  adjust_cali_para(HT7038_REG_P1_OFT_H, (uint8_t)(workpara.calibration.poffset1 >> 8));
////  adjust_cali_para(HT7038_REG_P1_OFT_L, (uint8_t)(workpara.calibration.poffset1));
//  
//  write_HT7038bu(HT7038_REG_WP, 0x00);
}

