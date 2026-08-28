
#ifndef	_TYPEDEF_H_
#define	_TYPEDEF_H_
////自定义数据类型----------------------------------------------------------------
typedef	unsigned char			INT8U;
typedef	unsigned short			INT16U;
typedef	unsigned long			INT32U;
typedef	unsigned long long		INT64U;

typedef	signed char				INT8S;
typedef	signed short			INT16S;
typedef	signed long				INT32S;
typedef	signed long	long		INT64S;
//typedef	enum
//{
//	FALSE	= (INT8U)0x55,		//逻辑假
//	TRUE	= (INT8U)0xAA,		//逻辑真
//	OTHER	= (INT8U)0x66,		//其他
//}BOOL;							//布尔型

//#define	DISABLE		0			//禁止
//#define	ENABLE		1			//使能

//#define	NULL		0			//无效

////函数指针类型------------------------------------------------------------------
//typedef	void		(*V_FUN_V)(void);		
//typedef	void		(*V_FUN_U8)(INT8U);
//typedef	BOOL		(*B_FUN_V)(void);
//typedef	BOOL		(*B_FUN_U8)(INT8U);

#define	__no_init		__attribute__((section("NOINIT"),zero_init))
#define __reserve		__attribute__((section("INROM2")))
/*******************************************************************************
		Macro Definition
*******************************************************************************/
//校表参数寄存器定义------------------------------------------------------------
#define	w_StartSig		0x00			//校表参数寄存器起始标志
#define	w_ModeCfg		0x01			//模式相关控制器
#define	w_PGACtrl		0x02			//ADC增益选择
#define	w_EMUCfg		0x03			//EMU模块配置寄存器
#define	w_PgainA		0x04			//A相有功功率增益
#define	w_PgainB		0x05			//B相有功功率增益
#define	w_PgainC		0x06			//C相有功功率增益
#define	w_QgainA		0x07			//A相无功功率增益
#define	w_QgainB		0x08			//B相无功功率增益
#define	w_QgainC		0x09			//C相无功功率增益
#define	w_SgainA		0x0A			//A相视在功率增益
#define	w_SgainB		0x0B			//B相视在功率增益
#define	w_SgainC		0x0C			//C相视在功率增益
#define	w_PhSregApq0	0x0D			//A相相位校正0
#define	w_PhSregBpq0	0x0E			//B相相位校正0
#define	w_PhSregCpq0	0x0F			//C相相位校正0
#define	w_PhSregApq1	0x10			//A相相位校正1
#define	w_PhSregBpq1	0x11			//B相相位校正1
#define	w_PhSregCpq1	0x12			//C相相位校正1
#define	w_PoffsetA		0x13			//A相有功功率offset校正
#define	w_PoffsetB		0x14			//B相有功功率offset校正
#define	w_PoffsetC		0x15			//C相有功功率offset校正
#define	w_QPhscal		0x16			//无功相位校正
#define	w_UgainA		0x17			//A相电压增益
#define	w_UgainB		0x18			//B相电压增益
#define	w_UgainC		0x19			//C相电压增益
#define	w_IgainA		0x1A			//A相电流增益
#define	w_IgainB		0x1B			//B相电流增益
#define	w_IgainC		0x1C			//C相电流增益
#define	w_Istartup		0x1D			//启动电流阈值设置
#define	w_HFConst		0x1E			//高频脉冲输出设置
#define	w_FailVolt		0x1F			//失压阈值设置
#define	w_GainADC7		0x20			//第七路ADC输入信号增益
#define	w_QoffsetA		0x21			//A相无功功率offset校正
#define	w_QoffsetB		0x22			//B相无功功率offset校正
#define	w_QoffsetC		0x23			//C相无功功率offset校正
#define	w_URmsoffsetA	0x24			//A相电压有效值offset校正
#define	w_URmsoffsetB	0x25			//B相电压有效值offset校正
#define	w_URmsoffsetC	0x26			//C相电压有效值offset校正
#define	w_IRmsoffsetA	0x27			//A相电流有效值offset校正
#define	w_IRmsoffsetB	0x28			//B相电流有效值offset校正
#define	w_IRmsoffsetC	0x29			//C相电流有效值offset校正
#define	w_UoffsetA		0x2A			//A相电压通道ADC offset校正
#define	w_UoffsetB		0x2B			//B相电压通道ADC offset校正
#define	w_UoffsetC		0x2C			//C相电压通道ADC offset校正
#define	w_IoffsetA		0x2D			//A相电流通道ADC offset校正
#define	w_IoffsetB		0x2E			//B相电流通道ADC offset校正
#define	w_IoffsetC		0x2F			//C相电流通道ADC offset校正
#define	w_EMUIE			0x30			//中断使能
#define	w_ModuleCfg		0x31			//电路模块配置寄存器
#define	w_AllGain		0x32			//全通道增益
#define	w_HFDouble		0x33			//脉冲常数加倍选择
#define	w_LineGain		0x34			//基波增益校正
#define	w_PinCtrl		0x35			//数字pin上下拉电阻选择控制
#define	w_Pstartup		0x36			//启动功率阈值设置
#define	w_Iregion0		0x37			//相位补偿区域设置寄存器0
#define	w_Cyclength		0x38			//SAG数据长度设置寄存器
#define	w_SAGLevel		0x39			//SAG检测阈值设置寄存器

#define	w_Iregion1		0x60			//相位补偿区域设置寄存器1
#define	w_PhSregApq2	0x61			//A相相位校正2
#define	w_PhSregBpq2	0x62			//B相相位校正2
#define	w_PhSregCpq2	0x63			//C相相位校正2
#define	w_PoffsetAL		0x64			//A相有功功率offset校正低字节
#define	w_PoffsetBL		0x65			//B相有功功率offset校正低字节
#define	w_PoffsetCL		0x66			//C相有功功率offset校正低字节
#define	w_QoffsetAL		0x67			//A相无功功率offset校正低字节
#define	w_QoffsetBL		0x68			//B相无功功率offset校正低字节
#define	w_QoffsetCL		0x69			//C相无功功率offset校正低字节
#define	w_IRmsoffsetT	0x6A			//电流矢量和offset校正寄存器
#define	w_TPSoffset		0x6B			//TPS初值校正寄存器
#define	w_TPSgain		0x6C			//TPS斜率校正寄存器
#define	w_TCoffA		0x6D			//Vrefgain的二次系数
#define	w_TCoffB		0x6E			//Vrefgain的一次系数
#define	w_TCoffC		0x6F			//Vrefgain的常数项
#define	w_EMCfg			0x70			//新增算法控制寄存器
#define	w_OILevel		0x71			//过流阈值设置寄存器

//特殊命令定义------------------------------------------------------------------
#define	w_BuffStr		0xC0			//采样数据缓冲启动命令
#define	w_BuffSet		0xC1			//缓冲数据读指针设置
#define	w_ClrCali		0xC3			//清校表数据
#define	w_SyncSet		0xC4			//同步数据系数设置
#define	w_SyncStr		0xC5			//同步数据启动命令
#define	w_EnRdCali		0xC6			//校表数据读写使能
#define	w_EnWrCali		0xC9			//校表数据写使能
#define	w_SoftRst		0xD3			//软复位
//校表参数常数定义--------------------------------------------------------------
static const __reserve	struct
{
	INT8U		addr;		//寄存器地址
	INT16U		rst;		//复位参数
	INT16U		set;		//设置参数
	INT8U		mod;		//参数类型
}TAB_CaliPara[] =
{
	{w_ModeCfg,		0x89AA,	0xB87E,	0},		//模式相关控制器			开启Ia,Ua,Ib,Ub,Ic,Uc通道ADC,关闭ADC Chop,开启REF chop,Femu=1.8MHz,ref偏置电流7.5uA,有效值更新慢速,稳定慢速,
	{w_PGACtrl,		0x0000,	0x0154,	0},		//ADC增益选择				A,B,C电流通道2倍增益,电压通道2倍增益,
	{w_EMUCfg,		0x0804,	0x78C4,	0},		//EMU模块配置寄存器			视在功率/能量采用PQS方式,3P4W代数和累加,能量读后清零,功率作启动判断,开启有功,无功,视在能量计算
	{w_PgainA,		0x0000,	0,		1},		//A相有功功率增益
	{w_PgainB,		0x0000,	1,		1},		//B相有功功率增益
	{w_PgainC,		0x0000,	2,		1},		//C相有功功率增益
	{w_QgainA,		0x0000,	0,		1},		//A相无功功率增益
	{w_QgainB,		0x0000,	1,		1},		//B相无功功率增益
	{w_QgainC,		0x0000,	2,		1},		//C相无功功率增益
	{w_SgainA,		0x0000,	0,		1},		//A相视在功率增益
	{w_SgainB,		0x0000,	1,		1},		//B相视在功率增益
	{w_SgainC,		0x0000,	2,		1},		//C相视在功率增益
	{w_PhSregApq0,	0x0000,	3,		1},		//A相相位校正0
	{w_PhSregBpq0,	0x0000,	4,		1},		//B相相位校正0
	{w_PhSregCpq0,	0x0000,	5,		1},		//C相相位校正0
	{w_PhSregApq1,	0x0000,	6,		1},		//A相相位校正1
	{w_PhSregBpq1,	0x0000,	7,		1},		//B相相位校正1
	{w_PhSregCpq1,	0x0000,	8,		1},		//C相相位校正1
	{w_PoffsetA,	0x0000,	9,		1},		//A相有功功率offset校正
	{w_PoffsetB,	0x0000,	10,		1},		//B相有功功率offset校正
	{w_PoffsetC,	0x0000,	11,		1},		//C相有功功率offset校正
	{w_QPhscal,		0x0000,	0xFF66,	0},		//无功相位校正
	{w_UgainA,		0x0000,	12,		1},		//A相电压增益
	{w_UgainB,		0x0000,	13,		1},		//B相电压增益
	{w_UgainC,		0x0000,	14,		1},		//C相电压增益
	{w_IgainA,		0x0000,	15,		1},		//A相电流增益
	{w_IgainB,		0x0000,	16,		1},		//B相电流增益
	{w_IgainC,		0x0000,	17,		1},		//C相电流增益
	{w_Istartup,	0x0160,	864u,	0},		//启动电流阈值设置			5A*(1000/144)*0.3%*2^13
	{w_HFConst,		0x0500,	500u*2,	0},		//高频脉冲输出设置			
	{w_FailVolt,	0x0600,	0x0600,	0},		//失压阈值设置				不用
	{w_GainADC7,	0x0000,	0x0000,	0},		//第七路ADC输入信号增益		reserved
	{w_QoffsetA,	0x0000,	0x0000,	0},		//A相无功功率offset校正
	{w_QoffsetB,	0x0000,	0x0000,	0},		//B相无功功率offset校正
	{w_QoffsetC,	0x0000,	0x0000,	0},		//C相无功功率offset校正
	{w_URmsoffsetA,	0x0000,	0x0000,	0},		//A相电压有效值offset校正
	{w_URmsoffsetB,	0x0000,	0x0000,	0},		//B相电压有效值offset校正
	{w_URmsoffsetC,	0x0000,	0x0000,	0},		//C相电压有效值offset校正
	{w_IRmsoffsetA,	0x0000,	0x0009,	0},		//A相电流有效值offset校正
	{w_IRmsoffsetB,	0x0000,	0x0009,	0},		//B相电流有效值offset校正
	{w_IRmsoffsetC,	0x0000,	0x0009,	0},		//C相电流有效值offset校正
	{w_UoffsetA,	0x0000,	0x0000,	0},		//A相电压通道ADC offset校正
	{w_UoffsetB,	0x0000,	0x0000,	0},		//B相电压通道ADC offset校正
	{w_UoffsetC,	0x0000,	0x0000,	0},		//C相电压通道ADC offset校正
	{w_IoffsetA,	0x0000,	0x0000,	0},		//A相电流通道ADC offset校正
	{w_IoffsetB,	0x0000,	0x0000,	0},		//B相电流通道ADC offset校正
	{w_IoffsetC,	0x0000,	0x0000,	0},		//C相电流通道ADC offset校正
	{w_EMUIE,		0x0001,	0x0001,	0},		//中断使能
	{w_ModuleCfg,	0x3527,	0x3437,	0},		//电路模块配置寄存器		开启电压,电流高通,关闭罗氏线圈,开启TPS,BOR,选择PN传感器
	{w_AllGain,		0x0000,	0x0000,	0},		//全通道增益
	{w_HFDouble,	0x0000,	0x0000,	0},		//脉冲常数加倍选择
	{w_LineGain,	0x2C59,	0x2C59,	0},		//基波增益校正
	{w_PinCtrl,		0x000F,	0x000F,	0},		//数字pin上拉电阻选择控制	内置上拉300K电阻
	{w_Pstartup,	0x0030,	216u,	0},		//启动功率阈值设置			(Un*Ib*HFConst*imp*0.3%*2^23/(2.592*10^10))
	{w_Iregion0,	0x7FFF,	3000u,	0},		//相位补偿区域设置寄存器0	5A*(1000/144)*270%*2^5
	{w_Cyclength,	0x1000,	0x1000,	0},		//SAG数据长度设置寄存器
	{w_SAGLevel,	0x4500,	0x4500,	0},		//SAG检测阈值设置寄存器

	{w_Iregion1,	0x0000,	180u,	0},		//相位补偿区域设置寄存器1	5A*(1000/144)*162%*2^5
	{w_PhSregApq2,	0x0000,	18,		1},		//A相相位校正2
	{w_PhSregBpq2,	0x0000,	19,		1},		//B相相位校正2
	{w_PhSregCpq2,	0x0000,	20,		1},		//C相相位校正2
	{w_PoffsetAL,	0x0000,	21,		1},		//A相有功功率offset校正低字节	//X
	{w_PoffsetBL,	0x0000,	22,		1},		//B相有功功率offset校正低字节	//X
	{w_PoffsetCL,	0x0000,	23,		1},		//C相有功功率offset校正低字节	//X
	{w_QoffsetAL,	0x0000,	0x0000,	0},		//A相无功功率offset校正低字节
	{w_QoffsetBL,	0x0000,	0x0000,	0},		//B相无功功率offset校正低字节
	{w_QoffsetCL,	0x0000,	0x0000,	0},		//C相无功功率offset校正低字节
	{w_IRmsoffsetT,	0x0000,	0x0009,	0},		//电流矢量和offset校正寄存器
	{w_TPSoffset,	0x0000,	24,		1},		//TPS初值校正寄存器
	{w_TPSgain,		0x0000,	0x0000,	0},		//TPS斜率校正寄存器
	{w_TCoffA,		0xFEFF,	0xFF11,	0},		//Vrefgain的二次系数
	{w_TCoffB,		0xEF7A,	0x2B52,	0},		//Vrefgain的一次系数
	{w_TCoffC,		0x047C,	0xD483,	0},		//Vrefgain的常数项
	{w_EMCfg,		0x0000,	0x0022,	0},		//新增算法控制寄存器			3P4W制,使用自动补偿,电流矢量算法1,角度算法1,无功能量全波无功,谐波能量,夹角参考Ua,
	{w_OILevel,		0x0000,	0x0000,	0},		//过流阈值设置寄存器
};


/*******************************************************************************
		End File
*******************************************************************************/
#endif
