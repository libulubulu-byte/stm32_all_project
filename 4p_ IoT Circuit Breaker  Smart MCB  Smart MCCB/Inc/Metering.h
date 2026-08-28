
#ifndef _METER_H_
#define _METER_H_
#include "TypeDef.h"
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

/*******************************************************************************
		Fcuction Declaration
*******************************************************************************/
//INT32U Read_Reg(INT8U addr);												//读计量,校表参数
//void Write_Reg(INT8U addr, INT32U para);									//写校表参数

//void Init_Metering(void);													//初始化计量模块（假）
//BOOL Check_Metering(void);													//检查计量模块
//void Repair_Metering(void);													//修复计量模块
//void Open_Current(void);													//开启电流采样通道

//BOOL Set_CaliPara(const INT8U *pPara, INT8U addr);							//设置校正参数
//void Get_MeterPara(INT8U addr, INT8U *pBuff);								//读取计量参数

/*******************************************************************************
		End File
*******************************************************************************/
#endif
