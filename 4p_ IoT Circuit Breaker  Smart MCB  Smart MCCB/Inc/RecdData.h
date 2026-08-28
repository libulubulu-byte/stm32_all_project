
#ifndef _RECDDATA_H_
#define _RECDDATA_H_

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
		Macro Definition
*******************************************************************************/
enum					//日期时间格式
{
	EM_Sec,				//秒
	EM_Min,				//分
	EM_Hour,			//时
	EM_Day,				//日
	EM_Month,			//月
	EM_Year				//年
};



//参数数据ID定义----------------------------------------------------------------
enum
{
	ID_PhsA,			//A相
	ID_PhsB,			//B相
	ID_PhsC,			//C相
	ID_PhsT,			//合相
	ID_PhsNum,
};
enum
{
	ID_PosActEny,		//正向有功
	ID_NegActEny,		//反向有功
	ID_Q1ReactEny,		//象限1无功
	ID_Q2ReactEny,		//象限2无功
	ID_Q3ReactEny,		//象限3无功
	ID_Q4ReactEny,		//象限4无功
	ID_CmbActEny,		//组合有功
	ID_Cmb1RctEny,		//组合1无功
	ID_Cmb2RctEny,		//组合2无功
	ID_EnergyNum,
};

/*******************************************************************************
		Macro Definition
*******************************************************************************/
//主动上报状态字定义------------------------------------------------------------
//按状态产生方式可分为两种：事件立即产生（如编程记录）；事件可持续一段时间，有进有出（如）
typedef enum
{								//事件类型				操作类	记录类	检测类
	RPT_RelayFail		=	0,	//负荷开关误动					+
	RPT_EsamErr			=	1,	//ESAM错误				+
//							2,	//内卡初始化错误
	RPT_BattVolt36		= 	3,	//时钟电池电压低						+
//							4,	//内部程序错误
//	RPT_MemoryErr		=	5,	//存储器故障			+
//						=	6,	//
	RPT_TimeErr			=	7,	//时钟故障				+
	RPT_BattVolt60		=	8,	//停电抄表电池欠压						+		***
	RPT_OverCash		=	9,	//透支状态				+
	RPT_OpenCover		=	10,	//开表盖						+
	RPT_OpenSheel		=	11,	//开端钮盖						+				***
	RPT_MagInter		=	12,	//恒定磁场干扰					+				***			??
	RPT_PowerFail		=	13,	//电源异常						+							??
	RPT_RelayOff		=	14,	//跳闸							+							??
	RPT_RelayOn			=	15,	//合闸							+							??
	RPT_LossVoltA		=	16,	//A相失压						+				***
	RPT_UndrVoltA		=	17,	//A相欠压						+				***
	RPT_OverVoltA		=	18,	//A相过压						+				***
	RPT_LossCurrA		=	19,	//A相失流						+				***
	RPT_OverCurrA		=	20,	//A相过流						+				***
	RPT_OverLoadA		=	21,	//A相过载						+				***
	RPT_RevPowerA		=	22,	//A相功率反向					+				***
	RPT_OffPhaseA		=	23,	//A相断相						+				***
	RPT_FailCurrA		=	24,	//A相断流						+				***
//							25,	//
//							26,	//
//							27,	//
//							28,	//
//							29,	//
//							30,	//
//							31,	//
	RPT_LossVoltB		=	32,	//B相失压						+				***
	RPT_UndrVoltB		=	33,	//B相欠压						+				***
	RPT_OverVoltB		=	34,	//B相过压						+				***
	RPT_LossCurrB		=	35,	//B相失流						+				***
	RPT_OverCurrB		=	36,	//B相过流						+				***
	RPT_OverLoadB		=	37,	//B相过载						+				***
	RPT_RevPowerB		=	38,	//B相功率反向					+				***
	RPT_OffPhaseB		=	39,	//B相断相						+				***
	RPT_FailCurrB		=	40,	//B相断流						+				***
//							41,	//
//							42,	//
//							43,	//
//							44,	//
//							45,	//
//							46,	//
//							47,	//
	RPT_LossVoltC		=	48,	//C相失压						+				***
	RPT_UndrVoltC		=	49,	//C相欠压						+				***
	RPT_OverVoltC		=	50,	//C相过压						+				***
	RPT_LossCurrC		=	51,	//C相失流						+				***
	RPT_OverCurrC		=	52,	//C相过流						+				***
	RPT_OverLoadC		=	53,	//C相过载						+				***
	RPT_RevPowerC		=	54,	//C相功率反向					+				***
	RPT_OffPhaseC		=	55,	//C相断相						+				***
	RPT_FailCurrC		=	56,	//C相断流						+				***
//							57,	//
//							58,	//
//							59,	//
//							60,	//
//							61,	//
//							62,	//
//							63,	//
	RPT_VoltNegPhs		=	64,	//电压逆相序					+				***
	RPT_CurrNegPhs		=	65,	//电流逆相序					+				***
	RPT_VoltUnBlnc		=	66,	//电压不平衡					+				***
	RPT_CurrUnBlnc		=	67,	//电流不平衡					+				***
//							68,	//辅助电压失电												??
	RPT_LostPower		=	69,	//掉电							+
	RPT_DemandOver		=	70,	//需量超限						+				***
	RPT_LowerPfT		=	71,	//总功率因数超下限				+				***
	RPT_SerUnBlnc		=	72,	//电流严重不平衡				+				***
	RPT_RevPowerT		=	73,	//潮流反向						+				***
	RPT_AllLstVolt		=	74,	//全失压						+				***
//							75,	//
//							76,	//
//							77,	//
//							78,	//
//							79,	//
	RPT_Program			=	80,	//编程							+
	RPT_ClrMeter		=	81,	//电表清零						+
	RPT_ClrDemand		=	82,	//需量清零						+				***
	RPT_ClrEvent		=	83,	//事件清零						+
	RPT_CaliTime		=	84,	//校时							+
	RPT_PrgSlotTab		=	85,	//时段表编程					+
	RPT_PrgZoneTab		=	86,	//时区表编程					+
	RPT_PrgOffDay		=	87,	//周休日编程					+
	RPT_PrgHoliday		=	88,	//节假日编程					+
	RPT_PrgActCmbZ		=	89,	//有功组合方式编程				+
	RPT_PrgRe1CmbZ		=	90,	//无功组合方式1编程				+				***
	RPT_PrgRe2CmbZ		=	91,	//无功组合方式2编程				+				***
	RPT_PrgCntDay		=	92,	//结算日编程					+
	RPT_PrgRateTab		=	93,	//费率表编程					+
	RPT_PrgStepTab		=	94,	//阶梯表编程					+
	RPT_RenewKey		=	95,	//密钥更新						+
	
	RPT_RptNum,					//状态字总数
	RPT_AllEvent,				//事件清零
	RPT_AllMeter,				//电表清零
	RPT_NoRpt
}RPT;

#define	LEN_CurkWh		(2+4+2)												//2+(4字节+2CRC)
#define	NUM_CurkWh		(3*6*1+1*6*5)										//(A,B,C相)*(有功正,有功反,无功1,无功2,无功3,无功4)
																			//(合相)*(有功正,有功反,无功1,无功2,无功3,无功4)*(总,尖,峰,平,谷)
#define	EE_kWhEnd		(LEN_CurkWh*NUM_CurkWh)
/*******************************************************************************
		Macro Definition
*******************************************************************************/
//事件记录ID定义----------------------------------------------------------------
typedef enum
{
	ID_ErrRecd,					//捕鼠器
	ID_AllLstVolt,				//全失压事件
	ID_VoltQlty,				//电压合格率
	ID_LostPower,				//掉电事件
	ID_PAcDmdOver,				//正向有功需量超限事件
	ID_NAcDmdOver,				//反向有功需量超限事件
	ID_Re1DmdOver,				//象限一无功需量超限事件
	ID_Re2DmdOver,				//象限二无功需量超限事件
	ID_Re3DmdOver,				//象限三无功需量超限事件
	ID_Re4DmdOver,				//象限四无功需量超限事件
	ID_Program,					//编程事件
	ID_ClrDemand,				//需量清零事件
	ID_CaliTime,				//校时事件
	ID_PrgSlotTab,				//*时段表编程事件
	ID_PrgZoneTab,				//时区表编程事件
	ID_PrgOffDay,				//周休日编程事件
	ID_PrgHoliday,				//*节假日编程事件
	ID_PrgActCmbZ,				//组合字编程事件
	ID_PrgRe1CmbZ,				//组合字编程事件
	ID_PrgRe2CmbZ,				//组合字编程事件
	ID_PrgCntDay,				//结算日编程事件
	ID_OpenCover,				//开表盖事件
	ID_OpenSheel,				//开端钮盖事件
	ID_RenewKey,				//密钥更新事件
	ID_MagInter,				//磁场干扰事件
	ID_RelayFail,				//开关误动作事件
	ID_PowerFail,				//电源异常事件
	ID_RelayOff,				//跳闸事件
	ID_RelayOn,					//合闸事件
	
	ID_LossVoltA,				//A相失压事件
	ID_LossVoltB,				//B相失压事件
	ID_LossVoltC,				//C相失压事件
	ID_UndrVoltA,				//A相欠压事件
	ID_UndrVoltB,				//B相欠压事件
	ID_UndrVoltC,				//C相欠压事件
	ID_OverVoltA,				//A相过压事件
	ID_OverVoltB,				//B相过压事件
	ID_OverVoltC,				//C相过压事件
	ID_OffPhaseA,				//A相断相事件
	ID_OffPhaseB,				//B相断相事件
	ID_OffPhaseC,				//C相断相事件
	
	ID_LossCurrA,				//A相失流事件
	ID_LossCurrB,				//B相失流事件
	ID_LossCurrC,				//C相失流事件
	ID_OverCurrA,				//A相过流事件
	ID_OverCurrB,				//B相过流事件
	ID_OverCurrC,				//C相过流事件
	ID_FailCurrA,				//A相断流事件
	ID_FailCurrB,				//B相断流事件
	ID_FailCurrC,				//C相断流事件
	
	ID_OverLoadA,				//A相过载事件
	ID_OverLoadB,				//B相过载事件
	ID_OverLoadC,				//C相过载事件
	ID_RevPowerA,				//A相功率反向事件
	ID_RevPowerB,				//B相功率反向事件
	ID_RevPowerC,				//C相功率反向事件
	ID_VoltNegPhs,				//电压逆相序事件
	ID_CurrNegPhs,				//电流逆相序事件
	
	ID_VoltUnBlnc,				//电压不平衡事件
	ID_CurrUnBlnc,				//电流不平衡事件
	ID_SerUnBlnc,				//电流严重不平衡事件
	
	ID_LowerPfA,				//A相功率因数超限事件
	ID_LowerPfB,				//B相功率因数超限事件
	ID_LowerPfC,				//C相功率因数超限事件
	ID_LowerPfT,				//合相功率因数超限事件
	
	ID_RevPowerT,				//潮流反向事件
	
	ID_PrgRateTab,				//费率表编程事件
	ID_PrgStepTab,				//阶梯表编程事件
	ID_BuyCashRcd,				//购电事件
	ID_WrongCard,				//异常插卡事件
	ID_SubCashRcd,				//退款事件

	ID_ClrEvent,				//事件清零事件（本id位置不可更改）
	ID_ClrMeter,				//电表清零事件（本id位置不可更改）
	
	ID_RecdNum,					//事件记录总数
	ID_AllEvent,				//事件清零	（清除ID_ClrEvent之前的事件）
	ID_AllMeter,				//电表清零	（清除ID_ClrMeter之前的事件）
	ID_AllRecd					//自定义清零（清超ID_RecdNum之前的事件）
}ID_RECD;
//事件记录次数定义--------------------------------------------------------------
#define	NUM_AllLstVolt	10u		//0305000A	全失压事件
#define	NUM_VoltQlty	10u		//03100X0A	电压合格率
#define	NUM_LostPower	10u		//0311000A	掉电事件
#define	NUM_DmdOver		10u		//03120X0A	需量超限事件
#define	NUM_Program		10u		//0330000A	编程事件
#define	NUM_ClrDemand	32u		//0330020A	需量清零事件
#define	NUM_CaliTime	10u		//0330040A	校时事件
#define	NUM_PrgSlotTab	10u		//0330050A	*时段表编程事件
#define	NUM_PrgZoneTab	10u		//0330060A	时区表编程事件
#define	NUM_PrgOffDay	10u		//0330070A	周休日编程事件
#define	NUM_PrgHoliday	10u		//0330080A	*节假日编程事件
#define	NUM_PrgActCmbZ	10u		//0330090A	组合字编程事件
#define	NUM_PrgRe1CmbZ	10u		//03300A0A	组合字编程事件
#define	NUM_PrgRe2CmbZ	10u		//03300B0A	组合字编程事件
#define	NUM_PrgCntDay	10u		//03300C0A	结算日编程事件
#define	NUM_OpenCover	10u		//03300D0A	开表盖事件
#define	NUM_OpenSheel	10u		//03300E0A	开端钮盖事件
#define	NUM_RenewKey	10u		//03301202	密钥更新事件
#define	NUM_MagInter	10u		//0335000A	磁场干扰事件
#define	NUM_RelayFail	10u		//0336000A	开关误动作事件
#define	NUM_PowerFail	10u		//0337000A	电源异常事件
#define	NUM_RelayOff	10u		//1D00FF0A	跳闸事件
#define	NUM_RelayOn		10u		//1E00FF0A	合闸事件
#define	NUM_LossVoltA	32u		//1001FF0A	A相失压事件
#define	NUM_LossVoltB	32u		//1002FF0A	B相失压事件
#define	NUM_LossVoltC	32u		//1003FF0A	C相失压事件
#define	NUM_UndrVoltA	32u		//1101FF0A	A相欠压事件
#define	NUM_UndrVoltB	32u		//1102FF0A	B相欠压事件
#define	NUM_UndrVoltC	32u		//1103FF0A	C相欠压事件
#define	NUM_OverVoltA	32u		//1201FF0A	A相过压事件
#define	NUM_OverVoltB	32u		//1202FF0A	B相过压事件
#define	NUM_OverVoltC	32u		//1203FF0A	C相过压事件
#define	NUM_OffPhaseA	32u		//1301FF0A	A相断相事件
#define	NUM_OffPhaseB	32u		//1302FF0A	B相断相事件
#define	NUM_OffPhaseC	32u		//1303FF0A	C相断相事件
#define	NUM_LossCurrA	32u		//1801FF0A	A相失流事件
#define	NUM_LossCurrB	32u		//1802FF0A	B相失流事件
#define	NUM_LossCurrC	32u		//1803FF0A	C相失流事件
#define	NUM_OverCurrA	32u		//1901FF0A	A相过流事件
#define	NUM_OverCurrB	32u		//1902FF0A	B相过流事件
#define	NUM_OverCurrC	32u		//1903FF0A	C相过流事件
#define	NUM_FailCurrA	32u		//1A01FF0A	A相断流事件
#define	NUM_FailCurrB	32u		//1A02FF0A	B相断流事件
#define	NUM_FailCurrC	32u		//1A03FF0A	C相断流事件
#define	NUM_OverLoadA	32u		//1C01FF0A	A相过载事件
#define	NUM_OverLoadB	32u		//1C01FF0A	B相过载事件
#define	NUM_OverLoadC	32u		//1C01FF0A	C相过载事件
#define	NUM_RevPowerA	32u		//1B01FF0A	A相功率反向事件
#define	NUM_RevPowerB	32u		//1B02FF0A	B相功率反向事件
#define	NUM_RevPowerC	32u		//1B03FF0A	C相功率反向事件
#define	NUM_VoltNegPhs	32u		//1400FF0A	电压逆相序事件
#define	NUM_CurrNegPhs	32u		//1500FF0A	电流逆相序事件
#define	NUM_VoltUnBlnc	32u		//1600FF0A	电压不平衡事件
#define	NUM_CurrUnBlnc	32u		//1700FF0A	电流不平衡事件
#define	NUM_SerUnBlnc	32u		//2000FF0A	电流严重不平衡事件
#define	NUM_LowerPfA	10u		//1F01FF0A	A相功率因数超限事件
#define	NUM_LowerPfB	10u		//1F02FF0A	B相功率因数超限事件
#define	NUM_LowerPfC	10u		//1F03FF0A	C相功率因数超限事件
#define	NUM_LowerPfT	10u		//1F00FF0A	合相功率因数超限事件
#define	NUM_RevPowerT	10u		//2100FF0A	潮流反向事件
#if	(TP_Meter == TP_Local)//本地表
#define	NUM_PrgRateTab	10u		//03300F0A	费率表编程事件
#define	NUM_PrgStepTab	10u		//0330100A	阶梯表编程事件
#define	NUM_BuyCashRcd	10u		//03330X0A	购电事件
#define	NUM_WrongCard	10u		//0330130A	异常插卡事件
#define	NUM_SubCashRcd	10u		//0334000A	退款事件
#else
#define	NUM_PrgRateTab	0u		//03300F0A	费率表编程事件
#define	NUM_PrgStepTab	0u		//0330100A	阶梯表编程事件
#define	NUM_BuyCashRcd	0u		//03330X0A	购电事件
#define	NUM_WrongCard	0u		//0330130A	异常插卡事件
#define	NUM_SubCashRcd	0u		//0334000A	退款事件
#endif
#define	NUM_ClrEvent	10u		//0330030A	事件清零事件
#define	NUM_ClrMeter	10u		//0330010A	电表清零事件
//事件记录长度定义--------------------------------------------------------------
#define	LEN_AllLstVolt	15u		//	全失压事件			（发生时刻.6 +电流值.3 +结束时刻.6）
#define	LEN_VoltQlty	108u	//	电压合格率			（合,A,B,C相）*（电压检测时间.3 +电压合格率.3 +电压超限率.3 +电压超上限时间.3 +电压超下限时间.3 +最高电压.2 +最高电压出现时间.4  +最低电压.2 +最低电压出现时间.4）
#define	LEN_LostPower	12u		//	掉电事件			（发生时刻.6 +结束时刻.6）
#define	LEN_DmdOver		20u		//*	X需量超限事件		（发生时刻.6 +结束时刻.6 +超限期间最大需量及发生时间.8）
#define	LEN_Program		50u		//	编程事件			（发生时刻.6 +操作者代码.4 +数据标识码.4*10）
#define	LEN_ClrDemand	202u	//~	需量清零事件		（发生时刻.6 +操作者代码.4 +需量清零前（合,A,B,C相）（正,反向有功,象限1,2,3,4无功）最大需量及发生时间.192）
#define	LEN_CaliTime	16u		//	校时事件			（操作者代码.4 +校时前时间.6 +校时后时间.6）
#define	LEN_PrgSlotTab	53u		//$	时段表编程事件		（发生时刻.6 +操作者代码.4 +表序号.1 +编程前第m套第n时段表.42）
#define	LEN_PrgZoneTab	94u		//	时区表编程事件		（发生时刻.6 +操作者代码.4 +编程前第一,二套时区表.84）
#define	LEN_PrgOffDay	11u		//	周休日编程事件		（发生时刻.6 +操作者代码.4 +编程前日时段表号.1）
#define	LEN_PrgHoliday	15u		//$	节假日编程事件		（发生时刻.6 +操作者代码.4 +节假日.1 +编程前节假日数据.4）
#define	LEN_PrgActCmbZ	11u		//	组合字编程事件		（发生时刻.6 +操作者代码.4 +编程前组合字.1）
#define	LEN_PrgRe1CmbZ	11u		//	组合字编程事件		（同上）
#define	LEN_PrgRe2CmbZ	11u		//	组合字编程事件		（同上）
#define	LEN_PrgCntDay	16u		//	结算日编程事件		（发生时刻.6 +操作者代码.4 +编程前3个结算日数据.6）
#define	LEN_OpenCover	60u		//	开表盖事件			（发生时刻.6 +结束时刻.6 +开盖（前,后）（正,反向有功,象限1,2,3,4无功）总电能.48）
#define	LEN_OpenSheel	60u		//	开端钮盖事件		（同上）
#define	LEN_RenewKey	15u		//	密钥更新事件		（发生时刻.6 +操作者代码.4 +密钥更新总条数.1+更新前的密钥状态字.4）
#define	LEN_MagInter	28u		//	磁场干扰事件		（发生时刻.6 +结束时刻.6 +发生时刻正反向总电能.8 +结束时刻正反向总电能.8）
#define	LEN_RelayFail	29u		//	开关误动作事件		（发生时刻.6 +结束时刻.6 +误动作后状态.1 +误动作前正反向总电能.8 +误动作后正反向总电能.8）
#define	LEN_PowerFail	28u		//	电源异常事件		（发生时刻.6 +结束时刻.6 +发生时刻正反向总电能.8  +结束时刻正反向总电能.8）
#define	LEN_RelayOff	34u		//	跳闸事件			（发生时刻.6 +操作者代码.4 +跳闸时（正,反向有功,象限1,2,3,4无功）总电能.24）
#define	LEN_RelayOn		34u		//	合闸事件			（同上）
#define	LEN_LossVolt	195u	//~	X相失压事件			（发生时刻.6 +发生时刻（正,反向有功,组合无功1,2）总电能.16 +发生时刻（A,B,C相）（正,反向有功,组合无功1,2总电能,电压,电流,有功功率,无功功率,功率因数）.3*(16+13) +失压期间（合,A,B,C相）安时数.16 +结束时刻.6 +结束时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 ）
#define	LEN_UndrVolt	195u	//~	X相欠压事件			（同上）
#define	LEN_OverVolt	195u	//~	X相过压事件			（同上）
#define	LEN_OffPhase	195u	//~	X相断相事件			（同上）
#define	LEN_LossCurr	179u	//~	X相失流事件			（发生时刻.6 +发生时刻（正,反向有功,组合无功1,2）总电能.16 +发生时刻（A,B,C相）（正,反向有功,组合无功1,2总电能,电压,电流,有功功率,无功功率,功率因数）.3*(16+13) +结束时刻.6 +结束时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64）
#define	LEN_OverCurr	179u	//~	X相过流事件			（同上）
#define	LEN_FailCurr	179u	//~	X相断流事件			（同上）
#define	LEN_OverLoad	140u	//~	X相过载事件			（发生时刻.6 +发生时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 +结束时刻.6 +结束时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 ）
#define	LEN_RevPower	140u	//~	X相功率反向事件		（同上）
#define	LEN_VoltNegPhs	140u	//~	电压逆相序事件		（同上）
#define	LEN_CurrNegPhs	140u	//~	电流逆相序事件		（同上）
#define	LEN_UnBalance	143u	//~	电压,流不平衡事件	（发生时刻.6 +发生时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 +最大不平衡率.3 +结束时刻.6 +结束时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64）
#define	LEN_LowerPf		44u		//	X相功率因数超限事件	（发生时刻.6 +发生时刻（正,反向有功,组合无功1,2）总电能.16 +结束时刻.6 +结束时刻（正,反向有功,组合无功1,2）总电能.16 ）
#define	LEN_RevPowerT	71u		//	潮流反向事件		（发生时刻.6 +总有功功率方向.1 +（合,A,B,C相）（正,反有功,组合无功1,2）总电能.64）
#define	LEN_PrgRateTab	42u		//	费率表编程事件		（发生时刻.6 +操作者代码.4 +编程前当前套数据.16+编程前备用套数据.16）
#define	LEN_PrgStepTab	138u	//	阶梯表编程事件		（发生时刻.6 +操作者代码.4 +编程前当前套数据.64+编程前备用套数据.64）
#define	LEN_BuyCashRcd	23u		//	购电事件			（发生时刻.5 +购电后购电次数.2 +购电金额.4 +购电前剩余金额.4 +购电后剩余金额.4 +购电后累计购电金额.4）
#define	LEN_WrongCard	36u		//	异常插卡事件		（发生时刻.6 +卡序列号.8 +错误信息字.1 +操作命令头.5 +错误相应状态.2 +购电总次数.2 +剩余金额.4 +正反向总电能.8）
#define	LEN_SubCashRcd	19u		//	退款事件			（发生时刻.5 +退款前购电次数.2 +退款金额.4 +退款前剩余金额.4 +退款后剩余金额.4）
#define	LEN_ClrEvent	14u		//	事件清零事件		（发生时刻.6 +操作者代码.4 +事件清零数据标识码.4）
#define	LEN_ClrMeter	106u	//	电表清零事件		（发生时刻.6 +操作者代码.4 +电表清零前（合,A,B,C相）（正,反向有功,象限1,2,3,4无功）总电能.96）
//事件记录索引地址定义----------------------------------------------------------
#define	IE_AllLstVolt	(IE_RecdStr								)			//全失压事件
#define	IE_VoltQlty		(IE_AllLstVolt	+(NUM_AllLstVolt?	5:0))			//电压合格率
#define	IE_LostPower	(IE_VoltQlty	+(NUM_VoltQlty?		2:0))			//掉电事件
#define	IE_PAcDmdOver	(IE_LostPower	+(NUM_LostPower?	2:0))			//正向有功需量超限事件
#define	IE_NAcDmdOver	(IE_PAcDmdOver	+(NUM_DmdOver?		2:0))			//反向有功需量超限事件
#define	IE_Re1DmdOver	(IE_NAcDmdOver	+(NUM_DmdOver?		2:0))			//象限一无功需量超限事件
#define	IE_Re2DmdOver	(IE_Re1DmdOver	+(NUM_DmdOver?		2:0))			//象限二无功需量超限事件
#define	IE_Re3DmdOver	(IE_Re2DmdOver	+(NUM_DmdOver?		2:0))			//象限三无功需量超限事件
#define	IE_Re4DmdOver	(IE_Re3DmdOver	+(NUM_DmdOver?		2:0))			//象限四无功需量超限事件
#define	IE_Program		(IE_Re4DmdOver	+(NUM_DmdOver?		2:0))			//编程事件
#define	IE_ClrDemand	(IE_Program		+(NUM_Program?		2:0))			//需量清零事件
#define	IE_CaliTime		(IE_ClrDemand	+(NUM_ClrDemand?	2:0))			//校时事件
#define	IE_PrgSlotTab	(IE_CaliTime	+(NUM_CaliTime?		2:0))			//*时段表编程事件
#define	IE_PrgZoneTab	(IE_PrgSlotTab	+(NUM_PrgSlotTab?	2:0))			//时区表编程事件
#define	IE_PrgOffDay	(IE_PrgZoneTab	+(NUM_PrgZoneTab?	2:0))			//周休日编程事件
#define	IE_PrgHoliday	(IE_PrgOffDay	+(NUM_PrgOffDay?	2:0))			//*节假日编程事件
#define	IE_PrgActCmbZ	(IE_PrgHoliday	+(NUM_PrgHoliday?	2:0))			//组合字编程事件
#define	IE_PrgRe1CmbZ	(IE_PrgActCmbZ	+(NUM_PrgActCmbZ?	2:0))			//组合字编程事件
#define	IE_PrgRe2CmbZ	(IE_PrgRe1CmbZ	+(NUM_PrgRe1CmbZ?	2:0))			//组合字编程事件
#define	IE_PrgCntDay	(IE_PrgRe2CmbZ	+(NUM_PrgRe2CmbZ?	2:0))			//结算日编程事件
#define	IE_OpenCover	(IE_PrgCntDay	+(NUM_PrgCntDay?	2:0))			//开表盖事件
#define	IE_OpenSheel	(IE_OpenCover	+(NUM_OpenCover?	2:0))			//开端钮盖事件
#define	IE_RenewKey		(IE_OpenSheel	+(NUM_OpenSheel?	2:0))			//密钥更新事件
#define	IE_MagInter		(IE_RenewKey	+(NUM_RenewKey?		2:0))			//磁场干扰事件
#define	IE_RelayFail	(IE_MagInter	+(NUM_MagInter?		2:0))			//开关误动作事件
#define	IE_PowerFail	(IE_RelayFail	+(NUM_RelayFail?	2:0))			//电源异常事件
#define	IE_RelayOff		(IE_PowerFail	+(NUM_PowerFail?	2:0))			//跳闸事件
#define	IE_RelayOn		(IE_RelayOff	+(NUM_RelayOff?		2:0))			//合闸事件
#define	IE_LossVoltA	(IE_RelayOn		+(NUM_RelayOn?		2:0))			//A相失压事件
#define	IE_LossVoltB	(IE_LossVoltA	+(NUM_LossVoltA?	5:0))			//B相失压事件
#define	IE_LossVoltC	(IE_LossVoltB	+(NUM_LossVoltB?	5:0))			//C相失压事件
#define	IE_UndrVoltA	(IE_LossVoltC	+(NUM_LossVoltC?	5:0))			//A相欠压事件
#define	IE_UndrVoltB	(IE_UndrVoltA	+(NUM_UndrVoltA?	5:0))			//B相欠压事件
#define	IE_UndrVoltC	(IE_UndrVoltB	+(NUM_UndrVoltB?	5:0))			//C相欠压事件
#define	IE_OverVoltA	(IE_UndrVoltC	+(NUM_UndrVoltC?	5:0))			//A相过压事件
#define	IE_OverVoltB	(IE_OverVoltA	+(NUM_OverVoltA?	5:0))			//B相过压事件
#define	IE_OverVoltC	(IE_OverVoltB	+(NUM_OverVoltB?	5:0))			//C相过压事件
#define	IE_OffPhaseA	(IE_OverVoltC	+(NUM_OverVoltC?	5:0))			//A相断相事件
#define	IE_OffPhaseB	(IE_OffPhaseA	+(NUM_OffPhaseA?	5:0))			//B相断相事件
#define	IE_OffPhaseC	(IE_OffPhaseB	+(NUM_OffPhaseB?	5:0))			//C相断相事件
#define	IE_LossCurrA	(IE_OffPhaseC	+(NUM_OffPhaseC?	5:0))			//A相失流事件
#define	IE_LossCurrB	(IE_LossCurrA	+(NUM_LossCurrA?	5:0))			//B相失流事件
#define	IE_LossCurrC	(IE_LossCurrB	+(NUM_LossCurrB?	5:0))			//C相失流事件
#define	IE_OverCurrA	(IE_LossCurrC	+(NUM_LossCurrC?	5:0))			//A相过流事件
#define	IE_OverCurrB	(IE_OverCurrA	+(NUM_OverCurrA?	5:0))			//B相过流事件
#define	IE_OverCurrC	(IE_OverCurrB	+(NUM_OverCurrB?	5:0))			//C相过流事件
#define	IE_FailCurrA	(IE_OverCurrC	+(NUM_OverCurrC?	5:0))			//A相断流事件
#define	IE_FailCurrB	(IE_FailCurrA	+(NUM_FailCurrA?	5:0))			//B相断流事件
#define	IE_FailCurrC	(IE_FailCurrB	+(NUM_FailCurrB?	5:0))			//C相断流事件
#define	IE_OverLoadA	(IE_FailCurrC	+(NUM_FailCurrC?	5:0))			//A相过载事件
#define	IE_OverLoadB	(IE_OverLoadA	+(NUM_OverLoadA?	5:0))			//B相过载事件
#define	IE_OverLoadC	(IE_OverLoadB	+(NUM_OverLoadB?	5:0))			//C相过载事件
#define	IE_RevPowerA	(IE_OverLoadC	+(NUM_OverLoadC?	5:0))			//A相功率反向事件
#define	IE_RevPowerB	(IE_RevPowerA	+(NUM_RevPowerA?	5:0))			//B相功率反向事件
#define	IE_RevPowerC	(IE_RevPowerB	+(NUM_RevPowerB?	5:0))			//C相功率反向事件
#define	IE_VoltNegPhs	(IE_RevPowerC	+(NUM_RevPowerC?	5:0))			//电压逆相序事件
#define	IE_CurrNegPhs	(IE_VoltNegPhs	+(NUM_VoltNegPhs?	5:0))			//电流逆相序事件
#define	IE_VoltUnBlnc	(IE_CurrNegPhs	+(NUM_CurrNegPhs?	5:0))			//电压不平衡事件
#define	IE_CurrUnBlnc	(IE_VoltUnBlnc	+(NUM_VoltUnBlnc?	5:0))			//电流不平衡事件
#define	IE_SerUnBlnc	(IE_CurrUnBlnc	+(NUM_CurrUnBlnc?	5:0))			//电流严重不平衡事件
#define	IE_LowerPfA		(IE_SerUnBlnc	+(NUM_SerUnBlnc?	5:0))			//A相功率因数超限事件
#define	IE_LowerPfB		(IE_LowerPfA	+(NUM_LowerPfA?		5:0))			//B相功率因数超限事件
#define	IE_LowerPfC		(IE_LowerPfB	+(NUM_LowerPfB?		5:0))			//C相功率因数超限事件
#define	IE_LowerPfT		(IE_LowerPfC	+(NUM_LowerPfC?		5:0))			//合相功率因数超限事件
#define	IE_RevPowerT	(IE_LowerPfT	+(NUM_LowerPfT?		5:0))			//潮流反向事件
#define	IE_PrgRateTab	(IE_RevPowerT	+(NUM_RevPowerT?	2:0))			//费率表编程事件
#define	IE_PrgStepTab	(IE_PrgRateTab	+(NUM_PrgRateTab?	2:0))			//阶梯表编程事件
#define	IE_BuyCashRcd	(IE_PrgStepTab	+(NUM_PrgStepTab?	2:0))			//购电事件
#define	IE_WrongCard	(IE_BuyCashRcd	+(NUM_BuyCashRcd?	2:0))			//异常插卡事件
#define	IE_SubCashRcd	(IE_WrongCard	+(NUM_WrongCard?	2:0))			//退款事件
#define	IE_ClrEvent		(IE_SubCashRcd	+(NUM_SubCashRcd?	2:0))			//事件清零事件
#define	IE_ClrMeter		(IE_ClrEvent	+(NUM_ClrEvent?		2:0))			//电表清零事件
#define	IE_RecdEnd		(IE_ClrMeter	+(NUM_ClrMeter?		2:0))			//
//事件记录数据地址EEPROM部分定义------------------------------------------------
#define	DE_AllLstVolt	(DE_RecdStr										)	//全失压事件
#define	DE_VoltQlty		(DE_AllLstVolt	+NUM_AllLstVolt	*LEN_AllLstVolt	)	//电压合格率
#define	DE_LostPower	(DE_VoltQlty	+NUM_VoltQlty	*LEN_VoltQlty	)	//掉电事件
#define	DE_PAcDmdOver	(DE_LostPower	+NUM_LostPower	*LEN_LostPower	)	//正向有功需量超限事件
#define	DE_NAcDmdOver	(DE_PAcDmdOver	+NUM_DmdOver	*LEN_DmdOver	)	//反向有功需量超限事件
#define	DE_Re1DmdOver	(DE_NAcDmdOver	+NUM_DmdOver	*LEN_DmdOver	)	//象限一无功需量超限事件
#define	DE_Re2DmdOver	(DE_Re1DmdOver	+NUM_DmdOver	*LEN_DmdOver	)	//象限二无功需量超限事件
#define	DE_Re3DmdOver	(DE_Re2DmdOver	+NUM_DmdOver	*LEN_DmdOver	)	//象限三无功需量超限事件
#define	DE_Re4DmdOver	(DE_Re3DmdOver	+NUM_DmdOver	*LEN_DmdOver	)	//象限四无功需量超限事件
#define	DE_Program		(DE_Re4DmdOver	+NUM_DmdOver	*LEN_DmdOver	)	//编程事件
#define	DE_CaliTime		(DE_Program		+NUM_Program	*LEN_Program	)	//校时事件
#define	DE_PrgSlotTab	(DE_CaliTime	+NUM_CaliTime	*LEN_CaliTime	)	//时段表编程事件
#define	DE_PrgZoneTab	(DE_PrgSlotTab	+NUM_PrgSlotTab	*LEN_PrgSlotTab	)	//时区表编程事件
#define	DE_PrgOffDay	(DE_PrgZoneTab	+NUM_PrgZoneTab	*LEN_PrgZoneTab	)	//周休日编程事件
#define	DE_PrgHoliday	(DE_PrgOffDay	+NUM_PrgOffDay	*LEN_PrgOffDay	)	//节假日编程事件
#define	DE_PrgActCmbZ	(DE_PrgHoliday	+NUM_PrgHoliday	*LEN_PrgHoliday	)	//组合字编程事件
#define	DE_PrgRe1CmbZ	(DE_PrgActCmbZ	+NUM_PrgActCmbZ	*LEN_PrgActCmbZ	)	//组合字编程事件
#define	DE_PrgRe2CmbZ	(DE_PrgRe1CmbZ	+NUM_PrgRe1CmbZ	*LEN_PrgRe1CmbZ	)	//组合字编程事件
#define	DE_PrgCntDay	(DE_PrgRe2CmbZ	+NUM_PrgRe2CmbZ	*LEN_PrgRe2CmbZ	)	//结算日编程事件
#define	DE_OpenCover	(DE_PrgCntDay	+NUM_PrgCntDay	*LEN_PrgCntDay	)	//开表盖事件
#define	DE_OpenSheel	(DE_OpenCover	+NUM_OpenCover	*LEN_OpenCover	)	//开端钮盖事件
#define	DE_RenewKey		(DE_OpenSheel	+NUM_OpenSheel	*LEN_OpenSheel	)	//密钥更新事件
#define	DE_MagInter		(DE_RenewKey	+NUM_RenewKey	*LEN_RenewKey	)	//磁场干扰事件
#define	DE_RelayFail	(DE_MagInter	+NUM_MagInter	*LEN_MagInter	)	//开关误动作事件
#define	DE_PowerFail	(DE_RelayFail	+NUM_RelayFail	*LEN_RelayFail	)	//电源异常事件
#define	DE_RelayOff		(DE_PowerFail	+NUM_PowerFail	*LEN_PowerFail	)	//跳闸事件
#define	DE_RelayOn		(DE_RelayOff	+NUM_RelayOff	*LEN_RelayOff	)	//合闸事件
#define	DE_LowerPfA		(DE_RelayOn		+NUM_RelayOn	*LEN_RelayOn	)	//A相功率因数超限事件
#define	DE_LowerPfB		(DE_LowerPfA	+NUM_LowerPfA	*LEN_LowerPf	)	//B相功率因数超限事件
#define	DE_LowerPfC		(DE_LowerPfB	+NUM_LowerPfB	*LEN_LowerPf	)	//C相功率因数超限事件
#define	DE_LowerPfT		(DE_LowerPfC	+NUM_LowerPfC	*LEN_LowerPf	)	//合相功率因数超限事件
#define	DE_RevPowerT	(DE_LowerPfT	+NUM_LowerPfT	*LEN_LowerPf	)	//潮流反向事件
#define	DE_PrgRateTab	(DE_RevPowerT	+NUM_RevPowerT	*LEN_RevPowerT	)	//费率表编程事件
#define	DE_PrgStepTab	(DE_PrgRateTab	+NUM_PrgRateTab	*LEN_PrgRateTab	)	//阶梯表编程事件
#define	DE_BuyCashRcd	(DE_PrgStepTab	+NUM_PrgStepTab	*LEN_PrgStepTab	)	//购电事件
#define	DE_WrongCard	(DE_BuyCashRcd	+NUM_BuyCashRcd	*LEN_BuyCashRcd	)	//异常插卡事件
#define	DE_SubCashRcd	(DE_WrongCard	+NUM_WrongCard	*LEN_WrongCard	)	//退款事件
#define	DE_ClrEvent		(DE_SubCashRcd	+NUM_SubCashRcd	*LEN_SubCashRcd	)	//事件清零事件
#define	DE_ClrMeter		(DE_ClrEvent	+NUM_ClrEvent	*LEN_ClrEvent	)	//电表清零事件
#define	DE_RecdEnd		(DE_ClrMeter	+NUM_ClrMeter	*LEN_ClrMeter	)	//
//事件记录数据地址FLASH部分定义-------------------------------------------------
#define	DE_ClrDemand	(FH_RecdStr								)			//需量清零事件
#define	DE_LossVoltA	(DE_ClrDemand	+(NUM_ClrDemand?	2:0))			//A相失压事件
#define	DE_LossVoltB	(DE_LossVoltA	+(NUM_LossVoltA?	2:0))			//B相失压事件
#define	DE_LossVoltC	(DE_LossVoltB	+(NUM_LossVoltB?	2:0))			//C相失压事件
#define	DE_UndrVoltA	(DE_LossVoltC	+(NUM_LossVoltC?	2:0))			//A相欠压事件
#define	DE_UndrVoltB	(DE_UndrVoltA	+(NUM_UndrVoltA?	2:0))			//B相欠压事件
#define	DE_UndrVoltC	(DE_UndrVoltB	+(NUM_UndrVoltB?	2:0))			//C相欠压事件
#define	DE_OverVoltA	(DE_UndrVoltC	+(NUM_UndrVoltC?	2:0))			//A相过压事件
#define	DE_OverVoltB	(DE_OverVoltA	+(NUM_OverVoltA?	2:0))			//B相过压事件
#define	DE_OverVoltC	(DE_OverVoltB	+(NUM_OverVoltB?	2:0))			//C相过压事件
#define	DE_OffPhaseA	(DE_OverVoltC	+(NUM_OverVoltC?	2:0))			//A相断相事件
#define	DE_OffPhaseB	(DE_OffPhaseA	+(NUM_OffPhaseA?	2:0))			//B相断相事件
#define	DE_OffPhaseC	(DE_OffPhaseB	+(NUM_OffPhaseB?	2:0))			//C相断相事件
#define	DE_LossCurrA	(DE_OffPhaseC	+(NUM_OffPhaseC?	2:0))			//A相失流事件
#define	DE_LossCurrB	(DE_LossCurrA	+(NUM_LossCurrA?	2:0))			//B相失流事件
#define	DE_LossCurrC	(DE_LossCurrB	+(NUM_LossCurrB?	2:0))			//C相失流事件
#define	DE_OverCurrA	(DE_LossCurrC	+(NUM_LossCurrC?	2:0))			//A相过流事件
#define	DE_OverCurrB	(DE_OverCurrA	+(NUM_OverCurrA?	2:0))			//B相过流事件
#define	DE_OverCurrC	(DE_OverCurrB	+(NUM_OverCurrB?	2:0))			//C相过流事件
#define	DE_FailCurrA	(DE_OverCurrC	+(NUM_OverCurrC?	2:0))			//A相断流事件
#define	DE_FailCurrB	(DE_FailCurrA	+(NUM_FailCurrA?	2:0))			//B相断流事件
#define	DE_FailCurrC	(DE_FailCurrB	+(NUM_FailCurrB?	2:0))			//C相断流事件
#define	DE_OverLoadA	(DE_FailCurrC	+(NUM_FailCurrC?	2:0))			//A相过载事件
#define	DE_OverLoadB	(DE_OverLoadA	+(NUM_OverLoadA?	2:0))			//B相过载事件
#define	DE_OverLoadC	(DE_OverLoadB	+(NUM_OverLoadB?	2:0))			//C相过载事件
#define	DE_RevPowerA	(DE_OverLoadC	+(NUM_OverLoadC?	2:0))			//A相功率反向事件
#define	DE_RevPowerB	(DE_RevPowerA	+(NUM_RevPowerA?	2:0))			//B相功率反向事件
#define	DE_RevPowerC	(DE_RevPowerB	+(NUM_RevPowerB?	2:0))			//C相功率反向事件
#define	DE_VoltNegPhs	(DE_RevPowerC	+(NUM_RevPowerC?	2:0))			//电压逆相序事件
#define	DE_CurrNegPhs	(DE_VoltNegPhs	+(NUM_VoltNegPhs?	2:0))			//电流逆相序事件
#define	DE_VoltUnBlnc	(DE_CurrNegPhs	+(NUM_CurrNegPhs?	2:0))			//电压不平衡事件
#define	DE_CurrUnBlnc	(DE_VoltUnBlnc	+(NUM_VoltUnBlnc?	2:0))			//电流不平衡事件
#define	DE_SerUnBlnc	(DE_CurrUnBlnc	+(NUM_CurrUnBlnc?	2:0))			//电流严重不平衡事件
#define	FH_RecdEnd		(DE_SerUnBlnc	+(NUM_SerUnBlnc?	2:0))			//

/*******************************************************************************
		Fcuction Declaration
*******************************************************************************/
BOOL Read_RecdData(ID_RECD id, INT8U *pBuff, INT8U lst, INT8U off, INT8U len);//读取事件纪录
BOOL Write_RecdData(ID_RECD id, const INT8U *pBuff, INT8U off, INT8U len, INT32U add);//写入事件纪录
BOOL Clear_RecdData(ID_RECD id);											//清除事件纪录

/*******************************************************************************
		End File
*******************************************************************************/
#endif
