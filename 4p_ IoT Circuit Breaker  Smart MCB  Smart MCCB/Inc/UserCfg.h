
#ifndef	_USERCFG_H_
#define	_USERCFG_H_

/*******************************************************************************
		User Macro Definition
*******************************************************************************/
//软件版本----------------------------------------------------------------------
#define	TP_Release		0					//正式版本
#define	TP_Debug		1					//调试版本
#define	TP_SoftVer		TP_Release

#define	EN_PLC			ENABLE				//载波通讯功能开关

//电能表类型--------------------------------------------------------------------
#define	TP_Normal		0					//普通表
#define	TP_Remote		1					//远程表
#define	TP_Local		2					//本地表
#define	TP_Meter		TP_Remote

//MCU类型-----------------------------------------------------------------------
#define	TP_HT602x		0					//HT602x
#define	TP_HT622x		1					//HT622x
#define	TP_HT603x		2					//HT603x
#define	TP_MCU			TP_HT602x
//时钟校准点（温度采样点）------------------------------------------------------
#define NUM_SampRTC		3u					//建议:HT602x用3点,HT622x用4点

//继电器类型--------------------------------------------------------------------
#define	TP_In			0					//内置继电器
#define	TP_Ex			1					//外置继电器
#define	TP_Relay		TP_In

//休眠模式类型------------------------------------------------------------------
#define	TP_Sleep		0					//Sleep模式
#define	TP_Hold			1					//Hold模式
#define	TP_Pause		TP_Hold

//PCB版本-----------------------------------------------------------------------
#define	VER_0			0					//V0版本
#define	VER_1			1					//V1版本
#define	VER_2			2					//V2版本
#define	VER_3			3					//V3版本
#define	VER_4			4					//V4版本
#define	VER_5			5					//V4版本
#define	VER_6			6					//V4版本
#define	VER_PCB			VER_0

//MCU速率-----------------------------------------------------------------------
#define	SPD_22000K		0
#define	SPD_11000K		1
#define	SPD_5500K		2
#define	SPD_2750K		3
#define	SPD_1375K		4
#define	SPD_688K		5
#define	SPD_344K		6
#define	SPD_172K		7
#define SPD_MCU			SPD_5500K

//系统节拍定义------------------------------------------------------------------
#define	C_SysTick		64u					//任务定时节拍频率（64Hz）

//计量部分定义------------------------------------------------------------------
#define	C_StepNum		4u					//阶梯数
#define	C_RateNum		4u					//费率数
#define	C_Un			220u				//额定电压(220V)
#define	C_Ib			5u					//基本电流(5A)
#define	C_IMP			4u					//脉冲常数(400imp/kWh)
#define C_Femu			2u					//EMU时钟频率   = 2MHz

/*******************************************************************************
		User Constant Declaration
*******************************************************************************/
extern const INT8U TAB_SoftVer[7];											//软件版本（内部用）

//TaskDisplay-------------------------------------------------------------------
extern const INT8U TAB_DispPara[];											//显示参数
extern const INT16U TAB_AutoItem[];											//轮显项目
extern const INT16U TAB_ButtItem[];											//按显项目

//TaskFreeze--------------------------------------------------------------------
extern const INT8U TAB_FrezModeZ[];											//冻结模式字
extern const INT8U TAB_FreezeSet[];											//冻结设置
extern const INT8U TAB_HourFzStr[];											//整点冻结起始时间
extern const INT8U TAB_LoadPara[];											//负荷纪录参数

//TaskMetering------------------------------------------------------------------
extern const INT8U TAB_EnyCmbZ[];											//组合方式特征字
extern const INT8U TAB_VoltQSet[];											//电压考核上下限,电压上下限
extern const INT8U TAB_EventPara[];											//事件记录判断参数
extern const INT8U TAB_DmdPara[];											//需量设置参数

//TaskRate----------------------------------------------------------------------
extern const INT8U TAB_RatePara[];											//费率参数
extern const INT8U TAB_ZoneSwSet[];											//时区表设置
extern const INT8U TAB_SlotSwSet[];											//时段表设置
extern const INT8U TAB_ZoneTab[];											//第1,2套时区表
extern const INT8U TAB_SlotTab[];											//第1,2套第1~8日时段表

//TaskRecord--------------------------------------------------------------------
extern const INT8U TAB_MeterRunZ[];											//电表运行特征字

//TaskRelay---------------------------------------------------------------------
extern const INT8U TAB_SwOffDly[];											//跳闸延时时间
extern const INT8U TAB_OffPrtSet[];											//保护电流触发下限
extern const INT8U TAB_OffPrtDly[];											//保护电流判断时间

//TaskReport--------------------------------------------------------------------
extern const INT8U TAB_ReportMod[];											//上报模式字
extern const INT8U TAB_RptRstTmr[];											//上报字复位延时时间

//TaskRmtFee--------------------------------------------------------------------
extern const INT8U TAB_IrAuthTmr[];											//红外认证有效时间
extern const INT8U TAB_KeyNums[];											//密钥条数
extern const INT8U TAB_KeyFlag[];											//密钥更新状态

//TaskLclFee--------------------------------------------------------------------
extern const INT8U TAB_OpenAccnt[];											//开户标志
extern const INT8U TAB_RateSwSet[];											//费率表设置
extern const INT8U TAB_StepSwSet[];											//阶梯表设置
extern const INT8U TAB_BuyNums[];											//累计购电次数
extern const INT8U TAB_BuyCash[];											//累计购电金额
extern const INT8U TAB_RateTar[];											//当前,备用套费率电价
extern const INT8U TAB_StepValue[];											//当前套阶梯值
extern const INT8U TAB_StepTar[];											//当前套阶梯电价
extern const INT8U TAB_YearCount[];											//当前套年结算日
extern const INT8U TAB_BillFlag[];											//月,年阶梯标志
//extern const INT8U TAB_CardNo,											//卡号
//extern const INT8U TAB_MeterNo,											//表号
extern const INT8U TAB_AlarmLmt[];											//报警金额1,2限值
extern const INT8U TAB_OverLmt[];											//透支金额限值
extern const INT8U TAB_StockLmt[];											//囤积金额限值
extern const INT8U TAB_PermitLmt[];											//合闸允许金额限值
extern const INT8U TAB_BackCash[];											//退费金额
extern const INT8U TAB_CashPulse[];											//金额小数电量
extern const INT8U TAB_RateTarB[];											//当前,备用套费率电价
extern const INT8U TAB_CurPrice[];											//当前电价
extern const INT8U TAB_NoLawNum[];											//错误插卡次数
extern const INT8U TAB_CashStatus[];										//剩余金额状态

//TaskComm----------------------------------------------------------------------
extern const INT8U TAB_MeterAddr[];											//通讯地址
extern const INT8U TAB_PassWord2[];											//02级密码
extern const INT8U TAB_PassWord4[];											//04级密码
extern const INT8U TAB_ComBaudZ[];											//波特率特征字

//DLT645-2007-------------------------------------------------------------------
extern const INT8U TAB_CustomNo[];											//客户编号
extern const INT8U TAB_CTRatio[];											//电压,电流互感器变比
extern const INT8U TAB_AssertNo[];											//资产管理编码,厂家软,硬件版本号
extern const INT8U TAB_RatedVot[];											//额定电压
extern const INT8U TAB_RatedCur[]; 											//额定电流
extern const INT8U TAB_MaxCurr[];											//最大电流
extern const INT8U TAB_ActAccu[];											//有功准确度
extern const INT8U TAB_RctAccu[];											//无功准确度
extern const INT8U TAB_MeterTyp[];											//电表型号
extern const INT8U TAB_PrdDate[];											//生产日期
extern const INT8U TAB_PrtclVer[];											//协议版本
extern const INT8U TAB_BackupVer[];											//软件备案号
extern const INT8U TAB_VenderNo[];											//厂家编号
extern const INT8U TAB_Position[];											//电能表位置信息

/*******************************************************************************
		End File
*******************************************************************************/
#endif
