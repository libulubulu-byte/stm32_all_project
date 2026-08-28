
///*******************************************************************************
//		Include files
//*******************************************************************************/
//#include "TypeDef.h"
//#include "UserCfg.h"

//#include "Metering.h"

//#include "TaskMetering.h"
//#include "RecdData.h"
///*******************************************************************************
//		Variable Definition
//*******************************************************************************/
//static	BOOL	g_VoltStatus					__no_init;					//上电状态
///*******************************************************************************
//		Macro Definition
//*******************************************************************************/
////#define	C_PulseOver		1024u												//脉冲溢出，保存到EEPROM
////#define	C_PulseAdd		20u													//脉冲每秒累加 = 20脉冲

//#define	C_120Un			2640u												//120%Un电压:264.0V
//#define	C_75Un			1650u												//75%Un电压:165.0V
//#define	C_70Un			1540u												//70%Un电压:143.0V
//#define	C_60Un			1320u												//60%Un电压:132.0V
//#define	C_10Un			220u												//10%Un电压:22V
////		
//#define	C_05Ib			250u												//5%Ib:		250mA
//#define	C_StrIb			20u													//0.4%Ib:	20mA

//#define	C_StrPow		216u			//启动功率阈值设置					(Un*Ib*HFConst*imp*0.3%*2^23/(2.592*10^10))			

///*******************************************************************************
//		Variable Definition
//*******************************************************************************/
//static	volatile	INT16U	g_EnyTimer;										//电能处理定时器

//static	INT16U	g_kWhPulse[8][4]				__no_init;					//电量尾数脉冲(A有,B有,C有,合有,A无,B无,C无,合无)
//static	INT8U	g_EnyQuad[1+1+2]				__no_init;					//能量象限寄存器(A有,B有,C有,合有,A无,B无,C无,合无)
//#if	(TP_Meter == TP_Local)//本地表
//static	INT8U	g_CashPulse						__no_init;					//金额脉冲电量
//#endif
////static	INT8U	g_EnyCmbZ[LEN_EnyCmbZ+2] 		__no_init;					//组合方式特征字

//static	INT16U	g_SFlag							__no_init;					//计量芯片标志状态
//static	INT8U	g_FlagUI						__no_init;					//电压电流标志(Ua,Ub,Uc,Ut,Ia,Ib,Ic,It)
//static	INT8U	g_PowQuad						__no_init;					//功率象限寄存器(A有,B有,C有,合有,A无,B无,C无,合无)
//static	INT32U	g_Volt[3]						__no_init;					//电压采样数据
//static	INT32U	g_Curr[3]						__no_init;					//电流采样数据
//static	INT32U	g_PowP[4]						__no_init;					//有功功率采样数据
//static	INT32U	g_PowQ[4]						__no_init;					//无功功率采样数据
////static	INT32U	g_PowS[4]						__no_init;					//视在功率采样数据
//static	INT32U	g_Pf[4]							__no_init;					//功率因素采样数据

////static	INT8U	g_EventSet[LEN_EventSet+2]		__no_init;					//事件记录判断参数
////static	BOOL	g_EventSta[EV_EventNum]			__no_init;					//事件记录标志
////static	INT8U	g_EventTmr[EV_EventNum]			__no_init;					//事件判断计时
////static	INT32U	g_EventSec[EV_EventNum]			__no_init;					//事件累计时间

//static	INT8U	g_CompFlg[NUM_Comp]				__no_init;					//电压,流判断标志

////static	INT8U	g_EngyDataT[16]					__no_init;					//合相电量数据
////static	INT8U	g_EngyDataX[3][16]				__no_init;					//分相电量数据
////static	INT8U	g_RealDataX[3][13]				__no_init;					//分相实时数据
//static	BOOL	b_EngyDataT						__no_init;					//合相电量标志
//static	BOOL	b_EngyDataX						__no_init;					//分相电量标志
//static	BOOL	b_RealDataX						__no_init;					//分相实时标志

////static struct
////{
////	INT16U	tmr;															//计时器
////	INT32U	amp[3];															//A,B,C相
////}g_AmpereHr[12]									__no_init;					//安时数
////static	INT16U	g_UnBalance[3]					__no_init;					//不平衡率
////static	INT32U	g_MaxDemand[6]					__no_init;					//最大需量
////static	INT8U	g_MaxDmdTmr[6][6]				__no_init;					//最大需量发生时间

//static	INT32U	g_OffPrtSet						__no_init;					//保护电流触发下限

////static	INT8U	g_VoltIdx						__no_init;					//电压采样数据
////static struct
////{
////	INT16U	dMins;															//电压检测时间.2
////	INT16U	stand;															//电压合格时间.2
////	INT16U	overH;															//电压超上限时间.2
////	INT16U	overL;															//电压超下限时间.2
////}g_VoltDetT										__no_init;					//合相电压合格率检测
////static struct
////{
////	INT32U	samp[6];														//电压采样数据
////	INT16U	dMins;															//电压检测时间.2
////	INT16U	overH;															//电压超上限时间.2
////	INT16U	overL;															//电压超下限时间.2
////	INT16U	voltH;															//最高电压.2
////	INT8U	timeH[4];														//最高电压出现时间.4
////	INT16U	voltL;															//最低电压.2
////	INT8U	timeL[4];														//最低电压出现时间.4
////}g_VoltDetX[3]									__no_init;					//分相电压合格率检测

////static	INT8U	g_DmdPara[LEN_DmdPara+2] 		__no_init;					//需量设置参数

//static	INT8U	g_DmdQuad						__no_init;					//需量象限寄存器(A有,B有,C有,合有,A无,B无,C无,合无)
//static	INT8U	g_DmdSec[5]						__no_init;					//需量秒计时器(有功,无功,组合无功1,组合无功2,四象限无功)
//static	INT8U	g_DmdMin[5]						__no_init;					//需量分计时器(有功,无功,组合无功1,组合无功2,四象限无功)
//static	INT32U	g_DmdAdd[5]						__no_init;					//累加需量(功率)(有功,无功,组合无功1,组合无功2,四象限无功)
//static	INT32U	g_DmdBuf[5][60]					__no_init;					//缓存需量(功率)(有功,无功,组合无功1,组合无功2,四象限无功)
//static	INT32U	g_DmdDat[5]						__no_init;					//当前需量(功率)(有功,无功,组合无功1,组合无功2,四象限无功)

//static	INT32U	g_AvgPowP						__no_init;					//一分钟平均有功功率

/////*******************************************************************************
////		Fcuction Declaration
////*******************************************************************************/
////static void Load_IntPulse(void);											//取出中断脉冲电量
////static void Save_IntPulse(void);											//保存中断脉冲电量

////static void Energy_Service(void);											//电能服务程序
////static void Read_MeterPara(void);											//读取计量参数
////static void Cmp_CompFlag(void);												//产生比较标志
////static void Clr_CompFlag(void);												//清除比较标志
////static void Chk_LossVolt(void);												//失压类事件判断
////static void Chk_LossCurr(void);												//失流类事件判断
////static void Chk_OverLoad(void);												//过载类事件判断
////static void Chk_UnBalance(void);											//不平衡类事件判断
////static void Chk_LowerPf(void);												//功率因数超限类事件判断
////static void Chk_RevPowerT(void);											//潮流反向事件判断
////static void Chk_DmdOver(void);												//需量超限事件判断
////static void Chk_LostPower(void);											//掉电事件判断
////static void Chk_LostVoltX(void);											//全失压事件判断
////static void Cls_EventRecd(void);											//关闭事件类记录
//////static void 	Cls_LostPower();											//掉电事件结束

////static void Recd_LossVolt(INT8U idx);										//失压类事件记录
////static void Recd_LossCurr(INT8U idx);										//失流类事件记录
////static void Recd_OverLoad(INT8U idx);										//过载类事件记录
////static void Recd_UnBalance(INT8U idx);										//不平衡类事件记录
////static void Recd_LowerPf(INT8U idx);										//功率因数超限类事件记录
////static void Recd_RevPowerT(BOOL flag);										//潮流反向事件记录
////static void Recd_DmdOver(INT8U idx);										//需量超限事件记录
////static void Recd_AllLstVolt(BOOL end, INT32U curr);							//全失压事件记录
////static void Recd_LostPower(BOOL end);										//掉电事件记录

////static void Rep_EngyDataT(void);											//准备合相电量数据
////static void Rep_EngyDataX(void);											//准备分相电量数据
////static void Rep_RealDataX(void);											//准备分相实时数据

////static void Count_VoltQlty(void);											//电压合格率统计
////static void Load_VoltQlty(void);											//取出当前电压合格率数据
////static void Save_VoltQlty(void);											//保存当前电压合格率数据

////static void Demand_Service(void);											//需量服务程序

/////*******************************************************************************
////功能描述：	初始化计量任务
////输入参数：
////返回参数：
////函数说明：
////*******************************************************************************/
////void Init_TaskMetering(void)
////{
////	INT8U	i;
////	INT8U	buff[LEN_EnyCmbZ];
////	
////	g_EnyTimer = 0;
////	
//////	Init_Metering();														//初始化计量模块（假）
//////	Init_EMUStatus();														//初始化计量状态
//////	g_EnyQuad = 0x00;														//A,B,C,合,有功,无功处于正向
////	Get_EnyQuad();															//获取电量象限
////	g_SFlag   = 0x0E00;														//A,B,C处于潜动状态
////	g_FlagUI  = 0x00;														//电压电流处于正常状态
////	g_PowQuad = 0x00;														//A,B,C,合,有功,无功处于正向
////	Load_IntPulse();														//取出中断脉冲电量
////	//组合特征字初始化----------------------------------------------------------
////	if (TRUE == Read_ParaData(ID_EnyCmbZ, &buff[0]))
////	{
////		Copy_Data(&g_EnyCmbZ[0], &buff[0], LEN_EnyCmbZ);
////		Fetch_CRC(&g_EnyCmbZ[0], LEN_EnyCmbZ);
////	}
////	else
////	{
////		if (TRUE != Check_CRC(&g_EnyCmbZ[0], LEN_EnyCmbZ))
////		{
////			Copy_Data(&g_EnyCmbZ[0], TAB_EnyCmbZ, LEN_EnyCmbZ);
////			Fetch_CRC(&g_EnyCmbZ[0], LEN_EnyCmbZ);
////		}
////	}
////	//需量设置参数初始化--------------------------------------------------------
////	if (TRUE == Read_ParaData(ID_DmdPara, &buff[0]))
////	{
////		Copy_Data(&g_DmdPara[0], &buff[0], LEN_DmdPara);
////		Fetch_CRC(&g_DmdPara[0], LEN_DmdPara);
////	}
////	else
////	{
////		if (TRUE != Check_CRC(&g_DmdPara[0], LEN_DmdPara))
////		{
////			Copy_Data(&g_DmdPara[0], TAB_DmdPara, LEN_DmdPara);
////			Fetch_CRC(&g_DmdPara[0], LEN_DmdPara);
////		}
////	}
////	if ((g_DmdPara[0] == 0) || (g_DmdPara[0] > 60)							//需量周期(1~60)
////	|| (g_DmdPara[1] == 0) || (g_DmdPara[1] > 20)							//滑差时间(1~20)
////	|| ((g_DmdPara[0] % g_DmdPara[1]) != 0x00))								//不整除
////	{
////		Copy_Data(&g_DmdPara[0], TAB_DmdPara, LEN_DmdPara);
////		Fetch_CRC(&g_DmdPara[0], LEN_DmdPara);
////	}
////	//事件标志初始化------------------------------------------------------------
////	for (i=0; i<EV_EventNum-2; i++)											//掉电,全失压记录不需初始化
////	{
////		g_EventSta[i] = FALSE;												//事件记录标志
////		g_EventTmr[i] = 0;													//事件判断计时
////	}
////	if ((g_EventSta[EV_LostPower] != FALSE)
////	&& (g_EventSta[EV_LostPower] != TRUE))
////	{
////		g_EventSta[EV_LostPower] = FALSE;
////		g_EventTmr[EV_LostPower] = 0;
////	}
////	if ((g_EventSta[EV_LostVoltX] != FALSE)
////	&& (g_EventSta[EV_LostVoltX] != TRUE))
////	{
////		g_EventSta[EV_LostVoltX] = FALSE;
////		g_EventTmr[EV_LostVoltX] = 0;
////	}
//////	for (i=0; i<12; i++)
//////	{
//////		g_AmpereHr[i].tmr = 0;												//计时器
//////		g_AmpereHr[i].amp[0] = 0;											//A相安时数
//////		g_AmpereHr[i].amp[1] = 0;											//B相安时数
//////		g_AmpereHr[i].amp[2] = 0;											//C相安时数
//////	}
//////	for (i=0; i<3; i++)
//////	{
//////		g_UnBalance[i] = 0;													//不平衡率
//////	}
//////	for (i=0; i<6; i++)
//////	{
//////		g_MaxDemand[i] = 0;													//最大需量
//////		Set_Data(&g_MaxDmdTmr[i][0], 0x00, 6);								//最大需量发生时间
//////	}
////	//判断标志初始化------------------------------------------------------------
////	g_OffPrtSet	= 0;														//保护电流触发下限
////	for (i=0; i<NUM_Comp; i++)
////	{
////		g_CompFlg[i] = 0xAA;												//判断标志无效
////	}
////	//电压合格率统计------------------------------------------------------------
////	Load_VoltQlty();														//取出当前电压合格率数据
////	//需量数据初始化------------------------------------------------------------
////	Clear_CurDmd(1);														//清除当前需量
////	//平均功率初始化------------------------------------------------------------
////	g_AvgPowP = 0;
////}
///*******************************************************************************
//功能描述：	设置RAM区数据
//输入参数：	*pDest:		目标数据指针
//			value:		填充数据
//			len:		填充字节数
//返回参数：
//函数说明：
//*******************************************************************************/
//void Set_Data(INT8U *pDest, INT8U value, INT16U len)
//{
//	while (len--)
//	{
//		*(pDest++) = value;
//	}
//}
///*******************************************************************************
//功能描述：	运行计量任务
//输入参数：
//返回参数：
//函数说明：
//*******************************************************************************/
//void Run_TaskMetering(void)
//{
////	MSG		msg;
//	
////	if (Is_PowerOff())														//掉电判断
////	{
////		return;
////	}
////	msg = Accept_Message(ID_TaskMetering);									//接收计量任务消息
////	if (msg != MSG_Null)													//有消息？
////	{
////		if (Is_Message(msg, MSG_SecStep))									//收到秒节拍消息
////		{
////			if (TRUE == Check_Metering())									//检查计量模块
//			{
//				Read_MeterPara();											//读取计量参数
//				
//			}
////			else
////			{
//////				Repair_Metering();											//修复计量模块
////				Clr_CompFlag();												//清除比较标志
////			}
////		}
////	}
//}

/////*******************************************************************************
////功能描述：	停止计量任务
////输入参数：
////返回参数：
////函数说明：	保存电量脉冲，及关闭脉冲中断
////*******************************************************************************/
////void Stop_TaskMetering(void)
////{
//////	INT8U	i;
////	
//////	for (i=0; i<8; i++)
//////	{
//////		Save_Pulse(i);														//保存脉冲电量
//////	}
////	Save_IntPulse();														//保存中断脉冲电量
////	Cls_EventRecd();														//关闭事件类记录
////	Save_VoltQlty();														//保存当前电压合格率数据
////}

/////*******************************************************************************
////功能描述：	初始化计量类参数
////输入参数：
////返回参数：
////函数说明：
////*******************************************************************************/
////BOOL Init_MeteringPara(void)
////{
////	INT8U	buff[4];
////	
////	Set_Data(&buff[0], 0x00, 4);
////	
////	if((TRUE != Write_ParaData(ID_VoltQSet,	TAB_VoltQSet))					//电压合格率统计参数
////	|| (TRUE != Write_ParaData(ID_EventSet,	TAB_EventPara))					//事件记录判断参数
////	|| (TRUE != Write_ParaData(ID_BillkWh,	&buff[0]))						//清除上次结算总电量
////	|| (TRUE != Write_ParaData(ID_MonthkWh,	&buff[0]))						//清除上月总电量
////#if	(TP_Meter == TP_Local)//本地表
////	|| (TRUE != Write_ParaData(ID_YearkWh, &buff[0]))						//清除上年总电量
////#endif
////	|| (TRUE != Write_ParaData(ID_EnyCmbZ, TAB_EnyCmbZ))					//组合方式特征字
////	|| (TRUE != Write_ParaData(ID_DmdPara, TAB_DmdPara)))					//需量设置参数
////	{
////		return FALSE;
////	}
////	
////	Copy_Data(&g_EventSet[0], TAB_EventPara, LEN_EventSet);
////	Fetch_CRC(&g_EventSet[0], LEN_EventSet);
////	
////	Copy_Data(&g_EnyCmbZ[0], TAB_EnyCmbZ, LEN_EnyCmbZ);
////	Fetch_CRC(&g_EnyCmbZ[0], LEN_EnyCmbZ);
////	
////	Copy_Data(&g_DmdPara[0], TAB_DmdPara, LEN_DmdPara);
////	Fetch_CRC(&g_DmdPara[0], LEN_DmdPara);
////	
////	Clr_EventSta(EV_EventNum);												//清除事件类记录状态
////	Init_VoltQlty();														//初始化电压合格率

//////	Init_EMUStatus();														//初始化计量状态
////	return TRUE;
////}

/////*******************************************************************************
////功能描述：	获取电量象限
////输入参数：
////返回参数：
////函数说明：
////*******************************************************************************/
////INT8U Get_EnyQuad(void)
////{
////	if (TRUE != Check_CRC(&g_EnyQuad[0], LEN_EnyQuad))
////	{
////		if (TRUE != Read_ParaData(ID_EnyQuad, &g_EnyQuad[0]))
////		{
////			g_EnyQuad[0] = 0x00;											//默认：正向
////			g_EnyQuad[1] = 0xFF;
////		}
////		Fetch_CRC(&g_EnyQuad[0], LEN_EnyQuad);
////	}
////	if (g_EnyQuad[0] != ((~g_EnyQuad[1]) & 0xFF))
////	{
////		g_EnyQuad[0] = 0x00;											//默认：正向
////		g_EnyQuad[1] = 0xFF;
////		Fetch_CRC(&g_EnyQuad[0], LEN_EnyQuad);
////		Write_ParaData(ID_EnyQuad, &g_EnyQuad[0]);
////	}
////	return g_EnyQuad[0];
////}

/////*******************************************************************************
////功能描述：	获取电量尾数脉冲
////输入参数：
////返回参数：	电量尾数脉冲
////函数说明：
////*******************************************************************************/
////INT16U Get_kWhPulse(INT8U idx)
////{
////	if ((g_kWhPulse[idx][0] ^ 0x5555) == g_kWhPulse[idx][1])				//检查第一份
////	{
////		g_kWhPulse[idx][2] = g_kWhPulse[idx][0];
////		g_kWhPulse[idx][3] = g_kWhPulse[idx][2] ^ 0xAAAA;
////	}
////	else if ((g_kWhPulse[idx][2] ^ 0xAAAA) == g_kWhPulse[idx][3])			//检查第二份
////	{
////		g_kWhPulse[idx][0] = g_kWhPulse[idx][2];
////		g_kWhPulse[idx][1] = g_kWhPulse[idx][0] ^ 0x5555;
////	}
////	else
////	{
////		if ((TRUE != Get_VoltStatus())										//掉电状态?
////		&& (TRUE == Read_EEprom((INT8U*)&g_kWhPulse[idx][0], EE_kWhPulse+2*idx, 2))
////		&& (g_kWhPulse[idx][0] < C_PulseOver+C_PulseAdd*10))				//最大脉冲
////		{
//////			g_kWhPulse[idx][0] = g_kWhPulse[idx][0];
////			g_kWhPulse[idx][1] = g_kWhPulse[idx][0] ^ 0x5555;
////			g_kWhPulse[idx][2] = g_kWhPulse[idx][0];
////			g_kWhPulse[idx][3] = g_kWhPulse[idx][2] ^ 0xAAAA;
////		}
////		else
////		{
////			g_kWhPulse[idx][0] = 0x0000;
////			g_kWhPulse[idx][1] = 0x5555;
////			g_kWhPulse[idx][2] = 0x0000;
////			g_kWhPulse[idx][3] = 0xAAAA;
////		}
////	}
////	return g_kWhPulse[idx][0];
////}

/////*******************************************************************************
////功能描述：	清除电量尾数脉冲
////输入参数：
////返回参数：
////函数说明：	包括中断脉冲
////*******************************************************************************/
////void Clear_kWhPulse(void)
////{
////	INT8U	i;
////	
////	for (i=0; i<8; i++)
////	{
////		g_kWhPulse[i][0] = 0x0000;											//清除电量尾数脉冲
////		g_kWhPulse[i][1] = 0x5555;
////		g_kWhPulse[i][2] = 0x0000;
////		g_kWhPulse[i][3] = 0xAAAA;
////	}
////#if	(TP_Meter == TP_Local)//本地表
////	g_CashPulse = 0;
////#endif
////}

/////*******************************************************************************
////功能描述：	保存脉冲电量
////输入参数：	idx
////				|__Epa,		//A相有功电能
////				|__Epb,		//B相有功电能
////				|__Epc,		//C相有功电能
////				|__Ept,		//合相有功电能
////				|__Eqa,		//A相无功电能
////				|__Eqb,		//B相无功电能
////				|__Eqc,		//C相无功电能
////				|__Eqt,		//合相无功电能
////返回参数：
////函数说明：	在费率切换，功率方向转换，掉电保存，电量进位时调用
////*******************************************************************************/
////static const INT8U TAB_Quadrant[18] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2};
////void Save_Pulse(INT8U idx)
////{
////	INT8U	phs, len;
////	INT16U	pulse, addr = EE_kWhStr;
////	INT32U	curkWh;
//////	INT8U	quad = Get_EnyQuad();											//获取电量象限
////	
////	if (idx >= 8)
////	{
////		return;
////	}
////	
////	pulse = Get_kWhPulse(idx);												//获取脉冲电量
////	if (pulse == 0)															//防止频繁保存
////	{
////		return;
////	}
////	phs = idx % 4;															//A相,B相,C相,合相
////	addr += phs*6*LEN_CurkWh;												//相别EE地址偏移
////	len  = (phs==3)? 5: 1;													//合相(总尖峰平谷),分相(总)
////	len *= LEN_CurkWh;
////	
////	if (idx <= 3)															//有功
////	{
////		addr += ((g_EnyQuad[0]>>phs)&0x01) *len;							//有功反向EE地址偏移
////	}
////	else																	//无功
////	{
////		addr += (2+TAB_Quadrant[(g_EnyQuad[0]>>phs)&0x11]) *len;			//无功象限EE地址偏移
////	}

////	//先总电量------------------------------------------------------------------
////	if (TRUE != VerRd_EEprom((INT8U*)&curkWh, addr, 4))						//读EE中电量
////	{
////		VerRd_EEprom((INT8U*)&curkWh, addr, 4);
////	}
////	curkWh += pulse;
////	if (TRUE != VerWr_EEprom(addr, (INT8U*)&curkWh, 4))						//写EE中电量
////	if (TRUE != VerWr_EEprom(addr, (INT8U*)&curkWh, 4))
////	if (TRUE != VerWr_EEprom(addr, (INT8U*)&curkWh, 4))
////	{
////		return;
////	}
////	//后费率电量----------------------------------------------------------------
////	if (phs == 3)//合相
////	{
////		addr += Get_RateNo()*LEN_CurkWh;									//EE中费率电量地址偏移
////		if (TRUE != VerRd_EEprom((INT8U*)&curkWh, addr, 4))					//读EE中电量
////		{
////			VerRd_EEprom((INT8U*)&curkWh, addr, 4);
////		}
////		curkWh += pulse;
////		if (TRUE != VerWr_EEprom(addr, (INT8U*)&curkWh, 4))					//写EE中电量
////		if (TRUE != VerWr_EEprom(addr, (INT8U*)&curkWh, 4))
////		if (TRUE != VerWr_EEprom(addr, (INT8U*)&curkWh, 4))
////		{
////			return;
////		}
////	}
////	//脉冲电量清零--------------------------------------------------------------
////	g_kWhPulse[idx][0] = 0x0000;											//清除电量脉冲尾数
////	g_kWhPulse[idx][1] = 0x5555;
////	g_kWhPulse[idx][2] = 0x0000;
////	g_kWhPulse[idx][3] = 0xAAAA;
////}

/////*******************************************************************************
////功能描述：	取出中断脉冲电量
////输入参数：
////返回参数：
////函数说明：	包括金额小数电量,上电时调用
////*******************************************************************************/
////static void Load_IntPulse(void)
////{
////#if	(TP_Meter == TP_Local)//本地表
////	INT8U	pulse;
////#endif
////	INT8U	i;
////	
////	for (i=0; i<8; i++)
////	{
////		if ((g_kWhPulse[i][0] ^ 0x5555) == g_kWhPulse[i][1])				//检查第一份
////		{
////			g_kWhPulse[i][2] = g_kWhPulse[i][0];
////			g_kWhPulse[i][3] = g_kWhPulse[i][2] ^ 0xAAAA;
////		}
////		else if ((g_kWhPulse[i][2] ^ 0xAAAA) == g_kWhPulse[i][3])			//检查第二份
////		{
////			g_kWhPulse[i][0] = g_kWhPulse[i][2];
////			g_kWhPulse[i][1] = g_kWhPulse[i][0] ^ 0x5555;
////		}
////		else
////		{
////			if ((TRUE == Read_EEprom((INT8U*)&g_kWhPulse[i][0], EE_kWhPulse+2*i, 2))
////			&& (g_kWhPulse[i][0] < C_PulseOver+C_PulseAdd*10))				//最大脉冲
////			{
//////				g_kWhPulse[i][0] = g_kWhPulse[i][0];
////				g_kWhPulse[i][1] = g_kWhPulse[i][0] ^ 0x5555;
////				g_kWhPulse[i][2] = g_kWhPulse[i][0];
////				g_kWhPulse[i][3] = g_kWhPulse[i][2] ^ 0xAAAA;
////			}
////			else
////			{
////				g_kWhPulse[i][0] = 0x0000;
////				g_kWhPulse[i][1] = 0x5555;
////				g_kWhPulse[i][2] = 0x0000;
////				g_kWhPulse[i][3] = 0xAAAA;
////			}
////		}
////	}
////#if	(TP_Meter == TP_Local)//本地表
////	if ((TRUE == Read_ParaData(ID_CashPulse, &pulse))
////	&& (pulse < C_IMP+50))
////	{
////		g_CashPulse = pulse;
////	}
////	if (g_CashPulse >= C_IMP+50)
////	{
////		g_CashPulse = 0;
////	}
////#endif
////}

/////*******************************************************************************
////功能描述：	保存中断脉冲电量
////输入参数：
////返回参数：
////函数说明：	包括金额小数电量,掉电时调用
////*******************************************************************************/
////static const INT8U TAB_RegAddr[8] = {0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25};	//寄存器地址(A有,B有,C有,合有,A无,B无,C无,合无)
////static void Save_IntPulse(void)
////{
////	INT8U	phs;
////	INT16U	pulse, temp;
////	INT8U	sec;
////	INT8U	buff0[LEN_kWhPulse];
////	INT8U	buff1[LEN_kWhPulse];
////	
////	sec = (g_EnyTimer+C_SysTick-1)/C_SysTick;								//累计时间
////	g_EnyTimer = 0;
////	for (phs=0; phs<8; phs++)
////	{
////		pulse = Get_kWhPulse(phs);											//获取尾数脉冲
////		
////		temp = Read_Reg(TAB_RegAddr[phs]);									//读取电量脉冲
////		if ((temp > 0) && (temp < C_PulseAdd*sec))							//有中断脉冲？
////		{
////			pulse += temp;													//累加中断脉冲
////			g_kWhPulse[phs][0] = pulse;										//更新尾数脉冲
////			g_kWhPulse[phs][1] = g_kWhPulse[phs][0] ^ 0x5555;
////			g_kWhPulse[phs][2] = pulse;
////			g_kWhPulse[phs][3] = g_kWhPulse[phs][2] ^ 0xAAAA;
////		}
////		buff0[2*phs+0] = g_kWhPulse[phs][0];
////		buff0[2*phs+1] = g_kWhPulse[phs][0]>>8;
////	}
////	
////	if ((TRUE != Read_ParaData(ID_kWhPulse, &buff1[0]))
////	|| (0 != Comp_Data(&buff0[0], &buff1[0], LEN_kWhPulse)))
////	{
////		Write_ParaData(ID_kWhPulse, &buff0[0]);								//保存尾数脉冲
////	}
////	
////#if	(TP_Meter == TP_Local)//本地表
////	if ((TRUE != Read_ParaData(ID_CashPulse, (INT8U*)&pulse))
////	|| (pulse != g_CashPulse))
////	{
////		Write_ParaData(ID_CashPulse, &g_CashPulse);
////	}
////#endif
////}

/////*******************************************************************************
////功能描述：	电能服务程序
////*******************************************************************************/
////static const INT8U TAB_QuadMod[18] = {0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,010};//四象限模式字
//////static const INT8U TAB_QuadMod[18] = {0x03,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,030};//四象限模式字
////static void Energy_Service(void)
////{
////	INT8U	phs;
////	INT8U	mask, pFlag = 0x88;												//功率方向寄存器
////	INT32U	pulse;
////	INT8U	sec = 1;
////	INT8U	quad = Get_EnyQuad();											//获取电量象限
////	
////	sec = (g_EnyTimer+C_SysTick-1)/C_SysTick;								//累计时间
////	g_EnyTimer = 0;
////	for (phs=0; phs<8; phs++)
////	{
////		pulse = Read_Reg(TAB_RegAddr[phs]);									//读取电量脉冲
////		if ((pulse > 0) && (pulse < C_PulseAdd*sec))						//累加脉冲判断
////		{
////			if (pFlag == 0x88)												//未读标志
////			{
////				pFlag = Read_Reg(r_PFlag);									//功率方向寄存器
////			}
////			if (phs <= 3)//有功-------------------------------------------------
////			{
////				mask = 0x01<<phs;
////				if ((quad & mask) != (pFlag & mask))
////				{
////					Save_Pulse(phs);										//保存脉冲电量
////					quad ^= mask;
////				}
////				if (phs == 3)												//合相有功
////				{
//////					g_DmdPulse[0].add += pulse;								//总有功需量脉冲
//////					g_DmdPulse[4].add += pulse;								//费率有功需量脉冲
////#if	(TP_Meter == TP_Local)//本地表
////					if (((g_EnyQuad[0] & mask) == 0x00)						//正向有功
////					|| ((Get_EnyCmbZ(0) & 0x04) == 0x04))					//反向正计
////					{
////						if (g_CashPulse >= C_IMP+50)
////						{
////							g_CashPulse = 0;
////						}
////						g_CashPulse += pulse;
////						while (g_CashPulse >= C_IMP)
////						{
////							g_CashPulse -= C_IMP;
////							Dec_RemainCash();								//剩余金额实时扣减0.01度
////						}
////					}
////#endif
////				}
////			}
////			else//无功----------------------------------------------------------
////			{
////				mask = 0x11<<(phs-4);
////				if ((quad & mask) != (pFlag & mask))
////				{
////					Save_Pulse(phs);										//保存脉冲电量
////					quad |=  (pFlag & mask);
////					quad &= ~(pFlag & mask);
////				}
//////				if (phs == 7)												//合相无功
//////				{
//////					if (Get_EnyCmbZ(1) & TAB_QuadMod[(g_EnyQuad>>3)&0x11])
//////					{
//////						g_DmdPulse[1].add += pulse;							//总组合无功1需量脉冲
//////						g_DmdPulse[5].add += pulse;							//费率组合无功1需量脉冲
//////					}
//////					if (Get_EnyCmbZ(2) & TAB_QuadMod[(g_EnyQuad>>3)&0x11])
//////					{
//////						g_DmdPulse[2].add += pulse;							//总组合无功2需量脉冲
//////						g_DmdPulse[6].add += pulse;							//费率组合无功2需量脉冲
//////					}
//////					g_DmdPulse[3].add += pulse;								//总四象限无功需量脉冲
//////					g_DmdPulse[7].add += pulse;								//费率四象限无功需量脉冲
//////				}
////			}
////			pulse += Get_kWhPulse(phs);										//检查脉冲电量
////			g_kWhPulse[phs][0] = pulse;										//更新尾数脉冲
////			g_kWhPulse[phs][1] = g_kWhPulse[phs][0] ^ 0x5555;
////			g_kWhPulse[phs][2] = pulse;
////			g_kWhPulse[phs][3] = g_kWhPulse[phs][2] ^ 0xAAAA;
////			if (pulse >= C_PulseOver)										//脉冲电量溢出
////			{
////				Save_Pulse(phs);											//保存脉冲电量
////			}
////		}
////	}
//////	if (g_EnyQuad[0] != quad)												//更新尾数极性
//////	{
//////		g_EnyQuad[0] = quad;
//////		g_EnyQuad[1] = ~quad;
//////		Fetch_CRC(&g_EnyQuad[0], LEN_EnyQuad);
//////		Write_ParaData(ID_EnyQuad, &g_EnyQuad[0]);
//////	}
////}

///*******************************************************************************
//功能描述：	读取计量参数
//输入参数：
//返回参数：
//函数说明：	得到全局变量g_SFlag,g_FlagUI,g_Volt[],g_Curr[],g_PowP[],g_PowQ[],g_Pf[]
//*******************************************************************************/
//static const INT16U TAB_Mask[] = {0x0200, 0x0400, 0x0800, 0x0E00};
//void Read_MeterPara(void)
//{
//	INT8U	i;
//	BOOL	isNeg;
//	
//	b_EngyDataT = FALSE;
//	b_EngyDataX = FALSE;
//	b_RealDataX = FALSE;
//	
//	g_SFlag  = Read_Reg(r_SFlag);											//读取EMU状态
//	g_FlagUI = 0x00;														//电压电流标志(正常)
//	//读取A,B,C相电压-----------------------------------------------------------
//	for (i=0; i<3; i++)
//	{
//		g_Volt[i] = Read_Reg(r_UaRms+i);									//A,B,C相电压
//		g_Volt[i] *= 17;
//		g_Volt[i] >>= 13;
//		g_Volt[i] ++;
//		g_Volt[i] >>= 1;
//		if (g_Volt[i] < C_10Un)												//<10%Un电压
//		{
//			g_Volt[i] = 0;
//			g_FlagUI |= 0x01<<i;											//无电压
//		}
//	}
//	//读取A,B,C相电流-----------------------------------------------------------
//	for (i=0; i<3; i++)
//	{
//		g_Curr[i] = Read_Reg(r_IaRms+i);									//A,B,C相电流
//		g_Curr[i] *= 9;														//N=1000/144
//		g_Curr[i] >>= 8;
//		g_Curr[i] ++;
//		g_Curr[i] >>= 1;
//		if (g_Curr[i] < C_StrIb)											//<0.2%Ib:25mA
//		{
//			g_Curr[i] = 0;
//			g_FlagUI |= 0x10<<i;											//无电流
//		}
//	}
//	//读取A,B,C,合相有功功率（K=2.592*10^10/(HFconst*EC*2^23)）-----------------
//	for (i=0; i<4; i++)
//	{
//		g_PowP[i] = 0;
//		isNeg = FALSE;
//		if ((g_SFlag & TAB_Mask[i]) != TAB_Mask[i])							//启动
//		{
//			g_PowP[i] = Read_Reg(r_Pa+i);
//			if (g_PowP[i] & 0x800000)
//			{
//				g_PowP[i] = 0x1000000 - g_PowP[i];
//				isNeg = TRUE;
//			}
//			if (i == 3)														//合相
//			{
//				g_PowP[i] <<= 1;											//加倍
//			}
//			if (g_PowP[i] > C_StrPow)
//			{
//				g_PowP[i] *= 125;
//				g_PowP[i] >>= 7;
//				g_PowP[i] *= 81;
//				g_PowP[i] >>= 8;
//				g_PowP[i] ++;
//				g_PowP[i] >>= 1;
//			}
//			else
//			{
//				g_PowP[i] = 0;
//				isNeg = FALSE;
//			}
//		}
//		if (g_PowP[i] != 0)
//		{
//			if (isNeg == TRUE)//反向
//			{
//				g_PowQuad |=  (0x01<<i);
//			}
//			else
//			{
//				g_PowQuad &= ~(0x01<<i);
//			}
//		}
//	}
//	//读取A,B,C,合相无功功率（K=2.592*10^10/(HFconst*EC*2^23)）-----------------
//	for (i=0; i<4; i++)
//	{
//		g_PowQ[i] = 0;
//		isNeg = FALSE;
//		if ((g_SFlag & TAB_Mask[i]) != TAB_Mask[i])							//启动
//		{
//			g_PowQ[i] = Read_Reg(r_Qa+i);
//			if (g_PowQ[i] & 0x800000)
//			{
//				g_PowQ[i] = 0x1000000 - g_PowQ[i];
//				isNeg = TRUE;
//			}
//			if (i == 3)														//合相
//			{
//				g_PowQ[i] <<= 1;											//加倍
//			}
//			if (g_PowQ[i] > C_StrPow)
//			{
//				g_PowQ[i] *= 125;
//				g_PowQ[i] >>= 7;
//				g_PowQ[i] *= 81;
//				g_PowQ[i] >>= 8;
//				g_PowQ[i] ++;
//				g_PowQ[i] >>= 1;
//			}
//			else
//			{
//				g_PowQ[i] = 0;
//				isNeg = FALSE;
//			}
//		}
//		if (g_PowQ[i] != 0)
//		{
//			if (isNeg == TRUE)//反向
//			{
//				g_PowQuad |=  (0x10<<i);
//			}
//			else
//			{
//				g_PowQuad &= ~(0x10<<i);
//			}
//		}
//	}
////	//读取A,B,C,合相视在功率（K=2.592*10^10/(HFconst*EC*2^23)）-----------------
////	for (i=0; i<4; i++)
////	{
////		g_PowS[i] = 0;
////		if ((g_SFlag & TAB_Mask[i]) != TAB_Mask[i])							//启动
////		{
////			g_PowS[i] = Read_Reg(r_Sa+i);
////			if (i == 3)														//合相
////			{
////				g_PowS[i] <<= 1;											//加倍
////			}
////			if (g_PowS[i] > C_StrPow)
////			{
////				g_PowS[i] *= 125;
////				g_PowS[i] >>= 7;
////				g_PowS[i] *= 81;
////				g_PowS[i] >>= 8;
////				g_PowS[i] ++;
////				g_PowS[i] >>= 1;
////			}
////			else
////			{
////				g_PowS[i] = 0;
////			}
////		}
////	}
//	//读取A,B,C,合相功率因数----------------------------------------------------
//	for (i=0; i<4; i++)
//	{
//		g_Pf[i] = 1000;
//		if ((g_SFlag & TAB_Mask[i]) != TAB_Mask[i])							//启动
//		{
//			g_Pf[i] = Read_Reg(r_Pfa+i);
//			if (g_Pf[i] & 0x800000)
//			{
//				g_Pf[i] = 0x1000000 - g_Pf[i];
//			}
//			g_Pf[i] *= 125;
//			g_Pf[i] >>= 19;
//			g_Pf[i] ++;
//			g_Pf[i] >>= 1;
//			if (g_Pf[i] > 1000)
//			{
//				g_Pf[i] = 1000;
//			}
//		}
//	}
//}

///*******************************************************************************
//功能描述：	获取功率象限
//输入参数：
//返回参数：
//函数说明：
//*******************************************************************************/
//INT8U Get_PowQuad(void)
//{
//	return g_PowQuad;
//}
///*******************************************************************************
//功能描述：	获取上电状态
//输入参数：
//返回参数：	上电状态
//				|__FASLE,		掉电
//				|__TRUE,		上电
//函数说明：
//*******************************************************************************/
//BOOL Get_VoltStatus(void)
//{
//	return g_VoltStatus;
//}
///*******************************************************************************
//功能描述：	获取变量数据
//输入参数：	id:		变量数据ID
//				|__ID_Pa,		//A相有功功率
//				|__ID_Pb,		//B相有功功率
//				|__ID_Pc,		//C相有功功率
//				|__ID_Pt,		//合相有功功率
//				|__ID_Qa,		//A相无功功率
//				|__ID_Qb,		//B相无功功率
//				|__ID_Qc,		//C相无功功率
//				|__ID_Qt,		//合相无功功率
//				|__ID_Sa,		//A相视在功率
//				|__ID_Sb,		//B相视在功率
//				|__ID_Sc,		//C相视在功率
//				|__ID_St,		//合相视在功率
//				|__ID_Ua,		//A相电压有效值
//				|__ID_Ub,		//B相电压有效值
//				|__ID_Uc,		//C相电压有效值
//				|__ID_Ut,		//合相电压有效值(矢量和)
//				|__ID_Ia,		//A相电流有效值
//				|__ID_Ib,		//B相电流有效值
//				|__ID_Ic,		//C相电流有效值
//				|__ID_It,		//合相电流有效值(矢量和)
////				|__ID_In,		//零线电流有效值
//				|__ID_Pfa,		//A相功率因素
//				|__ID_Pfb,		//B相功率因素
//				|__ID_Pfc,		//C相功率因素
//				|__ID_Pft,		//合相功率因素
//				|__ID_Pga,		//A相电流与电压夹角
//				|__ID_Pgb,		//B相电流与电压夹角
//				|__ID_Pgc,		//C相电流与电压夹角
//				|__ID_Freq,		//线频率
//返回参数：
//函数说明：
//*******************************************************************************/
//void Read_RealData(ID_REAL id, INT8U *pBuff)
//{
//	INT8U	len;
//	INT32U	value = 0;
//	BOOL	isNeg = FALSE;
//	
//	if (id >= ID_RealNum)
//	{
//		return;
//	}
//	if ((id <= ID_St)
//	|| ((id >= ID_Ia) && (id <= ID_It)))
//	{
//		len = 3;															//3字节
//	}
//	else
//	{
//		len = 2;															//2字节
//	}
//	if (TRUE != Get_VoltStatus())											//掉电状态？
//	{
//		Set_Data(pBuff, 0x00, len);
//		if ((id >= ID_Pfa)													//功率因数
//		&& (id <= ID_Pft))
//		{
//			pBuff[1] = 0x10;
//		}
//		return;
//	}
//	switch (id)
//	{
//		case ID_Pa://A相有功功率
//		case ID_Pb://B相有功功率
//		case ID_Pc://C相有功功率
//		case ID_Pt://合相有功功率（K=2.592*10^10/(HFconst*EC*2^23)）------------
//			value = g_PowP[id-ID_Pa];
//			if (g_PowQuad & (0x01<<(id-ID_Pa)))
//			{
//				isNeg = TRUE;
//			}
//			break;
//		case ID_Qa://A相无功功率
//		case ID_Qb://B相无功功率
//		case ID_Qc://C相无功功率
//		case ID_Qt://合相无功功率（K=2.592*10^10/(HFconst*EC*2^23)）------------
//			value = g_PowQ[id-ID_Qa];
//			if (g_PowQuad & (0x10<<(id-ID_Qa)))
//			{
//				isNeg = TRUE;
//			}
//			break;
//		case ID_Sa://A相视在功率
//		case ID_Sb://B相视在功率
//		case ID_Sc://C相视在功率
//		case ID_St://合相视在功率（K=2.592*10^10/(HFconst*EC*2^23)）------------
//			if ((g_SFlag & TAB_Mask[id-ID_Sa]) != TAB_Mask[id-ID_Sa])		//启动
//			{
//				value = Read_Reg(r_Sa+id-ID_Sa);
//				if (id == ID_St)											//合相
//				{
//					value <<= 1;											//加倍
//				}
//				if (value > C_StrPow)
//				{
//					value *= 125;
//					value >>= 7;
//					value *= 81;
//					value >>= 8;
//					value ++;
//					value >>= 1;
//				}
//				else
//				{
//					value = 0;
//				}
//			}
//			break;
//		case ID_Ua://A相电压有效值
//		case ID_Ub://B相电压有效值
//		case ID_Uc://C相电压有效值
//		case ID_Ut://合相电压有效值（Urms = Vrms/2^13,单位V）-------------------
//			value = g_Volt[id-ID_Ua];
//			break;
//		case ID_Ia://A相电流有效值
//		case ID_Ib://B相电流有效值
//		case ID_Ic://C相电流有效值（Irms = (Vrms/2^13)/N,单位A）----------------
//		case ID_It://合相电流有效值
////		case ID_In://零线电流有效值
//			value = g_Curr[id-ID_Ia];
//			if (g_PowQuad & (0x01<<(id-ID_Ia)))
//			{
//				isNeg = TRUE;
//			}
//			break;
//		case ID_Pfa://A相功率因数
//		case ID_Pfb://B相功率因数
//		case ID_Pfc://C相功率因数
//		case ID_Pft://合相功率因数（pf=PFF/2^23）-------------------------------
//			value = g_Pf[id-ID_Pfa];
//			if (g_PowQuad & (0x01<<(id-ID_Pfa)))
//			{
//				isNeg = TRUE;
//			}
//			break;
//		case ID_Pga://A相相角
//		case ID_Pgb://B相相角
//		case ID_Pgc://C相相角（Pg=(a/2^20)*180度）------------------------------
//			if (((g_SFlag & TAB_Mask[id-ID_Pga]) != TAB_Mask[id-ID_Pga])
//			&& ((g_FlagUI & (0x11<<(id-ID_Pga))) == 0x00))
//			{
//				value = Read_Reg(r_Pga+id-ID_Sa);
//				if (value & 0x800000)
//				{
//					value = 0x1000000 - value;
//					isNeg = TRUE;
//				}
//				value *= 225;
//				value >>= 16;
//				value ++;
//				value >>= 1;
//			}
//			break;
//		case ID_Freq://线频率（f=Freq/2^13,单位Hz）-----------------------------
//			value = Read_Reg(r_Freq);
//			value *= 25;
//			value >>= 10;
//			value ++;
//			value >>= 1;
//			break;
//		default:
//			break;
//	}
////	LHEX_nBCD(pBuff, value, len);
//	if ((isNeg == TRUE) && (value != 0))
//	{
//		pBuff[len-1] |= 0x80;
//	}
//}

/////*******************************************************************************
////功能描述：	产生比较标志
////输入参数：
////返回参数：
////函数说明：
////*******************************************************************************/
////static void Cmp_CompFlag(void)
////{
////	INT8U	i;
////	INT8U	buff[LEN_OffPrtSet];
////	
////	if (TRUE != Check_CRC(&g_EventSet[0], LEN_EventSet))					//事件记录判断参数
////	{
////		Read_ParaData(ID_EventSet, &g_EventSet[0]);
////		Fetch_CRC(&g_EventSet[0], LEN_EventSet);
////	}
////	for (i=0; i<EV_EventNum; i++)
////	{
////		if ((g_EventSta[i] != TRUE)
////		&& (g_EventSta[i] != FALSE))
////		{
////			g_EventSta[i] = FALSE;											//默认事件记录状态
////		}
////		g_EventSec[i]++;													//事件累计时间
////	}
////	//读取保护电流--------------------------------------------------------------
////	if ((g_OffPrtSet == 0)													//==0A
////	|| (g_OffPrtSet > 1000000))												//>1000A
////	{
////		g_OffPrtSet = 30000;												//默认30A
////		if (TRUE == Read_ParaData(ID_OffPrtSet, &buff[0]))
////		{
////			g_OffPrtSet = buff[0] | buff[1]<<8 | buff[2]<<16;
////			if (g_OffPrtSet == 0)											//不启用
////			{
////				g_OffPrtSet = 1000000;										//1000A
////			}
////			else if (g_OffPrtSet > 999999)
////			{
////				g_OffPrtSet = 30000;										//默认30A
////			}
////			else
////			{
////			}
////		}
////	}
////	//计算电压，电流------------------------------------------------------------
////	for (i=0; i<NUM_Comp; i++)
////	{
////		g_CompFlg[i] <<= 1;
////	}
////	//比较电压，电流------------------------------------------------------------
////	if((g_Volt[0] > C_60Un)
////	|| (g_Volt[1] > C_60Un)
////	|| (g_Volt[2] > C_60Un))												//比较60%Un电压
////	{
////		g_CompFlg[CMP_060Un] |= 0x01;
////	}
////	if((g_Volt[0] > C_70Un)
////	|| (g_Volt[1] > C_70Un)
////	|| (g_Volt[2] > C_70Un))												//比较70%Un电压
////	{
////		g_CompFlg[CMP_065Un] |= 0x01;
////	}
////	if((g_Volt[0] > C_75Un)
////	|| (g_Volt[1] > C_75Un)
////	|| (g_Volt[2] > C_75Un))												//比较75%Un电压
////	{
////		g_CompFlg[CMP_075Un] |= 0x01;
////	}
////	if((g_Volt[0] > C_120Un)
////	|| (g_Volt[1] > C_120Un)
////	|| (g_Volt[2] > C_120Un))												//比较120%Un电压
////	{
////		g_CompFlg[CMP_120Un] |= 0x01;
////	}
////	if((g_Curr[0] > g_OffPrtSet)
////	|| (g_Curr[1] > g_OffPrtSet)
////	|| (g_Curr[2] > g_OffPrtSet))											//比较拉闸保护电流
////	{
////		g_CompFlg[CMP_ProtI] |= 0x01;
////	}
//////	if((g_Curr[0] > C_StrIb)
//////	|| (g_Curr[1] > C_StrIb)
//////	|| (g_Curr[2] > C_StrIb))												//任一相启动
////	if ((g_SFlag & 0x0E00) != 0x0E00)										//任一相启动
////	{
////		g_CompFlg[CMP_StrPow] |= 0x01;
////	}
////}

/////*******************************************************************************
////功能描述：	清除比较标志
////输入参数：
////返回参数：
////函数说明：
////*******************************************************************************/
////static void Clr_CompFlag(void)
////{
//////	INT8U	i;
////	
//////	for (i=0; i<EV_EventNum; i++)
//////	{
//////		if ((g_EventSta[i] != TRUE)
//////		&& (g_EventSta[i] != FALSE))
//////		{
//////			g_EventSta[i] = FALSE;											//默认事件记录状态
//////		}
//////		g_EventSec[i]++;													//事件累计时间
//////	}
//////	for (i=0; i<NUM_Comp; i++)
//////	{
//////		g_CompFlg[i] <<= 1;
//////	}
////}

///*******************************************************************************
//功能描述：	获取比较标志
//输入参数：	idx:		比较项目		
//			num:		比较次数
//返回参数：	比较结果
//				|__0x00,	全部小于
//				|__0xFF,	全部大于
//				|__0xAA,	不明确
//函数说明：
//*******************************************************************************/
//INT8U Get_CompFlag(INT8U idx, INT8U num)
//{
//	INT8U	mask, temp;
//	
//	if ((idx >= NUM_Comp)
//	|| (num == 0) || (num > 8))
//	{
//		return 0xAA;
//	}
//	mask = 0xFF >> (8-num);
//	temp = g_CompFlg[idx] & mask;
//	if (temp == 0x00)
//	{
//		return 0x00;
//	}
//	else if (temp == mask)
//	{
//		return 0xFF;
//	}
//	else
//	{
//		return 0xAA;
//	}
//}

///*******************************************************************************
//功能描述：	设置保护电流触发下限
//输入参数：	para:		设置参数(单位0.001A)
//返回参数：
//函数说明：
//*******************************************************************************/
//void Set_OffPrtSet(INT32U para)
//{
//	if (para == 0)															//不启用
//	{
//		g_OffPrtSet = 1000000;												//1000A
//	}
//	else
//	{
//		g_OffPrtSet = para;
//	}
//}

/////*******************************************************************************
////功能描述：	失压类事件判断
////输入参数：
////返回参数：
////函数说明：	A,B,C相失压,欠压,过压,断相等12个事件记录
////*******************************************************************************/
////static void Chk_LossVolt(void)
////{
////	INT8U	i;
////	BOOL	state[12];														//事件即时状态
////	INT16U	Uupper, Ulower;													//电压触发上,下限		NNN.N
////	INT32U	Iupper,	Ilower;													//电流触发上,下限		NNN.NNN	
////	INT8U	delay;															//判断延时时间
////	
////	for (i=0; i<12; i++)
////	{
////		state[i] = FALSE;													//默认事件即时状态
////	}
////	//A,B,C相失压事件：某相电流大于电流触发下限，同时该相电压低于电压触发上限。（电压触发上限为0时，不启用）
////	Uupper = g_EventSet[0] | g_EventSet[1]<<8;								//电压触发上限		NNN.N
////	Ulower = g_EventSet[2] | g_EventSet[3]<<8;								//电压恢复下限		NNN.N
////	Ilower = g_EventSet[4] | g_EventSet[5]<<8 | g_EventSet[6]<<16;			//电流触发下限		NN.NNNN
////	Ilower /= 10;
////	if ((Uupper != 0) && (Ulower >= Uupper))								//功能启用,下限>上限
////	{
////		for (i=0; i<3; i++)
////		{
////			state[i] = g_EventSta[EV_LossVoltA+i];							//状态默认
////			if (g_EventSta[EV_LossVoltA+i] == FALSE)						//不在记录中
////			{
////				if ((g_Curr[i] > Ilower)									//电流大于触发下限
////				&& (g_Volt[i] < Uupper))									//电压小于触发上限
////				{
////					state[i] = TRUE;
////				}
////			}
////			else															//正在记录中
////			{
////				if (g_Volt[i] > Ulower)										//电压大于恢复下限
////				{
////					state[i] = FALSE;
////				}
////			}
////		}
////	}
////	//A,B,C相欠压事件：某相电压小于电压触发上限。（电压触发上限为0时，不启用）
////	Uupper = g_EventSet[8] | g_EventSet[9]<<8;								//电压触发上限		NNN.N
////	if (Uupper != 0)														//功能启用
////	{
////		for (i=0; i<3; i++)
////		{
////			if (g_Volt[i] < Uupper)											//电压小于触发上限
////			{
////				state[3+i] = TRUE;
////			}
////		}
////	}
////	//A,B,C相过压事件：某相电压大于电压触发下限。（电压触发下限为0时，不启用）
////	Ulower = g_EventSet[11] | g_EventSet[12]<<8;							//电压触发下限		NNN.N
////	if (Ulower != 0)														//功能启用
////	{
////		for (i=0; i<3; i++)
////		{
////			if (g_Volt[i] > Ulower)											//电压大于触发下限
////			{
////				state[6+i] = TRUE;
////			}
////		}
////	}
////	//A,B,C相断相事件：某相电压小于电压触发上限，同时该相电流小于电流触发上限。（电压触发上限为0时，不启用）
////	Uupper = g_EventSet[14] | g_EventSet[15]<<8;							//电压触发上限		NNN.N
////	Iupper = g_EventSet[16] | g_EventSet[17]<<8 | g_EventSet[18]<<16;		//电流触发上限		NN.NNNN
////	Iupper /= 10;
////	if (Iupper != 0)														//功能启用
////	{
////		for (i=0; i<3; i++)
////		{
////			if ((g_Volt[i] < Uupper)										//电压小于触发上限
////			&& (g_Curr[i] < Iupper))										//电流小于触发上限
////			{
////				state[9+i] = TRUE;											//断相开始
////				state[i]   = FALSE;											//失压结束
////			}
////		}
////	}
////	//记录状态判断--------------------------------------------------------------
////	for (i=0; i<12; i++)
////	{
////		if (g_EventSta[EV_LossVoltA+i] == state[i])
////		{
////			g_EventTmr[EV_LossVoltA+i] = 0;
////		}
////		else
////		{
////			delay = g_EventSet[TAB_RecdId[EV_LossVoltA+i].dly];
////			if ((delay < 10) || (delay > 99))
////			{
////				delay = 60;
////			}
////			if (++g_EventTmr[EV_LossVoltA+i] >= delay)
////			{
////				Recd_LossVolt(EV_LossVoltA+i);
////				g_EventSta[EV_LossVoltA+i] = state[i];
////				g_EventTmr[EV_LossVoltA+i] = 0;
////				g_AmpereHr[i].tmr = 22;										//计时器
////				g_AmpereHr[i].amp[0] = 0;									//A相安时数
////				g_AmpereHr[i].amp[1] = 0;									//B相安时数
////				g_AmpereHr[i].amp[2] = 0;									//C相安时数
////			}
////		}
////		if (g_EventSta[EV_LossVoltA+i] == TRUE)								//记录中
////		{
////			if (++g_AmpereHr[i].tmr >= 45)									//0.0125h
////			{
////				g_AmpereHr[i].tmr = 0;
////				g_AmpereHr[i].amp[0] += g_Curr[0]/100;						//A相安时数(0.1A)
////				g_AmpereHr[i].amp[1] += g_Curr[1]/100;						//B相安时数(0.1A)
////				g_AmpereHr[i].amp[2] += g_Curr[2]/100;						//C相安时数(0.1A)
////			}
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	失流类事件判断
////输入参数：
////返回参数：
////函数说明：	A,B,C相失流,过流,断流等9个事件记录
////*******************************************************************************/
////static void Chk_LossCurr(void)
////{
////	INT8U	i;
////	BOOL	state[9];														//事件即时状态
////	INT16U	Ulower;															//电压触发上,下限		NNN.N
////	INT32U	Iupper,	Ilower;													//电流触发上,下限		NNN.NNN	
////	INT8U	delay;															//判断延时时间
////	
////	for (i=0; i<9; i++)
////	{
////		state[i] = FALSE;													//默认事件即时状态
////	}
////	//A,B,C相失流事件：至少有一相电流大于电流触发下限，某相电压大于电压触发下限，同时该相电流小于电流触发上限。（电流触发上限为0时，不启用）
////	Ulower = g_EventSet[26] | g_EventSet[27]<<8;							//电压触发下限		NNN.N
////	Iupper = g_EventSet[28] | g_EventSet[29]<<8 | g_EventSet[30]<<16;		//电流触发上限		NN.NNNN
////	Iupper /= 10;
////	Ilower = g_EventSet[31] | g_EventSet[32]<<8 | g_EventSet[33]<<16;		//电流触发下限		NN.NNNN
////	Ilower /= 10;
////	if ((Iupper != 0)														//功能启用
////	&& ((g_Curr[0]>Ilower) || (g_Curr[1]>Ilower) || (g_Curr[2]>Ilower)))	//至少有一相电流大于电流触发下限
////	{
////		for (i=0; i<3; i++)
////		{
////			if ((g_Volt[i] > Ulower)										//电压大于触发下限
////			&& (g_Curr[i] < Iupper))										//电流小于触发上限
////			{
////				state[i] = TRUE;
////			}
////		}
////	}
////	//A,B,C相过流事件：某相电流大于电流触发下限。（电流触发下限为0时，不启用）
////	Ilower = g_EventSet[35] | g_EventSet[36]<<8;							//电压触发下限		NNN.N
////	Ilower*= 100;
////	if (Ilower != 0)														//功能启用
////	{
////		for (i=0; i<3; i++)
////		{
////			if (g_Curr[i] > Ilower)											//电流大于触发下限
////			{
////				state[3+i] = TRUE;
////			}
////		}
////	}
////	//A,B,C相断流事件：某相电压大于电压触发下限，同时该相电流小于电流触发上限。（电流触发上限为0时，不启用）
////	Ulower = g_EventSet[38] | g_EventSet[39]<<8;							//电压触发下限		NNN.N
////	Iupper = g_EventSet[40] | g_EventSet[41]<<8 | g_EventSet[42]<<16;		//电流触发上限		NN.NNNN
////	Iupper /= 10;
////	if (Iupper != 0)														//功能启用
////	{
////		for (i=0; i<3; i++)
////		{
////			if ((g_Volt[i] > Ulower)										//电压大于触发下限
////			&& (g_Curr[i] < Iupper))										//电流小于触发上限
////			{
////				state[6+i] = TRUE;
////			}
////		}
////	}
////	//记录状态判断--------------------------------------------------------------
////	for (i=0; i<9; i++)
////	{
////		if (g_EventSta[EV_LossCurrA+i] == state[i])
////		{
////			g_EventTmr[EV_LossCurrA+i] = 0;
////		}
////		else
////		{
////			delay = g_EventSet[TAB_RecdId[EV_LossCurrA+i].dly];
////			if ((delay < 10) || (delay > 99))
////			{
////				delay = 60;
////			}
////			if (++g_EventTmr[EV_LossCurrA+i] >= delay)
////			{
////				Recd_LossCurr(EV_LossCurrA+i);
////				g_EventSta[EV_LossCurrA+i] = state[i];
////				g_EventTmr[EV_LossCurrA+i] = 0;
////			}
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	过载类事件判断
////输入参数：
////返回参数：
////函数说明：	A,B,C相过载,功率反向,电压,电流逆相序等8个事件记录
////*******************************************************************************/
////static void Chk_OverLoad(void)
////{
////	INT8U	i;
////	BOOL	state[8];														//事件即时状态
////	INT32U	limit0, limit1;													//限值
////	INT8U	delay;															//判断延时时间
////	
////	for (i=0; i<8; i++)
////	{
////		state[i] = FALSE;													//默认事件即时状态
////	}
////	//A,B,C相过载事件：某相有功功率大于有功功率触发下限。（功率触发下限为0时，不启用）	
////	//A,B,C相有功功率反向事件：某相有功功率方向为反向，同时该相有功功率大于有功功率触发下限。（功率触发下限为0时，不启用）
////	limit0 = g_EventSet[48] | g_EventSet[49]<<8 | g_EventSet[50]<<16;		//功率触发下限		NN.NNNN
////	limit1 = g_EventSet[65] | g_EventSet[66]<<8 | g_EventSet[67]<<16;		//功率触发下限		NN.NNNN
////	if ((limit0 != 0) || (limit1 != 0))										//功能启用
////	{
////		for (i=0; i<3; i++)
////		{
////			if ((g_SFlag & TAB_Mask[i]) != TAB_Mask[i])						//某一相启动
////			{
////				if (g_PowP[i] > limit0)										//有功功率大于有功功率触发下限
////				{
////					state[0+i] = TRUE;										//过载
////				}
////				if ((g_PowP[i] > limit1)
////				&& (g_PowQuad & (0x01<<i)))
////				{
////					state[3+i] = TRUE;										//功率反向
////				}
////			}
////		}
////	}
////	//电压逆相序事件：三相电压均大于临界电压，三相电压逆相序，且持续时间大于60s。
////	//电流逆相序事件：三相电压均大于临界电压，三相电流均大于5%Ib，三相电流逆相序，且持续时间大于60s。
////	if (((g_FlagUI & 0x07) == 0x00)											//三相电压均启动
////	&& ((g_Volt[0]>C_60Un) && (g_Volt[1]>C_60Un) && (g_Volt[2]>C_60Un)))	//三相电压均大于临界电压
////	{
////		if (g_SFlag & 0x0008)
////		{
////			state[6] = TRUE;												//三相电压逆相序
////		}
////		if ((g_SFlag & 0x0010)												//三相电流逆相序
////		&& ((g_FlagUI & 0x70) == 0x00)										//三相电流均启动
////		&& ((g_Curr[0]>C_05Ib) && (g_Curr[1]>C_05Ib) && (g_Curr[2]>C_05Ib)))//三相电流均大于5%Ib
////		{
////			state[7] = TRUE;
////		}
////	}
////	//记录状态判断--------------------------------------------------------------
////	for (i=0; i<8; i++)
////	{
////		if (g_EventSta[EV_OverLoadA+i] == state[i])
////		{
////			g_EventTmr[EV_OverLoadA+i] = 0;
////		}
////		else
////		{
////			if (i < 6)
////			{
////				delay = g_EventSet[TAB_RecdId[EV_OverLoadA+i].dly];
////				if ((delay < 10) || (delay > 99))
////				{
////					delay = 60;
////				}
////			}
////			else
////			{
////				delay = 60;
////			}
////			if (++g_EventTmr[EV_OverLoadA+i] >= delay)
////			{
////				Recd_OverLoad(EV_OverLoadA+i);
////				g_EventSta[EV_OverLoadA+i] = state[i];
////				g_EventTmr[EV_OverLoadA+i] = 0;
////			}
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	不平衡类事件判断
////输入参数：
////返回参数：
////函数说明：	电压,电流,电流严重不平衡类等3个事件记录
////*******************************************************************************/
////static void Chk_UnBalance(void)
////{
////	INT8U	i;
////	INT32U	low, high;
////	BOOL	state[3];														//事件即时状态
////	INT16U	rate[3];														//电压,流不平衡率
////	INT32U	limit0, limit1;													//限值
////	INT8U	delay;															//判断延时时间
////	
////	for (i=0; i<3; i++)
////	{
////		state[i] = FALSE;													//默认事件即时状态
////	}
////	//电压不平衡事件：任一相电压大于临界电压，不平衡率大于不平衡限值。（不平衡率限值为0时，不启用）
////	limit0 = g_EventSet[20] | g_EventSet[21]<<8;							//不平衡率限值		NN.NN
////	if ((limit0 != 0)														//功能启用
////	&& ((g_FlagUI & 0x07) != 0x07)											//任一相电压启动
////	&& ((g_Volt[0]>C_60Un) || (g_Volt[1]>C_60Un) || (g_Volt[2]>C_60Un)))	//任一相电压大于临界电压
////	{
////		low  = g_Volt[0];
////		high = g_Volt[0];
////		for (i=1; i<3; i++)
////		{
////			if (g_Volt[i] < low)
////			{
////				low  = g_Volt[i];
////			}
////			else if (g_Volt[i] > high)
////			{
////				high = g_Volt[i];
////			}
////			else
////			{
////			}
////		}
////		low   = high-low;
////		while (low&0xFFFC0000)
////		{
////			low  >>= 1;
////			high >>= 1;
////		}
////		rate[0] = low *10000u /high;
////		if (rate[0] > limit0)												//不平衡率大于限值
////		{
////			state[0] = TRUE;
////		}
////	}
////	//电流不平衡事件：任一相电流大于5%Ib，不平衡率大于不平衡限值。（不平衡率限值为0时，不启用）
////	//电流严重不平衡事件：任一相电流大于5%Ib，不平衡率大于不平衡限值。（不平衡率限值为0时，不启用）
////	limit0 = g_EventSet[23] | g_EventSet[24]<<8;							//不平衡率限值		NN.NN
////	limit1 = g_EventSet[62] | g_EventSet[63]<<8;							//不平衡率限值		NN.NN
////	if (((limit0 != 0) ||  (limit1 != 0))									//功能启用
////	&& ((g_FlagUI & 0x70) != 0x70)											//任一相电流启动
////	&& ((g_Curr[0]>C_05Ib) || (g_Curr[1]>C_05Ib) || (g_Curr[2]>C_05Ib)))	//任一相电流大于5%Ib
////	{
////		low  = g_Curr[0];
////		high = g_Curr[0];
////		for (i=1; i<3; i++)
////		{
////			if (g_Curr[i] < low)
////			{
////				low  = g_Curr[i];
////			}
////			else if (g_Curr[i] > high)
////			{
////				high = g_Curr[i];
////			}
////			else
////			{
////			}
////		}
////		low   = high-low;
////		while (low&0xFFFC0000)
////		{
////			low  >>= 1;
////			high >>= 1;
////		}
////		rate[1] = low *10000u /high;
////		rate[2] = rate[1];
////		if ((limit0 != 0) && (rate[1] > limit0))							//不平衡率大于限值
////		{
////			state[1] = TRUE;
////		}
////		if ((limit1 != 0) && (rate[2] > limit1))							//不平衡率大于限值
////		{
////			state[2] = TRUE;
////		}
////	}
////	//记录状态判断--------------------------------------------------------------
////	for (i=0; i<3; i++)
////	{
////		if (g_EventSta[EV_VoltUnBlnc+i] == state[i])
////		{
////			g_EventTmr[EV_VoltUnBlnc+i] = 0;
////		}
////		else
////		{
////			delay = g_EventSet[TAB_RecdId[EV_VoltUnBlnc+i].dly];
////			if ((delay < 10) || (delay > 99))
////			{
////				delay = 60;
////			}
////			if (++g_EventTmr[EV_VoltUnBlnc+i] >= delay)
////			{
////				Recd_UnBalance(EV_VoltUnBlnc+i);
////				g_EventSta[EV_VoltUnBlnc+i] = state[i];
////				g_EventTmr[EV_VoltUnBlnc+i] = 0;
////			}
////		}
////		if (g_EventSta[EV_VoltUnBlnc+i] == TRUE)							//记录中
////		{
////			if (rate[i] > g_UnBalance[i])
////			{
////				g_UnBalance[i] = rate[i];									//最大不平衡率
////			}
////		}
////		else
////		{
////			g_UnBalance[i] = 0;												//不平衡率
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	功率因数超限类事件判断
////输入参数：
////返回参数：
////函数说明：	合,A,B,C相功率因数超限等4个事件记录
////*******************************************************************************/
////static void Chk_LowerPf(void)
////{
////	INT8U	i;
////	BOOL	state[4];														//事件即时状态
////	INT32U	limit;															//限值
////	INT8U	delay;															//判断延时时间
////	
////	for (i=0; i<4; i++)
////	{
////		state[i] = FALSE;													//默认事件即时状态
////	}
////	//A,B,C相功率因数超限事件：某相功率因数小于功率因数下限，同时该相电流大于5%Ib。（功率因数下限为0时，不启用）
////	//总功率因数超限事件：总功率因数小于功率因数下限，同时任一相电流大于5%Ib。（功率因数下限为0时，不启用）
////	limit = g_EventSet[59] | g_EventSet[60]<<8;								//功率因数超限阈值	N.NNN
////	if (limit != 0)															//功能启用
////	{
////		for (i=0; i<3; i++)
////		{
////			if (((g_SFlag & TAB_Mask[i]) != TAB_Mask[i])					//某一相启动
////			&& (g_Curr[i] > C_05Ib))										//某一相电流大于5%Ib
////			{
////				if (g_Pf[i] < limit)										//功率因数小于下限
////				{
////					state[i] = TRUE;
////				}
////			}
////		}
////		if (((g_SFlag & 0x0E00) != 0x0E00)									//任一相启动
////		&& ((g_FlagUI & 0x70) != 0x70)										//任一相电流启动
////		&& ((g_Curr[0]>C_05Ib) || (g_Curr[1]>C_05Ib) || (g_Curr[2]>C_05Ib)))//任一相电流大于5%Ib
////		{
////			if (g_Pf[3] < limit)											//功率因数小于下限
////			{
////				state[3] = TRUE;
////			}
////		}
////	}
////	//记录状态判断--------------------------------------------------------------
////	for (i=0; i<4; i++)
////	{
////		if (g_EventSta[EV_LowerPfA+i] == state[i])
////		{
////			g_EventTmr[EV_LowerPfA+i] = 0;
////		}
////		else
////		{
////			delay = g_EventSet[TAB_RecdId[EV_LowerPfA+i].dly];
////			if ((delay < 10) || (delay > 99))
////			{
////				delay = 60;
////			}
////			if (++g_EventTmr[EV_LowerPfA+i] >= delay)
////			{
////				Recd_LowerPf(EV_LowerPfA+i);
////				g_EventSta[EV_LowerPfA+i] = state[i];
////				g_EventTmr[EV_LowerPfA+i] = 0;
////			}
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	潮流反向事件判断
////输入参数：
////返回参数：
////函数说明：	潮流反向等1个事件记录
////记录数据：
////*******************************************************************************/
////static void Chk_RevPowerT(void)
////{
////	INT32U	limit;															//限值
////	INT8U	delay;															//判断延时时间
////	BOOL	flag;															//总有功功率方向（FALSE=正向/TRUE=反向）
////	static	BOOL	g_FlagPowerT = FALSE;									//总有功功率方向标志（FALSE=正向/TRUE=反向）
////	
//////	flag = (g_SFlag & 0x1000)? TRUE: FALSE;									//合相有功功率方向
////	flag = (g_PowQuad & 0x08)? TRUE: FALSE;									//合相有功功率方向
////	if ((g_FlagPowerT != FALSE) && (g_FlagPowerT != TRUE))
////	{
////		g_EventSta[EV_RevPowerT] = FALSE;
////		g_EventTmr[EV_RevPowerT] = 0;
////		g_FlagPowerT = flag;
////	}
////	if (g_EventSta[EV_RevPowerT] != TRUE)									//潮流反向记录未记录
////	{
////		if (((g_SFlag & 0x0E00) != 0x0E00)									//任一相启动
////		&& (g_FlagPowerT == flag))											//功率方向未变化
////		{
////			if (++g_EventTmr[EV_RevPowerT] >= 10)							//初期判断10秒,确定功率方向
////			{
////				g_EventSta[EV_RevPowerT] = TRUE;							//
////			}
////		}
////		else
////		{
////			g_FlagPowerT = flag;
////			g_EventTmr[EV_RevPowerT] = 0;
////		}
////	}
////	else
////	{
////		if (((g_SFlag & 0x0E00) != 0x0E00)									//任一相启动
////		&& (g_FlagPowerT != flag))											//功率方向已变化
////		{
////			//潮流反向：总有功功率方向改变，同时有功功率大于有功功率触发下限。（功率触发下限为0时，不启用）
////			limit = g_EventSet[44] | g_EventSet[45]<<8 | g_EventSet[46]<<16;//功率触发下限		NN.NNNN
////			if ((limit != 0)													//功能启用
////			&& (g_PowP[3] > limit))
////			{
////				delay = g_EventSet[TAB_RecdId[EV_RevPowerT].dly];
////				if ((delay < 10) || (delay > 99))
////				{
////					delay = 60;
////				}
////				if (++g_EventTmr[EV_RevPowerT] >= delay)
////				{
////					Recd_RevPowerT(flag);
////					g_FlagPowerT = flag;
////					g_EventTmr[EV_RevPowerT] = 0;
////				}
////			}
////			else
////			{
////				g_EventTmr[EV_RevPowerT] = 0;
////			}
////		}
////		else
////		{
////			g_EventTmr[EV_RevPowerT] = 0;
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	需量超限事件判断
////输入参数：
////返回参数：
////函数说明：	正,反向有功,象限1,2,3,4无功需量超限等6个事件记录
////记录数据：
////*******************************************************************************/
////static void Chk_DmdOver(void)
////{
////	INT8U	i;
////	BOOL	state[6];														//事件即时状态
////	INT32U	limit;															//限值
////	INT8U	delay;															//判断延时时间
////	INT32U	demand[6];														//最大需量
////	
////	for (i=0; i<6; i++)
////	{
////		state[i] = FALSE;													//默认事件即时状态
////	}
////	//需量超限：总有功需量大于需量触发下限。（需量触发下限为0时，不启用）
////	limit = g_EventSet[52] | g_EventSet[53]<<8 | g_EventSet[54]<<16;		//有功需量触发下限	NN.NNNN
////	if ((limit != 0)														//功能启用
////	&& (g_PowP[3] > limit))													//有功功率超限
////	{
////		state[(g_EnyQuad[0]>>3)&0x01] = TRUE;
////	}
////	//需量超限：总无功需量大于需量触发下限。（需量触发下限为0时，不启用）
////	limit = g_EventSet[55] | g_EventSet[56]<<8 | g_EventSet[57]<<16;		//无功需量触发下限	NN.NNNN
////	if ((limit != 0)														//功能启用
////	&& (g_PowQ[3] > limit))													//无功功率超限
////	{
////		state[2+TAB_Quadrant[(g_PowQuad>>3)&0x11]] = TRUE;
////	}
////	//记录状态判断--------------------------------------------------------------
////	for (i=0; i<6; i++)
////	{
////		if (g_EventSta[EV_PAcDmdOver+i] == state[i])
////		{
////			g_EventTmr[EV_PAcDmdOver+i] = 0;
////		}
////		else
////		{
////			delay = g_EventSet[TAB_RecdId[EV_PAcDmdOver+i].dly];
////			if ((delay < 10) || (delay > 99))
////			{
////				delay = 60;
////			}
////			if (++g_EventTmr[EV_PAcDmdOver+i] >= delay)
////			{
////				Recd_DmdOver(EV_PAcDmdOver+i);
////				g_EventSta[EV_PAcDmdOver+i] = state[i];
////				g_EventTmr[EV_PAcDmdOver+i] = 0;
////			}
////		}
////		if (g_EventSta[EV_PAcDmdOver+i] == TRUE)							//记录中
////		{
////			if (demand[i] > g_MaxDemand[i])
////			{
////				g_MaxDemand[i] = demand[i];									//最大不平衡率
////				Get_MeterTime(&g_MaxDmdTmr[i][0]);							//发生时间
////			}
////		}
////		else
////		{
////			g_MaxDemand[i] = 0;												//不平衡率
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	有电时掉电事件判断
////输入参数：
////返回参数：
////函数说明：	掉电等1个事件记录
////记录数据：
////*******************************************************************************/
////static void Chk_LostPower(void)
////{
////	BOOL	state;															//事件即时状态
////	INT8U	delay;															//判断延时时间
////	
////	state = FALSE;															//默认事件即时状态
////	//掉电事件：供电电压均低于临界电压，且三相负荷电流均不大于5%Ib。
////	if (((g_Volt[0]<C_60Un) && (g_Volt[1]<C_60Un) && (g_Volt[2]<C_60Un))	//三相电压均低于临界电压
////	&& ((g_Curr[0]<=C_05Ib) && (g_Curr[1]<=C_05Ib) && (g_Curr[2]<=C_05Ib)))	//三相电流均不大于5%Ib
////	{
////		state = TRUE;
////	}
////	//记录状态判断--------------------------------------------------------------
////	if (g_EventSta[EV_LostPower] == state)
////	{
////		g_EventTmr[EV_LostPower] = 0;
////	}
////	else
////	{
////		if (g_EventSta[EV_LostPower] == FALSE)								//进入记录
////		{
////			delay = 60;
////		}
////		else																//退出记录
////		{
////			delay = g_EventSet[19];											//断相延时判断时间
////			if ((delay < 10) || (delay > 99))
////			{
////				delay = 60;
////			}
////		}
////		if ((++g_EventTmr[EV_LostPower] >= delay)
////		&& (g_EventSta[EV_LostVoltX] == FALSE))								//未进入全失压记录
////		{
////			if (g_EventSta[EV_LostPower] == FALSE)							//进入记录
////			{
////				Recd_LostPower(FALSE);
////			}
////			else															//退出记录
////			{
////				Recd_LostPower(TRUE);
////			}
////			g_EventSta[EV_LostPower] = state;
////			g_EventTmr[EV_LostPower] = 0;
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	有电时全失压事件判断
////输入参数：
////返回参数：
////函数说明：	全失压等1个事件记录
////记录数据：
////*******************************************************************************/
////static void Chk_LostVoltX(void)
////{
////	BOOL	state;															//事件即时状态
////	INT8U	delay;															//判断延时时间
////	INT32U	curr = 0;
////	
////	state = FALSE;															//默认事件即时状态
////	//全失压事件：三相电压均低于临界电压，且有任一相或多相电流大于5%Ib，且持续时间大于60s。
////	if (((g_Volt[0]<C_60Un) && (g_Volt[1]<C_60Un) && (g_Volt[2]<C_60Un))	//三相电压均低于临界电压
////	&& ((g_Curr[0]>C_05Ib) || (g_Curr[1]>C_05Ib) || (g_Curr[2]>C_05Ib)))	//任一相电流大于5%Ib
////	{
////		state = TRUE;
////	}
////	//记录状态判断--------------------------------------------------------------
////	if (g_EventSta[EV_LostVoltX] == state)
////	{
////		g_EventTmr[EV_LostVoltX] = 0;
////	}
////	else
////	{
////		if (g_EventSta[EV_LostVoltX] == FALSE)								//进入记录
////		{
////			delay = 60;
////		}
////		else																//退出记录
////		{
////			delay = g_EventSet[7];											//失压延时判断时间
////			if ((delay < 10) || (delay > 99))
////			{
////				delay = 60;
////			}
////		}
////		if ((++g_EventTmr[EV_LostVoltX] >= delay)
////		&& (g_EventSta[EV_LostPower] == FALSE))								//未进入全失压记录
////		{
////			if (g_EventSta[EV_LostVoltX] == FALSE)							//进入记录
////			{
////				curr  = g_Curr[0] +g_Curr[1] +g_Curr[2];
////				curr /= 3;
////				Recd_AllLstVolt(FALSE, curr);
////			}
////			else															//退出记录
////			{
////				Recd_AllLstVolt(TRUE, 0);
////			}
////			g_EventSta[EV_LostVoltX] = state;
////			g_EventTmr[EV_LostVoltX] = 0;
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	掉电时掉电,全失压事件判断
////输入参数：
////返回参数：
////函数说明：	掉电,全失压事件等2个事件记录
////记录数据：
////*******************************************************************************/
////void Chk_LostPower2(void)
////{
////	INT8U	i;
////	INT32U	curr = 0;
////	
////	//掉电事件：供电电压均低于临界电压，且三相负荷电流均不大于5%Ib。
////	//全失压事件：三相电压均低于临界电压，且有任一相或多相电流大于5%Ib，且持续时间大于60s。
////	if ((g_EventSta[EV_LostPower] == FALSE)									//未进记录
////	&& (g_EventSta[EV_LostVoltX] == FALSE))
////	{
////#if	(TP_SoftVer == TP_Release)//正式版本
////		if (++g_EventTmr[EV_LostPower] >= 60)
////#else
////		if (++g_EventTmr[EV_LostPower] >= 10)
////#endif
////		{
////			Open_Current();													//开启电流采样通道
////			for (i=0; i<3; i++)
////			{
////				g_Curr[i] = Read_Reg(r_IaRms+i);							//A,B,C相电流
////				g_Curr[i] *= 9;												//N=1000/144
////				g_Curr[i] >>= 9;
////				if (g_Curr[i] < C_StrIb)
////				{
////					g_Curr[i] = 0;
////				}
////			}
////			if ((g_Curr[0]>C_05Ib) || (g_Curr[1]>C_05Ib) || (g_Curr[2]>C_05Ib))//任一相电流大于5%Ib
////			{
////				curr  = g_Curr[0] +g_Curr[1] +g_Curr[2];
////				curr /= 3;
////				Recd_AllLstVolt(FALSE, curr);
////				g_EventSta[EV_LostVoltX] = TRUE;
////			}
////			else
////			{
////				Recd_LostPower(FALSE);
////				g_EventSta[EV_LostPower] = TRUE;
////			}
////			g_EventTmr[EV_LostPower] = 0;
////			g_EventTmr[EV_LostVoltX] = 0;
////		}
////	}
////	else if (g_EventSta[EV_LostPower] == TRUE)								//已进纪录
////	{
////		g_EventSta[EV_LostVoltX] = FALSE;
////		g_EventTmr[EV_LostPower] = 0;
////		g_EventTmr[EV_LostVoltX] = 0;
////	}
////	else if (g_EventSta[EV_LostVoltX] == TRUE)								//已进纪录
////	{
////		g_EventSta[EV_LostPower] = FALSE;
////		g_EventTmr[EV_LostPower] = 0;
////		g_EventTmr[EV_LostVoltX] = 0;
////	}
////	else
////	{
////		g_EventSta[EV_LostPower] = FALSE;
////		g_EventSta[EV_LostVoltX] = FALSE;
////		g_EventTmr[EV_LostPower] = 0;
////		g_EventTmr[EV_LostVoltX] = 0;
////	}
////}

/////*******************************************************************************
////功能描述：	关闭事件类记录
////输入参数：
////返回参数：
////函数说明：
////*******************************************************************************/
////static void Cls_EventRecd(void)
////{
////	INT8U	i;
////	//记录状态判断--------------------------------------------------------------
////	for (i=0; i<12; i++)
////	{
////		if (g_EventSta[EV_LossVoltA+i] == TRUE)
////		{
////			Recd_LossVolt(EV_LossVoltA+i);									//失压类记录结束
////		}
////	}
////	for (i=0; i<9; i++)
////	{
////		if (g_EventSta[EV_LossCurrA+i] == TRUE)
////		{
////			Recd_LossCurr(EV_LossCurrA+i);									//失流类记录结束
////		}
////	}
////	for (i=0; i<8; i++)
////	{
////		if (g_EventSta[EV_OverLoadA+i] == TRUE)
////		{
////			Recd_OverLoad(EV_OverLoadA+i);									//过载类记录结束
////		}
////	}
////	for (i=0; i<3; i++)
////	{
////		if (g_EventSta[EV_VoltUnBlnc+i] == TRUE)
////		{
////			Recd_UnBalance(EV_VoltUnBlnc+i);								//不平衡记录结束
////		}
////	}
////	for (i=0; i<4; i++)
////	{
////		if (g_EventSta[EV_LowerPfA+i] == TRUE)
////		{
////			Recd_LowerPf(EV_LowerPfA+i);									//功率因数超限类事件结束
////		}
////	}
////	//																		//潮流反向事件结束
////	for (i=0; i<6; i++)
////	{
////		if (g_EventSta[EV_PAcDmdOver+i] == TRUE)
////		{
////			Recd_DmdOver(EV_PAcDmdOver+i);									//需量超限事件结束
////		}
////	}
////	g_EventTmr[EV_LostPower] = 0;											//清零计时
////	g_EventTmr[EV_LostVoltX] = 0;											//清零计时
////}

/////*******************************************************************************
////功能描述：	清除事件类记录状态
////输入参数：
////返回参数：
////函数说明：
////*******************************************************************************/
////void Clr_EventSta(INT8U idx)
////{
////	INT8U	i;
////	
////	if (idx < EV_EventNum)
////	{
////		g_EventSta[idx] = FALSE;
////		g_EventTmr[idx] = 0;
//////		if ((idx >= EV_LossVoltA) && (idx <= EV_OffPhaseC))
//////		if (idx <= EV_OffPhaseC)
//////		{
//////			g_AmpereHr[idx-EV_LossVoltA].tmr = 0;							//计时器
//////			g_AmpereHr[idx-EV_LossVoltA].amp[0] = 0;						//A相安时数
//////			g_AmpereHr[idx-EV_LossVoltA].amp[1] = 0;						//B相安时数
//////			g_AmpereHr[idx-EV_LossVoltA].amp[2] = 0;						//C相安时数
//////		}
//////		else if ((idx >= EV_VoltUnBlnc) && (idx <= EV_SerUnBlnc))
//////		{
//////			g_UnBalance[idx-EV_VoltUnBlnc] = 0;								//不平衡率
//////		}
//////		else if ((idx >= EV_PAcDmdOver) && (idx <= EV_Re4DmdOver))
//////		{
//////			g_MaxDemand[idx-EV_PAcDmdOver] = 0;								//最大需量
//////			Set_Data(&g_MaxDmdTmr[idx-EV_PAcDmdOver][0], 0x00, 6);			//最大需量发生时间
//////		}
//////		else
//////		{
//////		}
////	}
////	else
////	{
////		for (i=0; i<EV_EventNum; i++)										//掉电记录不需初始化????????
////		{
////			g_EventSta[i] = FALSE;											//事件记录标志
////			g_EventTmr[i] = 0;												//事件判断计时
////		}
//////		for (i=0; i<12; i++)
//////		{
//////			g_AmpereHr[i].tmr = 0;											//计时器
//////			g_AmpereHr[i].amp[0] = 0;										//A相安时数
//////			g_AmpereHr[i].amp[1] = 0;										//B相安时数
//////			g_AmpereHr[i].amp[2] = 0;										//C相安时数
//////		}
//////		for (i=0; i<3; i++)
//////		{
//////			g_UnBalance[i] = 0;												//不平衡率
//////		}
//////		for (i=0; i<6; i++)
//////		{
//////			g_MaxDemand[i] = 0;												//最大需量
//////			Set_Data(&g_MaxDmdTmr[i][0], 0x00, 6);							//最大需量发生时间
//////		}
////	}
////}

/////*******************************************************************************
////功能描述：	获取事件类记录状态
////输入参数：	idx:		比较项目		
////返回参数：
////函数说明：
////*******************************************************************************/
////BOOL Get_EventSta(INT8U idx)
////{
////	return g_EventSta[idx];
////}

/////*******************************************************************************
////功能描述：	获取事件类累计时间
////输入参数：	idx:		比较项目		
////返回参数：
////函数说明：
////*******************************************************************************/
////INT32U Get_EventMin(INT8U idx)
////{
////	return g_EventSec[idx] / 60;
////}

/////*******************************************************************************
////功能描述：	获取安时数记录
////输入参数：
////返回参数：
////函数说明：	失压期间（合,A,B,C相）安时数.16
////*******************************************************************************/
////void Get_AmperHr(INT8U idx, INT8U *pBuff)
////{
////	INT8U	i;
////	INT32U	ampere[4];
////	
////	ampere[0] = g_AmpereHr[idx].amp[0] +g_AmpereHr[idx].amp[1] +g_AmpereHr[idx].amp[2];//总安时数
////	ampere[0] >>= 3;
////	ampere[1] = g_AmpereHr[idx].amp[0] >>3;
////	ampere[2] = g_AmpereHr[idx].amp[1] >>3;
////	ampere[3] = g_AmpereHr[idx].amp[2] >>3;
////	for (i=0; i<4; i++)
////	{
////		LHEX_nBCD(&pBuff[i*4], ampere[i], 4);
////	}
////}

/////*******************************************************************************
////功能描述：	获取最大不平衡率
////输入参数：
////返回参数：
////函数说明：	电压,流不平衡期间最大不平衡率
////*******************************************************************************/
////void Get_UnBalance(INT8U idx, INT8U *pBuff)
////{
////	LHEX_nBCD(&pBuff[0], g_UnBalance[idx], 3);
////}

/////*******************************************************************************
////功能描述：	获取最大需量
////输入参数：
////返回参数：
////函数说明：	需量超限期间最大需量及发生时间
////*******************************************************************************/
////void Get_MaxDemand(INT8U idx, INT8U *pBuff)
////{
////	LHEX_nBCD(&pBuff[0], g_MaxDemand[idx], 3);								//最大需量
////	Copy_Data(&pBuff[3], &g_MaxDmdTmr[idx][1], 5);							//发生时间
////}

/////*******************************************************************************
////功能描述：	失压类事件记录
////输入参数：
////返回参数：
////函数说明：	A,B,C相失压,欠压,过压,断相等12个事件记录
////记录数据：	+发生时刻.6
////			+发生时刻（正,反向有功,组合无功1,2）总电能.16
////			+发生时刻（A,B,C相）（正,反向有功,组合无功1,2总电能,电压,电流,有功功率,无功功率,功率因数）.3*(16+13)
////			+失压期间（合,A,B,C相）安时数.16
////			+结束时刻.6
////			+结束时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 ）
////*******************************************************************************/
////static void Recd_LossVolt(INT8U idx)
////{
////	INT8U	i;
////	INT8U	buff[LEN_LossVolt];
////	INT32U	add;															//累加时间
////	ID_RECD	id  = TAB_RecdId[idx].id;
////	BOOL	sta = g_EventSta[idx];
////	
////	Read_RecdData(id, &buff[0], 1, 0, 125+6);								//读出上一次记录
////	if ((((buff[EM_Day]!=0x00) && (buff[EM_Day]!=0xFF))
////	|| ((buff[EM_Month]!=0x00) && (buff[EM_Month]!=0xFF)))					//开始时刻月日!=0?
////	&& (((buff[125+EM_Day]==0x00) || (buff[125+EM_Day]==0xFF))
////	&& ((buff[125+EM_Month]==0x00) || (buff[125+EM_Month]==0xFF))))			//结束时刻月日=0?（有头无尾）
////	{
////		if (TRUE != sta)
////		{
////			return;
////		}
////	}
////	else
////	{
////		if (FALSE != sta)
////		{
////			return;
////		}
////	}
////	
////	Rep_EngyDataT();														//准备合相电量数据
////	Rep_EngyDataX();														//准备分相电量数据
////	if (sta == FALSE)														//记录开始
////	{
////		Rep_RealDataX();													//准备分相实时数据
////		Get_MeterTime(&buff[0]);											//发生时刻
////		Copy_Data(&buff[6], &g_EngyDataT[0], 16);							//复制合相电量数据
////		for (i=0; i<3; i++)
////		{
////			Copy_Data(&buff[22+29*i], &g_EngyDataX[i][0], 16);				//复制分相电量数据
////			Copy_Data(&buff[38+29*i], &g_RealDataX[i][0], 13);				//复制分相实时数据
////		}
////		Write_RecdData(id, &buff[0], 0, 109, 0);							//写入事件纪录
////		Rpt_RecdSta(TAB_RecdId[idx].rpt);									//上报记录类状态（只报起始状态）
////		g_EventSec[idx] = 0;												//事件累计时间
////	}
////	else																	//记录结束
////	{
////		Get_AmperHr(idx, &buff[109]);										//安时数
////		Get_MeterTime(&buff[125]);											//记录时刻
////		Copy_Data(&buff[131], &g_EngyDataT[0], 16);							//复制合相电量数据
////		for (i=0; i<3; i++)
////		{
////			Copy_Data(&buff[147+16*i], &g_EngyDataX[i][0], 16);				//复制分相电量数据
////		}
//////		add = Diff_Mins(&buff[125+1], &buff[1]);							//累加时间
////		add = g_EventSec[idx] / 60;											//累加时间
////		Write_RecdData(id, &buff[109], 109, 86, add);						//写入事件纪录
////	}
////}

/////*******************************************************************************
////功能描述：	失流类事件记录
////输入参数：
////返回参数：
////函数说明：	A,B,C相失流,过流,断流等9个事件记录
////记录数据：	+发生时刻.6
////			+发生时刻（正,反向有功,组合无功1,2）总电能.16
////			+发生时刻（A,B,C相）（正,反向有功,组合无功1,2总电能,电压,电流,有功功率,无功功率,功率因数）.3*(16+13)
////			+结束时刻.6
////			+结束时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 ）
////*******************************************************************************/
////static void Recd_LossCurr(INT8U idx)
////{
////	INT8U	i;
////	INT8U	buff[LEN_LossCurr];
////	INT32U	add;															//累加时间
////	ID_RECD	id  = TAB_RecdId[idx].id;
////	BOOL	sta = g_EventSta[idx];
////	
////	Read_RecdData(id, &buff[0], 1, 0, 109+6);								//读出上一次记录
////	if ((((buff[EM_Day]!=0x00) && (buff[EM_Day]!=0xFF))
////	|| ((buff[EM_Month]!=0x00) && (buff[EM_Month]!=0xFF)))					//开始时刻月日!=0?
////	&& (((buff[109+EM_Day]==0x00) || (buff[109+EM_Day]==0xFF))
////	&& ((buff[109+EM_Month]==0x00) || (buff[109+EM_Month]==0xFF))))			//结束时刻月日=0?（有头无尾）
////	{
////		if (TRUE != sta)
////		{
////			return;
////		}
////	}
////	else
////	{
////		if (FALSE != sta)
////		{
////			return;
////		}
////	}
////	
////	Rep_EngyDataT();														//准备合相电量数据
////	Rep_EngyDataX();														//准备分相电量数据
////	if (sta == FALSE)														//记录开始
////	{
////		Rep_RealDataX();													//准备分相实时数据
////		Get_MeterTime(&buff[0]);											//发生时刻
////		Copy_Data(&buff[6], &g_EngyDataT[0], 16);							//复制合相电量数据
////		for (i=0; i<3; i++)
////		{
////			Copy_Data(&buff[22+29*i], &g_EngyDataX[i][0], 16);				//复制分相电量数据
////			Copy_Data(&buff[38+29*i], &g_RealDataX[i][0], 13);				//复制分相实时数据
////		}
////		Write_RecdData(id, &buff[0], 0, 109, 0);							//写入事件纪录
////		Rpt_RecdSta(TAB_RecdId[idx].rpt);									//上报记录类状态（只报起始状态）
////		g_EventSec[idx] = 0;												//事件累计时间
////	}
////	else																	//记录结束
////	{
////		Get_MeterTime(&buff[109]);											//记录时刻
////		Copy_Data(&buff[115], &g_EngyDataT[0], 16);							//复制合相电量数据
////		for (i=0; i<3; i++)
////		{
////			Copy_Data(&buff[131+16*i], &g_EngyDataX[i][0], 16);				//复制分相电量数据
////		}
//////		add = Diff_Mins(&buff[109+1], &buff[1]);							//累加时间
////		add = g_EventSec[idx] / 60;											//累加时间
////		Write_RecdData(id, &buff[109], 109, 70, add);						//写入事件纪录
////	}
////}

/////*******************************************************************************
////功能描述：	过载类事件记录
////输入参数：
////返回参数：
////函数说明：	电压,电流逆相序,A,B,C相过载,功率反向等8个事件记录
////记录数据：	+发生时刻.6
////			+发生时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 ）
////			+结束时刻.6
////			+结束时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 ）
////*******************************************************************************/
////static void Recd_OverLoad(INT8U idx)
////{
////	INT8U	i;
////	INT8U	buff[LEN_OverLoad];
////	INT32U	add;															//累加时间
////	ID_RECD	id  = TAB_RecdId[idx].id;
////	BOOL	sta = g_EventSta[idx];
////	
////	Read_RecdData(id, &buff[0], 1, 0, 70+6);								//读出上一次记录
////	if ((((buff[EM_Day]!=0x00) && (buff[EM_Day]!=0xFF))
////	|| ((buff[EM_Month]!=0x00) && (buff[EM_Month]!=0xFF)))					//开始时刻月日!=0?
////	&& (((buff[70+EM_Day]==0x00) || (buff[70+EM_Day]==0xFF))
////	&& ((buff[70+EM_Month]==0x00) || (buff[70+EM_Month]==0xFF))))			//结束时刻月日=0?（有头无尾）
////	{
////		if (TRUE != sta)
////		{
////			return;
////		}
////	}
////	else
////	{
////		if (FALSE != sta)
////		{
////			return;
////		}
////	}
////	
////	Rep_EngyDataT();														//准备合相电量数据
////	Rep_EngyDataX();														//准备分相电量数据
////	if (sta == FALSE)														//记录开始
////	{
////		Get_MeterTime(&buff[0]);											//记录时刻
////		Copy_Data(&buff[6], &g_EngyDataT[0], 16);							//复制合相电量数据
////		for (i=0; i<3; i++)
////		{
////			Copy_Data(&buff[22+16*i], &g_EngyDataX[i][0], 16);				//复制分相电量数据
////		}
////		Write_RecdData(id, &buff[0], 0, 70, 0);								//写入事件纪录
////		Rpt_RecdSta(TAB_RecdId[idx].rpt);									//上报记录类状态（只报起始状态）
////		g_EventSec[idx] = 0;												//事件累计时间
////	}
////	else																	//记录结束
////	{
////		Get_MeterTime(&buff[70]);											//记录时刻
////		Copy_Data(&buff[76], &g_EngyDataT[0], 16);							//复制合相电量数据
////		for (i=0; i<3; i++)
////		{
////			Copy_Data(&buff[92+16*i], &g_EngyDataX[i][0], 16);				//复制分相电量数据
////		}
//////		add = Diff_Mins(&buff[70+1], &buff[1]);								//累加时间
////		add = g_EventSec[idx] / 60;											//累加时间
////		Write_RecdData(id, &buff[70], 70, 70, add);							//写入事件纪录
////	}
////}

/////*******************************************************************************
////功能描述：	不平衡类事件记录
////输入参数：
////返回参数：
////函数说明：	电压,电流,电流严重不平衡类等3个事件记录
////记录数据：	+发生时刻.6
////			+发生时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 ）
////			+最大不平衡率.3
////			+结束时刻.6
////			+结束时刻（合,A,B,C相）（正,反向有功,组合无功1,2）总电能.64 ）
////*******************************************************************************/
////static void Recd_UnBalance(INT8U idx)
////{
////	INT8U	i;
////	INT8U	buff[LEN_UnBalance];
////	INT32U	add;															//累加时间
////	ID_RECD	id  = TAB_RecdId[idx].id;
////	BOOL	sta = g_EventSta[idx];
////	
////	Read_RecdData(id, &buff[0], 1, 0, 73+6);								//读出上一次记录
////	if ((((buff[EM_Day]!=0x00) && (buff[EM_Day]!=0xFF))
////	|| ((buff[EM_Month]!=0x00) && (buff[EM_Month]!=0xFF)))					//开始时刻月日!=0?
////	&& (((buff[73+EM_Day]==0x00) || (buff[73+EM_Day]==0xFF))
////	&& ((buff[73+EM_Month]==0x00) || (buff[73+EM_Month]==0xFF))))			//结束时刻月日=0?（有头无尾）
////	{
////		if (TRUE != sta)
////		{
////			return;
////		}
////	}
////	else
////	{
////		if (FALSE != sta)
////		{
////			return;
////		}
////	}
////	
////	Rep_EngyDataT();														//准备合相电量数据
////	Rep_EngyDataX();														//准备分相电量数据
////	if (sta == FALSE)														//记录开始
////	{
////		Get_MeterTime(&buff[0]);											//记录时刻
////		Copy_Data(&buff[6], &g_EngyDataT[0], 16);							//复制合相电量数据
////		for (i=0; i<3; i++)
////		{
////			Copy_Data(&buff[22+16*i], &g_EngyDataX[i][0], 16);				//复制分相电量数据
////		}
////		Write_RecdData(id, &buff[0], 0, 70, 0);								//写入事件纪录
////		Rpt_RecdSta(TAB_RecdId[idx].rpt);									//上报记录类状态（只报起始状态）
////		g_EventSec[idx] = 0;												//事件累计时间
////	}
////	else																	//记录结束
////	{
////		LHEX_nBCD(&buff[70], g_UnBalance[idx-EV_VoltUnBlnc], 3);			//最大不平衡率
////		Get_MeterTime(&buff[73]);											//记录时刻
////		Copy_Data(&buff[79], &g_EngyDataT[0], 16);							//复制合相电量数据
////		for (i=0; i<3; i++)
////		{
////			Copy_Data(&buff[95+16*i], &g_EngyDataX[i][0], 16);				//复制分相电量数据
////		}
//////		add = Diff_Mins(&buff[73+1], &buff[1]);								//累加时间
////		add = g_EventSec[idx] / 60;											//累加时间
////		Write_RecdData(id, &buff[70], 70, 73, add);							//写入事件纪录
////	}
////}

/////*******************************************************************************
////功能描述：	功率因数超限类事件记录
////输入参数：
////返回参数：
////函数说明：	合,A,B,C相功率因数超限等4个事件记录
////记录数据：	+发生时刻.6
////			+发生时刻合相（正,反向有功,组合无功1,2）总电能.16
////			+结束时刻.6
////			+结束时刻合相（正,反向有功,组合无功1,2）总电能.16
////*******************************************************************************/
////static void Recd_LowerPf(INT8U idx)
////{
////	INT8U	buff[LEN_LowerPf];
////	INT32U	add;															//累加时间
////	ID_RECD	id  = TAB_RecdId[idx].id;
////	BOOL	sta = g_EventSta[idx];
////	
////	Read_RecdData(id, &buff[0], 1, 0, 22+6);								//读出上一次记录
////	if (((buff[EM_Day]!=0x00) || (buff[EM_Month]!=0x00))					//开始时刻月日!=0?
////	&& ((buff[22+EM_Day]==0x00)	&& (buff[22+EM_Month]==0x00)))				//结束时刻月日=0?（有头无尾）
////	{
////		if (TRUE != sta)
////		{
////			return;
////		}
////	}
////	else
////	{
////		if (FALSE != sta)
////		{
////			return;
////		}
////	}
////	
////	Rep_EngyDataT();														//准备合相电量数据
////	if (sta == FALSE)														//记录开始
////	{
////		Get_MeterTime(&buff[0]);											//记录时刻
////		Copy_Data(&buff[6], &g_EngyDataT[0], 16);							//复制合相电量数据
////		Set_Data(&buff[22], 0x00, 22);										//结束时刻,结束时刻总电能
////		Write_RecdData(id, &buff[0], 0, LEN_LowerPf, 0);					//写入事件纪录
////		Rpt_RecdSta(TAB_RecdId[idx].rpt);									//上报记录类状态（只报起始状态）
////		g_EventSec[idx] = 0;												//事件累计时间
////	}
////	else																	//记录结束
////	{
////		Get_MeterTime(&buff[22]);											//记录时刻
////		Copy_Data(&buff[28], &g_EngyDataT[0], 16);							//复制合相电量数据
//////		add = Diff_Mins(&buff[22+1], &buff[1]);								//累加时间
////		add = g_EventSec[idx] / 60;											//累加时间
////		Write_RecdData(id, &buff[22], 22, 22, add);							//写入事件纪录
////	}
////}

/////*******************************************************************************
////功能描述：	潮流反向事件记录
////输入参数：
////返回参数：
////函数说明：	潮流反向等1个事件记录
////记录数据：	+发生时刻.6
////			+总有功功率方向.1
////			+合,A,B,C相（正,反有功,组合无功1,2）总电能.64
////*******************************************************************************/
////static void Recd_RevPowerT(BOOL flag)
////{
////	INT8U	i;
////	INT8U	buff[LEN_RevPowerT];
////	
////	Rep_EngyDataT();														//准备合相电量数据
////	Rep_EngyDataX();														//准备分相电量数据
////	Get_MeterTime(&buff[0]);												//记录时刻
////	buff[6] = (flag == FALSE)? 0x00: 0x01;									//总有功功率方向
////	Copy_Data(&buff[7], &g_EngyDataT[0], 16);								//复制合相电量数据
////	for (i=0; i<3; i++)
////	{
////		Copy_Data(&buff[23+16*i], &g_EngyDataX[i][0], 16);					//复制分相电量数据
////	}
////	Write_RecdData(ID_RevPowerT, &buff[0], 0, LEN_RevPowerT, 0);			//写入事件纪录
////	Rpt_RecdSta(RPT_RevPowerT);												//上报记录类状态
////}

/////*******************************************************************************
////功能描述：	需量超限事件记录
////输入参数：
////返回参数：
////函数说明：	正,反向有功,象限1,2,3,4无功需量超限等6个事件记录
////记录数据：	+发生时刻.6
////			+结束时刻.6
////			+超限期间最大需量及发生时间.8
////*******************************************************************************/
////static void Recd_DmdOver(INT8U idx)
////{
////	INT8U	buff[LEN_DmdOver];
////	ID_RECD	id  = TAB_RecdId[idx].id;
////	BOOL	sta = g_EventSta[idx];
////	
////	Read_RecdData(id, &buff[0], 1, 0, 6+6);									//读出上一次记录
////	if (((buff[EM_Day]!=0x00) || (buff[EM_Month]!=0x00))					//开始时刻月日!=0?
////	&& ((buff[6+EM_Day]==0x00) && (buff[6+EM_Month]==0x00)))				//结束时刻月日=0?（有头无尾）
////	{
////		if (TRUE != sta)
////		{
////			return;
////		}
////	}
////	else
////	{
////		if (FALSE != sta)
////		{
////			return;
////		}
////	}
////	
////	
////	if (sta == FALSE)														//记录开始
////	{
////		Get_MeterTime(&buff[0]);											//记录时刻
////		Set_Data(&buff[6], 0x00, 14);										//结束时刻,超限期间最大需量及发生时间.8
////		Write_RecdData(id, &buff[0], 0, LEN_DmdOver, 0);					//写入事件纪录
////		Rpt_RecdSta(TAB_RecdId[idx].rpt);									//上报记录类状态（只报起始状态）
////	}
////	else																	//记录结束
////	{
////		Get_MeterTime(&buff[6]);											//记录时刻
////		LHEX_nBCD(&buff[12], g_MaxDemand[idx-EV_PAcDmdOver], 3);			//最大需量
////		Copy_Data(&buff[15], &g_MaxDmdTmr[idx-EV_PAcDmdOver][1], 5);		//发生时间
////		Write_RecdData(id, &buff[6], 6, 14, 0);								//写入事件纪录
////	}
////}

/////*******************************************************************************
////功能描述：	全失压事件记录
////输入参数：
////返回参数：
////函数说明：
////记录数据：	全失压事件	（发生时刻.6 +电流值.3 +结束时刻.6）
////*******************************************************************************/
////static void Recd_AllLstVolt(BOOL end, INT32U curr)
////{
////	INT8U	buff[LEN_AllLstVolt];
////	INT32U	add;															//累加时间
////	
////	Read_RecdData(ID_AllLstVolt, &buff[0], 1, 0, LEN_AllLstVolt);			//读出上一次记录
////	if (((buff[EM_Day]!=0x00) || (buff[EM_Month]!=0x00))					//开始时刻月日!=0?
////	&& ((buff[9+EM_Day]==0x00) && (buff[9+EM_Month]==0x00)))				//结束时刻月日=0?（有头无尾）
////	{
////		if (TRUE != end)
////		{
////			return;
////		}
////	}
////	else
////	{
////		if (FALSE != end)
////		{
////			return;
////		}
////	}
////	
////	if (end == FALSE)														//记录开始
////	{
////		Get_MeterTime(&buff[0]);											//记录时刻
////		LHEX_nBCD(&buff[6], curr, 3);										//平均电流
////		Set_Data(&buff[9], 0x00, 6);										//结束时刻
////		Write_RecdData(ID_AllLstVolt, &buff[0], 0, LEN_AllLstVolt, 0);		//写入事件纪录
////		Rpt_RecdSta(RPT_AllLstVolt);										//上报记录类状态（只报起始状态）
////	}
////	else																	//记录结束
////	{
////		Get_MeterTime(&buff[9]);											//记录时刻
////		add = Diff_Mins(&buff[9+1], &buff[1]);
////		Write_RecdData(ID_AllLstVolt, &buff[9], 9, 6, add);					//写入事件纪录
////	}
////}

/////*******************************************************************************
////功能描述：	掉电,全失压事件记录
////输入参数：
////返回参数：
////函数说明：
////记录数据：	掉电事件	（发生时刻.6 +结束时刻.6）
////*******************************************************************************/
////static void Recd_LostPower(BOOL end)
////{
////	INT8U	buff[LEN_LostPower];
////	
////	Read_RecdData(ID_LostPower, &buff[0], 1, 0 , LEN_LostPower);			//读出上一次记录
////	if (((buff[EM_Day]!=0x00) || (buff[EM_Month]!=0x00))					//开始时刻月日!=0?
////	&& ((buff[6+EM_Day]==0x00) && (buff[6+EM_Month]==0x00)))				//结束时刻月日=0?（有头无尾）
////	{
////		if (TRUE != end)
////		{
////			return;
////		}
////	}
////	else
////	{
////		if (FALSE != end)
////		{
////			return;
////		}
////	}
////	
////	if (FALSE == end)														//记录开始
////	{
////		Get_MeterTime(&buff[0]);											//记录时刻
////		Set_Data(&buff[6], 0x00, 6);										//结束时刻
////		Write_RecdData(ID_LostPower, &buff[0], 0, LEN_LostPower, 0);		//写入事件纪录
////		Rpt_RecdSta(RPT_LostPower);											//上报记录类状态（只报起始状态）
////	}
////	else																	//记录结束
////	{
////		Get_MeterTime(&buff[6]);											//记录时刻
////		Write_RecdData(ID_LostPower, &buff[6], 6, 6, 0);					//写入事件纪录
////	}
////}

/////*******************************************************************************
////功能描述：	准备合相电量数据
////*******************************************************************************/
////static void Rep_EngyDataT(void)
////{
////	if (b_EngyDataT != TRUE)
////	{
////		b_EngyDataT = TRUE;
////		Read_CurkWh(&g_EngyDataT[0], ID_PhsT, ID_PosActEny, 0);				//读(当前)合相,正向有功总电量
////		Read_CurkWh(&g_EngyDataT[4], ID_PhsT, ID_NegActEny, 0);				//读(当前)合相,反向有功总电量
////		Read_CurkWh(&g_EngyDataT[8], ID_PhsT, ID_Cmb1RctEny, 0);			//读(当前)合相,组合1无功总电量
////		Read_CurkWh(&g_EngyDataT[12],ID_PhsT, ID_Cmb2RctEny, 0);			//读(当前)合相,组合2无功总电量
////	}
////}

/////*******************************************************************************
////功能描述：	准备分相电量数据
////*******************************************************************************/
////static void Rep_EngyDataX(void)
////{
////	INT8U	i;
////	if (b_EngyDataX != TRUE)
////	{
////		b_EngyDataX = TRUE;
////		for (i=0; i<3; i++)
////		{
////			Read_CurkWh(&g_EngyDataX[i][0], ID_PhsA+i, ID_PosActEny, 0);	//读(当前)分相,正向有功总电量
////			Read_CurkWh(&g_EngyDataX[i][4], ID_PhsA+i, ID_NegActEny, 0);	//读(当前)分相,反向有功总电量
////			Read_CurkWh(&g_EngyDataX[i][8], ID_PhsA+i, ID_Cmb1RctEny, 0);	//读(当前)分相,组合1无功总电量
////			Read_CurkWh(&g_EngyDataX[i][12],ID_PhsA+i, ID_Cmb2RctEny, 0);	//读(当前)分相,组合2无功总电量
////		}
////	}
////}

/////*******************************************************************************
////功能描述：	准备分相实时数据
////*******************************************************************************/
////static void Rep_RealDataX(void)
////{
////	INT8U	i;
////	if (b_RealDataX != TRUE)
////	{
////		b_RealDataX = TRUE;
////		for (i=0; i<3; i++)
////		{
////			LHEX_nBCD(&g_RealDataX[i][0], g_Volt[i], 2);					//读(当前)电压
////			LHEX_nBCD(&g_RealDataX[i][2], g_Curr[i], 3);					//读(当前)电流
////		//	Read_RealData(ID_Ua+i, &g_RealDataX[i][0]);						//读(当前)电压
////		//	Read_RealData(ID_Ia+i, &g_RealDataX[i][2]);						//读(当前)电流
////			Read_RealData((ID_REAL)(ID_Pa+i), &g_RealDataX[i][5]);			//读(当前)有功功率
////			Read_RealData((ID_REAL)(ID_Qa+i), &g_RealDataX[i][8]);			//读(当前)无功功率
////			Read_RealData((ID_REAL)(ID_Pfa+i),&g_RealDataX[i][11]);			//读(当前)功率因数
////		}
////	}
////}

///////*******************************************************************************
//////功能描述：	设置事件记录判断参数
//////输入参数：
//////返回参数：
//////函数说明：
//////*******************************************************************************/
//////BOOL Set_EventSet(INT16U dis, const INT8U *pPara)
//////{
//////	INT8U	i;
//////	INT8U	buff[LEN_EventSet];
//////	INT32U	temp = 0;;
//////	
//////	for (i=0; i<sizeof(TAB_EventSet)/sizeof(TAB_EventSet[0]); i++)
//////	{
//////		if (dis == TAB_EventSet[i].dis)
//////		{
//////			break;
//////		}
//////	}
//////	if ((i < sizeof(TAB_EventSet)/sizeof(TAB_EventSet[0]))
//////	&& (TRUE == Is_nBCD(&pPara[0], TAB_EventSet[i].len)))
//////	{
//////		if ((TAB_EventSet[i].len == 1)										//判断延时时间
//////		&& (pPara[0] <= 0x09))												//<10s
//////		{
//////			return FALSE;
//////		}
//////		temp = nBCD_LHEX(&pPara[0], TAB_EventSet[i].len);
//////		Read_ParaData(ID_EventSet, &buff[0]);
//////		Copy_Data(&buff[TAB_EventSet[i].off], (INT8U*)&temp, TAB_EventSet[i].len);
//////		if (TRUE == Write_ParaData(ID_EventSet, &buff[0]))
//////		{
//////			Copy_Data(&g_EventSet[0], &buff[0], LEN_EventSet);
//////			Fetch_CRC(&g_EventSet[0], LEN_EventSet);
//////			return TRUE;
//////		}
//////	}
//////	return FALSE;
//////}

///////*******************************************************************************
//////功能描述：	获取事件记录判断参数
//////输入参数：
//////返回参数：
//////函数说明：
//////*******************************************************************************/
//////INT8U Get_EventSet(INT16U dis, INT8U *pPara)
//////{
//////	INT8U	i;
//////	INT8U	buff[LEN_EventSet];
//////	INT32U	value = 0;
//////	
//////	for (i=0; i<sizeof(TAB_EventSet)/sizeof(TAB_EventSet[0]); i++)
//////	{
//////		if (dis == TAB_EventSet[i].dis)
//////		{
//////			break;
//////		}
//////	}
//////	if (i < sizeof(TAB_EventSet)/sizeof(TAB_EventSet[0]))
//////	{
//////		Read_ParaData(ID_EventSet, &buff[0]);
//////		Copy_Data((INT8U*)&value, &buff[TAB_EventSet[i].off], TAB_EventSet[i].len);
//////		LHEX_nBCD(&pPara[0], value, TAB_EventSet[i].len);
//////		return TAB_EventSet[i].len;
//////	}
//////	return 0;
//////}

///////*******************************************************************************
//////功能描述：	电压合格率统计
//////输入参数：
//////返回参数：
//////函数说明：
//////*******************************************************************************/
//////static void Count_VoltQlty(void)
//////{
//////	INT8U	i, j, idx, buff[LEN_VoltQlty];
//////	INT32U	volt[3];
//////	INT16U	limitH, limitL, voltH, voltL;
//////	INT16U	low, high;
//////	
//////	g_VoltIdx++;
//////	if ((g_VoltIdx%10) != 0)
//////	{
//////		return;
//////	}
//////	if (g_VoltIdx > 60)
//////	{
//////		g_VoltIdx = 0;
//////		return;
//////	}
//////	//数据采样------------------------------------------------------------------
//////	idx = g_VoltIdx/10 - 1;
//////	for (i=0; i<3; i++)
//////	{
//////		g_VoltDetX[i].samp[idx] = g_Volt[i];								//分相电压
//////	}
//////	//数据处理------------------------------------------------------------------
//////	if (g_VoltIdx == 60)//最后一个
//////	{
//////		g_VoltIdx = 0;
//////		
//////		for (i=0; i<3; i++)
//////		{
//////			low  = 0xFFFF;
//////			high = 0x0000;
//////			volt[i] = 0;
//////			for (j=0; j<6; j++)
//////			{
//////				if (g_VoltDetX[i].samp[j] < low)
//////				{
//////					low = g_VoltDetX[i].samp[j];
//////				}
//////				if (g_VoltDetX[i].samp[j] > high)
//////				{
//////					high = g_VoltDetX[i].samp[j];
//////				}
//////				volt[i] += g_VoltDetX[i].samp[j];
//////			}
//////			volt[i] -= high;												//去掉最高
//////			volt[i] -= low;													//去掉最低
//////			volt[i] >>= 2;													//取平均
//////		}
//////		//合相电压合格率统计----------------------------------------------------
//////		Read_ParaData(ID_VoltQSet, &buff[0]);								//电压合格率统计参数
//////		limitH = buff[0] | buff[1]<<8;										//电压考核上限
//////		limitL = buff[2] | buff[3]<<8;										//电压考核下限
//////		voltH  = buff[4] | buff[5]<<8;										//电压上限
//////		voltL  = buff[6] | buff[7]<<8;										//电压下限
//////		if((volt[0] > limitL) && (volt[0] < limitH)
//////		&& (volt[1] > limitL) && (volt[1] < limitH)
//////		&& (volt[2] > limitL) && (volt[2] < limitH))
//////		{
//////			g_VoltDetT.dMins++;												//检测时间+1;
//////			if((volt[0] > voltH) && (volt[0] < voltH)
//////			&& (volt[1] > voltH) && (volt[1] < voltH)
//////			&& (volt[2] > voltH) && (volt[2] < voltH))
//////			{
//////				g_VoltDetT.stand++;											//合格时间+1;
//////			}	
//////			if ((volt[0] > voltH) || (volt[1] > voltH) || (volt[2] > voltH))
//////			{
//////				g_VoltDetT.overH++;											//超上限时间+1
//////			}
//////			if ((volt[0] < voltL) || (volt[1] < voltL) || (volt[2] < voltL))
//////			{
//////				g_VoltDetT.overL++;											//超下限时间+1
//////			}
//////		}
//////		//分相电压合格率统计----------------------------------------------------
//////		for (i=0; i<3; i++)
//////		{
//////			if ((volt[i] > limitL) && (volt[i] < limitH))
//////			{
//////				g_VoltDetX[i].dMins++;										//检测时间+1;
//////				if (volt[i] > voltH)
//////				{
//////					g_VoltDetX[i].overH++;									//超上限时间+1
//////				}
//////				else if (volt[i] < voltL)
//////				{
//////					g_VoltDetX[i].overL++;									//超下限时间+1
//////				}
//////				else
//////				{
//////				}
//////				if (volt[i] > g_VoltDetX[i].voltH)							//>最高电压
//////				{
//////					g_VoltDetX[i].voltH = volt[i];
//////					Copy_Data(g_VoltDetX[i].timeH, pGet_Time(EM_Min), 4);	//最高电压出现时间
//////				}
//////				if (volt[i] < g_VoltDetX[i].voltL)							//<最低电压
//////				{
//////					g_VoltDetX[i].voltL = volt[i];
//////					Copy_Data(g_VoltDetX[i].timeL, pGet_Time(EM_Min), 4);	//最低电压出现时间
//////				}
//////			}
//////		}
//////	}
//////}

///////*******************************************************************************
//////功能描述：	获取当前电压合格率
//////输入参数：	pBuff:		数据出口
//////			idx			
//////				|__0,		合相
//////				|__1,		A相
//////				|__2,		B相
//////				|__2,		C相
//////返回参数：
//////函数说明：
//////*******************************************************************************/
//////void Get_VoltQlty(INT8U *pBuff, INT8U idx)
//////{
//////	INT32U	temp;
//////	INT8U	i;
//////	
//////	if (idx == 0)//合相---------------------------------------------------------
//////	{
//////		if (g_VoltDetT.dMins != 0)
//////		{
//////			LHEX_nBCD(&pBuff[0], g_VoltDetT.dMins, 3);						//电压检测时间
//////			temp  = g_VoltDetT.stand;
//////			temp *= 100;
//////			temp /= g_VoltDetT.dMins;
//////			LHEX_nBCD(&pBuff[3], temp, 3);									//电压合格率.3
//////			temp = 10000 - temp;
//////			LHEX_nBCD(&pBuff[6], temp, 3);									//电压超限率.3
//////			LHEX_nBCD(&pBuff[9], g_VoltDetT.overH, 3);						//电压超上限时间.3
//////			LHEX_nBCD(&pBuff[12], g_VoltDetT.overL, 3);						//电压超下限时间.3
//////			if (g_VoltDetX[0].voltH > g_VoltDetX[1].voltH)					//A > B
//////			{
//////				if (g_VoltDetX[0].voltH > g_VoltDetX[2].voltH)				//A > C
//////				{
//////					i = 0;
//////				}
//////				else
//////				{
//////					i = 2;
//////				}
//////			}
//////			else															//A < B
//////			{
//////				if (g_VoltDetX[1].voltH > g_VoltDetX[2].voltH)				//B > C
//////				{
//////					i = 1;
//////				}
//////				else														//A > C
//////				{
//////					i = 2;
//////				}
//////			}
//////			LHEX_nBCD(&pBuff[15], g_VoltDetX[i].voltH, 3);					//最高电压.2
//////			Copy_Data(&pBuff[17], g_VoltDetX[i].timeH, 4);					//最高电压出现时间.4
//////			if (g_VoltDetX[0].voltL < g_VoltDetX[1].voltL)					//A < B
//////			{
//////				if (g_VoltDetX[0].voltL < g_VoltDetX[2].voltL)				//A < C
//////				{
//////					i = 0;
//////				}
//////				else
//////				{
//////					i = 2;
//////				}
//////			}
//////			else															//A > B
//////			{
//////				if (g_VoltDetX[1].voltL < g_VoltDetX[2].voltL)				//B < C
//////				{
//////					i = 1;
//////				}
//////				else														//A < C
//////				{
//////					i = 2;
//////				}
//////			}
//////			LHEX_nBCD(&pBuff[21], g_VoltDetX[i].voltL, 3);					//最低电压.2
//////			Copy_Data(&pBuff[23], g_VoltDetX[i].timeL, 4);					//最低电压出现时间.4
//////		}
//////		else
//////		{
//////			Set_Data(&pBuff[0], 0x00, 27);
//////		}
//////	}
//////	else if (idx <= 3)//分相----------------------------------------------------
//////	{
//////		if (g_VoltDetX[idx-1].dMins != 0)
//////		{
//////			LHEX_nBCD(&pBuff[0], g_VoltDetX[idx-1].dMins, 3);				//电压检测时间
//////			temp  = g_VoltDetX[idx-1].overH + g_VoltDetX[idx-1].overL;
//////			temp *= 100;
//////			temp /= g_VoltDetX[idx-1].dMins;
//////			LHEX_nBCD(&pBuff[6], temp, 3);									//电压超限率.3
//////			temp = 10000 - temp;
//////			LHEX_nBCD(&pBuff[3], temp, 3);									//电压合格率.3
//////			LHEX_nBCD(&pBuff[9], g_VoltDetX[idx-1].overH, 3);				//电压超上限时间.3
//////			LHEX_nBCD(&pBuff[12], g_VoltDetX[idx-1].overL, 3);				//电压超下限时间.3
//////			LHEX_nBCD(&pBuff[15], g_VoltDetX[idx-1].voltH, 3);				//最高电压.2
//////			Copy_Data(&pBuff[17], g_VoltDetX[idx-1].timeH, 4);				//最高电压出现时间.4
//////			LHEX_nBCD(&pBuff[21], g_VoltDetX[idx-1].voltL, 3);				//最低电压.2
//////			Copy_Data(&pBuff[23], g_VoltDetX[idx-1].timeL, 4);				//最低电压出现时间.4
//////		}
//////		else
//////		{
//////			Set_Data(&pBuff[0], 0x00, 27);
//////		}
//////	}
//////	else
//////	{
//////		Set_Data(&pBuff[0], 0x00, 27);
//////	}
//////}

///////*******************************************************************************
//////功能描述：	初始化电压合格率
//////输入参数：
//////返回参数：
//////函数说明：
//////*******************************************************************************/
//////void Init_VoltQlty(void)
//////{
//////	INT8U	i;
//////	
//////	g_VoltIdx = 0;															//初次40(60-20)秒,以后周期60秒,每周采样10秒.
//////	g_VoltDetT.dMins = 0;													//合相电压检测时间.2
//////	g_VoltDetT.stand = 0;													//合相电压合格时间.2
//////	g_VoltDetT.overH = 0;													//合相电压超上限时间.2
//////	g_VoltDetT.overL = 0;													//合相电压超下限时间.2
//////	for (i=0; i<3; i++)
//////	{
//////		Set_Data((INT8U*)&g_VoltDetX[i].samp[0], 0x00, 6*4);				//分相电压采样数据
//////		g_VoltDetX[i].dMins = 0;											//分相电压检测时间.2
//////		g_VoltDetX[i].overH = 0;											//分相电压超上限时间.2
//////		g_VoltDetX[i].overL = 0;											//分相电压超下限时间.2
//////		g_VoltDetX[i].voltH = 0x0000;										//分相最高电压.2
//////		Set_Data(&g_VoltDetX[i].timeH[0], 0x00, 4);							//分相最高电压出现时间.4
//////		g_VoltDetX[i].voltL = 0xFFFF;										//分相最低电压.2
//////		Set_Data(&g_VoltDetX[i].timeL[0], 0x00, 4);							//分相最低电压出现时间.4
//////	}
//////}

///////*******************************************************************************
//////功能描述：	取出当前电压合格率数据
//////输入参数：
//////返回参数：
//////函数说明：	上电时用
//////*******************************************************************************/
//////static void Load_VoltQlty(void)
//////{
////////	INT8U	i;
////////	INT8U	buff[LEN_VQltyDat];
////////	
////////	Read_ParaData(ID_VQltyDat, &buff[0]);
////////	g_VoltIdx = 0;
////////	g_VoltDetT.dMins = buff[0] | buff[1]<<8;								//合相电压检测时间.2
////////	g_VoltDetT.stand = buff[2] | buff[3]<<8;								//合相电压合格时间.2
////////	g_VoltDetT.overH = buff[4] | buff[5]<<8;								//合相电压超上限时间.2
////////	g_VoltDetT.overL = buff[6] | buff[7]<<8;								//合相电压超下限时间.2
////////	for (i=0; i<3; i++)
////////	{
////////		Set_Data((INT8U*)&g_VoltDetX[i].samp[0], 0x00, 6*4);				//分相电压采样数据
////////		g_VoltDetX[i].dMins = buff[8+i*18] | buff[9+i*18]<<8;				//分相电压检测时间.2
////////		g_VoltDetX[i].overH = buff[10+i*18] | buff[11+i*18]<<8;				//分相电压超上限时间.2
////////		g_VoltDetX[i].overL = buff[12+i*18] | buff[13+i*18]<<8;				//分相电压超下限时间.2
////////		g_VoltDetX[i].voltH = buff[14+i*18] | buff[15+i*18]<<8;				//分相最高电压.2
////////		Copy_Data(&g_VoltDetX[i].timeH[0], &buff[16+i*18], 4);				//分相最高电压出现时间.4
////////		g_VoltDetX[i].voltL = buff[20+i*18] | buff[21+i*18]<<8;				//分相最低电压.2
////////		Copy_Data(&g_VoltDetX[i].timeL[0], &buff[22+i*18], 4);				//分相最低电压出现时间.4
////////	}
//////}

///////*******************************************************************************
//////功能描述：	保存当前电压合格率数据
//////输入参数：
//////返回参数：
//////函数说明：	掉电时用
//////*******************************************************************************/
//////static void Save_VoltQlty(void)
//////{
////////	INT8U	i;
////////	INT8U	buff[LEN_VQltyDat];
////////	
//////////	Read_ParaData(ID_VQltyDat, &buff[0]);
//////////	g_VoltIdx = 0;
////////	buff[0] = g_VoltDetT.dMins;												//合相电压检测时间.2
////////	buff[1] = g_VoltDetT.dMins>>8;
////////	buff[2] = g_VoltDetT.stand;												//合相电压合格时间.2
////////	buff[3] = g_VoltDetT.stand>>8;
////////	buff[4] = g_VoltDetT.overH;												//合相电压超上限时间.2
////////	buff[5] = g_VoltDetT.overH>>8;
////////	buff[6] = g_VoltDetT.overL;												//合相电压超下限时间.2
////////	buff[7] = g_VoltDetT.overL>>8;
////////	for (i=0; i<3; i++)
////////	{
//////////		Set_Data((INT8U*)&g_VoltDetX[i].samp[0], 0x00, 6*4);				//分相电压采样数据
////////		buff[8+i*18]  = g_VoltDetX[i].dMins;								//分相电压检测时间.2
////////		buff[9+i*18]  = g_VoltDetX[i].dMins>>8;
////////		buff[10+i*18] = g_VoltDetX[i].overH;								//分相电压超上限时间.2
////////		buff[11+i*18] = g_VoltDetX[i].overH>>8;
////////		buff[12+i*18] = g_VoltDetX[i].overL;								//分相电压超下限时间.2
////////		buff[13+i*18] = g_VoltDetX[i].overL>>8;
////////		buff[14+i*18] = g_VoltDetX[i].voltH;								//分相最高电压.2
////////		buff[15+i*18] = g_VoltDetX[i].voltH>>8;
////////		Copy_Data(&buff[16+i*18], &g_VoltDetX[i].timeH[0], 4);				//分相最高电压出现时间.4
////////		buff[20+i*18] = g_VoltDetX[i].voltL;								//分相最低电压.2
////////		buff[21+i*18] = g_VoltDetX[i].voltL>>8;
////////		Copy_Data(&buff[22+i*18], &g_VoltDetX[i].timeL[0], 4);				//分相最低电压出现时间.4
////////	}
////////	Write_ParaData(ID_VQltyDat, &buff[0]);
//////}

/////////*******************************************************************************
////////功能描述：	需量服务程序
////////输入参数：
////////返回参数：
////////函数说明：
////////*******************************************************************************/
////////static void Demand_Service(void)
////////{
////////	INT8U	i, j, now, old, comb1, comb2;
////////	INT32U	demand, addr;
////////	INT8U	buff[8];
////////	INT8U	read = FALSE;
////////	
////////	now = TAB_QuadMod[(g_PowQuad>>3)&0x11];
////////	old = TAB_QuadMod[(g_DmdQuad>>3)&0x11];
////////	comb1 = Get_EnyCmbZ(1);
////////	comb2 = Get_EnyCmbZ(2);
////////	
////////	//功率方向检查--------------------------------------------------------------
////////	if ((g_DmdQuad & 0x08) != (g_PowQuad & 0x08))							//有功功率方向变化
////////	{
////////		g_DmdSec[0] = 0;													//需量秒计时器
////////		g_DmdMin[0] = 0;													//需量分计时器
////////		g_DmdAdd[0] = 0;													//累加需量(功率)
////////		for (j=0; j<60; j++)
////////		{
////////			g_DmdBuf[0][j] = 0;												//缓存需量(功率)
////////		}
////////		g_DmdDat[0] = 0;													//当前需量(功率)
////////	}
////////	if ((g_DmdQuad & 0x80) != (g_PowQuad & 0x80))							//无功功率方向变化
////////	{
////////		g_DmdSec[1] = 0;													//需量秒计时器
////////		g_DmdMin[1] = 0;													//需量分计时器
////////		g_DmdAdd[1] = 0;													//累加需量(功率)
////////		for (j=0; j<60; j++)
////////		{
////////			g_DmdBuf[1][j] = 0;												//缓存需量(功率)
////////		}
////////		g_DmdDat[1] = 0;													//当前需量(功率)
////////	}
////////	if (g_DmdQuad != g_PowQuad)
////////	{
////////		if ((((comb1 & old) != 0x00) && ((comb1 & now) == 0x00))			//组合无功1方向变化
////////		|| (((comb1 & old) == 0x00)	&& ((comb1 & now) != 0x00)))
////////		{
////////			g_DmdSec[1] = 0;												//需量秒计时器
////////			g_DmdMin[2] = 0;												//需量分计时器
////////			g_DmdAdd[2] = 0;												//累加需量(功率)
////////			for (j=0; j<60; j++)
////////			{
////////				g_DmdBuf[2][j] = 0;											//缓存需量(功率)
////////			}
////////		}
////////		if ((((comb2 & old) != 0x00) && ((comb2 & now) == 0x00))			//组合无功2方向变化
////////		|| (((comb2 & old) == 0x00)	&& ((comb2 & now) != 0x00)))
////////		{
////////			g_DmdSec[3] = 0;												//需量秒计时器
////////			g_DmdMin[3] = 0;												//需量分计时器
////////			g_DmdAdd[3] = 0;												//累加需量(功率)
////////			for (j=0; j<60; j++)
////////			{
////////				g_DmdBuf[3][j] = 0;											//缓存需量(功率)
////////			}
////////		}
////////	}
////////	if ((g_DmdQuad & 0x88) != (g_PowQuad & 0x88))							//象限无功方向变化
////////	{
////////		g_DmdSec[4] = 0;													//需量秒计时器
////////		g_DmdMin[4] = 0;													//需量分计时器
////////		g_DmdAdd[4] = 0;													//累加需量(功率)
////////		for (j=0; j<60; j++)
////////		{
////////			g_DmdBuf[4][j] = 0;												//缓存需量(功率)
////////		}
////////	}
////////	g_DmdQuad = g_PowQuad;													//需量象限寄存器更新
////////	
////////	//存入需量缓存--------------------------------------------------------------
////////	g_DmdAdd[0] += g_PowP[3];		//有功
////////	g_DmdAdd[1] += g_PowQ[3];		//有功
////////	if ((comb1 & now) != 0x00)
////////	{
////////		g_DmdAdd[2] += g_PowQ[3];	//组合1无功
////////	}
////////	if ((comb2 & now) != 0x00)
////////	{
////////		g_DmdAdd[3] += g_PowQ[3];	//组合2无功
////////	}
////////	g_DmdAdd[4] += g_PowQ[3];		//象限无功
////////	
////////	//--------------------------------------------------------------------------
////////	for (i=0; i<5; i++)
////////	{
////////		if (++g_DmdSec[i] >= 60)
////////		{
////////			g_DmdSec[i] = 0;
////////			g_DmdAdd[i] /= 60;												//取1分钟平均
////////			g_DmdBuf[i][g_DmdMin[i]] = g_DmdAdd[i];							//存入缓存需量(功率)
////////			if (i == 0)
////////			{
////////				g_AvgPowP = g_DmdAdd[0];									//一分钟平均有功功率
////////			}
////////			g_DmdAdd[i] = 0;												//清除累加需量(功率)
////////			
////////			//读取需量设置参数--------------------------------------------------
////////			if (read != TRUE)
////////			{
////////				read = TRUE;
////////				if (TRUE != Check_CRC(&g_DmdPara[0], LEN_DmdPara))
////////				{
////////					if (TRUE != Read_ParaData(ID_DmdPara, &g_DmdPara[0]))
////////					{
////////						Copy_Data(&g_DmdPara[0], TAB_DmdPara, LEN_DmdPara);
////////					}
////////					Fetch_CRC(&g_DmdPara[0], LEN_DmdPara);
////////				}
////////				if ((g_DmdPara[0] == 0) || (g_DmdPara[0] > 60)				//需量周期(1~60)
////////				|| (g_DmdPara[1] == 0) || (g_DmdPara[1] > 20)				//滑差时间(1~20)
////////	//			|| ((g_DmdPara[0] % g_DmdPara[1]) != 0x00))					//周期整除滑差
////////				|| (g_DmdPara[0] < g_DmdPara[1]))							//周期大于滑差
////////				{
////////					Copy_Data(&g_DmdPara[0], TAB_DmdPara, LEN_DmdPara);
////////					Fetch_CRC(&g_DmdPara[0], LEN_DmdPara);
////////				}
////////			}
////////			
////////			if (++g_DmdMin[i] >= g_DmdPara[0])								//周期+滑差
////////			{
////////				g_DmdMin[i] = g_DmdPara[0] - g_DmdPara[1];					//周期
////////				
////////				demand = 0;
////////				for (j=0; j<g_DmdPara[0]; j++)
////////				{
////////					demand += g_DmdBuf[i][j];
////////				}
////////				demand /= g_DmdPara[0];										//需量周期内平均功率
////////				g_DmdDat[i] = demand;										//当前需量
////////				
////////				for (j=0; j<g_DmdPara[0]-g_DmdPara[1]; j++)
////////				{
////////					g_DmdBuf[i][j] = g_DmdBuf[i][g_DmdPara[1]+j];
////////				}
////////				for (j=g_DmdPara[0]-g_DmdPara[1]; j<g_DmdPara[0]; j++)
////////				{
////////					g_DmdBuf[i][j] = 0;
////////				}
////////			
////////				switch (i)
////////				{
////////				case 0://有功需量
////////					j = (g_DmdQuad>>3)&0x01;								//正向,反向有功
////////					break;
////////				case 1://无功需量
////////					break;
////////				case 2://组合无功1需量
////////					j = 2;
////////					break;
////////				case 3://组合无功2需量
////////					j = 3;
////////					break;
////////				case 4://四象限无功需量
////////					j = 4+TAB_Quadrant[(g_DmdQuad>>3)&0x11];				//无功象限
////////					break;
////////				default:
////////					break;
////////				}
////////				if (i != 1)													//无功需量不作记录
////////				{
////////					addr = EE_MaxDemand+8*j*5;								//总费率最大需量
////////					
////////					for (j=0; j<2; j++)
////////					{
////////						if ((TRUE == Read_EEprom(&buff[0], addr, 8))		//当前最大需量数据
////////						&& (demand > (buff[0] | buff[1]<<8 | buff[2]<<16)))
////////						{
////////							buff[0] = demand;
////////							buff[1] = demand>>8;
////////							buff[2] = demand>>16;
////////							Copy_Data(&buff[3], pGet_Time(EM_Min), 5);		//发生时刻
////////							Write_EEprom(addr, &buff[0], 8);				//当前最大需量数据
////////						}
////////						addr += 8*Get_RateNo();								//费率最大需量
////////					}
////////				}
////////			}
////////		}
////////	}
////////}

///////*******************************************************************************
//////功能描述：	获取当前最大需量
//////输入参数：	dmd:		需量序号
//////				|__0x00,	正向有功
//////				|__0x01,	反向有功
//////				|__0x02,	组合无功1
//////				|__0x03,	组合无功1
//////				|__0x04,	象限1无功
//////				|__0x05,	象限2无功
//////				|__0x06,	象限3无功
//////				|__0x07,	象限4无功
//////				|__0xFD,	正,反向有功,象限1,2,3,4无功
//////				|__0xFE,	正,反向有功,
//////				|__0xFF,	正,反向有功,组合无功1,2,象限1,2,3,4无功
//////			rateNo:		费率号
//////				|__0x00,	总
//////				|__0x01,	尖
//////				|__0x02,	峰
//////				|__0x03,	平
//////				|__0x04,	谷
//////				|__0xFF,	集合
//////返回参数：
//////函数说明：
//////*******************************************************************************/
//////BOOL Read_MaxDmd(INT8U *pBuff, INT8U dmd, INT8U rateNo)
//////{
////////	INT8U	i;
////////	INT32U	demand, addr;
////////	
////////	if ((dmd < 8) && (rateNo <= 4))
////////	{
////////		addr = EE_MaxDemand + 8*dmd*5 + 8*rateNo;
////////		if (TRUE != Read_EEprom(&pBuff[0], addr, 8))						//当前最大需量数据
////////		{
////////			return FALSE;
////////		}
////////		demand = pBuff[0] | pBuff[1]<<8 | pBuff[2]<<16;
////////		LHEX_nBCD(&pBuff[0], demand, 3);
////////	}
////////	else if ((dmd == 0xFD) && (rateNo == 0x00))								//清需量记录用
////////	{
////////		if((TRUE != Read_EEprom(&pBuff[0],  EE_MaxDemand,     8))
////////		|| (TRUE != Read_EEprom(&pBuff[8],  EE_MaxDemand+40,  8))
////////		|| (TRUE != Read_EEprom(&pBuff[16], EE_MaxDemand+160, 8))
////////		|| (TRUE != Read_EEprom(&pBuff[24], EE_MaxDemand+200, 8))
////////		|| (TRUE != Read_EEprom(&pBuff[32], EE_MaxDemand+240, 8))
////////		|| (TRUE != Read_EEprom(&pBuff[40], EE_MaxDemand+280, 8)))	
////////		{
////////			return FALSE;
////////		}
////////		for (i=0; i<6; i++)
////////		{
////////			demand = pBuff[i*8] | pBuff[i*8+1]<<8 | pBuff[i*8+2]<<16;
////////			LHEX_nBCD(&pBuff[i*8], demand, 3);
////////		}
////////	}
////////	else if ((dmd == 0xFE) && (rateNo == 0xFF))								//其他冻结用
////////	{
////////		if (TRUE != Read_EEprom(&pBuff[0], EE_MaxDemand, 2*5*8))
////////		{
////////			return FALSE;
////////		}
////////		for (i=0; i<10; i++)
////////		{
////////			demand = pBuff[i*8] | pBuff[i*8+1]<<8 | pBuff[i*8+2]<<16;
////////			LHEX_nBCD(&pBuff[i*8], demand, 3);
////////		}
////////	}
////////	else if ((dmd == 0xFF) && (rateNo == 0xFF))								//月冻结用
////////	{
////////		if (TRUE != Read_EEprom(&pBuff[0], EE_MaxDemand, LEN_MaxDemand))
////////		{
////////			return FALSE;
////////		}
////////		for (i=0; i<40; i++)
////////		{
////////			demand = pBuff[i*8] | pBuff[i*8+1]<<8 | pBuff[i*8+2]<<16;
////////			LHEX_nBCD(&pBuff[i*8], demand, 3);
////////		}
////////	}
////////	else
////////	{
////////		return FALSE;
////////	}
//////	return TRUE;
//////}

///////*******************************************************************************
//////功能描述：	获取当前需量
//////输入参数：	dmd:		需量序号
//////				|__0x00,	有功需量
//////				|__0x01,	无功需量
//////				|__0x02,	视在需量
//////				|__0x03,	一分钟平均有功功率
//////返回参数：
//////函数说明：
//////*******************************************************************************/
//////void Read_CurDmd(INT8U *pBuff, INT8U dmd)
//////{
//////	INT32U	tmp_P, tmp_Q, tmp_S, shift = 0;
//////	
//////	switch (dmd)
//////	{
//////	case 0://有功需量
//////		LHEX_nBCD(&pBuff[0], g_DmdDat[0], 3);
//////		if (g_DmdQuad & 0x08)
//////		{
//////			pBuff[2] |= 0x80;
//////		}
//////		break;
//////	case 1://无功需量
//////		LHEX_nBCD(&pBuff[0], g_DmdDat[1], 3);
//////		if (g_DmdQuad & 0x80)
//////		{
//////			pBuff[2] |= 0x80;
//////		}
//////		break;
//////	case 2://视在需量
//////		tmp_P = g_DmdDat[0];
//////		tmp_Q = g_DmdDat[1];
//////		while ((tmp_P&0xFFFFC000) || (tmp_Q&0xFFFFC000))
//////		{
//////			tmp_P >>= 1;
//////			tmp_Q >>= 1;
//////			shift++;
//////		}
//////		tmp_S = sqrt_16(tmp_P*tmp_P+tmp_Q*tmp_Q);
//////		tmp_S <<= shift;
//////		LHEX_nBCD(&pBuff[0], tmp_S, 3);
//////		break;
//////	case 3://一分钟平均有功功率
//////		LHEX_nBCD(&pBuff[0], g_AvgPowP, 3);
//////		if (g_DmdQuad & 0x08)
//////		{
//////			pBuff[2] |= 0x80;
//////		}
//////		break;
//////	default:
//////		break;
//////	}
//////}

///////*******************************************************************************
//////功能描述：	清除当前需量
//////输入参数：	type:		清除方式
//////				|__0,		清RAM和EE		（总清,需量清零,或过结算时用）
//////				|__1,		只清RAM			（更改时间,更改参数,时段切换,上电时用）
//////返回参数：
//////函数说明：
//////*******************************************************************************/
//////void Clear_CurDmd(INT8U type)
//////{
//////	INT8U	i, j;
//////	INT8U	buff[LEN_MaxDemand];
//////	
//////	g_DmdQuad = 0x00;														//需量象限寄存器
//////	for (i=0; i<5; i++)
//////	{
//////		g_DmdSec[i] = 0;													//需量秒计时器
//////		g_DmdMin[i] = 0;													//需量分计时器
//////		g_DmdAdd[i] = 0;													//累加需量(功率)
//////		for (j=0; j<60; j++)
//////		{
//////			g_DmdBuf[i][j] = 0;												//缓存需量(功率)
//////		}
//////		g_DmdDat[i] = 0;													//当前需量(功率)
//////	}
//////	
//////	if (type == 0)
//////	{
//////		Set_Data(&buff[0], 0x00, LEN_MaxDemand);
//////		Write_EEprom(EE_MaxDemand, &buff[0], LEN_MaxDemand);
//////	}
//////}

///////*******************************************************************************
//////功能描述：	设置需量设置参数
//////输入参数：	idx			
//////				|__0,		需量周期
//////				|__1,		滑差时间
//////			para:		设置参数
//////返回参数：	设置结果
//////				|__FALSE,	失败
//////				|__TRUE,	成功
//////函数说明：
//////*******************************************************************************/
//////BOOL Set_DmdPara(INT8U idx, INT8U para)
//////{
//////	INT8U	buff[LEN_DmdPara];
//////	
//////	if ((para == 0) || ((para&0x0F) > 0x09)									//BCD码?
//////	|| ((idx == 0) && (para > 0x60))										//需量周期(1~60)
//////	|| ((idx == 1) && (para > 0x20)))										//滑差时间(1~20)
//////	{
//////		return FALSE;
//////	}
//////	if (TRUE != Read_ParaData(ID_DmdPara, &buff[0]))
//////	{
//////		return FALSE;
//////	}
//////	para = BCD_HEX(para);
//////	if (buff[idx] != para)
//////	{
//////		buff[idx] = para;
//////		if (TRUE != Write_ParaData(ID_DmdPara, &buff[0]))
//////		{
//////			return FALSE;
//////		}
//////		Copy_Data(&g_DmdPara[0], &buff[0], LEN_DmdPara);
//////		if ((g_DmdPara[0] == 0) || (g_DmdPara[0] > 60)						//需量周期(1~60)
//////		|| (g_DmdPara[1] == 0) || (g_DmdPara[1] > 20)						//滑差时间(1~20)
//////		|| ((g_DmdPara[0] % g_DmdPara[1]) != 0x00))							//不整除
//////		{
//////			Copy_Data(&g_DmdPara[0], TAB_DmdPara, LEN_DmdPara);
//////		}
//////		Fetch_CRC(&g_DmdPara[0], LEN_DmdPara);
//////		Clear_CurDmd(1);													//清除当前需量
//////	}
//////	return TRUE;
//////}

///////*******************************************************************************
//////功能描述：	获取需量设置参数
//////输入参数：	idx			
//////				|__0,		需量周期
//////				|__1,		滑差时间
//////返回参数：
//////函数说明：
//////*******************************************************************************/
////////INT8U Get_DmdPara(INT8U idx)
////////{
////////	if (TRUE != Check_CRC(&g_DmdPara[0], LEN_DmdPara))
////////	{
////////		if (TRUE != Read_ParaData(ID_DmdPara, &g_DmdPara[0]))
////////		{
////////			Copy_Data(&g_DmdPara[0], TAB_DmdPara, LEN_DmdPara);
////////		}
////////		Fetch_CRC(&g_DmdPara[0], LEN_DmdPara);
////////	}
////////	return g_DmdPara[idx];
////////}

///////*******************************************************************************
//////功能描述：	设置组合特征字
//////输入参数：	idx			
//////				|__0,		有功
//////				|__1,		无功1
//////				|__2,		无功2
//////			comb:		组合方式特征字
//////返回参数：	设置结果
//////				|__FALSE,	失败
//////				|__TRUE,	成功
//////函数说明：
//////*******************************************************************************/
//////BOOL Set_EnyCmbZ(INT8U idx, INT8U comb)
//////{
//////	INT8U	buff[LEN_EnyCmbZ];
//////	
//////	if ((0x03==(comb&0x03)) || (0x0C==(comb&0x0C)) || (0x30==(comb&0x30)) || (0xC0==(comb&0xC0))
//////	|| ((idx == 0) && (0x00 == (comb&0x0F)))
//////	|| ((idx != 0) && (0x00 == comb)))
//////	{
//////		return FALSE;
//////	}
//////	
//////	if (TRUE != Read_ParaData(ID_EnyCmbZ, &buff[0]))
//////	{
//////		return FALSE;
//////	}
//////	buff[idx] = comb;
//////	if (TRUE == Write_ParaData(ID_EnyCmbZ, &buff[0]))
//////	{
//////		Copy_Data(&g_EnyCmbZ[0], &buff[0], LEN_EnyCmbZ);
//////		Fetch_CRC(&g_EnyCmbZ[0], LEN_EnyCmbZ);
//////		return TRUE;
//////	}
//////	return FALSE;
//////}

///////*******************************************************************************
//////功能描述：	获取组合特征字
//////输入参数：
//////返回参数：	组合方式特征字
//////函数说明：
//////*******************************************************************************/
//////INT8U Get_EnyCmbZ(INT8U idx)
//////{
//////	if (TRUE != Check_CRC(&g_EnyCmbZ[0], LEN_EnyCmbZ))
//////	{
//////		if (TRUE != Read_ParaData(ID_EnyCmbZ, &g_EnyCmbZ[0]))
//////		{
//////			Copy_Data(&g_EnyCmbZ[0], TAB_EnyCmbZ, LEN_EnyCmbZ);
//////		}
//////		Fetch_CRC(&g_EnyCmbZ[0], LEN_EnyCmbZ);
//////	}
//////	return g_EnyCmbZ[idx];
//////}


///////*******************************************************************************
//////功能描述：	电能脉冲检测
//////*******************************************************************************/
////////#define C_30mSec		(30u*C_SysTick/1000)								//30毫秒
////////#define C_96mSec		(96u*C_SysTick/1000)								//96毫秒
////////#define C_200mSec		(200u*C_SysTick/1000)								//200毫秒
////////#define C_900mSec		(900u*C_SysTick/1000)								//900毫秒
////////void INT_PulseKey(void)
////////{
////////	static volatile	INT8U	s_IntPulseP = 0;
////////	static volatile	INT8U	s_IntPulseQ = 0;
////////	
////////	if (PIN_PulseP)
////////	{
////////		if (++s_IntPulseP > C_900mSec)
////////		{
////////			s_IntPulseP = C_200mSec;
////////		}
////////	}
////////	else
////////	{
////////		if ((s_IntPulseP >= C_30mSec) && (s_IntPulseP <= C_96mSec+1))
////////		{
////////			g_IntPulse[0]++;
////////		}
////////		s_IntPulseP = 0;
////////	}
////////	if (PIN_PulseQ)
////////	{
////////		if (++s_IntPulseQ > C_900mSec)
////////		{
////////			s_IntPulseQ = C_200mSec;
////////		}
////////	}
////////	else
////////	{
////////		if ((s_IntPulseQ >= C_30mSec) && (s_IntPulseQ <= C_96mSec+1))
////////		{
////////			g_IntPulse[1]++;
////////		}
////////		s_IntPulseQ = 0;
////////	}
////////}

///////*******************************************************************************
//////功能描述：	电能处理定时器
//////函数说明：	中断中调用，注意程序运行效率（速度）
//////*******************************************************************************/
//////void INT_EnyTimer(void)
//////{
//////	g_EnyTimer++;
//////}
