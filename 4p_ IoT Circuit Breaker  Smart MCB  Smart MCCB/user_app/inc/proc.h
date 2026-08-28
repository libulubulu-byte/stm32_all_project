/***************************************************************************
* NAME:         eeprom.h
*----------------------------------------------------------------------------
* Copyright (c) 
*----------------------------------------------------------------------------
* DATE:			2019-07-12
* REVISION:     V1.0
*
*----------------------------------------------------------------------------
* NOTE:
*
*****************************************************************************/
#ifndef __PROC__H__
#define __PROC__H__

#ifdef __cplusplus
extern "C"
{
#endif
/*---------------------C O M P L I E R  C O M M A N D----------------*/
/*---------------------I N C L U D  F I L E S------------------------*/
#include <stdint.h>
#include "HT7038bu.h"
#define SW_OFF (adcvaluep4_avr*baifen_fen<ADC_Getvalue(3)-adcvaluep4_m?1:0)   //fenza 
#define SW_ON (adcvaluep3_avr*baifen_he<ADC_Getvalue(2)-adcvaluep3_m?1:0)	//heza

/*---------------------M A C R O  D E F I N I T I O N S--------------*/
#define FIRMWARE_VERSION  1
struct workpara_t{ 
    uint16_t    crc;
    uint16_t     version; 
    uint16_t     type; //设备类型
    uint16_t     np;
    uint16_t     mode;
    uint16_t    address; //地址
    uint16_t    limit_leakage;  
    uint16_t    limit_temperature; 
    
    uint16_t    k_ntc;      	//NTC常数  
    uint16_t    alarmcode;   
    uint16_t    limit_power_s; //0xffff：：不启用
    struct coefficient_t coefficient; 
    struct calibration_para_t calibration;
    uint32_t    limit_volt_high; //0xffffffff：不启用 
    uint32_t    limit_volt_low; //0xffffffff：不启用 
    uint32_t    limit_amp;  //0xffffffff：不启用        
    uint32_t    sn;   // 设备ID 
    uint16_t    pulse_mantissa_p;//有功脉冲尾数
    uint16_t    pulse_mantissa_q;//无功脉冲尾数
    uint16_t    pulse_mantissa_s;//视在脉冲尾数
    uint32_t    total_enery_p; //总有功电能
    uint32_t    total_enery_q; //总无功电能
    uint32_t    total_enery_s; //总视在电能	
    uint32_t    limit_enery;
    uint8_t     volt_action_time;
    uint16_t    volt_recovery_time;
    uint8_t     Leakage_detection;
    uint16_t    ShortTripp_times;
} ; 			    //工作数据保存
struct workdata_t{ 
    uint8_t  onlinestatus;
    uint8_t  switchhopestatus; //开关期望状态，远程控制
    uint8_t  leakageteststatus; //漏电检测状态
    uint8_t  switchcurrentstatus; //开关当前状态	
    uint8_t  switchnotallowremote;
    uint8_t  switchauto;
    uint8_t  switchnotset;
    uint8_t  thunderstatus;
    int8_t  ntc_temperature;  //ntc 温度
    uint16_t  alarmcode;	//警报事件
    int8_t   power_factor;//功率因素
    struct att_data_t attdata;
};

	extern uint16_t adcvalue;
/*---------------------T Y P E  D E F I N T I O N--------------------*/
#define TMS_INIT(ts)             do {ts = gettickms();}while(0)
#define TMS_IS_OVER(ts, over)    (gettickms() - ts >= over)
#define TMS_IS_UNDER(ts, over)    (gettickms() - ts <= over)
#define TMS_COUNT(ts)            (gettickms() - ts)
/*---------------------F U N C  E X P O R T--------------------------*/	
extern uint8_t last_switchstate;
extern  struct workpara_t workpara;
uint8_t swich_prama(uint8_t	hf_flag);
void inctickms(void);
uint32_t gettickms(void);
void HT7038pro(void);
void  adcpro(void);
void delayms(uint32_t ms);
void workparam_init(void);
int8_t cmddataproc(uint8_t* pdata, uint8_t len);
uint8_t swtich_pro(void);
void SW_close_speedfast(void);
void SW_close_speedslow(void);
void SW_break2(void);
void SW_break(void);
void adjust_cali_para_all(void);
void led_pro(void);
void switchleakageread(void);
void alarm_pro(void);
void test(void);
void saveparam(void);
uint16_t get_adcleakgevalue(void);
uint8_t keyscan();
#ifdef __cplusplus
}
#endif

#endif
