/***************************************************************************
* NAME:         att7053bu.h
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

#ifndef __ATT7053BU__H__
#define __ATT7053BU__H__

#ifdef __cplusplus
extern "C"
{
#endif
	
/*---------------------C O M P L I E R  C O M M A N D----------------*/
/*---------------------I N C L U D  F I L E S------------------------*/
#include <stdint.h>
/*---------------------M A C R O  D E F I N I T I O N S--------------*/
/*att7053 costant definitions */
#define ATT7053_REG_I1_ADVAL		0x00u
#define ATT7053_REG_I2_ADVAL		0x01u
#define ATT7053_REG_V_ADVAL			0x02u

#define ATT7053_REG_I1_RMS			0x06u
#define ATT7053_REG_I2_RMS			0x07u
#define ATT7053_REG_V_RMS			0x08u
#define ATT7053_REG_FREQ			0x09u

#define ATT7053_REG_POW_P1			0x0au
#define ATT7053_REG_POW_Q1			0x0bu
#define ATT7053_REG_POW_S			0x0cu
#define ATT7053_REG_ENE_P			0x0du	
#define ATT7053_REG_ENE_Q			0x0eu
#define ATT7053_REG_ENE_S			0x0fu
#define ATT7053_REG_POW_P2			0x10u
#define ATT7053_REG_POW_Q2			0x11u
#define ATT7053_REG_MAX_U			0x12u

#define ATT7053_REG_BAKDATA			0x16u
#define ATT7053_REG_COMM_CRC		0x17u
#define ATT7053_REG_PARA_CRC		0x18u
#define ATT7053_REG_EMU_SR			0x19u	
#define ATT7053_REG_SYS_SR			0x1au
#define ATT7053_REG_CHIPID			0x1bu
#define ATT7053_REG_DEVID			0x1cu

#define ATT7053_REG_EMU_IE			0x30u
#define ATT7053_REG_EMU_IF			0x31u
#define ATT7053_REG_WP				0x32u
#define ATT7053_REG_RST				0x33u	

#define ATT7053_REG_EMU_CFG			0x40u
#define ATT7053_REG_FREQ_CFG		0x41u
#define ATT7053_REG_EMU_EN			0x42u
#define ATT7053_REG_ADC_EN			0x43u
#define ATT7053_REG_IO_CFG			0x45u

#define ATT7053_REG_GP1				0x50u
#define ATT7053_REG_GQ1				0x51u
#define ATT7053_REG_GP2				0x54u
#define ATT7053_REG_GS1				0x52u
#define ATT7053_REG_GQ2				0x55u
#define ATT7053_REG_GS2				0x56u

#define ATT7053_REG_QPHA			0x58u
#define ATT7053_REG_ADC_G			0x59u

#define ATT7053_REG_I2_G			0x5bu
#define ATT7053_REG_I1_OFF			0x5cu
#define ATT7053_REG_I2_OFF			0x5du
#define ATT7053_REG_U_OFF			0x5eu
#define ATT7053_REG_P_START			0x5fu

#define ATT7053_REG_HFCONST			0x61u
#define ATT7053_REG_STEAL			0x62u
#define ATT7053_REG_I_STEAL			0x63u

#define ATT7053_REG_P1_OFT_H		0x65u
#define ATT7053_REG_P2_OFT_H		0x66u
#define ATT7053_REG_Q1_OFT_H		0x67u
#define ATT7053_REG_Q2_OFT_H		0x68u

#define ATT7053_REG_I1RMS_OFT		0x69u
#define ATT7053_REG_I2RMS_OFT		0x6au
#define ATT7053_REG_I_ZC			0x6cu
#define ATT7053_REG_GPHASE1			0x6du
#define ATT7053_REG_GPHASE2			0x6eu
#define ATT7053_REG_PF_CNT			0x6fu
#define ATT7053_REG_QF_CNT			0x70u
#define ATT7053_REG_SF_CNT			0x71u

#define ATT7053_REG_AD_CON			0x72u
#define ATT7053_REG_CRCCHK_L		0x73u
#define ATT7053_REG_CRCCHK_H		0x74u

#define ATT7053_REG_MODE_CFG		0x75u

#define ATT7053_REG_P1_OFT_L		0x76u
#define ATT7053_REG_P2_OFT_L		0x77u
#define ATT7053_REG_Q1_OFT_L		0x78u
#define ATT7053_REG_Q2_OFT_L		0x79u

#define ATT7053_REG_U_PEAK			0x7au
#define ATT7053_REG_U_SAG			0x7bu
#define ATT7053_REG_U_CYCLE			0x7cu

#define ATT7053_REG_DUMMY			0x7fu
	
#define ATT7053_CMD_WRITE			0x80u
#define ATT7053_CMD_READ			0x00u
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
	int16_t power_p;		//系统有功功率
	int16_t power_q;		//系统无功功率
	int16_t power_s;		//系统视在功率
    uint32_t volt;			//系统实际电压
    uint32_t leakageamp;            //漏电电流
	uint32_t amp;			//系统实际电流
	uint32_t total_enery_p;         //总有功电能
	uint32_t total_enery_q;         //总无功电能
	uint32_t total_enery_s;         //总视在电能	
};

/*---------------------F U N C  E X P O R T----------------*/
void init_att7053bu(void);
uint8_t write_att7053bu(uint8_t addr, uint32_t dt);
uint8_t read_att7053bu(uint8_t addr,uint8_t *rd_buff);
uint8_t read_rms(uint8_t addr,uint32_t *vi, float k);
uint8_t read_power(uint8_t addr,int16_t *power, float k);
uint8_t read_energy(uint8_t addr,uint16_t *energy, uint16_t ec);
uint8_t read_freq(uint16_t *freq);
void adjust_cali_para(uint8_t addr,uint16_t dt);
uint8_t read_att7053bu32(uint8_t addr,uint8_t *rd_buff);
uint8_t read_att7053buid(uint8_t addr,uint32_t *data);
#ifdef __cplusplus
}
#endif

#endif
