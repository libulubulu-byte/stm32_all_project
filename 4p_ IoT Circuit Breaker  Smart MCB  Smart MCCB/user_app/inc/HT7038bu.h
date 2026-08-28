/***************************************************************************
* NAME:         HT7038bu.h
*----------------------------------------------------------------------------
* Copyright (c) 
*----------------------------------------------------------------------------
* DATE:			2018-02-26
* REVISION:     V1.0
*
*----------------------------------------------------------------------------
* NOTE:
*
*****************************************************************************/

#ifndef __HT7038BU__H__
#define __HT7038BU__H__

#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
		Macro Definition
*******************************************************************************/
//计量参数寄存器定义------------------------------------------------------------
#define	r_DeviceID		0x00					//Device ID
#define	r_Pa			0x01					//A相有功功率
#define	r_Pb			0x02					//B相有功功率
#define	r_Pc			0x03					//C相有功功率
#define	r_Pt			0x04					//合相有功功率
#define	r_Qa			0x05					//A相无功功率
#define	r_Qb			0x06					//B相无功功率
#define	r_Qc			0x07					//C相无功功率
#define	r_Qt			0x08					//合相无功功率
#define	r_Sa			0x09					//A相视在功率
#define	r_Sb			0x0A					//B相视在功率
#define	r_Sc			0x0B					//C相视在功率
#define	r_St			0x0C					//合相视在功率
#define	r_UaRms			0x0D					//A相电压有效值
#define	r_UbRms			0x0E					//B相电压有效值
#define	r_UcRms			0x0F					//C相电压有效值
#define	r_IaRms			0x10					//A相电流有效值
#define	r_IbRms			0x11					//B相电流有效值
#define	r_IcRms			0x12					//C相电流有效值
#define	r_ItRms			0x13					//合相电流有效值(矢量和)
#define	r_Pfa			0x14					//A相功率因素
#define	r_Pfb			0x15					//B相功率因素
#define	r_Pfc			0x16					//C相功率因素
#define	r_Pft			0x17					//合相功率因素
#define	r_Pga			0x18					//A相电流与电压夹角
#define	r_Pgb			0x19					//B相电流与电压夹角
#define	r_Pgc			0x1A					//C相电流与电压夹角
#define	r_IFlag			0x1B					//中断标志（读后清零）						**
#define	r_Freq			0x1C					//线频率
#define	r_EFlag			0x1D					//电能寄存器状态（读后清零）				*
#define	r_Epa			0x1E					//A相有功电能（可配置读后清零）
#define	r_Epb			0x1F					//B相有功电能（可配置读后清零）
#define	r_Epc			0x20					//C相有功电能（可配置读后清零）
#define	r_Ept			0x21					//合相有功电能（可配置读后清零）
#define	r_Eqa			0x22					//A相无功电能（可配置读后清零）
#define	r_Eqb			0x23					//B相无功电能（可配置读后清零）
#define	r_Eqc			0x24					//C相无功电能（可配置读后清零）
#define	r_Eqt			0x25					//合相无功电能（可配置读后清零）
#define	r_YUaUb			0x26					//Ua与Ub的电压夹角
#define	r_YUaUc			0x27					//Ua与Uc的电压夹角
#define	r_YUbUc			0x28					//Ub与Uc的电压夹角
#define	r_InRms			0x29					//零线电流有效值
#define	r_TPSD			0x2A					//温度传感器的输出
#define	r_UtRms			0x2B					//合相电压有效值(矢量和)
#define	r_SFlag			0x2C					//断相,相序,SIG等标志状态					**
#define	r_ComReg		0x2D					//通讯数据备份寄存器						*
#define	r_ComSum		0x2E					//通讯校验和寄存器							*
#define	r_SampleIA		0x2F					//A相电流通道ADC采样数据
#define	r_SampleIB		0x30					//B相电流通道ADC采样数据
#define	r_SampleIC		0x31					//C相电流通道ADC采样数据
#define	r_SampleUA		0x32					//A相电压通道ADC采样数据
#define	r_SampleUB		0x33					//B相电压通道ADC采样数据
#define	r_SampleUC		0x34					//C相电压通道ADC采样数据
#define	r_Esa			0x35					//A相视在电能（可配置读后清零）
#define	r_Esb			0x36					//B相视在电能（可配置读后清零）
#define	r_Esc			0x37					//C相视在电能（可配置读后清零）
#define	r_Est			0x38					//合相视在电能（可配置读后清零）
#define	r_FstCntA		0x39					//A相快速脉冲计数
#define	r_FstCntB		0x3A					//B相快速脉冲计数
#define	r_FstCntC		0x3B					//C相快速脉冲计数
#define	r_FstCntT		0x3C					//合相快速脉冲计数
#define	r_PFlag			0x3D					//功率方向（0正1负）						**
#define	r_ChkSum0		0x3E					//校表数据校验和（0x01~0x39）（三线0x01E0CD四线0x01D4CD）*
#define	r_InstADC7		0x3F					//第7路ADC采样数据输出

#define	r_LinePa		0x40					//A相基波有功功率
#define	r_LinePb		0x41					//B相基波有功功率
#define	r_LinePc		0x42					//C相基波有功功率
#define	r_LinePt		0x43					//合相基波有功功率
#define	r_LineEPa		0x44					//A相基波有功电能（可配置读后清零）
#define	r_LineEPb		0x45					//B相基波有功电能（可配置读后清零）
#define	r_LineEPc		0x46					//C相基波有功电能（可配置读后清零）
#define	r_LineEPt		0x47					//合相基波有功电能（可配置读后清零）
#define	r_LineUaRms		0x48					//A相基波电压有效值
#define	r_LineUbRms		0x49					//B相基波电压有效值
#define	r_LineUcRms		0x4A					//C相基波电压有效值
#define	r_LineIaRms		0x4B					//A相基波电流有效值
#define	r_LineIbRms		0x4C					//B相基波电流有效值
#define	r_LineIcRms		0x4D					//C相基波电流有效值
#define	r_LEFlag		0x4E					//基波电能寄存器工作状态（读后清零）		*
#define	r_SAGFlag		0x4F					//SAG标志寄存器								**
#define	r_PeakUa		0x50					//A相电压最大值
#define	r_PeakUb		0x51					//B相电压最大值
#define	r_PeakUc		0x52					//C相电压最大值
#define	r_LineQa		0x57					//A相基波无功功率
#define	r_LineQb		0x58					//B相基波无功功率
#define	r_LineQc		0x59					//C相基波无功功率
#define	r_LineQt		0x5A					//合相基波无功功率
#define	r_Vrefgain		0x5C					//Vref自动补偿系数							*
#define	r_ChipID		0x5D					//芯片版本指示								*
#define	r_ChkSum1		0x5E					//校表数据校验和（0x60~0x71）				*
#define	r_PtrWaveBuff	0x7E					//缓冲数据指针
#define	r_WaveBuff		0x7F					//缓冲数据寄存器	
/*---------------------C O M P L I E R  C O M M A N D----------------*/
/*---------------------I N C L U D  F I L E S------------------------*/
#include <stdint.h>
/*---------------------M A C R O  D E F I N I T I O N S--------------*/
/*HT7038 costant definitions */
//#define HT7038_REG_I1_ADVAL		0x00u
//#define HT7038_REG_I2_ADVAL		0x01u
//#define HT7038_REG_V_ADVAL			0x02u

//#define HT7038_REG_I1_RMS			0x06u
//#define HT7038_REG_IA_RMS			0x07u
#define HT7038_REG_IA_RMS			0x10u
#define HT7038_REG_IB_RMS			0x11u
#define HT7038_REG_IC_RMS			0x12u
#define HT7038_REG_IABC_RMS		0x13u
#define HT7038_REGA_V_RMS			0x0Du
#define HT7038_REGB_V_RMS			0x0Eu
#define HT7038_REGC_V_RMS			0x0Fu
#define HT7038_REGsum_V_RMS		0x2Bu
#define HT7038_REG_FREQ			0x09u

#define HT7038_REG_POW_P1			0x0au
#define HT7038_REG_POW_Q1			0x0bu
#define HT7038_REG_POW_AS			0x09u
#define HT7038_REG_POW_BS			0x0Au
#define HT7038_REG_POW_CS			0x0Bu
#define HT7038_REG_POW_ABCS		0x0Cu	
#define HT7038_REG_POW_AP			0x01u	
#define HT7038_REG_POW_BP			0x02u	
#define HT7038_REG_POW_CP			0x03u	
#define HT7038_REG_POW_ABCP		0x04u	
#define HT7038_REG_POW_AQ			0x05u
#define HT7038_REG_POW_BQ			0x06u
#define HT7038_REG_POW_CQ			0x07u
#define HT7038_REG_POW_ABCQ		r_Qt
#define HT7038_SUM_ENE_P			0x21u
#define HT7038_SUM_ENE_S			0x38u
#define HT7038_SUM_ENE_Q			0x25u
#define HT7038_REG_POW_P2			0x10u
#define HT7038_REG_POW_Q2			0x11u
#define HT7038_REG_MAX_U			0x12u

#define HT7038_REG_BAKDATA			0x16u
#define HT7038_REG_COMM_CRC		0x17u
#define HT7038_REG_PARA_CRC		0x18u
#define HT7038_REG_EMU_SR			0x19u	
#define HT7038_REG_SYS_SR			0x1au
#define HT7038_REG_CHIPID			0x1bu
#define HT7038_REG_DEVID			0x1cu

#define HT7038_REG_EMU_IE			0x30u
#define HT7038_REG_EMU_IF			0x31u
#define HT7038_REG_WP				0x32u
#define HT7038_REG_RST				0x33u	

#define HT7038_REG_EMU_CFG			0x40u
#define HT7038_REG_FREQ_CFG		0x41u
#define HT7038_REG_EMU_EN			0x42u
#define HT7038_REG_ADC_EN			0x43u
#define HT7038_REG_IO_CFG			0x45u

#define HT7038_REG_GP1				0x50u
#define HT7038_REG_GQ1				0x51u
#define HT7038_REG_GP2				0x54u
#define HT7038_REG_GS1				0x52u
#define HT7038_REG_GQ2				0x55u
#define HT7038_REG_GS2				0x56u

#define HT7038_REG_QPHA			0x58u
#define HT7038_REG_ADC_G			0x59u

#define HT7038_REG_I2_G			0x5bu
#define HT7038_REG_I1_OFF			0x5cu
#define HT7038_REG_I2_OFF			0x5du
#define HT7038_REG_U_OFF			0x5eu
#define HT7038_REG_P_START			0x5fu

#define HT7038_REG_HFCONST			0x61u
#define HT7038_REG_STEAL			0x62u
#define HT7038_REG_I_STEAL			0x63u

//#define HT7038_REG_P1_OFT_H		0x65u
//#define HT7038_REG_P2_OFT_H		0x66u
//#define HT7038_REG_Q1_OFT_H		0x67u
//#define HT7038_REG_Q2_OFT_H		0x68u

//#define HT7038_REG_I1RMS_OFT		0x69u
//#define HT7038_REG_I2RMS_OFT		0x6au
//#define HT7038_REG_I_ZC			0x6cu
//#define HT7038_REG_GPHASE1			0x6du
//#define HT7038_REG_GPHASE2			0x6eu
//#define HT7038_REG_PF_CNT			0x6fu
//#define HT7038_REG_QF_CNT			0x70u
//#define HT7038_REG_SF_CNT			0x71u

//#define HT7038_REG_AD_CON			0x72u
//#define HT7038_REG_CRCCHK_L		0x73u
//#define HT7038_REG_CRCCHK_H		0x74u

//#define HT7038_REG_MODE_CFG		0x75u

//#define HT7038_REG_P1_OFT_L		0x76u
//#define HT7038_REG_P2_OFT_L		0x77u
//#define HT7038_REG_Q1_OFT_L		0x78u
//#define HT7038_REG_Q2_OFT_L		0x79u

//#define HT7038_REG_U_PEAK			0x7au
//#define HT7038_REG_U_SAG			0x7bu
//#define HT7038_REG_U_CYCLE			0x7cu

#define HT7038_REG_DUMMY			0x7fu
	
#define HT7038_CMD_WRITE			0x80u
#define HT7038_CMD_READ			0x00u
/*---------------------T Y P E  D E F I N T I O N----------*/
typedef union
{
	/* this definition only works for little endian */
	uint8_t  buf[4];
	uint32_t dt;
}UINT_32_8_T;

struct coefficient_t{
	float k_v;			//电压系数
	uint16_t k_i1;			//通道1电流系数 
	float k_i2;			//通道2电流系数
	uint16_t k_pqs1;		//通道1功率系数 
	float k_pqs2;		//通道2功率系数
	uint16_t ec;			//脉冲常数
	
}; 			//保存到flash里面
struct calibration_para_t{
	uint16_t hf_const;
	int16_t gp1;
	uint16_t gphase1;
	int16_t poffset1;
	uint16_t gp2;//无功有功视在功率校准
	uint16_t poffset2;	
};
struct att_data_t{	
	
	uint16_t power_ps;		//功率因数	
	//uint16_t maxuwave;      
	uint16_t freq;	
	int16_t power_Ap;		//系统有功功率
	int16_t power_Bp;		//系统有功功率
	int16_t power_Cp;		//系统有功功率
	int16_t power_q;		//系统无功功率
	int16_t power_s;		//系统视在功率
    uint32_t Avolt;			//系统A实际电压
	 uint32_t Bvolt;			//系统B实际电压
	 uint32_t Cvolt;			//系统C实际电压
	uint32_t sumvolt;			//系统he实际电压
    uint32_t leakageamp;            //漏电电流
	uint32_t Aamp;			//系统A实际电流
		uint32_t Bamp;			//系统B实际电流
		uint32_t Camp;			//系统C实际电流
	uint32_t sumamp;			//系统he实际电流
	uint32_t total_enery_p;         //总有功电能
	uint32_t total_enery_q;         //总无功电能
	uint32_t total_enery_s;         //总视在电能	
};
uint8_t read_powerfact(uint8_t addr,uint16_t *power, float k);
/*---------------------F U N C  E X P O R T----------------*/
void init_HT7038bu(void);
uint8_t write_HT7038bu(uint8_t addr, uint32_t dt);
uint8_t read_HT7038bu(uint8_t addr,uint8_t *rd_buff);
uint8_t read_rms(uint8_t addr,uint32_t *vi, float k);
uint8_t read_power(uint8_t addr,int16_t *power, float k);
uint8_t read_energy(uint8_t addr,uint16_t *energy, uint16_t ec);
uint8_t read_freq(uint16_t *freq);
void adjust_cali_para(uint8_t addr,uint16_t dt);
uint8_t read_HT7038bu32(uint8_t addr,uint8_t *rd_buff);
uint8_t read_HT7038buid(uint8_t addr,uint32_t *data);
#ifdef __cplusplus
}
#endif

#endif
