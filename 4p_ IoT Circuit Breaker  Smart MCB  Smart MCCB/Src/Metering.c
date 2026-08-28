
///*******************************************************************************
//		Include files
//*******************************************************************************/
//#include "TypeDef.h"
//#include "UserCfg.h"
////#include "Common.h"
//#include "Metering.h"
////#include "Storage.h"

////#include "TaskRecord.h"
////参数数据长度定义（双备份）----------------------------------------------------
//#define	LEN_CaliPara	50u				//校表参数
//#define	LEN_MeterAddr	6u				//通讯地址



//#define	NUM_CaliPara	(sizeof(TAB_CaliPara)/sizeof(TAB_CaliPara[0]))

//////通讯引脚定义------------------------------------------------------------------
////#define	HIGH_CS()		SPI_CS_SET
////#define	LOW_CS()		SPI_CS_CLEAR()							//计量SPICS		PH1
////#define	HIGH_CLK()		IO_L(SPI1_Clk)
////#define	LOW_CLK()		HT_GPIOH->PTCLR  = (1<<0)							//计量SPICLK	PH0
////#define	HIGH_DIN()		HT_GPIOC->PTSET  = (1<<10)
////#define	LOW_DIN()		HT_GPIOC->PTCLR  = (1<<10)							//计量SPIDIN	PC10
////#define	PIN_DOUT		(HT_GPIOC->PTDAT & (1<<9))							//计量SPIDOUT	PC9
////#define PIN_PulseP		(HT_GPIOH->PTDAT & (1<<3))							//有功电能脉冲	PH3
////#define PIN_PulseQ		(HT_GPIOH->PTDAT & (1<<2))							//无功电能脉冲	PH2

///*******************************************************************************
//		Variable Definition
//*******************************************************************************/
//static	INT32U	g_CheckSum0						__no_init;					//计量芯片校验和
//static	INT32U	g_CheckSum1						__no_init;					//计量芯片校验和
//static	BOOL	g_EmuWork						__no_init;					//计量芯片工作状态（FALSE:无效/TRUE:有效）
////static	INT16U	g_CaliPara[LEN_CaliPara/2]		__no_init;					//计量芯片参数
//	
///*******************************************************************************
//		Fcuction Declaration
//*******************************************************************************/
////#define SPI_Delay()		NOP();NOP();NOP()									//SPI通讯速率375kBps

////extern INT8U Get_RateNo(void);												//获取费率号
////extern BOOL Get_VoltStatus(void);											//获取上电状态
////extern BOOL Is_CoverOpen(INT8U idx);										//获取开盖状态

///*******************************************************************************
//功能描述：	硬复位ATT702x
//输入参数：
//返回参数：
//函数说明：	//PA[5]:		IRQ					输入
//			//PC[9]:		DIN					输入
//			//PC[10]:		DOUT				输出0
//			//PH[0]:		CLK					输出0
//			//PH[1]:		CS					输出1
//			//PH[4]:		VCTL				输出X
//*******************************************************************************/
//void Reset_Meter(void)
//{
////#if	(TP_Pause == TP_Hold)													//Hold模式
////	if (TRUE != Get_VoltStatus())											//掉电状态?
////	{
////		SwitchTo_Fpll();													//切换到PLL时钟
////	}
////#endif
////	if (((HT_GPIOA->IOCFG & 0x0020) != 0x0000)
////	|| ((HT_GPIOC->IOCFG & 0x0300) != 0x0000)
////	|| ((HT_GPIOH->IOCFG & 0x0013) != 0x0000))
////	{
////		EnWr_WPREG();
////		HT_GPIOA->IOCFG &= ~0x0020;											//配置为GPIO
////		HT_GPIOC->IOCFG &= ~0x0300;											//配置为GPIO
////		HT_GPIOH->IOCFG &= ~0x0013;											//配置为GPIO
////		DisWr_WPREG();
////	}
////	HT_GPIOA->PTOD  |= 0x0020;												//1不开漏
//////	HT_GPIOA->PTUP  |= 0x0020;												//0上拉
////	HT_GPIOA->PTDIR &=~0x0020;												//输入
////	
////	HT_GPIOC->PTCLR  = 0x0400;												//输出0
////	HT_GPIOC->PTOD  |= 0x0600;												//1不开漏
////	HT_GPIOC->PTUP  |= 0x0200;												//0上拉
////	HT_GPIOC->PTDIR &=~0x0200;												//输入
////	HT_GPIOC->PTDIR |= 0x0400;												//输出

////	HT_GPIOH->PTSET  = 0x0012;												//输出1	
////	HT_GPIOH->PTCLR  = 0x0001;												//输出0
////	HT_GPIOH->PTOD  |= 0x0013;												//1不开漏
//////	HT_GPIOH->PTUP  |= 0x0013;												//0上拉
////	HT_GPIOH->PTDIR |= 0x0013;												//输出
////	
////	Delay_mSec(80);
////	HT_GPIOH->PTCLR  = 0x0010;												//输出0
//}

///*******************************************************************************
//功能描述：	读计量参数
//输入参数：	addr:		寄存器地址
//返回参数：	寄存器数据
//函数说明：	发送450kbps,接收520bps
//*******************************************************************************/
//INT32U Read_Reg(INT8U addr)
//{
////	INT8U	i;
//	INT32U	temp = 0;

////	addr &= 0x7F;
////	HIGH_CS();
////	SPI_Delay();
////	LOW_CLK();
////	SPI_Delay();
////	LOW_CS();
////	SPI_Delay();
////	for (i=0; i<8; i++)
////	{
//////		SPI_Delay();
////		HIGH_CLK();
////		if (addr&0x80)
////		{
////			HIGH_DIN();
////		}
////		else
////		{
////			LOW_DIN();
////		}
////		addr <<= 1;
////		LOW_CLK();
////	}
////	SPI_Delay();
////	SPI_Delay();
////	SPI_Delay();
////	SPI_Delay();
////	for (i=0; i<24; i++)
////	{
//////		SPI_Delay();
////		temp <<= 1;
////		HIGH_CLK();
//////		SPI_Delay();
////		if (PIN_DOUT)
////		{
////			temp |= 0x01;
////		}
////		LOW_CLK();
////	}
////	SPI_Delay();
//// 	HIGH_CS();
//	return temp;
//}

///*******************************************************************************
//功能描述：	写校表参数
//输入参数：	addr:		寄存器地址
//			temp:		寄存器数据
//*******************************************************************************/
//void Write_Reg(INT8U addr, INT32U temp)
//{
////	INT8U	i;
////	
////	addr |= 0x80;
////	HIGH_CS();
////	SPI_Delay();
////	LOW_CLK();
////	SPI_Delay();
////	LOW_CS();
////	SPI_Delay();
////	for (i=0; i<8; i++)
////	{
////		SPI_Delay();
////		HIGH_CLK();
////		if (addr&0x80)
////		{
////			HIGH_DIN();
////		}
////		else
////		{
////			LOW_DIN();
////		}
////		addr <<= 1;
////		LOW_CLK();
////	}
////	for (i=0; i<24; i++)
////	{
////		SPI_Delay();
////		HIGH_CLK();
////		SPI_Delay();
////		if (temp&0x800000)
////		{
////			HIGH_DIN();
////		}
////		else
////		{
////			LOW_DIN();
////		}
////		temp <<= 1;
////		LOW_CLK();
////	}
////	SPI_Delay();
////	HIGH_CS();
//}

///*******************************************************************************
//功能描述：	读取校表参数
//输入参数：
//返回参数：
//函数说明：
//*******************************************************************************/
//static void Read_CaliPara(void)
//{
////	INT8U	i;
////	
////	if (TRUE != Check_CRC((INT8U*)&g_CaliPara[0], LEN_CaliPara))
////	{
////		if (TRUE != Read_ParaData(ID_CaliPara, (INT8U*)&g_CaliPara[0]))
////		{
////			Set_Data((INT8U*)&g_CaliPara[0], 0x00, LEN_CaliPara);
////		}
////		for (i=21; i<=23; i++)
////		{
////			g_CaliPara[i] &= 0x00FF;
////		}
////		Fetch_CRC((INT8U*)&g_CaliPara[0], LEN_CaliPara);
////	}
//}

///*******************************************************************************
//功能描述：	初始化计量模块（假）
//输入参数：
//返回参数：
//函数说明：	因计量模块的校验和寄存器的更新需要一定的时间
//			如在调表程序中实时初始化计量模块，如果再马上作定时检查，则无意义
//			故本函数仅将RAM中的校验和复位，以使定时检查时发生错误，而对计量模块作初始化。
//*******************************************************************************/
//void Init_Metering(void)
//{
//	g_EmuWork = FALSE;
//	g_CheckSum0 += 0x55555555;
//	g_CheckSum1 += 0x55555555;
//}

///*******************************************************************************
//功能描述：	检查计量模块
//输入参数：
//返回参数：	计量模块状态
//				|__FALSE,	异常
//				|__TRUE,	正常
//函数说明：	本函数由应用层1秒调用一次
//*******************************************************************************/
//BOOL Check_Metering(void)
//{
//	static	INT8U	s_RegChk = 0;
//	INT16U	para;
//	INT8U	i;
//	
//	if (g_EmuWork != TRUE)
//	{
//		return FALSE;
//	}
//	if ((g_CheckSum0 != Read_Reg(r_ChkSum0))
//	&& (g_CheckSum0 != Read_Reg(r_ChkSum0)))								//二次通讯错误
//	{
//		return FALSE;
//	}
//	if ((g_CheckSum1 != Read_Reg(r_ChkSum1))
//	&& (g_CheckSum1 != Read_Reg(r_ChkSum1)))								//二次通讯错误
//	{
//		return FALSE;
//	}
//	if (++s_RegChk >= NUM_CaliPara/3)										//1次3个
//	{
//		s_RegChk = 0;
//	}
//	for (i=0; i<3; i++)
//	{
//		if (TAB_CaliPara[s_RegChk*3+i].mod == 0)							//固定参数
//		{
//			para = TAB_CaliPara[s_RegChk*3+i].set;
//		}
//		else																//校表参数
//		{
//			Read_CaliPara();												//读取校表参数
////			para = g_CaliPara[TAB_CaliPara[s_RegChk*3+i].set];
//		}
//		Write_Reg(w_EnRdCali, 0x00005A);									//开启读校表参数
//		if ((para != Read_Reg(TAB_CaliPara[s_RegChk*3+i].addr))
//		&& (para != Read_Reg(TAB_CaliPara[s_RegChk*3+i].addr)))
//		{
//			Write_Reg(w_EnRdCali, ~0x00005A);								//禁止读校表参数
//			return FALSE;
//		}
//		Write_Reg(w_EnRdCali, ~0x00005A);									//禁止读校表参数
//	}
//	return TRUE;
//}	
//	
///*******************************************************************************
//功能描述：	修复计量模块
//输入参数：
//返回参数：
//函数说明：	本函数由应用层1秒调用一次
//*******************************************************************************/
//void Repair_Metering(void)
//{
////	INT8U	i;
////	INT16U	para;
//	
////	g_EmuWork = FALSE;
////#if	(TP_SoftVer == TP_Debug)//调试版本
////	Open_RelayLed();
////#endif
//////	Reset_Meter();															//硬复位ATT702x
////	Write_Reg(w_SoftRst, 0x000000);											//软复位ATT702x
////	
////	Read_CaliPara();														//读取校表参数
////	Delay_mSec(5);
////	Write_Reg(w_EnWrCali, 0x00005A);										//开启写校表参数

////	g_CheckSum0 = 0;
////	g_CheckSum1 = 0;
////	for (i=0; i<NUM_CaliPara; i++)
////	{
////		if (TAB_CaliPara[i].mod == 0)										//固定参数
////		{
////			para = TAB_CaliPara[i].set;
////		}
////		else																//校表参数
////		{
////			para = g_CaliPara[TAB_CaliPara[i].set];
////		}
////		if (para != TAB_CaliPara[i].rst)
////		{
////			Write_Reg(TAB_CaliPara[i].addr, para);
////		}
////		if (TAB_CaliPara[i].addr <= w_SAGLevel)
////		{
////			g_CheckSum0 += para;
////		}
////		else
////		{
////			g_CheckSum1 += para;
////		}
////	}
////	Write_Reg(w_EnWrCali, ~0x00005A);										//禁止写校表参数
////	g_EmuWork = TRUE;
//}

/////*******************************************************************************
////功能描述：	开启电流采样通道
////输入参数：
////返回参数：
////函数说明：	用于掉电时作全失压,掉电纪录判断
////*******************************************************************************/
////void Open_Current(void)
////{
////	
////////	Open_RelayLed();
//////	Reset_Meter();															//硬复位ATT702x
//////	Read_CaliPara();														//读取校表参数
//////	Delay_mSec(5);
//////	Write_Reg(w_EnWrCali,	0x00005A);										//开启写校表参数
//////	Write_Reg(w_ModeCfg,	(TAB_CaliPara[0].set & 0xCFAA));				//01,模式相关控制器(有效值更新慢,稳定慢,关闭3个电流通道ADC)
//////	Write_Reg(w_PGACtrl,	TAB_CaliPara[1].set);							//02,ADC增益选择
//////	Write_Reg(w_IgainA,		g_CaliPara[15]);								//1A,A相电流增益
//////	Write_Reg(w_IgainB,		g_CaliPara[16]);								//1B,B相电流增益
//////	Write_Reg(w_IgainC,		g_CaliPara[17]);								//1C,C相电流增益
////////	Write_Reg(w_EnWrCali,	~0x00005A);										//禁止写校表参数
//////	Delay_mSec(250);														//3.6个采样周器,(1s/14.4Hz=69.4ms)
////}

///*******************************************************************************
//功能描述：	设置校正参数
//输入参数：	pPara:		校正参数
//			addr:		寄存器地址
//返回参数：
//函数说明：
//*******************************************************************************/
//BOOL Set_CaliPara(const INT8U *pPara, INT8U addr)
//{
////	INT8U	phs;
////	INT32S	s32, err;
////	INT32U	u32, rms;
////	INT16S	s16, tmp;
////	INT16U	u16;
////	
////	if (addr < 0x20)														//电压,电流增益
////	{
////		rms = nBCD_LHEX(&pPara[0], 3);
////	}
////	else if (addr < 0x70)													//功率增益,相位校准,功率偏移
////	{
////		err = nBCD_LHEX(&pPara[0], 3);
////		if (pPara[3] >= 0x80)
////		{
////			err = -err;
////		}
////	}
////	else
////	{
////	}
////	phs = addr & 0x0F;
////	Read_CaliPara();														//读取校表参数
////	Write_Reg(w_EnWrCali, 0x00005A);										//开启写校表参数
////	switch (addr)
////	{
////	case 0x00://A相电压增益(Ugain = (Ur/Urms-1)*2^15)
////	case 0x01://B相电压增益
////	case 0x02://C相电压增益
////		rms /= 100;
////		if ((rms < 2100) || (rms > 2300))
////		{
////			return FALSE;
////		}
////		u32 = Read_Reg(r_UaRms+phs);										//A,B,C相电压
////		u32 *= 17;
////		u32 >>= 13;
////		u32 ++;
////		u32 >>= 1;
////		if ((u32 < 1800) || (u32 > 2600))
////		{
////			return FALSE;
////		}
////		rms <<= 15;
////		rms /= u32;
////		s16 = rms - 32768;
////		tmp = (INT16S)g_CaliPara[12+phs];
////		s16 += tmp;
////		u16 = (INT16U)s16;
////		g_CheckSum0 -= g_CaliPara[12+phs];
////		g_CheckSum0 += u16;
////		g_CaliPara[12+phs] = u16;
////		Write_Reg(w_UgainA+phs, u16);
////		break;
////	case 0x10://A相电流增益(Igain = (Ir/Irms-1)*2^15)
////	case 0x11://B相电流增益
////	case 0x12://C相电流增益
////		if ((rms < 4500) || (rms > 5500))
////		{
////			return FALSE;
////		}
////		u32 = Read_Reg(r_IaRms+phs);										//A,B,C相电流
////		u32 *= 9;
////		u32 >>= 8;
////		u32 ++;
////		u32 >>= 1;
////		if ((u32 < 4000) || (u32 > 6000))
////		{
////			return FALSE;
////		}
////		rms <<= 15;
////		rms /= u32;
////		s16 = rms - 32768;
////		tmp = (INT16S)g_CaliPara[15+phs];
////		s16 += tmp;
////		u16 = (INT16U)s16;
////		g_CheckSum0 -= g_CaliPara[15+phs];
////		g_CheckSum0 += u16;
////		g_CaliPara[15+phs] = u16;
////		Write_Reg(w_IgainA+phs, u16);
////		break;
////	case 0x20://A相有功功率增益(Pgain = (-err%/(1+err%))*2^15)
////	case 0x21://B相有功功率增益
////	case 0x22://C相有功功率增益
////		s16 = -32768*err/(100000+err);
////		tmp = (INT16S)g_CaliPara[0+phs];
////		s16 += tmp;
////		u16 = (INT16U)s16;
////		g_CheckSum0 -= 3*g_CaliPara[0+phs];
////		g_CheckSum0 += 3*u16;
////		g_CaliPara[0+phs] = u16;
////		Write_Reg(w_PgainA+phs,   u16);
////		Write_Reg(w_PgainA+phs+3, u16);
////		Write_Reg(w_PgainA+phs+6, u16);
////		break;
////	case 0x30://A相相位校正0(Phgain = (err%/1.732)*2^15)
////	case 0x31://B相相位校正0
////	case 0x32://C相相位校正0
////		s16 = -(32768*err+86602)/173205;
////		tmp = (INT16S)g_CaliPara[3+phs];
////		s16 += tmp;
////		u16 = (INT16U)s16;
////		g_CheckSum0 -= g_CaliPara[3+phs];
////		g_CheckSum0 += u16;
////		g_CaliPara[3+phs] = u16;
////		Write_Reg(w_PhSregApq0+phs, u16);
////		break;
////	case 0x40://A相相位校正1(Phgain = (err%/1.732)*2^15)
////	case 0x41://B相相位校正1
////	case 0x42://C相相位校正1
////		s16 = -(32768*err+86602)/173205;
////		tmp = (INT16S)g_CaliPara[6+phs];
////		s16 += tmp;
////		u16 = (INT16U)s16;
////		g_CheckSum0 -= g_CaliPara[6+phs];
////		g_CheckSum0 += u16;
////		g_CaliPara[6+phs] = u16;
////		Write_Reg(w_PhSregApq1+phs, u16);
////		break;
////	case 0x50://A相相位校正2(Phgain = (err%/1.732)*2^15)
////	case 0x51://B相相位校正2
////	case 0x52://C相相位校正2
////		s16 = -(32768*err+86602)/173205;
////		tmp = (INT16S)g_CaliPara[18+phs];
////		s16 += tmp;
////		u16 = (INT16U)s16;
////		g_CheckSum1 -= g_CaliPara[18+phs];
////		g_CheckSum1 += u16;
////		g_CaliPara[18+phs] = u16;
////		Write_Reg(w_PhSregApq2+phs, u16);
////		break;
////	case 0x60://A相有功功率偏移(Poffset = (Preal*EC*HFConst*2^31*(-Err%))/(2.592*10^10))
////	case 0x61://B相有功功率偏移
////	case 0x62://C相有功功率偏移
////		s32  = -err/20;														//有功增益微调
////		err  = -err;
//////		err *= C_Ib;
////		err *= C_IMP;
////		err *= 500;
////		err *= 2;
////		err /= 439;
////		s16 = err;
////		tmp = (INT16S)((g_CaliPara[9+phs]<<8) + (g_CaliPara[21+phs]&0xFF));
////		s16 += tmp;
////		u16 = (INT16U)s16;
////		
////		g_CheckSum1 -= g_CaliPara[21+phs];
////		g_CheckSum1 += u16&0xFF;
////		g_CaliPara[21+phs] = u16&0xFF;
////		Write_Reg(w_PoffsetAL+phs, u16&0xFF);
////		
////		g_CheckSum0 -= g_CaliPara[9+phs];
////		u16 >>= 8;
////		if (u16 & 0x80)
////		{
////			u16 |= 0xFF00;
////		}
////		g_CheckSum0 += u16;
////		g_CaliPara[9+phs] = u16;
////		Write_Reg(w_QoffsetA+phs, u16>>8);
////		
////		s16 = -32768*s32/(100000+s32);
////		tmp = (INT16S)g_CaliPara[0+phs];
////		s16 += tmp;
////		u16 = (INT16U)s16;
////		g_CheckSum0 -= 3*g_CaliPara[0+phs];
////		g_CheckSum0 += 3*u16;
////		g_CaliPara[0+phs] = u16;
////		Write_Reg(w_PgainA+phs,   u16);
////		Write_Reg(w_PgainA+phs+3, u16);
////		Write_Reg(w_PgainA+phs+6, u16);
////		break;
////	case 0x70://TPS初值校正寄存器
////		u16 = Read_Reg(r_TPSD);												//温度传感器的输出
////		g_CheckSum1 -= g_CaliPara[24];
////		g_CheckSum1 += u16;
////		g_CaliPara[24] = u16;
////		Write_Reg(w_TPSoffset, u16);
////		break;
////	case 0xFF://计量初始化
////		Set_Data((INT8U*)&g_CaliPara[0], 0x00, LEN_CaliPara);
////		g_EmuWork = FALSE;
////		break;
////	default:
////		break;
////	}
////	Write_Reg(w_EnWrCali, ~0x00005A);										//禁止写校表参数
////	Fetch_CRC((INT8U*)&g_CaliPara[0], LEN_CaliPara);
////	if (TRUE != Write_ParaData(ID_CaliPara, (INT8U*)&g_CaliPara[0]))		//写入校表参数
////	{
////		return FALSE;
////	}
//	return TRUE;
//}

///*******************************************************************************
//功能描述：	读取计量参数
//输入参数：
//返回参数：
//函数说明：
//*******************************************************************************/
//void Get_MeterPara(INT8U addr, INT8U *pBuff)
//{
//	INT32U	value;
//	
//	if (addr & 0x80)	//读校表寄存器
//	{
//		Write_Reg(w_EnRdCali, 0x00005A);									//开启读校表参数
//		value = Read_Reg(addr);
//		Write_Reg(w_EnRdCali, ~0x00005A);									//禁止读校表参数
//	}
//	else
//	{
//		value = Read_Reg(addr);
//	}
//	pBuff[0] = value;
//	pBuff[1] = value>>8;
//	pBuff[2] = value>>16;
//}
