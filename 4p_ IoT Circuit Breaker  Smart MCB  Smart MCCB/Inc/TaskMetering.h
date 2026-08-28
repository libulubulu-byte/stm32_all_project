
#ifndef _TASKMETERING_H_
#define _TASKMETERING_H_

/*******************************************************************************
		Macro Definition
*******************************************************************************/
enum
{
	EV_LossVoltA,		//1001FF0A	A相失压事件
	EV_LossVoltB,		//1002FF0A	B相失压事件
	EV_LossVoltC,		//1003FF0A	C相失压事件
	EV_UndrVoltA,		//1101FF0A	A相欠压事件
	EV_UndrVoltB,		//1102FF0A	B相欠压事件
	EV_UndrVoltC,		//1103FF0A	C相欠压事件
	EV_OverVoltA,		//1201FF0A	A相过压事件
	EV_OverVoltB,		//1202FF0A	B相过压事件
	EV_OverVoltC,		//1203FF0A	C相过压事件
	EV_OffPhaseA,		//1301FF0A	A相断相事件
	EV_OffPhaseB,		//1302FF0A	B相断相事件
	EV_OffPhaseC,		//1303FF0A	C相断相事件
	
	EV_LossCurrA,		//1801FF0A	A相失流事件
	EV_LossCurrB,		//1802FF0A	B相失流事件
	EV_LossCurrC,		//1803FF0A	C相失流事件
	EV_OverCurrA,		//1901FF0A	A相过流事件
	EV_OverCurrB,		//1902FF0A	B相过流事件
	EV_OverCurrC,		//1903FF0A	C相过流事件
	EV_FailCurrA,		//1A01FF0A	A相断流事件
	EV_FailCurrB,		//1A02FF0A	B相断流事件
	EV_FailCurrC,		//1A03FF0A	C相断流事件
	
	EV_OverLoadA,		//1C01FF0A	A相过载事件
	EV_OverLoadB,		//1C02FF0A	B相过载事件
	EV_OverLoadC,		//1C03FF0A	C相过载事件
	EV_RevPowerA,		//1B01FF0A	A相功率反向事件
	EV_RevPowerB,		//1B02FF0A	B相功率反向事件
	EV_RevPowerC,		//1B03FF0A	C相功率反向事件
	EV_VoltNegPhs,		//1400FF0A	电压逆相序事件
	EV_CurrNegPhs,		//1500FF0A	电流逆相序事件
	
	EV_VoltUnBlnc,		//1600FF0A	电压不平衡事件
	EV_CurrUnBlnc,		//1700FF0A	电流不平衡事件
	EV_SerUnBlnc,		//2000FF0A	电流严重不平衡事件
	
	EV_LowerPfA,		//1F01FF0A	A相功率因数超限事件
	EV_LowerPfB,		//1F02FF0A	B相功率因数超限事件
	EV_LowerPfC,		//1F03FF0A	C相功率因数超限事件
	EV_LowerPfT,		//1F00FF0A	合相功率因数超限事件
	
	EV_RevPowerT,		//2100FF0A	潮流反向事件
	
	EV_PAcDmdOver,		//0312010A	正向有功需量超限事件
	EV_NAcDmdOver,		//0312020A	反向有功需量超限事件
	EV_Re1DmdOver,		//0312030A	象限一无功需量超限事件
	EV_Re2DmdOver,		//0312040A	象限二无功需量超限事件
	EV_Re3DmdOver,		//0312050A	象限三无功需量超限事件
	EV_Re4DmdOver,		//0312060A	象限四无功需量超限事件
	
	EV_LostPower,		//0311000A	掉电事件
	EV_LostVoltX,		//0305000A	全失压事件
	
//	EV_VoltQlty,		//0310 00-03 0A	电压合格率

	EV_EventNum,
	EV_NoEvent
};

typedef enum
{
	ID_Pa,			//A相有功功率
	ID_Pb,			//B相有功功率
	ID_Pc,			//C相有功功率
	ID_Pt,			//合相有功功率
	ID_Qa,			//A相无功功率
	ID_Qb,			//B相无功功率
	ID_Qc,			//C相无功功率
	ID_Qt,			//合相无功功率
	ID_Sa,			//A相视在功率
	ID_Sb,			//B相视在功率
	ID_Sc,			//C相视在功率
	ID_St,			//合相视在功率
	ID_Ua,			//A相电压有效值
	ID_Ub,			//B相电压有效值
	ID_Uc,			//C相电压有效值
	ID_Ut,			//合相电压有效值(矢量和)
	ID_Ia,			//A相电流有效值
	ID_Ib,			//B相电流有效值
	ID_Ic,			//C相电流有效值
	ID_It,			//合相电流有效值(矢量和)
	ID_Pfa,			//A相功率因素
	ID_Pfb,			//B相功率因素
	ID_Pfc,			//C相功率因素
	ID_Pft,			//合相功率因素
	ID_Pga,			//A相电流与电压夹角
	ID_Pgb,			//B相电流与电压夹角
	ID_Pgc,			//C相电流与电压夹角
//	ID_In,			//零线电流有效值
	ID_Freq,		//线频率
	
	ID_RealNum		//
}ID_REAL;			//实时数据项id定义

enum
{
	CMP_060Un,				//60%Un
	CMP_065Un,				//65%Un
	CMP_075Un,				//75%Un
	CMP_120Un,				//120%Un
	CMP_ProtI,				//拉闸保护电流
	CMP_StrPow,				//启动功率
	NUM_Comp				//比较项目
};

/*******************************************************************************
		Fcuction Declaration
*******************************************************************************/
void Init_TaskMetering(void);												//初始化计量任务
void Run_TaskMetering(void);												//运行计量任务
void Stop_TaskMetering(void);												//停止计量任务

void Read_MeterPara(void);




//void INT_PulseKey(void);													//电能脉冲检测
void INT_EnyTimer(void);													//电能处理定时器

/*******************************************************************************
		End File
*******************************************************************************/
#endif
